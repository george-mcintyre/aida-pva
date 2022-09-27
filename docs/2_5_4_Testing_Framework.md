# 2.5.4 - Testing Framework
The test framework is found in the [AIDA-PVA test git repository](https://github.com/slaclab/aida-pva-tests).

## Summary
The test framework consists of a number of classes that can help you build simple tests for your Channel Provider.
  - edu.stanford.slac.aida.test.utils.AidaPvaTestUtils - Utility class to facilitate running all the AIDA-PVA tests 
  - edu.stanford.slac.aida.test.utils.AidaPvaRequest  - Class to create and execute AIDA-PVA requests
  - edu.stanford.slac.aida.test.utils.AidaType - Enumerated type representing an AIDA-PVA data type 

## Creating Tests
For developers of new Providers this repository is where you'll write tests for functional verification of your new provider before it is released into production.

1. First create your main
```java
package edu.stanford.slac.aida.test;
 
import edu.stanford.slac.aida.test.utils.AidaPvaTestUtils.*;
 
import java.util.Arrays;
 
public class MyProviderTest {

  public static void main(String[] args) {
    var argString = Arrays.toString(args).replace("]", ",").replace("[", " ");
    NO_COLOR_FLAG = !argString.contains("-c") && !argString.contains("-color");
    var allTests = (NO_COLOR_FLAG ? args.length == 0 : args.length == 1);
    var testId = 0;
    
    // Test Suites Go Here ...
    
    // Because of threads started in background to process requests
    System.exit(0);
  }
}
```
2. Add your first test suite
```java
    testSuiteHeader("My Channel Provider Test Suite");
```
3. Add a test
```java
    testHeader(testId, "My First Test");
```
4. Then add as many test cases as you want
```java
    getRequest("MYCHANNEL:NAME", AidaType.FLOAT, "Check Float Return with no parameter");
        
    request("MYCHANNEL:NAME", "Check Integer Return with X parameter").with("X", 55).get();
```
5. To add tests with lists do the following
```java
     request("MYCHANNEL:", "BPM Values")
        .with("BPMD", 57)
        .with("NRPOS", 180)
        .with("BPMS", List.of(
                "BPMS:LI11:501",
                "BPMS:LI11:601",
                "BPMS:LI11:701",
                "BPMS:LI11:801"))
        .get();
```
6. To add tests with complex structures do the following
```java
    request("MAGNETSET:BDES", "Magnet Set")
            .with("MAGFUNC", "TRIM")
            .set(Map.of(
                    "names", List.of("XCOR:LI31:41"),
                    "values", List.of(4.0f)
            ));
```
7. To add tests with JSON do the following
```java
    var value = "{" +
            "\"names\": [\"XCOR:LI31:41\"]," +
            "\"values\": [4.0]" +
            "}";
    request("MAGNETSET:BDES", "Magnet Set")
            .with("MAGFUNC", "TRIM")
            .set(value);
```
8. If you expect the tests to fail use the following patterns
```java
    request("KLYS:LI31:31:TACT", "PDES")
      .with("BEAM", "XYZ")
      .with("DGRP", "DEV_DGRP")
      .setAndExpectFailure(1);

    request("KLYS:LI31:31:TACT", "PDES")
      .with("BEAM", 1)
      .with("DGRP", "LIN_KLYS")
      .returning(AidaType.SHORT)
      .getAndExpectFailure();
```

## More test examples
### e.g. 1: Simple get

```java
     testSuiteHeader("AIDA-PVA SLC TESTS");
     testHeader(1, "Acquire scalar types SLC PMUS");
     getRequest("XCOR:LI03:120:LEFF", FLOAT, "Float BACT"
```

### e.g. 2: Multiple arguments

```java
     testSuiteHeader("AIDA-PVA SLC Buffered Acquisition TESTS");
     testHeader(2, "Get values of 4 BPMs");
     request("NDRFACET:BUFFACQ", "BPM Values")
                    .with("BPMD", 57)
                    .with("NRPOS", 180)
                    .with("BPMS", List.of(
                            "BPMS:LI11:501",
                            "BPMS:LI11:601",
                            "BPMS:LI11:701",
                            "BPMS:LI11:801"))
                    .get();
```

### e.g. 3: Simple set

```java
     testSuiteHeader("AIDA-PVA SLC TESTS");
     testHeader(testId, "Set value test");
     setRequest("XCOR:LI31:41:BCON", 5.0f);

```
### e.g. 4: Advanced set

```java
     testSuiteHeader("AIDA-PVA SLC Klystron TESTS");
     testHeader(testId, "Deactivate the specified klystron");
     request("KLYS:LI31:31:TACT", "Deactivated")
                    .with("BEAM", 8)
                    .with("DGRP", "DEV_DGRP")
                    .set(0);

```

### e.g. 5: Selecting the return value type

```java
     testSuiteHeader("AIDA-PVA SLC Klystron TESTS");
     testHeader(testId, "Acquire STRING type");
     request("KLYS:LI31:31:TACT", "String")
                    .with("BEAM", 8)
                    .with("DGRP", "DEV_DGRP")
                    .returning(STRING)
                    .get();
```

## Building and Running Tests

### To build the tests

```shell 
git clone git@github.com:slaclab/aida-pva-tests.git
cd aida-pva-tests
mvn install
```
The shaded jar file is found in the target directory. `./target/aida-pva-tests-1.0.jar`

### To run the tests
Running the tests are equally simple. You need to make sure that your environment is set up so that you can find the epics providers. All you need to do is to set the `EPICS_PVA_ADDR_LIST` variable to identify the network or host you want to search.

Test are all named starting with SLC and then the short code identifying the provider followed by Test. e.g. SlcBpmTest.
Each test is in the `edu.stanford.slac.aida.test` package.

As the jar is shaded it includes everything it needs so no need for a complicated classpath. The full procedure to run the tests is as follows:

#### e.g. 1 Run the SLC Utility Channel Provider tests
_commands_
```shell 
export EPICS_PVA_ADDR_LIST=mccdev.slac.stanford.edu
cd ./target
java -cp aida-pva-tests-1.0.jar  "edu.stanford.slac.aida.test.SlcTest"
```

_output_
```shell
#################################################
AIDA-PVA SLC TESTS
#################################################

████ Test 1: Acquire scalar types SLC PMUS
_________________________________________________

get: XCOR:LI03:120:LEFF => FLOAT
    Float BACT: 0.262 ✔
get: XCOR:LI03:120//LEFF => FLOAT
    Backwards compatibility - using legacy channel format: Float BACT: 0.262 ✔
get: XCOR:LI03:120:LEFF => DOUBLE
    Double BACT: 0.2619999945163727 ✔
get: XCOR:LI03:120:VSTA => BOOLEAN
    Boolean VSTA: true ✔
get: XCOR:LI03:120:SETL => SHORT
    Short SETL: 3000 ✔
get: XCOR:LI03:120:ADCP => BYTE
    Byte ADCP: 48 ✔
get: XCOR:LI03:120:ADCP => CHAR
    Char ADCP: '0' ✔
get: XCOR:LI03:120:ADCP => CHAR
    WCHAR not possible in AIDA-PVA: Falling back to CHAR: ADCP: '0' ✔
get: XCOR:LI03:120:PSNM => STRING
    String PSNM: UNKNOWN  ✔
get: XCOR:LI03:120:PSNM => STRING
    WSTRING not possible in AIDA-PVA: Falling back to STRING: PSNM: UNKNOWN  ✔
get: XCOR:LI03:120:PSNM => BYTE_ARRAY
    Byte array PSNM:
        85 ✔
        78 ✔
        75 ✔
        78 ✔
        79 ✔
        87 ✔
        78 ✔
        32 ✔
get: XCOR:LI03:120:PSNM => CHAR_ARRAY
    Char array PSNM:
        'U' ✔
        'N' ✔
        'K' ✔
        'N' ✔
        'O' ✔
        'W' ✔
        'N' ✔
        ' ' ✔

████ Test 2: Acquire FLOAT type SLC PMUS
_________________________________________________

get: XCOR:LI03:120:BACT => FLOAT
    BACT: 0.0 ✔

████ Test 3: Acquire LONG INT type SLC PMUS
_________________________________________________

get: XCOR:LI03:120:VSTA => LONG
    VSTA: 8256 ✔

████ Test 4: Acquire array of FLOAT type SLC PMUS
_________________________________________________

get: XCOR:LI03:120:IMMS => FLOAT_ARRAY
    IMMS:
        -5.0 ✔
        5.0 ✔
        0.0 ✔

████ Test 5: Acquire array of DOUBLE type SLC PMUS
_________________________________________________

get: XCOR:LI03:120:IMMS => DOUBLE_ARRAY
    IMMS:
        -5.0 ✔
        5.0 ✔
        0.0 ✔

████ Test 6: Acquire array of SHORT type SLC PMUS
_________________________________________________

get: XCOR:LI03:120:NSCY => SHORT_ARRAY
    NSCY:
        0 ✔
        0 ✔

████ Test 7: Acquire array of LONG type SLC PMUS
_________________________________________________

get: XCOR:LI03:120:RAMP => LONG_ARRAY
    RAMP:
        0 ✔
        -1149219448956125168 ✔

████ Test 8: Acquire array of BOOLEAN type SLC PMUS
_________________________________________________

get: XCOR:LI03:120:RAMP => BOOLEAN_ARRAY
    RAMP:
        true ✔
        false ✔

████ Test 9: Acquire FLOAT type SLC Database PV
_________________________________________________

get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔

████ Test 10: Looping acquire FLOAT type SLC Database PV pausing 10 seconds between each.
_________________________________________________

get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔
get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔
get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔
get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔
get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔
get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔
get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔
get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔
get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔
get: XCOR:LI03:120:LEFF => FLOAT
    LEFF: 0.262 ✔

████ Test 11: ASTS channel name test
_________________________________________________

get: ASTS:LI00:ISOPLAN2:DATA => FLOAT
    DATA: 0.0 ✔
get: ASTS:PR02:VP3012:DATA => FLOAT
    DATA: 0.0 ✔
get: ASTS:PR02:VP3012:STAT => SHORT
    STAT: 40 ✔
get: ASTS:PR02:T2092BLW:LIM1 => FLOAT
    LIM1: 32.0 ✔
get: ASTS:PR02:T2092BLW:LIM2 => FLOAT
    LIM2: 150.0 ✔
get: ASTS:PR02:T2092BLW:LIMS => FLOAT_ARRAY
    LIMS:
        32.0 ✔
        150.0 ✔
get: ASTS:PR02:T2092QUA:SCAL => FLOAT_ARRAY
    SCAL:
        32.0 ✔
        1.8 ✔
get: ASTS:PR02:T2092QUA:RAW => FLOAT
    RAW: 1.0E-9 ✔
get: ASTS:PR02:T2092QUA:STAT => STRING
    STAT: CamErr   RED        ✔

████ Test 12: ASTS channel name with one of the pseudo-secondaries STAT, CTRL, or VSTA
_________________________________________________

get: ASTS:PR02:T2092QUA:STAT => STRING_ARRAY
    STAT:
        CamErr   ✔
        RED      ✔
          ✔

████ Test 13: Set value test
_________________________________________________

set: XCOR:LI31:41:BCON (VALUE=[5.0]) ✔    
```

#### e.g. 2 Run one test from the SLC Utility Channel Provider test suite

_commands_

```shell
export EPICS_PVA_ADDR_LIST=mccdev.slac.stanford.edu
cd ./target
java -cp aida-pva-tests.jar "edu.stanford.slac.aida.test.SlcUtilTest" 2
```

_output_

```shell
#################################################
AIDA-PVA SLC Utility TESTS
#################################################

████ Test 2: set value for MKB.  sleeping for 5 seconds between runs
_________________________________________________

set: MKB:VAL (MKB=mkb:gregstestli31.mkb, VALUE=1.0)
    MKB VAL:  2 rows retrieved: ✔
       Device Name          Value
              name          value
 XCOR:LI31:41:BDES -1.5949696E-23
 YCOR:LI31:41:BDES  9.0401356E-33
set: MKB:VAL (MKB=mkb:gregstestli31.mkb, VALUE=1.0)
    MKB VAL:  2 rows retrieved: ✔
       Device Name          Value
              name          value
 XCOR:LI31:41:BDES   4.1969155E-8
 YCOR:LI31:41:BDES -1.50365043E14
set: MKB:VAL (MKB=mkb:gregstestli31.mkb, VALUE=1.0)
    MKB VAL:  2 rows retrieved: ✔
       Device Name         Value
              name         value
 XCOR:LI31:41:BDES -1.08137696E8
 YCOR:LI31:41:BDES -2.317549E-17

████ Test 2: reversing sign of relative delta
_________________________________________________

set: MKB:VAL (MKB=mkb:gregstestli31.mkb, VALUE=-1.0)
    MKB VAL:  2 rows retrieved: ✔
       Device Name          Value
              name          value
 XCOR:LI31:41:BDES   4.1969155E-8
 YCOR:LI31:41:BDES -1.50365043E14
set: MKB:VAL (MKB=mkb:gregstestli31.mkb, VALUE=-1.0)
    MKB VAL:  2 rows retrieved: ✔
       Device Name          Value
              name          value
 XCOR:LI31:41:BDES -1.5949696E-23
 YCOR:LI31:41:BDES  9.0401356E-33
set: MKB:VAL (MKB=mkb:gregstestli31.mkb, VALUE=-1.0)
    MKB VAL:  2 rows retrieved: ✔
       Device Name        Value
              name        value
 XCOR:LI31:41:BDES 0.0079956055
 YCOR:LI31:41:BDES  0.058290116
```
