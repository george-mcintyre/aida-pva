#-*-makefile-*-
#==============================================================
#
#  Abs:  Makefile.Host for Channel Archiver Data Server (CHADS).
#
#  Name: Makefile.Host
#
#  Side: 
#
#  Auth: Jan 02, 2003, Ron MacKenzie
#  Rev:  dd-mmm-yyyy, First Last (username)
#
#--------------------------------------------------------------
# MODS:
#   05-May-2004, Bob Hall (rdh)
#     Merged information from dpChads old style Makefile.Host to
#     create Makefile.Host for dpChads.  Used the work done on
#     Jan. 15, 2004 by Greg White for now obsolete dpTestHist
#     as the base for Makefile.Host for dpChads.
#   15-Jan-2004, Greg White (greg)
#     Converted to "new" makefiles (delivering into UDE release system),
#     and hence link to R3.13.6 Archiver library files rather than R3.13.2.
#     Added header.
#   10-Mar-2003, Ron MacKenzie (ronm)
#     Add HOST_OPT=NO because Orbacus 4.1 README says gcc optimization
#     is unreliable.
#   Jan-23-2003, Ron MacKenzie (ronm)
#     Add rule for dpTestHist_skel.  Needed for first make from scratch.
#   Feb-20-2003, Ron MacKenzie 
#     Added Oracle includes and libs.
#   Feb-23-2003, Ron MacKenzie (ronm)
#     Add stuff for linking to Channel Archiver.
#==============================================================
# 
TOP = /afs/slac.stanford.edu/package/epics/R3.14.6/extensions
include $(TOP)/configure/CONFIG

#       Orbacus libs
#
OP_SYS_LDLIBS += $(ORBACUS)/OB-4.2.2/ob/lib/libOB.so
OP_SYS_LDLIBS += $(ORBACUS)/OB-4.2.2/jtc/lib/libJTC.so
OP_SYS_LDLIBS += $(ORBACUS)/OB-4.2.2/event/lib/libCosEvent.so

#	Message system
#
OP_SYS_LDLIBS += $(EXCEPT)/lib/libexcept.so
OP_SYS_LDLIBS += $(ERR)/lib/libErr.so

#
OP_SYS_LDLIBS += /afs/slac.stanford.edu/package/xerces/sun4x_58/2.6.0/lib/libxerces-c.so

# GCC optimization is not releable.
HOST_OPT=NO

# Turn off most warnings
HOST_WARN=NO

# 	Orbacus libs
#
OB_LIBS = $(ORBACUS)/OB-4.2.2/ob/lib
JTC_LIBS = $(ORBACUS)/OB-4.2.2/jtc/lib
COSEVENT_LIBS = $(ORBACUS)/OB-4.2.2/event/lib

#	Message system
#
except_LIBS = $(EXCEPT)/lib
Err_LIBS = $(ERR)/lib

# 	Archiver
#
Storage_LIBS = /afs/slac.stanford.edu/package/epics/extensions/lib/solaris-sparc-gnu
Tools_LIBS = /afs/slac.stanford.edu/package/epics/extensions/lib/O.solaris-sparc-gnu

USR_LDFLAGS_DEFAULT += -L/afs/slac.stanford.edu/package/epics/extensions/lib
USR_LDFLAGS_DEFAULT += -R/afs/slac.stanford.edu/package/xerces/sun4x_58/2.6.0/lib

#GCC = g++-2.95.3
#
# This trick makes it so we don't pick up egcs-2.91.66/libstdc++.a from 
# the higher level epics makefiles.
# The epics makefiles had set OP_SYS_LDLIBS = -lm gcc(to 2.91.66).
#OP_SYS_LDLIBS = -lm

#
#      orbacus and other includes.
USR_INCLUDES += -I$(ERR)/inc
USR_INCLUDES += -I$(EXCEPT)/inc

USR_INCLUDES += -I$(ORBACUS)/OB-4.2.2/event/include
USR_INCLUDES += -I$(ORBACUS)/OB-4.2.2/ob/include
USR_INCLUDES += -I$(ORBACUS)/OB-4.2.2/jtc/include

