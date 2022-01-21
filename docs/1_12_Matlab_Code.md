# 1.12 - User Guide for Matlab Users

## Function Summary

### Key to the terms used in the Function Summary below

| key           | field / method       | description                                                                                                                                                                      |
|---------------|----------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| _any_type_    |                      | The argument to ML can be a variable  of any type.                                                                                                                               |  
| _builder_     |                      | A Java class that allows you to  build of requests before <br/> execution                                                                                                        | 
| _dynamic_     |                      | Any dynamically instantiated matlab  type                                                                                                                                        |
| _fieldName_   |                      | Within an AidaPvaStruct this is  a field's name                                                                                                                                  |  
| _exception_   |                      | exception caught by a matlab try catch block                                                                                                                                     |  
| _fieldValue_  |                      | Within an AidaPvaStruct this is  a field's value                                                                                                                                 |  
| _jstruct_     |                      | A Java structure that can be used as an argument or value                                                                                                                        |
| _nturi_       |                      | A special type of `PVStructure`  that corresponds to <br/> the `NTURI`  Normative Type                                                                                           |
| _message_     |                      | Custom message for handleExceptions                                                                                                                                              |
| _name_        |                      | The name of a parameter / argument  to the request. <br/>                                                                                                                        |  
| _nturi_       |                      | A special type of `PVStructure`  that corresponds to <br/> the `NTURI`  Normative Type.                                                                                          |  
| _Object_      |                      | A Java Object                                                                                                                                                                    |
| _PvaTable_    |                      | A Java PvaTable object                                                                                                                                                           |
|               | _size_               | Contains size of vectors in this  table                                                                                                                                          |
|               | _labels_             | A Java `Object`[] array of Java  Strings                                                                                                                                         |
|               | _get(name)_          | Method that will return a Java `Object`[]  array of <br/> Java `Objects` for the  named vector in this table. <br/> The objects are of the Java type  of the column vector <br/> | 
| _pvName_      |                      | The name of the Process Variable / EPICS channel that <br/> you'll be  requesting data from                                                                                      |  
| _PVStructure_ |                      | An EPICS data type conforming to the Normative Type <br/> specifications.   (see [Normative Types](2_2_Normative_Types.md))                                                      |
| _scalar_      |                      | A scalar matlab type                                                                                                                                                             |
| _type_        |                      | An AIDA-PVA type from the list below:                                                                                                                                            |  
|               | _AIDA_BOOLEAN_       |                                                                                                                                                                                  |  
|               | _AIDA_BYTE_          |                                                                                                                                                                                  |  
|               | _AIDA_SHORT_         |                                                                                                                                                                                  |  
|               | _AIDA_INTEGER_       |                                                                                                                                                                                  |  
|               | _AIDA_LONG_          |                                                                                                                                                                                  |  
|               | _AIDA_STRING_        |                                                                                                                                                                                  |  
|               | _AIDA_BOOLEAN_ARRAY_ |                                                                                                                                                                                  |  
|               | _AIDA_BYTE_ARRAY_    |                                                                                                                                                                                  |  
|               | _AIDA_SHORT_ARRAY_   |                                                                                                                                                                                  |  
|               | _AIDA_INTEGER_ARRAY_ |                                                                                                                                                                                  |  
|               | _AIDA_LONG_ARRAY_    |                                                                                                                                                                                  |  
|               | _AIDA_STRING_ARRAY_  |                                                                                                                                                                                  |  
|               | _AIDA_TABLE_         |                                                                                                                                                                                  |  
| _value_       |                      | Any matlab scalar value or array you want to set the given <br/> Process Variable / channel, or  request argument to.                                                            |  
| _void_        |                      | No value is returned                                                                                                                                                             |

### Matlab 2012 vs 2020
#### Accessing aida-pva-client api in function scopes
In functions, you need to use the `aidapva` script to bring the aida-pva-client api into the function scope.  From 
the commandline or in scripts this is not necessary.

