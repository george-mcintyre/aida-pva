/*
 * @file The AIDA-PVA Service which provides connection to the AIDA-PVA Channel Providers and the SLAC Network.
 */
package edu.stanford.slac.aida.lib;

import edu.stanford.slac.aida.lib.model.*;
import edu.stanford.slac.aida.lib.util.AtomicResult;
import edu.stanford.slac.except.*;
import org.epics.nt.NTURI;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvaccess.server.rpc.RPCService;
import org.epics.pvdata.pv.PVField;
import org.epics.pvdata.pv.PVString;
import org.epics.pvdata.pv.PVStructure;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import java.util.logging.Logger;
import java.util.regex.Pattern;

import static edu.stanford.slac.aida.lib.model.AidaType.*;
import static edu.stanford.slac.aida.lib.util.AidaPVHelper.*;
import static java.lang.Thread.sleep;
import static org.epics.pvdata.pv.Status.StatusType.ERROR;

/**
 * The AIDA-PVA Service which provides connection to the AIDA-PVA Channel Providers and the SLAC Network.
 */
public class AidaRPCService implements RPCService {
    /**
     * Logger to log info
     */
    private static final Logger logger = Logger.getLogger(AidaRPCService.class.getName());

    /**
     * The maximum number of concurrent requests allowed before requests are rejected with failure
     */
    private static final Integer MAX_CONCURRENT_REQUESTS = 50000;

    /**
     * Counter to count maximum number of concurrent request that are allowed to wait on this thread
     */
    private static final AtomicInteger concurrencyCounter = new AtomicInteger(0);

    /**
     * Stores the list of current pending requests and the result as well as a counter to the number of concurrent
     * requests that are actively requesting the same value.  Each entry is used to provide the
     * result to the concurrent threads when the executing thread completes and to synchronise access and clean up
     * resources
     */
    private static final ConcurrentMap<String, AtomicResult> requestResultMap = new ConcurrentHashMap<String, AtomicResult>();

    /**
     * This is the execution lock object that mediates access to the lower level provider code which is not
     * multi-entrant.  When a thread needs to execute the lower level provider code it acquires the lock
     * while other threads for non-similar requests are blocked and wait for the resource to be available.
     * Other threads waiting for similar request results simply wait for the results from the first thread to become
     * available and reuse it.
     */
    private final Lock executionLock = new ReentrantLock();

    /**
     * The metering logging window size in milliseconds
     */
    private static final long METERED_LOGGING_SAMPLE_WINDOW_MS = 60000L;
    /**
     * The number of events withing the metering window that will trigger metering
     */
    private static final int METERING_TRIGGER = 60;

    /**
     * The number of events to skip when metering is on
     */
    private static final int METERING_SKIP = 9;

    /**
     * The list of unix times for the last log events that fall within the metering sample window
     */
    private static final List<Long> logEventTimesWithinInMeteringWindow = new ArrayList<Long>();

    /**
     * The current number of log events that have been skipped in this metering window
     */
    private static int logEventsSkipped = 0;


    /**
     * The aida-pva channel provider
     */
    private final ChannelProvider aidaChannelProvider;

    /**
     * The constructor. will simply store the given AIDA-PVA Channel Provider for use later.
     *
     * @param aidaChannelProvider the given AIDA-PVA Channel Provider
     */
    public AidaRPCService(ChannelProvider aidaChannelProvider) {
        this.aidaChannelProvider = aidaChannelProvider;
    }

