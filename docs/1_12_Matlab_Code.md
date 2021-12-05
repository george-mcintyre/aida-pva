# 1.12 - Matlab Code

## Overview

The following utility functions have been added to Matlab to support AIDA:

- `void` **aidainit**() - to initialise access to the AIDA framework
- `void` **aidaget**(`aidaName`, `aidaType`, `aidaParams`) - gets SCALAR or SCALAR_ARRAY from EPICS/AIDA-PVA data
  providers
- `NTURI` **nturi**(`pvName`, `varargin`) - to create an `NTURI` Structure (
  see [Normative Types](2_2_Normative_Types.md)) for use with EPICS/AIDA-PVA data providers
- `matlab_structure` **nttable2struct** - to convert from NTTables to matlab structures
- `PVStructure` **ezrpc**(`nturi`) - takes an `NTURI` and executes it using EasyPva
- `PVStructure` **pvarpc**(`nturi`) - takes an `NTURI` and executes it using PvaClient
- `matlab_dynamic_type` **request**(`pvName`) - takes a `pvName` and executes a **get()** or **set()** request with
  builder pattern
    - **with**(`name`, `value`) - specifies a parameter for the request
    - **returning**(`aidaType`) - specified the aida type to return from the request
    - **get**() - executes the get request
    - **set**(`value`) - executes the set request with the given value

These have all been updated/added to be able to interact with the new AIDA-PVA framework.

## AIDA-PVA Channel Provider data access patterns

### aidaget for simple data

```matlab
    aidainit;
    bval=aidaget('PHAS:LI09:12:VACT');
```

### EasyPVA

#### Get

```matlab
    aidainit;
    response = ezrpc(nturi('PHAS:LI09:12:VACT')) ;
    bval= response.getSubField('value').get;
```

#### Get with arguments

```matlab
    aidainit;
    table = nttable2struct(ezrpc(nturi('NDRFACET:BUFFACQ', ...
     'BPMD', 57, ...
      'NRPOS', 180, ...
        'BPMS', '["BPMS:LI11:501","BPMS:LI11:601","BPMS:LI11:701","BPMS:LI11:801"]')));
    names = table.value.name;
```

#### Set 

```matlab
    aidainit;
    ezrpc(nturi('XCOR:LI31:41:BCON', 'VALUE', 5.0));
```

#### Set returning a table

```matlab
    aidainit;
    table = nttable2struct(ezrpc(nturi('KLYS:LI31:31:TACT', 'VALUE', 0)));
    satus = table.value.status
```

### PvaClient

#### Get

```matlab
    aidainit;
    response = pvarpc(nturi('PHAS:LI09:12:VACT')) ;
    bval= response.getSubField('value').get;
```

#### Get with arguments

```matlab
    aidainit;
    table = nttable2struct(pvarpc(nturi('NDRFACET:BUFFACQ', ...
     'BPMD', 57, ...
      'NRPOS', 180, ...
        'BPMS', '["BPMS:LI11:501","BPMS:LI11:601","BPMS:LI11:701","BPMS:LI11:801"]')));
    names = table.value.name;
```

#### Set 

```matlab
    aidainit;
    pvarpc(nturi('XCOR:LI31:41:BCON', 'VALUE', 5.0));
```

#### Set returning a table

```matlab
    aidainit;
    table = nttable2struct(pvarpc(nturi('KLYS:LI31:31:TACT', 'VALUE', 0)));
    satus = table.value.status
```


### aida-pva-client

#### Simple Get

```matlab
    aidainit;
    bval=getRequest('PHAS:LI09:12:VACT', SHORT); % LEM uses VACT
```

#### Simple Set

```matlab
    aidainit;
    setRequest('XCOR:LI31:41:BCON', 5.0); % LEM uses VACT
```

#### Get

```matlab
    aidainit;
    table=request('NDRFACET:BUFFACQ') ...
        .with('BPMD', 57) ...
        .with('NRPOS', 180) ...
        .with('BPMS', [ ...
                'BPMS:LI11:501' ...
                'BPMS:LI11:601' ...
                'BPMS:LI11:701' ...
                'BPMS:LI11:801' ]) ...
        .get();
    names = table.getValues().get('name');
```

#### Set returning a table

```matlab
    aidainit;
    table=request('KLYS:LI31:31:TACT').setReturningTable(0);
    status = table.getValues().get('status').get(0);
    
```

