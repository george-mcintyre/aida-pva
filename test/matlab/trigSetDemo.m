function [status] = trigSetDemo(query, beam, value)

  % [status] = trigSetDemo(query, beam, value)
  %
  % Usage example:
  %   [status] = trigSetDemo('TRIG:LI31:109//TACT', '1', 1);
  %
  % Aida Trigger Set demonstration function.  This deactivates
  % or reactivates a specified trigger device on a specified beam
  % code.  It also returns a status code indicating the resulting
  % status after the operation (0 => device is deactivated on the
  % beam code, 1 => device is activated on the beam code).
  %
  % query - string consisting of a Aida instance name (e.g.,
  % primary:micro:unit), double slashes, and the Aida attribute
  % name TACT.
  %
  % beam - string containing a beam code number.
  %
  % value - integer flag indicating whether the specified trigger 
  % device is to be deactivated or reactivated on the specified
  % beam code (0 => deactivate, 1 => reactivate).
  %
  % status - returned integer status code (0 => device is
  % deactivated, 1 => device is activated).
  %

aidainit;
import java.util.Vector;

err = Err.getInstance('trigSetDemo');

da = DaObject();

da.setParam('BEAM', beam);

inData = DaValue(java.lang.Short(value));

status = da.setDaValue(query, inData);

da.reset();

return;