    /**
     * Callback when a channel is called
     *
     * @param pvUri the uri passed to the channel containing the name, query, and arguments
     * @return the result of the call
     * @throws RPCRequestException              if any error occurs formulating the request or decoding the response
     * @throws AidaInternalException            if any error occurs because of an implementation error in aida server code
     * @throws MissingRequiredArgumentException when a required argument was not supplied
     * @throws UnableToGetDataException         when server fails to retrieve data
     * @throws UnableToSetDataException         when server fails to set data
     * @throws UnsupportedChannelException      when server does not yet support the specified channel.
     *                                          Usually caused when channel matches a pattern specified in the Channel Configuration File
     *                                          but is not yet supported in the service implementation
     */
    public final PVStructure request(final PVStructure pvUri) throws RPCRequestException, UnableToGetDataException, UnsupportedChannelException, UnableToSetDataException, AidaInternalException, MissingRequiredArgumentException {
        try {
            // Optimise requests by limiting concurrent requests to this AIDA-PVA provider to MAX_CONCURRENT_REQUESTS
            if (concurrencyCounter.getAndIncrement() >= MAX_CONCURRENT_REQUESTS) {
                throw new RPCRequestException(ERROR, "Too many concurrent requests: " + concurrencyCounter.get() + ".  Please reduce request rate");
            }

            // Check that the parameter is always a normative type
            final String type = pvUri.getStructure().getID();
            if (!NTURI.is_a(pvUri.getStructure())) {
                throw new RPCRequestException(ERROR, "Unable to get data, unexpected request type: " + type);
            }

            // Retrieve the PV name
            final PVString pvPathField = pvUri.getStringField("path");
            if (pvPathField == null) {
                throw new RPCRequestException(ERROR, "unable to determine the channel from the request specified: " + pvUri);
            }

            final String channelName = pvPathField.get();
            if (channelName == null || channelName.length() == 0) {
                throw new RPCRequestException(ERROR, "unable to determine the channel from the request specified: <blank>");
            }

            // Retrieve arguments, if any given to this RPC PV channel.
            final PVStructure pvUriQuery = pvUri.getStructureField("query");
            final List<AidaArgument> arguments = getArguments(pvUriQuery);

            // To identify requests that are semantically the same
            final String requestString = makeRequestString(channelName, arguments);
            // New result object to be used if it turns out that no other thread is executing this request
            final AtomicResult atomicResult = new AtomicResult();
            // The result object for a concurrent thread that is executing this request, or null if none exists
            AtomicResult concurrentAtomicResult = requestResultMap.putIfAbsent(requestString, atomicResult);
            // Check that concurrent thread (if exists) is not in the process of delivering results to subscribers
            concurrentAtomicResult = resultsDeliveryCheck(requestString, concurrentAtomicResult, atomicResult);

            if (concurrentAtomicResult == null) {
                // If there is no concurrent thread executing this request, we will execute it in this thread
                return executeRequest(channelName, arguments, requestString, atomicResult);
            } else {
                // There is a concurrent thread executing this request, so wait for result availability, then return it
                return returnResultWhenAvailable(requestString, concurrentAtomicResult);
            }
        } catch (RPCRequestException e) {
            throw e;
        } catch (UnableToGetDataException e) {
            throw new RPCRequestException(ERROR, e.getMessage(), e);
        } catch (UnsupportedChannelException e) {
            throw new RPCRequestException(ERROR, e.getMessage(), e);
        } catch (UnableToSetDataException e) {
            throw new RPCRequestException(ERROR, e.getMessage(), e);
        } catch (AidaInternalException e) {
            throw new RPCRequestException(ERROR, e.getMessage(), e);
        } catch (MissingRequiredArgumentException e) {
            throw new RPCRequestException(ERROR, e.getMessage(), e);
        } catch (Throwable e) {
            throw new RPCRequestException(ERROR, e.getMessage(), e);
        } finally {
            // Whatever happens, error or not, decrease the concurrency counter
            concurrencyCounter.decrementAndGet();
        }
    }

    /**
     * Returns the concurrent result when available by waiting for the prior request to end.
     *
     * @param requestString          the string uniquely identifying the request
     * @param concurrentAtomicResult the AtomicResult object representing the concurrent request's atomic result
     * @return the PVStructure representing the result
     * @throws RPCRequestException if there is an exception while handling the RPC request
     */
    private PVStructure returnResultWhenAvailable(final String requestString, final AtomicResult concurrentAtomicResult) throws RPCRequestException {
        try {
            // Wait for the request to end
            if (!concurrentAtomicResult.waitForResult()) {
                System.err.println("Timeout waiting for result to become available, or process was interrupted");
                throw new InterruptedException("Timeout waiting for result to become available, or process was interrupted");
            }

            // Return the result
            return concurrentAtomicResult.getResult();
        } catch (InterruptedException e) {
            logger.info("AIDA " + requestString + ": Timeout getting results");
            System.err.println(e.getMessage());
            throw new RPCRequestException(ERROR, e.getMessage());
        } finally {
            // If this is the last subscriber then mark delivery of result as complete
            if (concurrentAtomicResult.isLastSubscriber()) {
                concurrentAtomicResult.deliveryComplete(requestResultMap, requestString);
            }
        }
    }

