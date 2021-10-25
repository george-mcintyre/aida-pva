# Building AIDA-PVA Native Channel Provider
 
- [ ] Make sure all include files have correct CMS header
```c
  /*   **CMS**=C_INC   */
```
- [ ] Copy your code (1 C file and 1 header usually ) to a directory on MCCDEV
- [ ] On VMS, Go into development directory
```shell
MCCDEV> set def [-.<your-module-name>] 
```
- [ ] Compile this source code into DEVLIB.OLB
```shell
MCCDEV> LIBRARY /CREATE AIDASLCDB_DEVLIB.OLB
MCCDEV> CINC *.C

```
- [ ] Link your AIDASLCDB, using SLCLIBS:AIDAPVA.OPT.
```shell
MCCDEV> BUILDTEST AIDASLCDB /DEFAULT

```
