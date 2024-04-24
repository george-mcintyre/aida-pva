package edu.stanford.slac.aida.lib.util;

import lombok.Getter;
import lombok.Setter;
import org.epics.pvdata.pv.PVStructure;

import java.util.concurrent.ConcurrentMap;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * User: george
 */
@Getter
public class AtomicResult {
    /**
     * Maximum amount of time to wait for a prior request to finish before failing
     */
    private static final Integer REQUEST_TIMEOUT_MS = 50000;   // 50 seconds
    private static final Integer DELIVERY_TIMEOUT_MS = 50000;  // 50 seconds

    /**
     * The result that we're waiting for
     */
    @Setter
    PVStructure result;
    /**
     * The number of threads waiting on this result
     */
    final AtomicInteger refCount = new AtomicInteger(0);
    /**
     * Triggered when the result is available
     */
    final CountDownLatch resultAvailableLatch = new CountDownLatch(1);
    /**
     * Triggered when the result has been delivered to all subscribers
     */
    final CountDownLatch resultsDeliveredLatch = new CountDownLatch(1);

    /**
     * Notify all listening threads waiting for the same result that the results are ready
     */
    final public void notifyListeners() {
        synchronized (refCount) { // No new references while returning results
            resultAvailableLatch.countDown();
        }
    }

    /**
     * Is the result available
     *
     * @return True if results are available for this atomic result
     */
    public final boolean resultAvailable() {
        return resultAvailableLatch.getCount() == 0;
    }

    /**
     * Call this to wait for results to become available if you are a concurrent listener thread that is waiting for the
     * same result as a prior request thread that is requesting the same thing but hasn't returned the result yet.
     *
     * @return true if results are now available
     * @throws InterruptedException if timeout before results are available or process interrupted.
     */
    final public boolean waitForResult() throws InterruptedException {
        refCount.incrementAndGet();
        return resultAvailableLatch.await(REQUEST_TIMEOUT_MS, TimeUnit.MILLISECONDS);
    }

    /**
     * Is the caller the last thread subscribing to this result?
     *
     * @return true if the caller is the last subscriber to this result
     */
    final public boolean isLastSubscriber() {
        return refCount.decrementAndGet() <= 0;
    }

    /**
     * Are there any threads subscribing to this result?
     *
     * @return true if there are subscriber for this result
     */
    final public boolean hasSubscribers() {
        return refCount.get() != 0;
    }

    /**
     * Call this once the last concurrent listener for this result has consumed it.
     * At this point we will remove the atomic result from the map so that subsequent requests
     * will create a new mapping.
     * This will allow threads that are waiting to submit new requests with the same parameters
     * to go ahead and submit them.
     *
     * @param map the request/result map that contains the request key that we're waiting for delivery of results from
     * @param key the key (requestString) that we're waiting for the result to be fully delivered from
     */
    final public void deliveryComplete(final ConcurrentMap<String, AtomicResult> map, final String key) {
        // Remove the request string if last reference
        map.remove(key);
        resultsDeliveredLatch.countDown();
    }

    /**
     * Call when we realise that we want to submit a request that is identical to a prior request that has completed
     * but its result has not yet been delivered to all concurrent threads that are waiting for it.
     * We need to wait till they have been delivered then create a new request/result mapping
     *
     * @return true if all results have been delivered
     * @throws InterruptedException if timeout before results are delivered or process interrupted.
     */
    final public boolean waitForResultsToBeDelivered() throws InterruptedException {
        return resultsDeliveredLatch.await(DELIVERY_TIMEOUT_MS, TimeUnit.MILLISECONDS);
    }
}
