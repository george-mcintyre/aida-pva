# 1.12 - Matlab Code

## Overview

The following utility functions have been added/updated in Matlab to support AIDA-PVA:

| category      | return                | function           | parameters                           | description                                                                                                               |
|---------------|-----------------------|--------------------|--------------------------------------|---------------------------------------------------------------------------------------------------------------------------|
| general       | `void`                | **aidainit**       | ()                                   | to initialise access to the AIDA framework                                                                                |
|               | `structure`           | **nttable2struct** | (`pvName` [, `name`, `value` ]`...`) | to convert from NTTables to matlab structures, (see [Normative Types](2_2_Normative_Types.md))                            |
|               | `NTURI`               | **nturi**          | (`pvName`, `varargin`)               | to create an `NTURI` Structure (see [Normative Types](2_2_Normative_Types.md)) for use with EPICS/AIDA-PVA data providers | 
| AidaPvaClient | `dynamic`             | **pvaGet**         | (`pvName`[, `type`])                 | takes a `pvName` and an optional type and executes a **get()**                                                            | 
|               | `empty` or `PvaTable` | **pvaSet**         | (`pvName`, `value`)                  | **set()** the `pvName` to the given value                                                                                 | 
|               | `builder`             | **pvaRequest**     | (`pvName`)                           | takes a `pvName` and executes a **get()** or **set()** request with builder pattern                                       | 
|               | `builder`             | `.with`            | (`name`, `value`)                    | specifies a parameter for the request                                                                                     | 
|               | `builder`             | `.returning`       | (`aidaType`)                         | specified the aida type to return from the request                                                                        | 
|               | `dynamic`             | `.get`             | ()                                   | executes the get request                                                                                                  | 
|               | `empty` or `PvaTable` | `.set`             | (`value`)                            | executes the get request with the given value                                                                             | 
| PvaClient     | `PVStructure`         | **pvarpc**         | (`nturi`)                            | takes an `NTURI` and executes it using PvaClient                                                                          | 
| EasyPVA       | `PVStructure`         | **ezrpc**          | (`nturi`)                            | takes an `NTURI` and executes it using EasyPVA                                                                            | 


## AIDA-PVA Channel Provider data access patterns

<table class="markdownTable">
<tr class="markdownTableHead">
<th class="markdownTableHeadNone">description</th><th class="markdownTableHeadNone">AidaPvaClient</th><th class="markdownTableHeadNone">PvaClient</th><th class="markdownTableHeadNone">EasyPVA</th>
</tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">
Simple Get
</td>
<td class="markdownTableBodyNone">

```matlab
bval=pvaGet('PHAS:LI09:12:VACT', SHORT);
```

</td>
<td class="markdownTableBodyNone">

```matlab
response = pvarpc(nturi('PHAS:LI09:12:VACT', 'TYPE', 'SHORT')) ;
bval= response.getSubField('value').get;
```

</td>
<td class="markdownTableBodyNone">

```matlab
response = ezrpc(nturi('PHAS:LI09:12:VACT', 'TYPE', 'SHORT')) ;
bval= response.getSubField('value').get;
```

</td>
</tr>

<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">
Get with arguments
</td>
<td class="markdownTableBodyNone">

```matlab
    table=pvaRequest('NDRFACET:BUFFACQ') ...
        .with('BPMD', 57) ...
        .with('NRPOS', 180) ...
        .with('BPMS', '["BPMS:LI11:501" , "BPMS:LI11:601" , "BPMS:LI11:701" , "BPMS:LI11:801" ]') ...
        .get();
    names = table.getValues().get('name');
```

</td>
<td class="markdownTableBodyNone">

```matlab
table = nttable2struct(pvarpc(nturi('NDRFACET:BUFFACQ', ...
 'BPMD', 57, ...
  'NRPOS', 180, ...
    'BPMS', '["BPMS:LI11:501","BPMS:LI11:601","BPMS:LI11:701","BPMS:LI11:801"]')));
names = table.value.name;
```

