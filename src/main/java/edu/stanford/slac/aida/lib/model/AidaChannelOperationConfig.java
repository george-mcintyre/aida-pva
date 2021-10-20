package edu.stanford.slac.aida.lib.model;

import lombok.Data;
import lombok.NoArgsConstructor;
import lombok.NonNull;

import java.util.List;

import static edu.stanford.slac.aida.lib.model.AidaType.STRING;

/**
 * @file
 * This model class represents a channel operation's configuration.  A channel can have
 * support `get` operations and/or `set` operations.  This class represents the configuration for
 * any one of these operations.  A channel, therefore, can have up to two {@link edu.stanford.slac.aida.lib.model.AidaChannelOperationConfig}s.
 *
 * This class encapsulates an {@link AidaChannelOperationConfig}.
 * The {@link AidaChannelOperationConfig} class controls how the AIDA-PVA Provider responds to a channel `get` or `set` request.
 * There is a different {@link AidaChannelOperationConfig} for `get` requests and `set` requests.
 * <p>
 * All `set` requests are differentiated by containing a `VALUE` {@link AidaArgument} - that is one with the name = VALUE.
 * <p>
 * The default {@link AidaChannelOperationConfig} for `set` requests is NONE meaning that no `set` are allowed.
 * To enable `set` requests an {@link AidaChannelOperationConfig} of {@link AidaChannelOperationConfig#type} = {@link AidaType#NONE},
 * or {@link AidaChannelOperationConfig#type} = {@link AidaType#TABLE} must be set.
 * <p>
 * @note
 * It uses the `@Data` annotation to provide all the getters and setters,
 * a constructor with all required arguments,
 * and an equals(), hashcode() and toString()  method.
 * @note
 * It also uses the `@NoArgsConstructor` annotation to provide a constructor
 * with no arguments.
 */
@Data
@NoArgsConstructor
public class AidaChannelOperationConfig {
    /**
     * - If the the {@link AidaChannel#getterConfig}'s {@link AidaChannelOperationConfig#type} is set to {@link AidaType#ANY} then it uses
     * the supplied `TYPE` parameter in `get` requests to determine which end point to send
     * requests to.
     * e.g. if a `get` request contains an argument `TYPE`=`SHORT` then requests are sent to
     * {@link slac.aida.NativeChannelProvider#aidaRequestShort(String, AidaArguments)}.
     * <p>
     * - If the the {@link AidaChannel#getterConfig}'s {@link AidaChannelOperationConfig#type} is set to {@link AidaType#SCALAR} then it uses
     * the supplied `TYPE` parameter in `get` requests to determine which end point to send
     * requests to similarly to the first example, but the `TYPE` parameter is constrained to be any Scalar
     * {@link AidaType} or {@link AidaType#TABLE}.
     * <p>
     * - If the the {@link AidaChannel#getterConfig}'s {@link AidaChannelOperationConfig#type} is set to {@link AidaType#SCALAR_ARRAY} then it uses
     * the supplied `TYPE` parameter in `get` requests to determine which end point to send
     * requests to similarly to the first example, but the `TYPE` parameter is constrained to be any Scalar Array
     * {@link AidaType} or {@link AidaType#TABLE}.
     * <p>
     * - The the {@link AidaChannel#setterConfig}'s {@link AidaChannelOperationConfig#type} can only be set to
     * {@link AidaType#NONE}, {@link AidaType#VOID}, {@link AidaType#TABLE}, or {@link AidaType#ANY},
     * <p>
     * - If the the {@link AidaChannel#setterConfig}'s {@link AidaChannelOperationConfig#type} is set to {@link AidaType#ANY} then it uses
     * the supplied `TYPE` parameter in `set` requests to determine which end point to send
     * requests to noting that the `TYPE` parameter will be constrained to `VOID` or `TABLE`.
     * e.g. if a `set` request contains an argument `TYPE`=`TABLE` then requests are sent to
     * {@link slac.aida.NativeChannelProvider#aidaSetValueWithResponse(String, AidaArguments)} endpoint.
     * <p>
     * - If the {@link AidaChannel#getterConfig}'s {@link AidaChannelOperationConfig#type} is set to {@link AidaType#NONE} then no `get` requests are supported for the channel
     * <p>
     * - If the {@link AidaChannel#setterConfig}'s {@link AidaChannelOperationConfig#type} is set to {@link AidaType#NONE} then no `set` requests are supported for the channel
     */
    private @NonNull AidaType type;

    /**
     * Stores the list of valid arguments for the `get` or `set` requests that this {@link AidaChannelOperationConfig} is configuring.
     * Any argument that is delivered with the request but is not in this list is trapped by the framework
     * and reported as an error
     */
    private List<String> arguments;

    /**
     * - If the {@link AidaChannelOperationConfig#type} is set to {@link AidaType#TABLE},
     * or if a `TYPE` argument is specified with value = `TABLE` then {@link AidaChannelOperationConfig#fields} contains the
     * configuration of the fields that will be in the returned structure.
     *
     * Note that the Channel Provider may only return tables of a single configuration for any one channel.
     * The table configuration cannot change with different parameters.
     *
     * The type of data returned in a column in a table MAY change based on parameters because the
     * table configuration is type agnostic!
     *
     */
    private List<AidaField> fields;

    /**
     * To set type from a string
     *
     * @param stringType type string
     */
    public void setType(String stringType) {
        if (stringType == null) {
            this.type = STRING;
        } else {
            this.type = AidaType.valueOf(stringType);
        }
    }
}