```matlab
function example()
  aidapva;
  
  builder = pvaRequest('XCOR:LI31:41:BCON');
  builder.returning(AIDA_FLOAT);
  response = builder.get();
end
```

#### Function chaining
In matlab 2012 you can't chain functions together so instead of writing this:

```matlab
mstruct = ML(pvaRequest('NDRFACET:BUFFACQ').with('BPMD', 57).with('BPMS', { 'BPMS:LI11:501' }).get())
mstruct =
  struct with fields:
            size: 1
          labels: {'BPM Name'  'pulse id'  'x offset (mm)'  'y offset (mm)'  'num particles (coulomb)'  'stat'  'good measurement'}
           units: []
    descriptions: []
      fieldnames: {'name'  'pulseId'  'x'  'y'  'tmits'  'stat'  'goodmeas'}
          values: [1x1 struct]
```

you have to write it on separate lines thus:

```matlab
builder = pvaRequest('NDRFACET:BUFFACQ');
builder.with('BPMD', 57);
builder.with('BPMS', { 'BPMS:LI11:501' });
mstruct = ML(builder.get())
mstruct =
            size: 1
          labels: {'BPM Name'  'pulse id'  'x offset (mm)'  'y offset (mm)'  'num particles (coulomb)'  'stat'  'good measurement'}
           units: []
    descriptions: []
      fieldnames: {'name'  'pulseId'  'x'  'y'  'tmits'  'STAT'  'goodmeas'}
          values: [1x1 struct]
```

@note All examples below are written for Matlab 2012 so they are much more verbose than they need to be.  You can 
chain the function calls together when you use them in matlab 2020.

#### Exception handling
Matlab 2012 creates an exception message that includes much of the stack trace instead of just the short 
reason message that the original exception contains.  Matlab 2020 contains the correct message.  So instead of 
seeing this for errors:

```matlab
try
    response = pvaRequest('NDRFACET:BUFFACQ').with('BPMD', 57).with('NRPOS', 10).with('BPMSWRONG', {'BPMS:LI02:501' 'BPMS:DR12:334' } ).get();
catch e
    handleExceptions(e);
end
NDRFACET:BUFFACQ(NRPOS=10.0, BPMD=57.0, BPMSWRONG=[BPMS:LI02:501, BPMS:DR12:334]) :NDRFACET:BUFFACQ:  BPMSWRONG is not a valid argument for get requests to this channel
```

You will see this in Matlab 2012

```matlab
try
    builder = pvaRequest('NDRFACET:BUFFACQ');
    builder.with('BPMD', 57);
    builder.with('NRPOS', 10);
    builder.with('BPMSWRONG', {'BPMS:LI02:501' 'BPMS:DR12:334' } );
    response = builder.get();
catch e
    handleExceptions(e);
end
Java exception occurred:
org.epics.pvaccess.server.rpc.RPCRequestException: NDRFACET:BUFFACQ(BPMSWRONG=[BPMS:LI02:501, BPMS:DR12:334], BPMD=57.0, NRPOS=10.0) :NDRFACET:BUFFACQ:  BPMSWRONG is not a valid argument for get requests to this channel
	at edu.stanford.slac.aida.client.AidaPvaRequest.execute(AidaPvaRequest.java:182)
	at edu.stanford.slac.aida.client.AidaPvaRequest.getter(AidaPvaRequest.java:152)
	at edu.stanford.slac.aida.client.AidaPvaRequest$2.execute(AidaPvaRequest.java:112)
	at edu.stanford.slac.aida.client.AidaPvaClientUtils.executeRequest(AidaPvaClientUtils.java:219)
	at edu.stanford.slac.aida.client.AidaPvaRequest.get(AidaPvaRequest.java:109)
```
#### Strings
In matlab 2020 you can represent a string as as follows:

```matlab
    string = "matlab 2020 string";
```