    /**
     * Executes the request and notifies the listeners when complete.
     *
     * @param channelName   the name of the channel
     * @param arguments     the list of AidaArguments for the request
     * @param requestString the request string
     * @param atomicResult  the AtomicResult object to store the result of the request
     * @return the PVStructure object that represents the result of the request
     * @throws RPCRequestException if an error occurs during the request execution
     */
    private PVStructure executeRequest(final String channelName, final List<AidaArgument> arguments, final String requestString, final AtomicResult atomicResult) throws RPCRequestException {
        try {
            // Acquire the execution lock to execute the request with mutual exclusion
            // This will also block other threads with different queries from executing until this one is done
            // Note: Don't use synchronised blocks because that will block other threads from lining up to wait
            // for similar request results while this one is executing
            executionLock.lock();

            // Execute the request with the underlying AIDA-PVA provider
            // Set the result in the atomic result so that other threads that are waiting for the similar
            // request can retrieve it
            final PVStructure retVal = request(channelName, arguments);
            try {
                sleep(1000L);
            } catch (InterruptedException ignored) {
            }
            atomicResult.setResult(retVal);

            synchronized (requestResultMap) {
                // If this is the only subscriber then mark delivery of result as complete
                if (!atomicResult.hasSubscribers()) {
                    atomicResult.deliveryComplete(requestResultMap, requestString);
                }
            }

            // Allow other threads (similar requests) that were waiting on this result to pick them up
            atomicResult.notifyListeners();
            return retVal;
        } finally {
            // Allow another thread to execute a different query - dissimilar - by releasing the lock.
            // All dissimilar requests will be waiting for this lock to be unlocked and one of them will
            // manage to acquire the lock.
            executionLock.unlock();

            // Be careful: Other threads that have the same request (PV + arguments) that call
            // requestResultMap.putIfAbsent(requestString, atomicResult) will return this redundant result
            // and start waiting but will never get a atomicResult.notifyListeners()
            // So we need to make sure that all similar threads are blocked until
            // requestString is removed from the requestResultMap.
        }
    }

    /**
     * Check if we are already in the process of delivering results for this request (requestString).
     * If so then wait until they are all delivered to subscribers then create a new request and continue.
     * Note that we try to create a new request but if another thread that is in the same state gets there
     * first we become subscribers to that thread's new request
     *
     * @param requestString          the string that uniquely identifies this request
     * @param concurrentAtomicResult null if there is no concurrent request, AtomicResult of concurrent request otherwise
     * @param atomicResult           the desired new AtomicResult if we need to create a new request
     * @return The new or concurrent AtomicResult to use for this request
     * @throws InterruptedException if waiting for the results to be delivered times out or is interrupted
     */
    private static AtomicResult resultsDeliveryCheck(final String requestString, final AtomicResult concurrentAtomicResult, final AtomicResult atomicResult) throws InterruptedException {
        if (concurrentAtomicResult != null && concurrentAtomicResult.resultAvailable()) { // The prior thread is delivering results, so wait
            if (!concurrentAtomicResult.waitForResultsToBeDelivered()) {
                System.err.println("Timeout waiting for result to be delivered to concurrent threads, or process was interrupted");
                throw new InterruptedException("Timeout waiting for result to be delivered to concurrent threads, or process was interrupted");
            }
            return requestResultMap.putIfAbsent(requestString, atomicResult);  // Force starting new request (if another thread in this state hasn't already done so)
        }
        return concurrentAtomicResult;
    }

