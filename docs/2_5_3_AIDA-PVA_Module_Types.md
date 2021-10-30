# 2.5.3 - AIDA-PVA Module Types

The AIDA-PVA Module is a module contained in the `STANDALONELIB` library that provides all the boilerplate functionality
needed to respond to **get** and **set** requests, marshal and unmarshal objects, and simple types across the JNI
boundary and between your Channel Provider, VMS, and the Channel Provider module in `AIDASHR`.

## Types provided in AIDA-PVA Module

There are some special data types that you'll need to be able to get Arguments in the endpoint implementation, and to
return package up data for return.

These are defined in aida_pva_types.h but are also automatically loaded by including aida_pva.h.

- Type - _Types of return value, or arguments see [Supported Types Documentation](Supported_Types.md)_
- Arguments - _All the arguments passed from the request to the Channel Provider_
- Argument - _A single request argument_
- Value - _Contents of the VALUE argument_
- Table - _Represents a TABLE to be returned to the client_
- Array - _Represents an Array of scalar types to be returned to the client_
- StringArray - _Represents an Array of string type to be returned to the client_

