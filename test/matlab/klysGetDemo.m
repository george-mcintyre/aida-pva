function [status] = klysGetDemo(query, beam, dgrp)

  % [status] = klysGetDemo(query, beam, dgrp)
  %
  % Usage example:
  %   [status] = klysGetDemo('KLYS:LI31:31//TACT', '8', 'DEV_DGRP');
  %
  % Aida Klystron Get demonstration function.  This function
  % obtains a string indicating whether a specified klystron is
  % activated or deactivated on a specified beam code.
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
  % status - returned string indicating whether the specified klystron
  % is activated or deactivated on the specified beam code. 
  %
aidainit;
import java.util.Vector;

err = Err.getInstance('klysGetDemo');

da = DaObject();

da.setParam('BEAM', beam);

da.setParam('DGRP', dgrp);

status = da.get(query, 10);  % 10 is the code for DaValue.Type.STRING

da.reset();

return;