    /**
     * Make request to the specified channel with the uri and arguments specified
     * and return the NT_TABLE of results.
     *
     * @param channelName   channel name
     * @param argumentsList arguments if any
     * @return the structure containing the results.
     * @throws AidaInternalException            if any error occurs because of an implementation error in aida server code
     * @throws MissingRequiredArgumentException when a required argument was not supplied
     * @throws UnableToGetDataException         when server fails to retrieve data
     * @throws UnableToSetDataException         when server fails to set data
     * @throws UnsupportedChannelException      when server does not yet support the specified channel.
     *                                          Usually caused when channel matches a pattern specified in the Channel Configuration File
     *                                          but is not yet supported in the service implementation
     */
    private PVStructure request(String channelName, final List<AidaArgument> argumentsList) throws UnableToGetDataException, UnsupportedChannelException, UnableToSetDataException, AidaInternalException, MissingRequiredArgumentException, RPCRequestException {
        AidaType aidaType;
        AidaChannelOperationConfig config;
        String typeArgument = null;
        boolean isSetterRequest = false;

        {
            final AidaChannelOperationConfig getterConfig = aidaChannelProvider.getGetterConfig(channelName);
            final AidaChannelOperationConfig setterConfig = aidaChannelProvider.getSetterConfig(channelName);
            final AidaType aidaGetterType = getterConfig == null ? NONE : getterConfig.getType();
            final AidaType aidaSetterType = setterConfig == null ? NONE : setterConfig.getType();

            // Get special arguments `TYPE` and `VALUE` used to determine which APIs will be called
            for (final AidaArgument argument : argumentsList) {
                final String argumentName = argument.getName();
                if (argumentName.equalsIgnoreCase("TYPE")) {
                    typeArgument = argument.getValue().toUpperCase();
                } else if (argumentName.equalsIgnoreCase("VALUE")) {
                    isSetterRequest = true;
                }
            }

            aidaType = isSetterRequest ? aidaSetterType : aidaGetterType;
            config = isSetterRequest ? setterConfig : getterConfig;
        }

        // Check for split provider-operation implementation and show an error message directing client to use prefixed channel reference
        checkForAliasAndThrowExceptionAppropriately(channelName, config.getPrefix(), isSetterRequest, aidaType);

        // See if the request type is supported for the channel
        checkThatRequestedOperationIsSupported(channelName, argumentsList, isSetterRequest, aidaType);

        if (typeArgument == null) { // If no TYPE has been specified then see if it was mandatory
            checkIfMissingTypeArgumentIsMandatory(channelName, argumentsList, isSetterRequest, aidaType);
        } else { // If a TYPE has been specified then override the Config's type with the specified one if it is compatible
            aidaType = checkIfTypeArgumentIsCompatible(channelName, argumentsList, isSetterRequest, aidaType, typeArgument);
        }

        // Verify that, if the request is a table request, the configuration defines fields
        checkThatConfigDefinesFieldsForTableRequests(aidaType, config);

        // Validate that the given arguments are allowed for this operation on this channel
        validateArguments(channelName, argumentsList, isSetterRequest, config);

        // If channelName contains a service (<service>::channelName) then remove the service part before calling the service implementation
        // This is a special measure put in place to disambiguate some channels that are used by more than one Service.
        // It allows for channels to be prefixed by "<serviceName>::".  This is done in the channel configuration file and also
        // by clients accessing the service.  But the Channel Provider will not know about this prefix, so we need to
        // remove it before passing the request on to the Channel Provider
        channelName = removeServicePrefixIfPresent(channelName);

        // If the client has specified the channel with the legacy formatted channel name we need to change it to the new format before
        // passing it to the Channel Provider which will be expecting only new format names
        channelName = ensureNewFormatChannelName(channelName);

        // Display the log entry that indicated the request that is being passed to the Channel Provider with its parameters and its expected return type
        logRequest(channelName, argumentsList, isSetterRequest, aidaType);

        // Make an arguments object to pass to requests
        final AidaArguments arguments = new AidaArguments(argumentsList);

        // Call entry point based on return type
        return callNativeChannelProvider(channelName, arguments, isSetterRequest, aidaType, config);
    }