In matlab 2012 you can only use single quotes
```matlab
    string = 'matlab 2020 string';
```

### Initialisation 

| return | function          | parameters | description                                                                                                                                             |
|--------|-------------------|------------|---------------------------------------------------------------------------------------------------------------------------------------------------------|
| _void_ | **aidapvainit**   | ()         | to initialise access to the AIDA-PVA  framework.                                                                                                        |
| _void_ | **aidapvafninit** | ()         | to initialise access to the AIDA-PVA  framework for functions.  Do not call this function directly, it is called automatically from aidapvainit.m       |
| _void_ | **aidapva**       | ()         | to bring aida-pva-client functions into a function's scope.  Only used at the top of function definitions before calling any aida-pva-client functions. |

@note aidapvainit.m will be called automatically when matlab starts up so there is no need to call it manually.

### Simple Get: get the value of a process variable
| return                           | function    | parameters           | description                                                                                               |
|----------------------------------|-------------|----------------------|-----------------------------------------------------------------------------------------------------------|
| _scalar_, _Object[]_, _PvaTable_ | **pvaGet**  | (_pvName_[, _type_]) | takes a `pvName` and an optional  `type` and makes a <br/> data request                                   | 
| _dynamic_                        | **pvaGetM** | (_pvName_[, _type_]) | takes a `pvName` and an optional  `type` and makes a <br/> data request  returning a matlab type directly | 

e.g. **PvaGetM**
```matlab
response = pvaGet('XCOR:LI31:41:BCON', AIDA_FLOAT)
response =
     5

response = pvaGetM('XCOR:LI31:41:BCON', AIDA_FLOAT_ARRAY)
response =
     5

response = pvaGetM('DEV_DGRP:XCOR:BDES')
response =
            size: 4
          labels: {'name of magnet'  'secondary values'}
           units: []
    descriptions: []
          values: [1x1 struct]

response.size
ans =
     4

response.labels
ans =
    'name of magnet'    'secondary values'

response.values.name
ans =
    'XCOR:LI31:41'    'XCOR:LI31:201'    'XCOR:LI31:301'    'XCOR:LI31:401'
```

e.g. **PvaGet**
```matlab
response = pvaGet('XCOR:LI31:41:BCON', AIDA_FLOAT)
response =
     5

response = pvaGet('XCOR:LI31:41:BCON', AIDA_FLOAT_ARRAY)
response =
java.lang.Object[]:
    [5]

response = pvaGet('DEV_DGRP:XCOR:BDES')
response =
PvaTable(size=4, labels=[name of magnet, secondary values], fieldNames=[name, secondary], descriptions=[], units=[], values={name=[Ljava.lang.Object;@19050a0, secondary=[Ljava.lang.Object;@19d3b3a})

response.size
ans =
4

response.labels
ans =
java.lang.String[]:
    'name of magnet'
    'secondary values'

response.get('name')
ans =
java.lang.Object[]:
    'XCOR:LI31:41'
    'XCOR:LI31:201'
    'XCOR:LI31:301'
    'XCOR:LI31:401'
```

### Simple Set: set the value of a process variable 
| return             | function    | parameters          | description                                                                       |
|--------------------|-------------|---------------------|-----------------------------------------------------------------------------------|
| _void_, _PvaTable_ | **pvaSet**  | (_pvName_, _value_) | **set()** the `pvName` to the given  value                                        |
| _dynamic_          | **pvaSetM** | (_pvName_, _value_) | **set()** the `pvName` to the given  value returning <br/> a matlab type directly | 

e.g. **pvaSet**
```matlab
pvaSet('XCOR:LI31:41:BCON', 5.0);
```

e.g. **pvaSetM**
```matlab
response = pvaSetM('KLYS:LI31:31:KPHR', 60.0)
response =
            size: 1
          labels: {'phas'}
           units: []
    descriptions: []
          values: [1x1 struct]

response.size
ans =
     1

response.labels
ans =
    'phas'

response.values.phas
ans =
    5.0
```


