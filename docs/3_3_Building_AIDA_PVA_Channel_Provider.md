# 3.3 - Building an AIDA-PVA Channel Provider

1. Make sure all include files have correct CMS header

```c
  /*   **CMS**=C_INC   */
```

2. Copy your code (1 C file and 1 header usually ) to a directory on MCCDEV
3. On VMS, Go into development directory

```shell
MCCDEV> set def [-.<your-module-name>] 
```

4. Compile this source code into DEVLIB.OLB

```shell
MCCDEV> LIBRARY /CREATE AIDASLCDB_DEVLIB.OLB
MCCDEV> CINC *.C

```

5. Link your AIDASLCDB, using SLCLIBS:AIDAPVA.OPT.

```shell
MCCDEV> BUILDTEST AIDASLCDB /DEFAULT

```