    /**
     * Call the correct entry point based on the selected channel, the channel configuration, the type of operation requested (get/set)
     *
     * @param channelName     the name of the channel
     * @param arguments       the request's arguments
     * @param isSetterRequest true if this is a set operation
     * @param aidaType        the return type
     * @param config          the configuration
     * @return the PVStructure returned from the native call
     * @throws UnsupportedChannelException if operation is invalid for channel
     */
    private PVStructure callNativeChannelProvider(final String channelName, final AidaArguments arguments, final boolean isSetterRequest, final AidaType aidaType, final AidaChannelOperationConfig config) throws UnsupportedChannelException, AidaInternalException {
        if (isSetterRequest) {  // Setter requests
            if (aidaType == VOID) { // Returning VOID
                this.aidaChannelProvider.setValue(channelName, arguments);

            } else if (aidaType == TABLE) { // Returning TABLES
                return asNtTable(this.aidaChannelProvider.setValueWithResponse(channelName, arguments), config);
            }

        } else { // Getter requests
            final AidaType metaType = aidaType.metaType();

            if (metaType == SCALAR) { // Returning SCALAR
                return asScalar(this.aidaChannelProvider.requestScalar(channelName, arguments, aidaType));

            } else if (metaType == SCALAR_ARRAY) { // Returning SCALAR_ARRAY
                return asScalarArray(this.aidaChannelProvider.requestScalarArray(channelName, arguments, aidaType));

            } else { // Returning TABLE
                return asNtTable(this.aidaChannelProvider.requestTable(channelName, arguments), config);
            }
        }
        return NT_SCALAR_EMPTY_STRUCTURE;
    }

    /**
     * If channelName contains a service (<service>::channelName) then remove the service part before calling the service implementation
     * This is a special measure put in place to disambiguate some channels that are used by more than one Service.
     * It allows for channels to be prefixed by "<serviceName>::".  This is done in the channel configuration file and also
     * by clients accessing the service.  But the Channel Provider will not know about this prefix so we need to
     * remove it before passing the request on to the Channel Provider
     *
     * @param channelName the channel name to check for prefix
     * @return the channel name without the prefix if it was present
     */
    private String removeServicePrefixIfPresent(final String channelName) {
        final int servicePrefix = channelName.indexOf("::");
        if (servicePrefix != -1) {
            return channelName.substring(servicePrefix + 2);
        } else {
            return channelName;
        }
    }

    /**
     * If the client has specified the channel with the legacy formatted channel name we need to change it to the new format before
     * passing it to the Channel Provider which will be expecting only new format names
     *
     * @param channelName the channel name
     * @return the channel name in the new format
     */
    private String ensureNewFormatChannelName(String channelName) {
        int indexOfLastLegacySeparator = channelName.lastIndexOf("//");
        if (indexOfLastLegacySeparator != -1) {
            channelName = channelName.substring(0, indexOfLastLegacySeparator) + ":" + channelName.substring(indexOfLastLegacySeparator + 2);
        }
        return channelName;
    }

    /**
     * See if the request type is supported for the channel
     *
     * @param channelName     the channel name
     * @param argumentsList   the list of arguments
     * @param isSetterRequest is this a set/get operation?
     * @param aidaType        the request type
     * @throws UnsupportedChannelException if the requested operation is not supported
     */
    private void checkThatRequestedOperationIsSupported(final String channelName, final List<AidaArgument> argumentsList, final boolean isSetterRequest, final AidaType aidaType) throws UnsupportedChannelException {
        if (aidaType == NONE) {
            throw new UnsupportedChannelTypeException(channelName + argumentsList + ".  " + (isSetterRequest ? "Set" : "Get") + " requests are not supported for this channel");
        }
    }