### Complex Request Builder: to build complex get, and set, requests
| return                           | function               | parameters                | description                                                                                                                               |
|----------------------------------|------------------------|---------------------------|-------------------------------------------------------------------------------------------------------------------------------------------|
| _builder_                        | **pvaRequest**         | (_pvName_)                | takes a `pvName` and creates a request  builder                                                                                           | 
| _builder_                        | builder. **with**      | (_name_, _value/jstruct_) | specifies a request parameter and its value                                                                                               | 
| _builder_                        | builder. **returning** | (_type_)                  | specified the `type` to return from  the request                                                                                          | 
| _scalar_, _Object[]_, _PvaTable_ | builder. **get**       | ()                        | executes the **get** request                                                                                                              | 
| _void_, _PvaTable_               | builder. **set**       | (_value/jstruct_)         | executes the **set** request with  the given `value`  or `jstruct` <br/>                                                                  | 
| _nturi_                          | builder. **uri**       | ()                        | to create an `NTURI` PVStructure  (see <br/> [Normative Types](2_2_Normative_Types.md)) for use with <br/> EPICS/AIDA-PVA data  providers | 

e.g. complex **get** request
```matlab
builder = pvaRequest('NDRFACET:BUFFACQ');
builder.with('BPMD', 57);
builder.with('BPMS', { 'BPMS:LI11:501' });
mstruct = ML(builder.get())
mstruct =
            size: 1
          labels: {'BPM Name'  'pulse id'  'x offset (mm)'  'y offset (mm)'  'num particles (coulomb)'  'stat'  'good measurement'}
           units: []
    descriptions: []
      fieldnames: {'name'  'pulseId'  'x'  'y'  'tmits'  'stat'  'goodmeas'}
          values: [1x1 struct]

mstruct.values.pulseId
ans =
       75785

mstruct.values.stat
ans =
     1

mstruct.values.tmits
ans =
   1.0000e-10

mstruct.values.name
ans =
    'BPMS:LI11:501'

mstruct.values.x
ans =
    0.4598

mstruct.values.y
ans =
    0.1861
```

e.g with **returning**
```matlab
builder = pvaRequest('PHAS:LI09:12:VACT');
builder.returning(AIDA_SHORT);
response = builder.get()
resonse =
     0
```

e.g. complex **set** request
```matlab
builder = pvaRequest('MAGNETSET:BCON');
jstruct = AidaPvaStruct();
jstruct.put('names', { 'XCOR:LI31:41'});
jstruct.put('values', { 5.0 } );
builder.set(jstruct);
```

e.g. another complex **set** request
```matlab
builder = pvaRequest('MAGNETSET:BDES');
builder.with('MAGFUNC', 'NOFUNC');
jstruct = AidaPvaStruct();
jstruct.put('names', { 'XCOR:LI31:41'});
jstruct.put('values', { 4.0 } );
mstruct= ML(builder.set(jstruct))
mstruct =
            size: 1
          labels: {'status'  'bact/vact'}
           units: []
    descriptions: []
      fieldnames: {'status'  'bact_vact'}
          values: [1x1 struct]

mstruct.values.status(1)
ans =
    'OUTOFTOL '

mstruct.values.bact_vact(1)
ans =
    0.2983          
```

e.g. use **uri** to obtain an NTURI for use with PvaClient

```matlab
nturi = pvaRequest('PHAS:LI09:12:VACT').returning(AIDA_SHORT).uri();
response = pvarpc(nturi) ;
```

e.g. use **uri** to obtain an NTURI for use with EasyPVA

```matlab
builder = pvaRequest('PHAS:LI09:12:VACT');
builder.returning(AIDA_SHORT);
nturi = builder.uri();
response = ezrpc(nturi) ;
```

