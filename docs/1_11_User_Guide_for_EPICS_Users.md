# 1.11 - User Guide for EPICS Users

## Overview

AIDA-PVA is a system for providing programmatic access to SLC Control System devices, data and services. Since its
introduction in 2002 it has been used in other laboratories in the US and around the world. It provides a naming
convention to address all devices and their attributes, a way of specifying arguments for remote procedure calls and
obtaining results in any format, including rich formats with including metadata. Finally, it provides security, logging
and reliability to allow it to be used in a variety of applications from logging, and monitoring, to modeling and
control.

AIDA-PVA has recently been upgraded to use EPICS-7's PVAccess mechanism to transport requests to the Data Providers.

## Components

AIDA-PVA uses EPICS as shown below.

![EPICS in AIDA-PVA](images/aida-pva-system-components.png)

## Some basics

- AIDA-PVA uses PVAccess but only the RPC protocol. So other protocols won't work with AIDA-PVA.
- AIDA-PVA uses [Normative Types](2_2_Normative_Types.md) to encapsulate both data and requests. The types used are:
    - `NTTable` - to return tabular data from RPC requests
    - `NTScalar` - to return scalar data from RPC requests
    - `NTScalarArray` - to return scalar arrays from RPC requests
    - `NTUri` - to create requests to AIDA-PVA Channel Providers

## What works the same way

- Environment variables - all EPICS environment variables work the same way as they do in EPICS. The most important are:
    - `EPICS_PVA_ADDR_LIST` - this is the list of addresses to search for Channel Providers. Set it
      to `mccdev.slac.stanford.edu` for testing against the development environment.
    - `EPICS_PVA_AUTO_ADDR_LIST` - set this to `FALSE` so that EPICS won't try to automatically create your address list
- The channel addressing scheme does not change. All of the implemented Channel Providers are fully compatible with the
  channel addressing scheme.

## What's a little bit different

- Disambiguation - some channels in the SLAC namespace are supported by more than one Channel Provider. So to allow
  clients have coherent and reliable results disambiguation is achieved by prefixing the channel name with the service
  provider short code and two colons. e.g. `SLC::KLYS:LI31:31:PDES`
  @note In the case of a name clash a provider will automatically forward calls it receives for another provider so that the client seamlessly receives data from the correct provider.
- EPICS **RPC** does not have the concept of setting a value, normally that is reserved for **put**. In AIDA-PVA we've
  added the "set-value" semantic to RPC by using the `VALUE` argument. In AIDA-PVA the `VALUE` argument is a reserved
  word and any request that has the `VALUE` argument is treated as a setter.
```java
import org.epics.pvaccess.ClientFactory;
import org.epics.pvaccess.client.rpc.RPCClientImpl;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvdata.pv.*;
 
public class AidaPvaRunner {
    public static void main(String[] args) {
        RPCClientImpl client = new RPCClientImpl("CHANNEL");
        
        // ...
      
        // Only do this if the request is to set a value
        PVStructure query = request.getStructureField("query");
        ((PVString) (query.getSubField("VALUE"))).put("A Value");
        
        // Execute the query with a timeout of 3 seconds
        try {
            PVStructure result = client.request(request, 3.0);
 
            // Use result ...
        } catch (RPCRequestException e) {
            // Do something with error
        } finally {
          client.destroy();
        }
    }
}
```
- EPICS **RPC** does not have the concept of returning different types from the same channel. Normally a channel is
  associated wit a static return type.  In AIDA-PVA the `TYPE` argument is a reserved word and any request that has
  the `TYPE` argument is specifying the return type of the request. Additionally, some Channel Providers
  implement `TABLE_TYPE`, which is also a reserved word, to specify the desired return-type of a single column table
  request.  
```java
// Set the request query return type
PVStructure query = request.getStructureField("query");
((PVString) (query.getSubField("TYPE"))).put("FLOAT");

// Execute the query with a timeout of 3 seconds
try {
    PVStructure result = client.request(request, 3.0);

    // Use result ...
} catch (RPCRequestException e) {
    // Do something with error
}
```
