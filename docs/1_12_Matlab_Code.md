# 1.12 - Matlab Code
## Overview
Matlab includes the following utility functions:

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

## aidainit
This has been updated to add aida-pva-client