### Structured Data: to create data structures to be used as request arguments 
| return    | function          | parameters                  | description                                                                                       |
|-----------|-------------------|-----------------------------|---------------------------------------------------------------------------------------------------|
| _jstruct_ | **AidaPvaStruct** | ()                          | to create a Java structure that can be  passed to <br/> AIDA-PVA request builders as an argument. | 
|           | jstruct. **put**  | (_fieldName_, _fieldValue_) | specifies a value for given field in the AidaPvaStruct <br/>                                      | 

e.g. creating a **AidaPvaStruct** Java structure for an argument or value 
```matlab
jstruct = AidaPvaStruct();
jstruct.put('names', { 'XCOR:LI31:41'});
jstruct.put('values', { 4.0 } );

```

### Data Conversion: to convert returned data into matlab types
| return    | function | parameters   | description                                    |
|-----------|----------|--------------|------------------------------------------------|
| _dynamic_ | **ML**   | (_any_type_) | convert any parameters to dynamic  matlab type | 

e.g. **ML** data conversion of results
```matlab
mstruct = ML(builder.get())
mstruct =
            size: 4
          labels: {'name of magnet'  'secondary values'}
           units: []
    descriptions: []
          values: [1x1 struct]

mstruct.size
ans =
     4

mstruct.labels
ans =
    'name of magnet'    'secondary values'

mstruct.values.name
ans =
    'XCOR:LI31:41'    'XCOR:LI31:201'    'XCOR:LI31:301'    'XCOR:LI31:401'

mstruct.values.secondary
ans =
    5.0000         0         0    0.0300
```

### PvaClient Executor: to execute requests using PvaClient
| return        | function   | parameters | description                                       |
|---------------|------------|------------|---------------------------------------------------|
| _PVStructure_ | **pvarpc** | (_nturi_)  | takes an `NTURI` and executes it  using PvaClient | 

e.g. **pvarpc** PvaClient executor example 
```matlab
nturi = pvaRequest('PHAS:LI09:12:VACT').returning(AIDA_SHORT).uri();
response = pvarpc(nturi) ;
bval= ML(response)
bval =
     0
```


### EasyPVA Executor: to execute requests using EasyPVA
| return        | function  | parameters | description                                    |
|---------------|-----------|------------|------------------------------------------------|
| _PVStructure_ | **ezrpc** | (_nturi_)  | takes an `NTURI` and executes it using EasyPVA | 

e.g. **ezrpc** EasyPVA executor example
```matlab
builder = pvaRequest('PHAS:LI09:12:VACT');
builder.returning(AIDA_SHORT);
nturi = builder.uri();
response = ezrpc(nturi) ;
bval= ML(response)
bval =
     0
```

### Error handling
| return | function             | parameters                 | description                                                                                                 |
|--------|----------------------|----------------------------|-------------------------------------------------------------------------------------------------------------|
| _void_ | **handleExceptions** | (_exception_, [_message_]) | takes a matlab exception object, and an optional error message text and handles the error in a standard way | 

EasyPVA and aida-pva-client propagate errors back from the Channel Provider to the matlab client however PvaClient does not.  
You can write code to intercept error that occur and display a short message.  We've provided a small 
function that does this for you.

```matlab
pvName=strcat(dbname,':EDES:VAL');
try
    pvaSet(pvName, MAGNET(n).energy0)
catch e
    handleExceptions(e);
end
```

### PvaTables as Matlab Structures 
All data returned from AIDA-PVA comes back in the form of a PVStructure. This PVStructure conforms to 
the Normative Type <br/> specifications.  Tables come back as a 
special type of Normative type, the NTTable.  (see [Normative Types](2_2_Normative_Types.md)).  

When we convert this table data to a matlab structure the structure contains the following fields:
- `size`: integer size of the homogenous vectors
- `labels`: array of labels for the vectors
- `units`: if available an array containing the units for the vectors
- `description`: if available an array containing the description for the vectors
- `values.<field>`: array containing the value of the specified field.  e.g. `table.values.names` contains an array of `names`