</td>
<td class="markdownTableBodyNone">

```matlab
table = nttable2struct(ezrpc(nturi('NDRFACET:BUFFACQ', ...
 'BPMD', 57, ...
  'NRPOS', 180, ...
    'BPMS', '["BPMS:LI11:501","BPMS:LI11:601","BPMS:LI11:701","BPMS:LI11:801"]')));
names = table.value.name;
```

</td>
</tr>

<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">
Simple Set
</td>
<td class="markdownTableBodyNone">

```matlab
pvaSet('XCOR:LI31:41:BCON', 5.0);
```

</td>
<td class="markdownTableBodyNone">

```matlab
pvarpc(nturi('XCOR:LI31:41:BCON', 'VALUE', 5.0));
```

</td>
<td class="markdownTableBodyNone">

```matlab
ezrpc(nturi('XCOR:LI31:41:BCON', 'VALUE', 5.0));
```

</td>
</tr>

<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">
Set returning a table
</td>
<td class="markdownTableBodyNone">

```matlab
table=pvaRequest('KLYS:LI31:31:TACT').set(0);
status = table.getValues().get('status');    
```

</td>
<td class="markdownTableBodyNone">

```matlab
table = nttable2struct(ezrpc(nturi('KLYS:LI31:31:TACT', 'VALUE', 0)));
status = table.value.status
```

</td>
<td class="markdownTableBodyNone">

```matlab
table = nttable2struct(pvarpc(nturi('KLYS:LI31:31:TACT', 'VALUE', 0)));
status = table.value.status
```

</td>
</tr>
</table>

## Migration patterns

Depending on the library you want to use behind the scenes there are three different migration patterns you can choose
from.

- ezrpc
- pvarpc
- aida-pva-client

<table class="markdownTable">
<tr class="markdownTableHead">
<th class="markdownTableHeadNone">pattern</th><th class="markdownTableHeadNone">AidaPvaClient</th><th class="markdownTableHeadNone">PvaClient</th><th class="markdownTableHeadNone">EasyPVA</th>
</tr>
<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

```matlab
import edu.stanford.slac.aida.lib.da.DaObject;
```

</td>
<td colspan=3>
Remove
</td>
</tr>

<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">

```matlab
da = DaObject();    
```

</td>
<td colspan=3>
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
requestBuilder = pvaRequest('KLYS:LI31:31:TACT');
requestBuilder = requestBuilder.with('BEAM', '1');

