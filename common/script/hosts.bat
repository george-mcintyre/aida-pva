:
: Abs: Defines default hosts for Aida servers
:
: Usage: %AIDASCRIPT%\hosts.bat
:
: Sys: win32
:
: Rem: The symbol assignments defined in this file are used by the 
:      IMR utility "imradmin" when registering CORBA servers to hosts
:      See any imradmin<ServerName>.bat file.
:
: Auth: Greg White (greg@slac.stanford.edu) 08-Feb-2002
:
: Copyright: Stanford Linear Accelerator Center, 2002.
:
: -----------------------------------------------------------------------------
: Mod: 29-Oct-2002, Greg
:      Removed MAGNETHOST and GENERICHOST, added DPTESTHOST and DPTESTHISTHOST
:      Changed all cdntwks17 hosts to slcs6
:      26-Feb-2002, Greg 
:      Changed CAHOST to SLCS1.
:      12-Feb-2002, Greg. 
:      Using setx rather than set.
: =============================================================================
setx IMRHOST   slcs6.slac.stanford.edu
setx EVENTHOST slcs6.slac.stanford.edu
setx NAMEHOST  slcs6.slac.stanford.edu
setx DAHOST    slcs6.slac.stanford.edu
setx DPTESTHOST slcs6.slac.stanford.edu
setx DPTESTHISTHOST slcs6.slac.stanford.edu
setx CAHOST    slcs1.slac.stanford.edu
setx SLCHOST   mccdev.slac.stanford.edu