#### Using EasyPVA and PvaClient
EasyPVA and PvaClient don't convert a returned NTTable to a matlab structure.  But you can use the ML function to convert it for you.

e.g. Convert EasyPVA and PvaClient NTTables to matlab structure
```matlab
builder = pvaRequest('DEV_DGRP:XCOR:BDES');
nturi = builder.uri();
nttable = ezrpc(nturi);
ML(nttable)
ans =
            size: 4
          labels: {'name of magnet'  'secondary values'}
           units: []
    descriptions: []
          values: [1x1 struct]
```

#### Using pvaRequest.get() and pvaRequest.set()
These methods return tables in Java PvaTable format.  

e.g. You can convert the response like this.

```matlab
builder = pvaRequest('DEV_DGRP:XCOR:BDES');
ML(builder.get())
ans =
            size: 4
          labels: {'name of magnet'  'secondary values'}
           units: []
    descriptions: []
          values: [1x1 struct]
```

e.g. Or you can use the PvaTable directly like this.

```matlab
builder = pvaRequest('DEV_DGRP:XCOR:BDES');
pvatable = builder.get();
pvatable.size
ans =
4

pvatable.labels
ans =
java.lang.String[]:
    'name of magnet'
    'secondary values'

names = pvatable.get('name')
names =
java.lang.Object[]:
    'XCOR:LI31:41'
    'XCOR:LI31:201'
    'XCOR:LI31:301'
    'XCOR:LI31:401'

names(1)
ans =
XCOR:LI31:41
```

#### Using PvaGetM(), and PvaSetM()
When these functions return tables they are automatically formatted as matlab structures.

e.g.: Automatically generated matlab structures

```matlab
mstruct = pvaGetM('DEV_DGRP:XCOR:BDES')
mstruct =
            size: 4
          labels: {'name of magnet'  'secondary values'}
           units: []
    descriptions: []
          values: [1x1 struct]

mstruct.size
ans =
     4

mstruct.labels
ans =
    'name of magnet'    'secondary values'

mstruct.values.name
ans =
    'XCOR:LI31:41'    'XCOR:LI31:201'    'XCOR:LI31:301'    'XCOR:LI31:401'

mstruct.values.secondary
ans =
    5.0000         0         0    0.0300
```

## Legacy AIDA Type migration

In legacy AIDA types were coded using constant numbers from this table.  Replace codes or types with the corresponding AIDA-PVA type.

