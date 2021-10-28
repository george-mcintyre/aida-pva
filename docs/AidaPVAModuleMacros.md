@tableofcontents
# AIDA-PVA Module Macros

The AIDA-PVA Module is a module contained in the `STANDALONELIB` library that provides all the boilerplate
functionality needed to respond to **get** and **set** requests, marshal and unmarshal objects, and simple types across the
JNI boundary and between your Channel Provider, VMS, and the Channel Provider module in `AIDASHR`.

## Macros provided in AIDA-PVA Module
There are a bunch of Macros that are provided with the AIDA-PVA Module, in the following header files:
- aida_pva_convert.h
  - CONVERT_TO_VMS_FLOAT() - _Convert a single float or an array of floats from ieee to VMS format._
  - CONVERT_TO_VMS_DOUBLE() - _Convert a single double or an array of doubles from ieee to VMS format._
  - CONVERT_FROM_VMS_FLOAT() - _Convert a single float or an array of floats from VMS to ieee format._
  - CONVERT_FROM_VMS_DOUBLE() - _Convert a single double or an array of doubles from VMS to ieee format._
- aida_pva_exceptions.h
  - ON_EXCEPTION_RETURN_()
    - _check to see if an exception has been raised,_ 
    - _return the given value._
  - ON_EXCEPTION_FREE_MEMORY_AND_RETURN_()
    - _check to see if an exception has been raised,_ 
    - _free local all tracked memory, and_ 
    - _return the given value._
  - ON_EXCEPTION_RETURN_VOID()
    - _check to see if an exception has been raised,_ 
    - _return void._
  - ON_EXCEPTION_FREE_ARGUMENTS_AND_RETURN_()
    - _check to see if an exception has been raised,_ 
    - _free local Arguments variable `arguments`, and_ 
    - _return the given value._
  - ON_EXCEPTION_FREE_ARGUMENTS_AND_RETURN_VOID()
    - _check to see if an exception has been raised,_ 
    - _free local Arguments variable `arguments`, and_ 
    - _return void._
  - ON_EXCEPTION_FREE_STRING_AND_ARGS_AND_RETURN_() 
    - _check to see if an exception has been raised,_ 
    - _free local char * variable `string`,_ 
    - _free local Arguments variable `arguments`, and_ 
    - _return the given value._
  - ON_EXCEPTION_FREE_STRING_AND_RETURN__()
    - _Check to see if an exception has been raised, 
    - free local char * variable `string`,_ 
    - _free local Arguments variable `arguments`, and_ 
    - _return the given value._
  - ON_EXCEPTION_FREE_ARRAY_AND_ARGS_AND_RETURN_()
    - _Check to see if an exception has been raised,_ 
    - _free local Array variable `array`,_ 
    - _free local variable `arguments`, and_ 
    - _return the given value._
  - ON_EXCEPTION_FREE_STRING_ARRAY_AND_ARGS_AND_RETURN_NULL()
    - _Check to see if an exception has been raised,_ 
    - _free local StringArray variable `array`,_ 
    - _free local variable `arguments`, and_ 
    - _return the given value._
  - ON_EXCEPTION_FREE_TABLE_AND_ARGS_AND_RETURN_()
    - _Check to see if an exception has been raised,_ 
    - _free local Table variable `table`,_ 
    - _free local Arguments variable `arguments`, and_ 
    - _return the given value._
  - ON_EXCEPTION_FREE_VALUE_AND_ARGS_AND_RETURN_VOID()
    - _Check to see if an exception has been raised,_ 
    - _free local Value variable `value`,_ 
    - _free local Arguments variable `arguments`, and_ 
    - _return void._
  - ON_EXCEPTION_FREE_VALUE_AND_ARGS_AND_RETURN_()
    - _check to see if an exception has been raised,_ 
    - _local Value variable `value`,_ 
    - _free local Arguments variable `arguments`, and_ 
    - _return the given value._
  - SPRINTF_ERROR_FREE_MEMORY_AND_RETURN_()
    - _Format an error message,_ 
    - _throw it in an exception,_,
    - _free any allocated memory and_ 
    - _return the error code._
  - PRINT_ERROR_FREE_MEMORY_AND_RETURN_()
    - _Throw error message in an exception,_ 
    - _free any allocated memory and_ 
    - _return the error code._
  - UNSUPPORTED_ARRAY_REQUEST() - _Throw unsupported channel exception and return a blank array._
  - UNSUPPORTED_STRING_ARRAY_REQUEST() - _Throw an unsupported channel exception and return an empty string array._
  - UNSUPPORTED_TABLE_REQUEST() - _Throw an unsupported channel exception and return an empty table._
- aida_pva_memory.h
  - TRACK_ALLOCATED_MEMORY() - _Create tracking variables so that memory can be freed with FREE_MEMORY()_.
  - TRACK_MEMORY() - _Register this newly allocated memory so that it will be freed by FREE_MEMORY()_
  - ALLOCATE_MEMORY() - _Allocate Memory with checking_
  - ALLOCATE_AND_COPY_MEMORY() - _Allocate memory and set its contents to the given buffer of given size_
  - ALLOCATE_STRING() - _Allocate memory for a string and copy the given string into this allocated space_
  - ALLOCATE_FIXED_LENGTH_STRING() - _Allocate space for a fixed length string and copy data from the given string into the newly allocated space_
  - ALLOCATE_MEMORY_AND_ON_ERROR_RETURN_() - _Allocate memory and on error return the given value_
  - ALLOCATE_STRING_AND_ON_ERROR_RETURN_() - _Allocate memory for a string and copy the given string into this allocated space and on error return the given value_
  - ALLOCATE_STRING_AND_ON_ERROR_RETURN_VOID() - _Allocate memory for a string and copy the given string into this allocated space and on error return void_
  - ALLOCATE_FIXED_LENGTH_STRING_AND_ON_ERROR_RETURN_VOID() - _Allocate memory for a fixed length string and copy the given string into this allocated space and on error return void_
  - ALLOCATE_AND_TRACK_FIXED_LENGTH_STRING_AND_ON_ERROR_RETURN_() - _Allocate memory for a fixed length string and copy the given string into this allocated space and on error return the given value_
  - ALLOCATE_AND_TRACK_MEMORY_AND_ON_ERROR_RETURN_() - _Allocate memory and add it to the tracked memory list so that it can be freed automatically later returning the given value on error_
  - ALLOCATE_COPY_AND_TRACK_MEMORY_AND_ON_ERROR_RETURN_() - _Allocate and track memory and set its contents to the given buffer of given size returning the given value on error_
  - ALLOCATE_COPY_AND_TRACK_STRING_AND_ON_ERROR_RETURN_() - _Allocate and track memory and set its contents to the given string returning the given value on error_
  - FREE_TRACKED_MEMORY() - _Free any tracked memory_
- aida_pca_uri.h
  - PMU_STRING_FROM_URI() - _Get a PMU (Primary-Micro-Unit) string from the supplied URI_
  - TO_SLC_NAME() - _Get a slcName from the provided uri and store it in the given variable name_
  - TO_LEGACY_NAME() - _Get a legacy AIDA name from the provided uri and store it in the given variable name_
  - TO_DGROUP() - _Get a display group name from the provided uri and store it in the given variable name_

You don't have to load all of these header files individually, just:
```c
#include "aida_pva.h"
```