```

- You need to know the `channel` name first
- And create a channel object and set parameters on it
- Repeat this for as many parameters as you have

</td>
<td colspan=2>

```matlab
requestBuilder = pvaRequest('KLYS:LI31:31:TACT');
requestBuilder = requestBuilder.with('BEAM', '1');
NTURI = requestBuilder.uri();
```

- You need to know the `channel` name first
- You need to add all parameters in the same nturi() call.
- Then use the resulting NTURI in call to pvarpc() or ezrpc()

</td>
</tr>

<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">

```matlab
da.getValue(channel);
```

</td>
<td class="markdownTableBodyNone">

```matlab
RESULT = CHANNEL.get();
```

or

```matlab
RESULT = pvaRequest(channel).get();
```

or

```matlab
RESULT = pvaGet(channel, FLOAT);
```

</td>
<td class="markdownTableBodyNone">

```matlab
NTURI = nturi(channel);
RESULT = pvarpc(NTURI);
```

- You will need to have created an `NTURI` earlier

</td>
<td class="markdownTableBodyNone">

```matlab
NTURI = nturi(channel);
RESULT = ezrpc(NTURI);
```

- You will need to have created an `NTURI` earlier

</td>
</tr>

<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

```matlab
RESULT.getAsDouble
```

- various getAs...() methods on the DaObject for scalar

</td>
<td class="markdownTableBodyNone">

- The RESULT will already be in the correct type for all scalar results. No conversion is required

</td>
<td colspan=2>

```matlab
RESULT.getSubField('value').get
```

- The RESULTS returned are a PVStructure

</td>
</tr>

<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">

```matlab
RESULT.getAsDoubles
```

- various getAs...s() methods on the DaObject for scalar arrays

</td>
<td class="markdownTableBodyNone">

- The RESULT will already be in the correct type for all scalar array results. No conversion is required

</td>
<td colspan=2>

```matlab
RESULT.getSubField('value').get
```

- The RESULTS returned are a PVStructure

</td>
</tr>

<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

```matlab
values = RESULT.get(4).getAsDoubles;
```

- da.Any for tables.
- For example a table with the 4th vector named "x", containing doubles.

</td>
<td class="markdownTableBodyNone">

```matlab
values = RESULT.getValues().get('x');
```

- The RESULT will already be in the correct table type for all table results. No conversion is required

</td>
<td colspan=2>

```matlab
values = nttable2struct(RESULT).value.x;
```

- The RESULTS returned are a PVStructure, use nttable2struct to convert

</td>
</tr>

<tr class="markdownTableRowEven">
<td class="markdownTableBodyNone">

```matlab
value=DaValue(java.lang.Short(10));
RESULT=da.setDaValue(channel, value);
```

- Setting values

</td>
<td class="markdownTableBodyNone">

```matlab
    RESULT=pvaRequest(channel).set(10);
```

or

```matlab
    RESULT=pvaSet(channel, 10);
```

</td>
<td class="markdownTableBodyNone">

```matlab
RESULT=pvarpc(nturi(channel, 'VALUE', 10));
```

- Set a `VALUE` parameter

</td>
<td class="markdownTableBodyNone">

```matlab
RESULT=ezrpc(nturi(channel, 'VALUE', 10));
```

- Set a `VALUE` parameter

</td>
</tr>


<tr class="markdownTableRowOdd">
<td class="markdownTableBodyNone">

```matlab
IN:ST:ANCE//ATTRIBUTE
```

- Channel Names - PVs - pvNames

</td>
<td colspan=3>

```matlab
IN:ST:ANCE:ATTRIBUTE
```

</td>

</tr>
</table>

## aidainit

This has been updated to add aida-pva-client jars and define function aliases.

```matlab
global aidainitdone
if isempty(aidainitdone)
    global pvaRequest
    global pvaGet
    global pvaSet

    setupjavapath(strcat(getenv('PHYSICS_TOP'),'/release/aida-pva-client/R1.0.0/lib/aida-pva-client.jar'))

    % aida-pva-client imports
    import('edu.stanford.slac.aida.client.AidaPvaClientUtils.*');
    import('edu.stanford.slac.aida.client.AidaType.*');

    % Epics request exceptions
    import('org.epics.pvaccess.server.rpc.RPCRequestException');

    AIDA_BOOLEAN = [edu.stanford.slac.aida.client.AidaType.BOOLEAN];
    AIDA_BYTE = [edu.stanford.slac.aida.client.AidaType.BYTE];
    AIDA_CHAR = [edu.stanford.slac.aida.client.AidaType.CHAR];
    AIDA_SHORT = [edu.stanford.slac.aida.client.AidaType.SHORT];
    AIDA_INTEGER = [edu.stanford.slac.aida.client.AidaType.INTEGER];
    AIDA_LONG = [edu.stanford.slac.aida.client.AidaType.LONG];
    AIDA_FLOAT = [edu.stanford.slac.aida.client.AidaType.FLOAT];
    AIDA_DOUBLE = [edu.stanford.slac.aida.client.AidaType.DOUBLE];
    AIDA_STRING = [edu.stanford.slac.aida.client.AidaType.STRING];
    AIDA_BOOLEAN_ARRAY = [edu.stanford.slac.aida.client.AidaType.BOOLEAN_ARRAY];
    AIDA_BYTE_ARRAY = [edu.stanford.slac.aida.client.AidaType.BYTE_ARRAY];
    AIDA_CHAR_ARRAY = [edu.stanford.slac.aida.client.AidaType.CHAR_ARRAY];
    AIDA_SHORT_ARRAY = [edu.stanford.slac.aida.client.AidaType.SHORT_ARRAY];
    AIDA_INTEGER_ARRAY = [edu.stanford.slac.aida.client.AidaType.INTEGER_ARRAY];
    AIDA_LONG_ARRAY = [edu.stanford.slac.aida.client.AidaType.LONG_ARRAY];
    AIDA_FLOAT_ARRAY = [edu.stanford.slac.aida.client.AidaType.FLOAT_ARRAY];
    AIDA_DOUBLE_ARRAY = [edu.stanford.slac.aida.client.AidaType.DOUBLE_ARRAY];
    AIDA_STRING_ARRAY = [edu.stanford.slac.aida.client.AidaType.STRING_ARRAY];
    AIDA_TABLE = [edu.stanford.slac.aida.client.AidaType.TABLE];

    pvaRequest = @(channel) edu.stanford.slac.aida.client.AidaPvaClientUtils.pvaRequest(channel);
    pvaGet = @(channel, type) edu.stanford.slac.aida.client.AidaPvaClientUtils.pvaGet(channel, type);
    pvaSet = @(channel, value) edu.stanford.slac.aida.client.AidaPvaClientUtils.pvaSet(channel, value);

    aidainitdone = 1;
    disp 'Aida client initialization completed';