| Code | Legacy AIDA Type | AIDA-PVA Type                                                                                     |  
|------|------------------|---------------------------------------------------------------------------------------------------|
| 	0   | `STRUCT`         | @ref edu.stanford.slac.aida.client.AidaPvaClientUtils.AidaPvaStruct() "AidaPvaStruct"             |
| 	1   | `BOOLEAN`        | @ref edu.stanford.slac.aida.client.AidaType.AIDA_BOOLEAN "AIDA_BOOLEAN"                           |
| 	2   | `BYTE`           | @ref edu.stanford.slac.aida.client.AidaType.AIDA_BYTE "AIDA_BYTE"                                 |
| 	3   | `CHAR`           | unsupported use @ref edu.stanford.slac.aida.client.AidaType.AIDA_BYTE "AIDA_BYTE"                 |
| 	4   | `DOUBLE`         | @ref edu.stanford.slac.aida.client.AidaType.AIDA_DOUBLE "AIDA_DOUBLE"                             |
| 	5   | `FLOAT`          | @ref edu.stanford.slac.aida.client.AidaType.AIDA_FLOAT "AIDA_FLOAT"                               |
| 	6   | `LONG`           | @ref edu.stanford.slac.aida.client.AidaType.AIDA_LONG "AIDA_LONG"                                 |
| 	7   | `LONGDOUBLE`     | unsupported use @ref edu.stanford.slac.aida.client.AidaType.AIDA_DOUBLE "AIDA_DOUBLE"             |
| 	8   | `LONGLONG`       | unsupported use @ref edu.stanford.slac.aida.client.AidaType.AIDA_LONG "AIDA_LONG"                 |
| 	9   | `SHORT`          | @ref edu.stanford.slac.aida.client.AidaType.AIDA_SHORT "AIDA_SHORT"                               |
| 	10  | `STRING`         | @ref edu.stanford.slac.aida.client.AidaType.AIDA_STRING "AIDA_STRING"                             |
| 	11  | `ULONG`          | unsupported use @ref edu.stanford.slac.aida.client.AidaType.AIDA_LONG "AIDA_LONG"                 |
| 	12  | `ULONGLONG`      | unsupported use @ref edu.stanford.slac.aida.client.AidaType.AIDA_LONG "AIDA_LONG"                 |
| 	13  | `USHORT`         | unsupported use @ref edu.stanford.slac.aida.client.AidaType.AIDA_SHORT "AIDA_SHORT"               |
| 	14  | `WCHAR`          | unsupported use @ref edu.stanford.slac.aida.client.AidaType.AIDA_BYTE "AIDA_BYTE"                 |
| 	15  | `WSTRING`        | unsupported use @ref edu.stanford.slac.aida.client.AidaType.AIDA_STRING "AIDA_STRING"             |
| 	51  | `BOOLEANA`       | @ref edu.stanford.slac.aida.client.AidaType.AIDA_BOOLEAN_ARRAY "AIDA_BOOLEAN_ARRAY"               |
| 	52  | `BYTEA`          | @ref edu.stanford.slac.aida.client.AidaType.AIDA_BYTE_ARRAY "AIDA_BYTE_ARRAY"                     |
| 	53  | `CHARA`          | unsupported use @ref edu.stanford.slac.aida.client.AidaType.AIDA_BYTE_ARRAY "AIDA_BYTE_ARRAY"     |
| 	54  | `DOUBLEA`        | @ref edu.stanford.slac.aida.client.AidaType.AIDA_DOUBLE_ARRAY "AIDA_DOUBLE_ARRAY"                 |
| 	55  | `FLOATA`         | @ref edu.stanford.slac.aida.client.AidaType.AIDA_FLOAT_ARRAY "AIDA_FLOAT_ARRAY"                   |
| 	56  | `LONGA`          | @ref edu.stanford.slac.aida.client.AidaType.AIDA_LONG_ARRAY "AIDA_LONG_ARRAY"                     |
| 	57  | `LONGDOUBLEA`    | unsupported use @ref edu.stanford.slac.aida.client.AidaType.AIDA_DOUBLE_ARRAY "AIDA_DOUBLE_ARRAY" |
| 	58  | `LONGLONGA`      | unsupported use @ref edu.stanford.slac.aida.client.AidaType.AIDA_LONG_ARRAY "AIDA_LONG_ARRAY"     |
| 	59  | `SHORTA`         | @ref edu.stanford.slac.aida.client.AidaType.AIDA_SHORT_ARRAY "AIDA_SHORT_ARRAY"                   |
| 	60  | `STRINGA`        | @ref edu.stanford.slac.aida.client.AidaType.AIDA_STRING_ARRAY "AIDA_STRING_ARRAY"                 |
| 	61  | `ULONGA`         | unsupported use @ref edu.stanford.slac.aida.client.AidaType.AIDA_LONG_ARRAY "AIDA_LONG_ARRAY"     |
| 	62  | `ULONGLONGA`     | unsupported use @ref edu.stanford.slac.aida.client.AidaType.AIDA_LONG_ARRAY "AIDA_LONG_ARRAY"     |
| 	63  | `USHORTA`        | unsupported use @ref edu.stanford.slac.aida.client.AidaType.AIDA_SHORT_ARRAY "AIDA_SHORT_ARRAY"   |
| 	64  | `WCHARA`         | unsupported use @ref edu.stanford.slac.aida.client.AidaType.AIDA_BYTE_ARRAY "AIDA_BYTE_ARRAY"     |
| 	65  | `WSTRINGA`       | unsupported use @ref edu.stanford.slac.aida.client.AidaType.AIDA_STRING_ARRAY "AIDA_STRING_ARRAY" |
| 	99  | `ANYA`           | unsupported                                                                                       |

