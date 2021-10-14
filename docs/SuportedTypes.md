# Supported Data Types

## Scalar Types

* **BOOLEAN**            - Getter for this Channel returns a `PVStructure` that conforms to the `NTScalar` norm
  containing a single `PVBoolean`
* **BYTE**               - Getter for this Channel returns a `PVStructure` that conforms to the `NTScalar` norm
  containing a single `PVByte`. Note there is no CHAR so clients are required to use BYTE and marshal the results
  appropriately
* **SHORT**              - Getter for this Channel returns a `PVStructure` that conforms to the `NTScalar` norm
  containing a single `PVShort`
* **INTEGER**            - Getter for this Channel returns a `PVStructure` that conforms to the `NTScalar` norm
  containing a single `PVInteger`
* **LONG**               - Getter for this Channel returns a `PVStructure` that conforms to the `NTScalar` norm
  containing a single `PVLong`
* **FLOAT**              - Getter for this Channel returns a `PVStructure` that conforms to the `NTScalar` norm
  containing a single `PVFloat`
* **DOUBLE**             - Getter for this Channel returns a `PVStructure` that conforms to the `NTScalar` norm
  containing a single `PVDouble`
* **STRING**             - Getter for this Channel returns a `PVStructure` that conforms to the `NTScalar` norm
  containing a single `PVString`

## Scalar Array Types

* **BOOLEAN_ARRAY**      - Getter for this Channel returns a `PVStructure` that conforms to the `NTScalarArray` norm
  containing a `PVBooleanArray` array
* **BYTE_ARRAY**         - Getter for this Channel returns a `PVStructure` that conforms to the `NTScalarArray` norm
  containing a `PVByteArray` array. Note there is no CHAR_ARRAY so clients are required to use BYTE_ARRAY and marshal
  the results appropriately
* **SHORT_ARRAY**        - Getter for this Channel returns a `PVStructure` that conforms to the `NTScalarArray` norm
  containing a `PVShortArray` array
* **INTEGER_ARRAY**      - Getter for this Channel returns a `PVStructure` that conforms to the `NTScalarArray` norm
  containing a `PVIntegerArray` array
* **LONG_ARRAY**         - Getter for this Channel returns a `PVStructure` that conforms to the `NTScalarArray` norm
  containing a `PVLongArray` array
* **FLOAT_ARRAY**        - Getter for this Channel returns a `PVStructure` that conforms to the `NTScalarArray` norm
  containing a `PVFloatArray` array
* **DOUBLE_ARRAY**       - Getter for this Channel returns a `PVStructure` that conforms to the `NTScalarArray` norm
  containing a `PVDoubleArray` array
* **STRING_ARRAY**       - Getter for this Channel returns a `PVStructure` that conforms to the `NTScalarArray` norm
  containing a `PVStringArray` array

## Structures

* **TABLE**              - Getter or setter returns a `PVStructure` that conforms to the `NTTable` norm containing a set
  of homogenous congruent vectors which can be any of the `PVScalarArray` types supported under Scalar Array Types
  above.

## Special configuration only types.

* **NONE**               - Means that this getter or setter is not supported
* **VOID**               - Means that this setter does not return a value (only valid for setters)
* **ANY**                - Getter or setter returns any type defined by the mandatory accompanying `TYPE` argument, for
  setters this can only be VOID or TABLE
* **SCALAR**             - Constrains the `TYPE` parameter to be set to any scalar type or `TABLE`
* **SCALAR_ARRAY**       - Constrains the `TYPE` parameter to be set to any scalar array type or `TABLE`
