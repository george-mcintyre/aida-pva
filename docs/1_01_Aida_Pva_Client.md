# 1.1 - Aida-pva-client
## Connecting to AIDA-PVA Channel Providers
AIDA-PVA clients can use various methods to connect to the supported EPICs Channel Providers.  These include: 
- EasyPVA - see: [EasyPVA Documentation](http://epics-pvdata.sourceforge.net/docbuild/easyPVAJava/tip/documentation/easyPVA.html).  EasyPVA is end of life and is no longer supported.
- PvaClient - see: [PvaClient documentation](https://github.com/epics-base/pvaClientJava).  PvaClient does not propagate server errors back to the client.
- Plain PvAccess - see: [AIDA-PVA User Guide](1_00_User_Guide.md) for an example.  Is quite verbose.
- aida-pva-client - a purpose built replacement for the legacy AIDA client.  Lightweight and full functionality.

## aida-pva-client
aida-pva-client is delivered in the aida-pva-client jar.  This jar includes everything 
your client application needs to access any AIDA-PVA and compatible EPICs Channel Provider.

Behind the scenes it is implemented with plain PvAccess, PvaClient, or EasyPVA.  The implementation may be selected as follows:
- default implementation is plain PvAccess.
- if a property `aida.pva.client.request.executor` is set it overrides the default as follows:
  - **PvAccess** - Use plain PvAccess
  - **PvaClient** - Use PvaClient implementation
  - **EasyPVA** - Use EasyPVA implementation
- if an environment variable `AIDA_PVA_CLIENT_REQUEST_EXECUTOR` is set it overrides the default or any property as follows:
  - **PvAccess** - Use plain PvAccess
  - **PvaClient** - Use PvaClient implementation
  - **EasyPVA** - Use EasyPVA implementation

@note EasyPVA maven repository is unsupported and so it does not compile.  The implementation has therefore been commented out.

### Imports
These are the only imports you need.

```java
import org.epics.pvaccess.server.rpc.RPCRequestException;
import static edu.stanford.slac.aida.client.AidaPvaClientUtils.*;
import static edu.stanford.slac.aida.client.AidaType.*;

```

### Types
#### AidaType
This encapsulates all the types that a client application can use to access AIDA-PVA Channel 
Providers.  
* Scalar types
    * `AIDA_BOOLEAN`            to return a java Boolean
    * `AIDA_BYTE`               to return a java Byte
    * `AIDA_CHAR`               to return a java Char
    * `AIDA_SHORT`              to return a java Short
    * `AIDA_INTEGER`            to return a java Integer
    * `AIDA_LONG`               to return a java Long
    * `AIDA_FLOAT`              to return a java Float
    * `AIDA_DOUBLE`             to return a java Double
    * `AIDA_STRING`             to return a java String
* Scalar array types
    * `AIDA_BOOLEAN_ARRAY`      to return a java List<Boolean>
    * `AIDA_BYTE_ARRAY`         to return a java List<Byte>
    * `AIDA_CHAR_ARRAY`         to return a java List<Byte> - once character strings
    * `AIDA_SHORT_ARRAY`        to return a java List<Short>
    * `AIDA_INTEGER_ARRAY`      to return an java List<Integer>
    * `AIDA_LONG_ARRAY`         to return a java List<Long>
    * `AIDA_FLOAT_ARRAY`        to return a java List<Float>
    * `AIDA_DOUBLE_ARRAY`       to return a java List<Double>
    * `AIDA_STRING_ARRAY`       to return a java List<String>
* Tables
    * `AIDA_TABLE`              to return a table : PvaTable

### API
#### Synopsis

    pvaRequest(channel) [.with(name, value) ...] [ .returning(type) ] [ .timeout(timeout) ] [ .get() | .set(value) ]
    pvaGet(channel [, type])
    pvaSet(channel , value)
    pvaUnpack(result)

- **pvaRequest**(`channel`) - creates a request builder for the specified channel.
  - **with**(`name`, `value`) - Used to set argument called `name` to `value`, on a request
  - **returning**(`type`) - Used to set the return `type` for a request.  This is equivalent to setting the `TYPE` argument.
  - **get**()** - To execute the request and return the results
  - **set**(`value`) - To execute the request setting the `value` and returning nothing or a table
  - **timeout**() - To set the timeout for the request.
  - **uri**() - returns an NTURI for the builder.
- **pvaGet**(`channel` [, `type`]) - Executes a simple get on a channel specifying an optional type for the return.
- **pvaSet**(`channel`, `value`) - Executes a simple set of a channel to the given value.
- **pvaUnpack**(`result`) - unpacks a PVStructure result into a Scalar Object, Scalar Object array or PvaTable

##### e.g. 1: Simple get
```java
   Float bact = pvaRequest("XCOR:LI03:120:LEFF").returning(FLOAT).get();
```

##### e.g. 2: Multiple arguments
```java
   PvaTable table = pvaRequest("NDRFACET:BUFFACQ")
     .with("BPMD", 57)
     .with("NRPOS", 180)
     .with("BPMS", List.of(
             "BPMS:LI11:501",
             "BPMS:LI11:601",
             "BPMS:LI11:701",
             "BPMS:LI11:801"))
     .timeout(10.0)
     .get();
   String firstName = table.getValues().get("name").get(0)
```

##### e.g. 3: Simple set

```java
   pvaSet("XCOR:LI31:41:BCON", 5.0f);
```

##### e.g. 4: Alternative simple set

```java
   pvaRequest("XCOR:LI31:41:BCON").set(5.0f);
```

##### e.g. 5: Advanced set
```java
   Short status = ((PvaTable)request("KLYS:LI31:31:TACT")
     .with("BEAM", 8)
     .with("DGRP", "DEV_DGRP")
     .set(0)
     ).getValues().get("status").get(0);
```

##### e.g. 6: Selecting the return value type

```java
  String value = request("KLYS:LI31:31:TACT")
     .with("BEAM", 8)
     .with("DGRP", "DEV_DGRP")
     .returning(STRING)
     .get();
```

#### getRequest()
- **getRequest**(`channel`, `type`) - For very simple get requests that don't take any arguments, use getRequest().

##### e.g. 1: Get a simple value
```java
  Integer response = (Integer) getRequest("XCOR:LI31:41:BCON", INTEGER);
```

#### setRequest()
- **setRequest**(`channel`, `value`) - For very simple set requests that take only a value argument and don't return anything, use setRequest()

##### e.g. 1: Set a simple value
```java
  setRequest("XCOR:LI31:41:BCON", 5.0);
```