## Migration patterns

General substitutions to apply in legacy AIDA code.

<table class="markdownTable">
<tr class="markdownTableHead">
<th class="markdownTableHeadNone">pattern</th><th class="markdownTableHeadNone">Migration Action/Replacement</th>
</tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

```matlab
import edu.stanford.slac.aida.lib.da.DaObject;
```

```matlab
import edu.stanford.slac.aida.lib.da.*;
```

```matlab
import edu.stanford.slac.aida.lib.util.common.*;
```

</td>
<td>

```matlab
aidapva;
```
</td>
</tr>

<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">

```matlab
da = DaObject();    
```

</td>
<td>
Remove
</td>
</tr>

<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

```matlab
da.setParam('BEAM', 1);
```

</td>
<td class="markdownTableBodyNone">

```matlab
builder = pvaRequest('KLYS:LI31:31:TACT');
builder = builder.with('BEAM', '1');

```

- You need to know the `pvName` name first
- Then create a builder and set parameters on it
- Repeat this for as many parameters as you have

</td>
</tr>

<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">

```matlab
result=da.get(pvName,4);
```
</td>
<td class="markdownTableBodyNone">

```matlab
result = ML(pvaRequest(pvName).returning(AIDA_DOUBLE).get());
```

or

```matlab
result = pvaGetM(pvName, AIDA_DOUBLE);
```

- Note that you need to look up the type code `4` in the conversion table above.

</td>
</tr>

<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

```matlab
result.getAsDouble
```

- various getAs...() methods on the DaObject for scalar

</td>
<td class="markdownTableBodyNone">

```matlab
result
```

- The result will already be in the correct type for all scalar results. No conversion is required

</td>
</tr>

<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">

```matlab
result.getAsDoubles
```

- various getAs...s() methods on the DaObject for scalar arrays

</td>
<td class="markdownTableBodyNone">

```matlab
result
```

- The result will already be in the correct type for all scalar array results. No conversion is required

</td>
</tr>

<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

```matlab
values = result.get(4).getAsDoubles;
```

- da.Any for tables.
- For example a table with the 4th vector named "x", containing an array of doubles.

</td>
<td class="markdownTableBodyNone">

```matlab
values = result.values.x;
```

- The result will already be in the correct table type for all table results. No conversion is required

</td>
</tr>

<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">

```matlab
value=DaValue(java.lang.Short(10));
result=da.setDaValue(channel, value);
```

</td>
<td class="markdownTableBodyNone">

```matlab
    result=pvaRequest(channel).set(10);
```

or

```matlab
    result=pvaSet(channel, 10);
```

or (if the result is a table)

```matlab
    result=pvaSetM(channel, 10);
```

</td>
</tr>


<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

```matlab
IN:ST:ANCE//ATTRIBUTE
```

</td>
<td>

```matlab
IN:ST:ANCE:ATTRIBUTE
```

</td>

</tr>
</table>

## Function Source code in matlab
The following source code is delivered in matlab under the `./src` directory

- `src/`aidapva.m
- `src/`aidapvainit.m
- `src/`aidapvafninit.m
- `src/`ezrpc.m  - A script that can be called to make requests based on ezPVA has been created. Extracted from erpc.m.
- `src/`handleExceptions.m
- `src/`ML.m
- `src/`pvaGet.m
- `src/`pvaGetM.m
- `src/`pvarpc.m  - A script that can be called to make requests based on PvaClient has been created. Extracted from erpc.m.
- `src/`toArray.m - To convert to an array.  Do not call this directly use ML.m if necessary.
