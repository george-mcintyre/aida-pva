@echo on
:==============================================================
:
: Name: /afs/slac/package/aida/common/script/aidaSetEnv.bat
:
: Abs:  Environment variables needed for aida on Win32
:
: Usage: example:
:         /afs/slac/package/aida/common/script/aidaSetEnv.bat
:
: Rem:
: **** IMPOTRANT NOTE ****
:       
:       This file is NOT MAINTAINED. Presently Aida does not run on Windows.
: 
: Facility: Aida environment setup
:
: Auth:  08-Feb-2002, Greg White
: Rev:   dd-mmm-1990, Whoever. 
:
:==============================================================
: Mods:  29-Oct-2002, Greg White
:        Changed AIDA, AIDAROOT, AIDASCRIPT to new AIDA location in CD_SOFT
:        13-Feb-2002, Greg
:        Added IMRDBDIR, used by StartIMRAdmin.bat
:============================================================== 

set AIDA=I:\g\cd\soft\ref\package\aida
set AIDAROOT=I:\g\cd\soft\ref\package\aida
set AIDASCRIPT=I:\g\cd\soft\ref\package\aida\common\script
set ORBACUS=I:\package\iona\orbacus\prod\win32
set IMRDBDIR=C:\aida
: Reflect these in the global environment variables
setx AIDA %AIDA%
setx AIDAROOT %AIDAROOT%
setx AIDASCRIPT %AIDASCRIPT%
setx ORBACUS %ORBACUS%
setx IMRDBDIR %IMRDBDIR%

:
::: There is currently no Win32 equivalent of the aidaSetCVS.sh file
: source ${AIDASCRIPT}/aidaSetCVS.sh
:
REM can't put set PATH here becaus eit appears every time this file executes
REM path is just added to.
REM setx PATH %PATH%;.;%ORBACUS%\ooc\bin;%AIDASCRIPT%
:
set CLASSPATH=.;%AIDAROOT%;%AIDA%;%ORBACUS%\Ooc\lib\OB.jar;%ORBACUS%\Ooc\lib\OBEvent.jar;%ORBACUS%\Ooc\lib\OBIMR.jar;%ORBACUS%\Ooc\lib\OBUtil.jar;%AIDA%\tool\win32\jdbc\classes111.zip;
setx CLASSPATH %CLASSPATH%
 
:
CALL %AIDASCRIPT%\hosts.bat
exit