    /**
     * Check to see if the implementation for this channel is split between two different providers.  If it is then
     * throw an error back to the client informing it to add the prefix and try again.
     *
     * @param channelAlias    the given channel name
     * @param prefix          the configured prefix or null if none is configured
     * @param isSetterRequest true if this request is a for a set operation
     * @throws AidaInternalException if the alias is mis-configured in the config files
     * @throws RPCRequestException   if the request should be prefixed
     */
    private void checkForAliasAndThrowExceptionAppropriately(final String channelAlias, final String prefix, final boolean isSetterRequest, final AidaType aidaType) throws AidaInternalException, RPCRequestException {
        if (aidaType == ALIAS) {
            if (prefix == null) {
                throw new AidaInternalException("Aliased provider defined without configuring target provider's prefix in channel configuration file");
            }

            // Add the prefix to get the real channel name
            final String channelName = prefix + "::" + channelAlias;

            // Log the alias
            logAliasRequest(channelAlias, channelName, isSetterRequest);

            throw new RPCRequestException(ERROR, "Missing prefix for " + (isSetterRequest ? "Set" : "Get") + " request: " + channelAlias + " => " + channelName);
        }
    }

    /**
     * Check that the configuration defines fields for table requests so that we will know how to create the PVStructure to return the result in
     *
     * @param aidaType the request type
     * @param config   the configuration
     * @throws AidaInternalException if the configuration file has been mis-configured
     */
    private void checkThatConfigDefinesFieldsForTableRequests(final AidaType aidaType, final AidaChannelOperationConfig config) throws AidaInternalException {
        if (aidaType == TABLE && (config == null || config.getFields() == null)) {
            throw new AidaInternalException("Table channel configured without defining fields");
        }
    }

    /**
     * Get the {@link AidaType }  of the specified TYPE argument but check that it is compatible with the operation requested on the channel
     *
     * @param channelName     the channel name
     * @param argumentsList   the list of arguments
     * @param isSetterRequest is this a set/get operation?
     * @param aidaType        the request type
     * @param typeArgument    the specified TYPE argument
     * @return the {@link AidaType} of the specified TYPE argument
     * @throws UnsupportedChannelException if not compatible
     */
    private AidaType checkIfTypeArgumentIsCompatible(final String channelName, final List<AidaArgument> argumentsList, final boolean isSetterRequest, final AidaType aidaType, final String typeArgument) throws UnsupportedChannelException {
        final AidaType specifiedAidaType = AidaType.valueOf(typeArgument);

        // Check that it matches the type class from the channels file
        // if channel accepts any type or if the specified type is TABLE (which matches any configuration)
        // or if the specified type is in the class of types allowed by the channel
        if (!isSpecifiedTypeCompatibleWithConfiguredType(specifiedAidaType, aidaType)) {
            throw new UnsupportedChannelTypeException(channelName + argumentsList + ".  The type specified by the 'Type' parameter must be " + (isSetterRequest ? "either VOID or TABLE" : ("a " + aidaType)) + ", but you specified " + specifiedAidaType);
        }

        return specifiedAidaType;
    }

    /**
     * When a TYPE parameter has not been specified, check to see if it was indeed required and raise an exception if so
     *
     * @param channelName     the channel name
     * @param argumentsList   the arguments list
     * @param isSetterRequest is this a set/get request?
     * @param aidaType        the request type
     * @throws UnsupportedChannelException if the TYPE parameter was required but not provided
     */
    private void checkIfMissingTypeArgumentIsMandatory(final String channelName, final List<AidaArgument> argumentsList, final boolean isSetterRequest, final AidaType aidaType) throws UnsupportedChannelException {
        if (isSetterRequest) {  // If a meta-type is set as the setter type, but the client request didn't explicitly specify a TYPE argument then error out
            if (aidaType == ANY) {
                throw new UnsupportedChannelTypeException(channelName + argumentsList + ".  The 'Type' parameter must be set 'VOID' or 'TABLE' but you didn't specify one");
            }
        } else { // If a meta-type is set as the getter type, but the client request didn't explicitly specify a TYPE argument then error out
            switch (aidaType) {
                case SCALAR:
                case SCALAR_ARRAY:
                case ANY:
                    throw new UnsupportedChannelTypeException(channelName + argumentsList + ".  The 'Type' parameter must be set to a type corresponding to " + aidaType + ", but you didn't specify one");
            }
        }
    }

