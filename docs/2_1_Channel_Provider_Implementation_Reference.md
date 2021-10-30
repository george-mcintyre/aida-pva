# 2.1 Channel Provider Implementation Reference
## Overview

There are three things to write before you can compile, run, test, and deploy your service.

* Write a `CHANNELS.YML` file.
* Create your Channel Provider source file - one C file and optionally one header file.
* Create some tests using the test framework.

## Creating a CHANNELS.YML file

Definition of the channels supported by your Channel Service Provider is done in the `CHANNELS.YML` file. If you're
unfamiliar with the YAML (Yet Another Markup Language) format you
can [familiarise yourself with the syntax and format](https://www.redhat.com/sysadmin/yaml-beginners) before reading
further.

Please read [documentation on the CHANNELS.YML](2_3_CHANNELS_YML_file.md) file for information on how to create one. An
example configuration file is shown below:

```yaml
!!edu.stanford.slac.aida.lib.model.AidaProvider
# Sample Channel Provider
id: 42
name: Channel Provider
description: Your Channel Provider
configurations:
  - name: Boolean Getter Channels
    getterConfig:
      type: BOOLEAN
    channels:
      - AIDA:CHAN:*:INT
      - AIDA:CHAN:P01:BOOL
  - name: Float Getter Channels
    getterConfig:
      type: FLOAT
      arguments:
        - X
        - Y
    channels:
      - AIDA:CHAN:???:FLT
  - name: String Array Getter Channels
    getterConfig:
      type: STRING_ARRAY
    channels:
      - AIDA:CHAN:P01:STRA
  - name: Table Getter Channels
    getterConfig:
      type: TABLE
      fields:
        - name: isActive
          label: "Device is active?"
          description: "Device activity status.  Active if true"
        - name: mode
          label: "Device Mode Code"
          description: "Device mode code"
    channels:
      - AIDA:CHAN:P01:TABL
  - name: Void Setter Channels
    setterConfig:
      type: VOID
    channels:
      - AIDA:CHAN:S01:VOID
  - name: Table Setter Channels
    setterConfig:
      type: TABLE
      fields:
        - name: status
          label: "Result of setting value"
          description: "True if the value was set successfully"
    channels:
      - AIDA:CHAN:S01:DEFA
      - AIDA:CHAN:S01:TABL
```

## Writing your Channel Provider
There is only one file to write.  You have to implement all the endpoints defined in aida_pva_api.h.  

- Initialisation
    - aidaServiceInit() - _Called by the AIDA-PVA SERVICE on startup to initialise the Channel Provider._
- Scalar Getters
    - aidaRequestBoolean() - _Called by the AIDA-PVA SERVICE when a boolean value is requested._
    - aidaRequestByte() - _Called by the AIDA-PVA SERVICE when a byte value is requested._
    - aidaRequestShort() - _Called by the AIDA-PVA SERVICE when a short value is requested._
    - aidaRequestInteger() - _Called by the AIDA-PVA SERVICE when a integer value is requested._
    - aidaRequestLong() - _Called by the AIDA-PVA SERVICE when a long value is requested._
    - aidaRequestFloat() - _Called by the AIDA-PVA SERVICE when a float value is requested._
    - aidaRequestDouble() - _Called by the AIDA-PVA SERVICE when a double value is requested._
    - aidaRequestString() - _Called by the AIDA-PVA SERVICE when a string value is requested._
- Scalar Array Getters
    - aidaRequestBooleanArray() - _Called by the AIDA-PVA SERVICE when a boolean array is requested._
    - aidaRequestByteArray() - _Called by the AIDA-PVA SERVICE when a byte array is requested._
    - aidaRequestShortArray() - _Called by the AIDA-PVA SERVICE when a short array is requested._
    - aidaRequestIntegerArray() - _Called by the AIDA-PVA SERVICE when a integer array is requested._
    - aidaRequestLongArray() - _Called by the AIDA-PVA SERVICE when a long array is requested._
    - aidaRequestFloatArray() - _Called by the AIDA-PVA SERVICE when a float array is requested._
    - aidaRequestDoubleArray() - _Called by the AIDA-PVA SERVICE when a double array is requested._
    - aidaRequestStringArray() - _Called by the AIDA-PVA SERVICE when a string array is requested._
    - aidaRequestTable() - _Called by the AIDA-PVA SERVICE when a table of data is requested._
- Setters
    - aidaSetValue() - _Called by the AIDA-PVA SERVICE when a request to set a value is made._
    - aidaSetValueWithResponse() - _Called by the AIDA-PVA SERVICE when a request to set a value and return a table is
      made._

Thanks to the AIDA-PVA Module doing this is easy. 

For all the endpoints that you will not implement simply include the appropriate STUB Macro. 
aida_pva_api.h defines the STUB Macros.

- Scalar Endpoints
  - REQUEST_STUB_BOOLEAN
  - REQUEST_STUB_BYTE
  - REQUEST_STUB_SHORT
  - REQUEST_STUB_INTEGER
  - REQUEST_STUB_LONG
  - REQUEST_STUB_FLOAT
  - REQUEST_STUB_DOUBLE
  - REQUEST_STUB_STRING
- Scalar Array Endpoints
  - REQUEST_STUB_BOOLEAN_ARRAY
  - REQUEST_STUB_BYTE_ARRAY
  - REQUEST_STUB_SHORT_ARRAY
  - REQUEST_STUB_INTEGER_ARRAY
  - REQUEST_STUB_LONG_ARRAY
  - REQUEST_STUB_FLOAT_ARRAY
  - REQUEST_STUB_DOUBLE_ARRAY
  - REQUEST_STUB_STRING_ARRAY
- Table Requests
  - REQUEST_STUB_TABLE
- Setter Endpoints
  - SET_STUB_VOID
  - SET_STUB_TABLE
  
### Example Channel Provider with all endpoints stubbed out
```c
#include "aida_pva.h"

// API Stubs
REQUEST_STUB_BOOLEAN
REQUEST_STUB_BYTE
REQUEST_STUB_SHORT
REQUEST_STUB_INTEGER
REQUEST_STUB_LONG
REQUEST_STUB_FLOAT
REQUEST_STUB_DOUBLE
REQUEST_STUB_STRING
REQUEST_STUB_BOOLEAN_ARRAY
REQUEST_STUB_BYTE_ARRAY
REQUEST_STUB_SHORT_ARRAY
REQUEST_STUB_INTEGER_ARRAY
REQUEST_STUB_LONG_ARRAY
REQUEST_STUB_FLOAT_ARRAY
REQUEST_STUB_DOUBLE_ARRAY
REQUEST_STUB_STRING_ARRAY
REQUEST_STUB_TABLE
SET_STUB_VOID
SET_STUB_TABLE

/**
 * Initialise the service
 * @param env to be used to throw exceptions using aidaThrow() and aidaNonOsExceptionThrow()
 * @throws ServerInitialisationException if the service fails to initialise
 */
void aidaServiceInit(JNIEnv* env)
{
	printf("My Service Provider Initialised\n");
}
```

For any of the types that your Channel Provider will support you need to remove the corresponding STUB Macro and
replace it with the implementation.

#### aidaRequestInteger() example
This example calls a legacy AIDA Module function -  JNI_DBGETINT() - in `AIDASHR`, to get an integer, and return it.

```c
int aidaRequestInteger(JNIEnv* env, const char* uri, Arguments arguments)
{
	int val = 0;
	TO_SLC_NAME(uri, slcName)
	vmsstat_t status = JNI_DBGETINT(slcName, &val);

	if (!SUCCESS(status)) {
		aidaThrow(env, status, UNABLE_TO_GET_DATA_EXCEPTION, "getting SLC db integer device data");
	}

	return val;
}
```

### AIDA-PVA Module

The AIDA-PVA Module is a module contained in the `STANDALONELIB` library that provides all the boilerplate functionality
needed to respond to **get** and **set** requests, marshal and unmarshal objects, and simple types across the JNI
boundary and between your Channel Provider, VMS, and any AIDA Module in `AIDASHR` that you need.

It also includes header files from `C_INC` that define its functions and many typedefs, MACROS, enumerations, and unions that 
you will need to implement your Channel Provider.

@see [AIDA-PVA Module Functions documentation](2_5_1_AIDA-PVA_Module_Functions.md) for more information on the available
functions. 

@see [AIDA-PVA Module Macros documentation](2_5_2_AIDA-PVA_Module_Macros.md) for more information on MACROS
you can leverage. 

@see [AIDA-PVA Module Types documentation](2_5_3_AIDA-PVA_Module_Types.md) for more information available
types, enumerations, and unions.

## Create some tests using the test framework 
The test framework consists of a number of classes that can help you build simple tests for your provider.
  - edu.stanford.slac.aida.test.utils.AidaPvaTestUtils - Utility class to facilitate running all the AIDA-PVA tests 
  - edu.stanford.slac.aida.test.utils.AidaPvaRequest  - Class to create and execute AIDA-PVA requests
  - edu.stanford.slac.aida.test.utils.AidaType - Enumerated type representing an AIDA-PVA data type 

1. First create your main
```java
package edu.stanford.slac.aida.test;
 
import edu.stanford.slac.aida.test.utils.AidaPvaTestUtils;
 
import java.util.Arrays;
 
public class MyProviderTest {

  public static void main(String[] args) {
    var argString = Arrays.toString(args).replace("]", ",").replace("[", " ");
    AidaPvaTestUtils.NO_COLOR_FLAG = !argString.contains("-c") && !argString.contains("-color");
    var allTests = (AidaPvaTestUtils.NO_COLOR_FLAG ? args.length == 0 : args.length == 1);
    var testId = 0;
    
    // Test Suites Go Here ...
    
    // Because of threads started in background to process requests
    System.exit(0);
  }
}
```
2. Add your first test suite
```java
    AidaPvaTestUtils.testSuiteHeader("My Channel Provider Test Suite");
```
3. Add a test
```java
    AidaPvaTestUtils.testHeader(testId, "My First Test");
```
4. Then add as many test cases as you want
```java
    AidaPvaTestUtils.getWithNoArguments("MYCHANNEL:NAME", AidaType.FLOAT, "Check Float Return with no parameter");
    AidaPvaTestUtils.channel("MYCHANNEL:NAME", "Check Integer Return with X parameter").with("X", 55).get();
```
5. To add tests with lists do the following
```java
     AidaPvaTestUtils.channel("MYCHANNEL:", "BPM Values")
        .with("BPMD", 57)
        .with("NRPOS", 180)
        .with("BPMS", Arrays.asList(
                "BPMS:LI11:501",
                "BPMS:LI11:601",
                "BPMS:LI11:701",
                "BPMS:LI11:801"))
        .get();
```
6. To add tests with complex structures do the following
```java
    var value = "{" +
            "\"names\": [\"XCOR:LI31:41\"]," +
            "\"values\": [4.0]" +
            "}";
    AidaPvaTestUtils.channel("MAGNETSET:BDES", "Magnet Set")
            .with("MAGFUNC", "TRIM")
            .set(value);
```
7. If you expect the tests to fail use the following patterns
```java
    AidaPvaTestUtils.channel("KLYS:LI31:31:TACT", "PDES")
      .with("BEAM", "XYZ")
      .with("DGRP", "DEV_DGRP")
      .setAndExpectFailure(1);

    AidaPvaTestUtils.channel("KLYS:LI31:31:TACT", "PDES")
      .with("BEAM", 1)
      .with("DGRP", "LIN_KLYS")
      .returning(AidaType.SHORT)
      .getAndExpectFailure();
```

## Building and Deploying your Channel Provider

Follow instructions [here](3_3_Building_AIDA_PVA_Channel_Provider.md) to build your AIDA-PVA Channel Provider.

## Reference Service
 
AIDA-PVA provides a reference service implementation. You can explore this code to help you understand how to implement
your Service Provider. The code is available [here](https://github.com/slaclab/aida-pva/tree/master/src/cpp/providers/Reference).