## Migration patterns

Depending on the library you want to use behind the scenes there are three different migration patters you can choose
from.

- ezrpc
- pvarpc
- aida-pva-client

### 1. import DaObject

* for all three migration patterns, simply remove this line:

e.g. remove these lines

```matlab
    import edu.stanford.slac.aida.lib.da.DaObject;
```

### 2. da = DaObject()

* for all three migration patterns, simply replace as follows if aidainit has not already been called:

```matlab
    da = DaObject();    
```

⬇

```matlab
    aidainit;
```

### 3. da.setParam()

- **ezrpc** and **pvarpc**
    - You need to know the `channel` name first

```matlab
    da.setParam('BEAM', 1)
```

⬇

```matlab
    NTURI = nturi(channel, 'BEAM', '1');
```

-
    - You need to add all parameters in the same nturi() call.
    - Then use the resulting NTURI in call to ezrpc() or pvarpc()
- **aida-pva-client**
    - You need to know the `channel` name first and create a request object `REQUEST = request(channel)`

```matlab
    da.setParam('BEAM', 1)
```

⬇

```matlab
    REQUEST = REQUEST.with('BEAM', '1')
```

-
    - Repeat this for as many parameters as you have

### 4. da.getValue()

- **ezrpc**
    - You will need to have created an `NTURI` earlier `NTURI = nturi(channel);`

```matlab
    da.getValue(channel)
```

⬇

```matlab
    RESULT = ezrpc(NTURI)
```

- **pvarpc**
    - You will need to have created an `NTURI` earlier `NTURI = nturi(channel);`

```matlab
    da.getValue(channel)
```

⬇

```matlab
    RESULT = pvarpc(NTURI)
```

- **aida-pva-client**

```matlab
    da.getValue(channel)
```

⬇

```matlab
    RESULT = request(channel).get()
```

-
    - If you've created a `REQUEST` object then simply:

```matlab
    RESULT = REQUEST.get()
```

### 5. getting scalar results value: RESULT.getAsDouble

- **ezrpc** and **pvarpc**
    - The RESULTS returned are a PVStructure to convert to scalar use the following pattern

```matlab
    RESULT.getAsDouble
```

⬇

```matlab
    RESULT.getSubField('value').get
```

- **aida-pva-client**
    - The RESULT will already be in the correct type for scalar results

### 6. getting scalar array results value: RESULT.getAsDoubles

- **ezrpc** and **pvarpc**
    - The RESULTS returned are a PVStructure to convert to scalar array use the following pattern

```matlab
    RESULT.getAsDoubles
```

⬇

```matlab
    RESULT.getSubField('value').get
```

- **aida-pva-client**
    - The RESULT will already be in the correct type for scalar results

### 7. getting tables (da.Any) results value: RESULT.get(4).getAsDoubles

- **ezrpc** and **pvarpc**
    - The RESULTS returned are a PVStructure to convert to scalar array use the following pattern
    - If the 4th vector's name is `x` then ...

```matlab
    RESULT.get(4).getAsDoubles
```

⬇

```matlab
    nttable2struct(RESULT).value.x
```

- **aida-pva-client**
    - If the 4th vector's name is `x` then ...

```matlab
    RESULT.get(4).getAsDoubles
```

⬇

```matlab
    RESULT.getValues().get('x')
```

### 8. da.setDaValue(channel, value)

- **ezrpc**
    - Set a `VALUE` parameter

```matlab
    value=DaValue(java.lang.Short(10));
    RESULT=da.setDaValue(channel, value);
```

⬇

```matlab
    RESULT=ezrpc(nturi(channel, 'VALUE', 10));
```

- **pvarpc**
    - Set a `VALUE` parameter

```matlab
    value=DaValue(java.lang.Short(10));
    RESULT=da.setDaValue(channel, value);
```

⬇

```matlab
    RESULT=pvarpc(nturi(channel, 'VALUE', 10));
```

- **aida-pva-client**

```matlab
    value=DaValue(java.lang.Short(10));
    RESULT=da.setDaValue(channel, value);
```

⬇

```matlab
    RESULT=request(channel).set(10);
```

### 9. For all Channel names

- For all migration patterns
    - replace all '//' in channel names (pv names) with ':'

## aidainit

This has been updated to add aida-pva-client jars