    /**
     * Validate arguments:  Check that the given arguments are valid for the operation on the channel that has been requested
     *
     * @param channelName     the channel
     * @param argumentsList   the list of arguments
     * @param isSetterRequest true if this is a setter request
     * @param config          the configuration
     * @throws UnsupportedChannelException if any argument is not valid
     */
    private void validateArguments(final String channelName, final List<AidaArgument> argumentsList, final boolean isSetterRequest, final AidaChannelOperationConfig config) throws UnsupportedChannelException {
        for (AidaArgument argument : argumentsList) {
            final String argumentName = argument.getName().toUpperCase();

            if (!"TYPE".equalsIgnoreCase(argumentName) && !"VALUE".equalsIgnoreCase(argumentName) && !"TIMEOUT".equalsIgnoreCase(argumentName)) { // Ignore TYPE, VALUE, and TIMEOUT arguments because they are validated elsewhere
                final List<String> arguments = config.getArguments();
                if (arguments == null || !arguments.contains(argumentName)) { // if the given argument name is not in the acceptable set of argument names then error out
                    throw new UnsupportedChannelException(channelName +
                            ":  " + argumentName +
                            " is not a valid argument for " +
                            (isSetterRequest ? "set" : "get") +
                            " requests to this channel.  " +
                            (arguments == null ? "No arguments are allowed" : ("Valid arguments are: " + arguments)));
                }
            }
        }
    }

    /**
     * Display the log entry that indicated the request that is being passed to the
     * Channel Provider with its parameters and its expected return type
     *
     * @param channelName     the channel name
     * @param argumentsList   the arguments
     * @param isSetterRequest is this a set/get operation
     * @param aidaType        the request type
     */
    private void logRequest(final String channelName, final List<AidaArgument> argumentsList, final boolean isSetterRequest, final AidaType aidaType) {
        if (shouldLog()) {
            String normativeType = ntTypeOf(aidaType);
            logger.info("AIDA " + (isSetterRequest ? "SetValue" : "GetValue") + ": " + channelName + argumentsList + " => " + aidaType + (normativeType == null ? "" : ("::" + normativeType)));
        }
    }

    /**
     * Display the log entry that indicates that the request is being delegated
     *
     * @param channelAlias    the channel alias
     * @param channelName     the channel name
     * @param isSetterRequest true if this request is a for a set operation
     */
    private void logAliasRequest(final String channelAlias, final String channelName, final boolean isSetterRequest) {
        if (shouldLog()) {
            logger.warning("AIDA " + (isSetterRequest ? "SetValue" : "GetValue") + ": " + channelAlias + " missing prefix => " + channelName);
        }
    }

    /**
     * Returns true if the metered logging criteria are met.
     *
     * @return true if we can log false otherwise
     */
    private boolean shouldLog() {
        final long now = System.currentTimeMillis();
        final long meteringWindowStart = now - METERED_LOGGING_SAMPLE_WINDOW_MS;

        synchronized (this) {
            // Remove all log events times that are before the log metering window
            for (int i = 0; i < logEventTimesWithinInMeteringWindow.size(); i++) {
                if (logEventTimesWithinInMeteringWindow.get(0) < meteringWindowStart) {
                    logEventTimesWithinInMeteringWindow.remove(0);
                }
            }

            // Add the time of this log event to the log events list
            logEventTimesWithinInMeteringWindow.add(now);

            // Get rate of log events
            final int loggingRate = logEventTimesWithinInMeteringWindow.size();

            // If we don't have to meter the logs, or we've already skipped enough events then return ok
            if (loggingRate <= METERING_TRIGGER || logEventsSkipped >= METERING_SKIP) {
                if (logEventsSkipped != 0) {
                    System.out.println(" ... " + logEventsSkipped + " more");
                }

                logEventsSkipped = 0;
                return true;
            }
            logEventsSkipped++;
            return false;
        }
    }

    /**
     * Get the arguments for the specified request.  Returns the list of AidaArgument for the
     * given Normative Type query PVStructure
     *
     * @param pvUriQuery the given Normative Type query PVStructure
     * @return the list of AidaArgument
     * @throws RPCRequestException if there is a problem reading the arguments
     */
    private List<AidaArgument> getArguments(final PVStructure pvUriQuery) throws RPCRequestException {
        final List<AidaArgument> arguments = new ArrayList<AidaArgument>();
        if (pvUriQuery != null) {
            final PVField[] pvFields = pvUriQuery.getPVFields();
            for (final PVField field : pvFields) {
                arguments.add(getArgument(field));
            }
        }
        return arguments;
    }