end

```

## pvarpc

A script that can be called to make requests based on PvaClient has been created. Extracted from erpc.m.

```matlab
function [ PVDATA ] = pvarpc( NTURI )
    aidainit;

    PVDATA = NaN;
    nturi_pvs = NTURI;

    % Get an PVA interface.
    provider = 'pva';
    client = PvaClient.get(provider);

    % Create a channel to the optics pv.
    pvname = nturi_pvs.getStringField('path').get();
    channel = client.createChannel(pvname);

    pvs = channel.rpc(nturi_pvs);

    % Reset output var if all went well.
    PVDATA = pvs;

```

## ezrpc

A script that can be called to make requests based on ezPVA has been created. Extracted from erpc.m.

```matlab
function [ PVDATA ] = ezrpc( NTURI )
    aidainit;

    PVDATA = NaN;
    nturi_pvs = NTURI;

    % Get an easyPVA interface.
    easypva = EasyPVAFactory.get();

    % Create a channel to the given pv, and attempt connection.
    pvname = nturi_pvs.getStringField('path').get();
    easychan = easypva.createChannel(pvname);
    iss=easychan.connect(5.0); % 5 second timeout

    % If channel connection to the given PV was successful, proceed.
    if (iss==true)
        easyrpc = easychan.createRPC();

        % iss = easypva.getStatus();
        % if ~isempty(easyrpc)
        iss = easyrpc.getStatus();
        % If successful, get data from the channel
        if ( iss.isOK() )
            % Connect the RPC to service PV and if successful
            % request data given arguments.
            if (easyrpc.connect())
                pvs = easyrpc.request(nturi_pvs);
                iss=easyrpc.getStatus();
                if (~iss.isOK())
                    % Issue result of statment that got twiss data. Server
                    % side generated errors will be issued by this.
                    error(servererr,char(iss.getMessage()));
                end
            else
                % Issue diagnostic msg of connect if unsuccessful.
                error(connecterr,char(easypva.getStatus().getMessage()));
            end
        else
            % For infrastrcuture errors, issue whole status object toString.
            error(pvasystemerr, char(iss) );
        end
    else
        % Could not create channel connection, probably a mistake in pv name.
        error(createchannelerror,createchannelerrormsg,char(pvname));
    end

    % Reset output var if all went well.
    if ( iss.isOK() )
        PVDATA = pvs;
    end

```
