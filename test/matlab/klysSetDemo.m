function [status] = klysSetDemo(query, beam, dgrp, value)

  % [status] = klysSetDemo(query, beam, dgrp, value)
  %
  % Usage example:
  %   [status] = klysSetDemo('KLYS:LI31:31//TACT', '8', 'DEV_DGRP', 1);
  %
  % Aida Klystron Set demonstration function.  This deactivates
  % or reactivates a specified klystron on a specified beam code.
  % It also returns a status code indicating the resulting status 
  % after the operation (the bit interpretation of the status code
  % can be found in the VMS file slctxt:linklysta.txt).
  %
  % query - string consisting of a Aida instance name (e.g.,
  % primary:micro:unit), double slashes, and the Aida attribute
  % name TACT.
  %
  % beam - string containing a beam code number.
  %
  % dgrp - string containing a display group name to which the
  % specified klystron belongs.  For development simulated klystrons,
  % this display group is DEV_DGRP.  For production klystrons, this
  % display group can be LIN_KLYS.
  %
  % value - integer flag indicating whether the specified klystron
  % is to be deactivated or reactivated on the specified beam code
  % (0 => deactivate, 1 => reactivate).
  %  
  %
  % status - returned integer status code.  See the VMS file
  % slctxt:linklysta.txt for the bit interpretation of this code).
  %

aidainit;
import java.util.Vector;

err = Err.getInstance('klysSetDemo');

da = DaObject();

da.setParam('BEAM', beam);

da.setParam('DGRP', dgrp);

inData = DaValue(java.lang.Short(value));

status = da.setDaValue(query, inData);

da.reset();

return;