#
USR_INCLUDES += -I/afs/slac.stanford.edu/package/epics/R3.14.6/base/include
USR_INCLUDES += -I/afs/slac.stanford.edu/package/epics/R3.14.6/base/include/os/solaris
USR_INCLUDES += -I/afs/slac.stanford.edu/package/epics/R3.14.6/extensions/src/archiver/Storage
USR_INCLUDES += -I/afs/slac.stanford.edu/package/epics/R3.14.6/extensions/src/archiver/Tools
#
# Notice below, that this program, which uses Err, needs to link to the
# things that Err links to (like CosEvent). Is there a way around that?
# Is there a way to encapsulate the knowledge of the fact that CosEvent
# is being used within Err and not have his program even know about it?
# Maybe I just don't have LD_LIBRARY_PATH SET right?
#
#PROD_LIBS += CosEvent Err OB JTC except dl posix4 pthread
#PROD_LIBS += CosEvent Err OB JTC except

PROD_LIBS += Storage Tools ca Com 
CosEvent_DIR = $(COSEVENT_LIBS)
Err_DIR = $(Err_LIBS)
OB_DIR = $(OB_LIBS)
JTC_DIR = $(JTC_LIBS)
except_DIR = $(except_LIBS)
Storage_DIR = $(Storage_LIBS)
Tools_DIR = $(Tools_LIB)
#

# Source files
dpChadsServer_SRCS += dpChadsServer.cc
# ChadServer_SRCS += ChadServer.cc
# ChadsClient_SRCS += ChadsClient.cc
#
# IDL Generated files follow. Generated from dpTestHist.idl, daNameServer.idl,
#     and aidaObjectRef.idl
#
SRCS += aidaObjectRef.cc
SRCS += aidaUtil.cc
SRCS += chadsErrlog.cc
SRCS += daNameServer.cc
SRCS += dpChads.cc
SRCS += dpChads_impl.cc
SRCS += dpChads_skel.cc
SRCS += webUtil.cc 
#
#
# We build two executables.  server is the extension.  client is a test
# program.
#
PROD = dpChadsServer

# 
# Execute the corba idl command to compile the idl file.
# This happens before the compile and link of client and server.
#
# Careful: You could do --impl.  but we wrote own based on doing it once.
#
# The following rules cause the IDL command to be executed if the .idl
# file has changed.
#
# The rm command below is needed to force the epics makefiles to re-compile
# the corresponding .cc files.  We want to recompile because they include
# include files created by the idl command.
# 
CHADS_IDL = ../../../../../../../idl
OB_IDL = ${ORBACUS}/OB-4.2.2/ob/idl 
EXCEPT_IDL = /afs/slac/g/cd/soft/ref/package/except/idl 
#
include $(TOP)/configure/RULES
#
#	Add Rules Here:
#
../dpChads.cc : $(CHADS_IDL)/dpChads.idl 
	idl --c-suffix .cc -I$(OB_IDL) -I$(EXCEPT_IDL) --output-dir ../ \
	$(CHADS_IDL)/dpChads.idl
	rm -f dpChadsServer.o dpChads.o dpChads_impl.o \
	dpChads_skel.o

../dpChads_skel.cc : $(CHADS_IDL)/dpChads.idl 
	idl --c-suffix .cc -I$(OB_IDL) -I$(EXCEPT_IDL) --output-dir ../ \
	$(CHADS_IDL)/dpChads.idl
	rm -f dpChadsServer.o dpChads.o dpChads_impl.o \
	dpChads_skel.o
#
../aidaObjectRef.cc : $(CHADS_IDL)/aidaObjectRef.idl 
	idl --c-suffix .cc -I$(OB_IDL) -I$(EXCEPT_IDL) -I$(CHADS_IDL)  \
	--output-dir ../ $(CHADS_IDL)/aidaObjectRef.idl
#
../daNameServer.cc : $(CHADS_IDL)/daNameServer.idl 
	idl --c-suffix .cc -I$(OB_IDL) -I$(EXCEPT_IDL) -I$(CHADS_IDL)  \
	--output-dir ../ $(CHADS_IDL)/daNameServer.idl