    /**
     * Convert a PVField into an AidaArgument
     *
     * @param field the PVField to convert into an AidaArgument
     * @return the AidaArgument
     * @throws RPCRequestException if something bad happens
     */
    private AidaArgument getArgument(final PVField field) throws RPCRequestException {
        final String name = field.getFieldName();
        if (name == null) {
            throw new RPCRequestException(ERROR, "Invalid argument name: <blank>");
        }

        // To store list of all floating point numbers found in this argument.  They are
        // lists because the arguments can be structures that contain many floating points
        // in various elements.  When extracting those elements the full json path to
        // the element in the structure is saved
        final List<FloatArgument> floatArgumentList = new ArrayList<FloatArgument>();
        final List<DoubleArgument> doubleArgumentList = new ArrayList<DoubleArgument>();

        // Convert all arguments to string (json or otherwise)
        // But collect all doubles and floats unchanged in ieee format
        final String value = fieldToString(field, name, floatArgumentList, doubleArgumentList);

        if (value == null) {
            throw new RPCRequestException(ERROR, "Invalid argument value: <blank>");
        }

        // json parser in AIDA-PVA Module in AIDA_PVALIB is a bit flaky and crashes when it does not
        // get properly formed json, so we'll parse it here to make sure it's ok
        final String trimmedValue = value.trim();
        if (trimmedValue.startsWith("[")) {
            try {
                new JSONObject("{\"_array\": " + trimmedValue + "}");
            } catch (JSONException e) {
                throw new RPCRequestException(ERROR, "Invalid json detected: argument name: " + name + ", value: " + value);
            }
            checkForLongsTerminatedByL(name, value, trimmedValue);
        } else if (trimmedValue.startsWith("{")) {
            try {
                new JSONObject(trimmedValue);
            } catch (JSONException e) {
                throw new RPCRequestException(ERROR, "Invalid json detected: argument name: " + name + ", value: " + value);
            }
            checkForLongsTerminatedByL(name, value, trimmedValue);
        }

        return new AidaArgument(name, value, floatArgumentList, doubleArgumentList);
    }

    /**
     * Simple Check for json containing literal long numbers with a suffix of "L" these are too much for the
     * simple json parser on the C side of the JNI interface
     *
     * @param name         argument name
     * @param value        argument value
     * @param trimmedValue trimmed argument value
     * @throws RPCRequestException if contains an invalid literal long
     */
    private void checkForLongsTerminatedByL(final String name, final String value, final String trimmedValue) throws RPCRequestException {
        if (Pattern.compile(".*[0-9]+L.*").matcher(trimmedValue).matches()) {
            throw new RPCRequestException(ERROR, "JSON Long values cannot be terminated by 'L' with this json parser: argument name: " + name + ", value: " + value);
        }
    }

    /**
     * Make a request string that uniquely identifies a request by PV name and argument list.  Requests identified in
     * this way can safely wait on the first request that arrives and return the same result when it is done executing.
     *
     * @param channelName the name of the channel PV
     * @param arguments   the arguments to the rpc request
     * @return the request string that uniquely identifies the request by PV name and argument list
     */
    private String makeRequestString(final String channelName, final List<AidaArgument> arguments) {
        // Further optimise requests by:
        //    recognising duplicate requests and rather than making the request multiple times, simply waiting for
        //    the first one to finish and return the same result.  This will allow the request queue to be reduced more rapidly,
        //    therefore providing more capacity.
        StringBuilder requestStringBuilder = new StringBuilder(channelName.toLowerCase());
        for (final AidaArgument argument : arguments) {
            requestStringBuilder.append(",").append(argument.getName().toLowerCase()).append("=").append(argument.getValue().toLowerCase());
        }
        return requestStringBuilder.toString();
    }
}

