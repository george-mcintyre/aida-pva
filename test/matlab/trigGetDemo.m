function [status] = trigGetDemo(query, beam)

  % [status] = trigGetDemo(query, beam)
  %
  % Usage example:
  %   [status] = trigGetDemo('TRIG:LI31:109//TACT', '1');
  %
  % Aida Trigger Get demonstration function.  This function
  % obtains a string indicating whether a specified trigger
  % device is activated or deactivated on a specified beam code.
  %
  % query - string consisting of a Aida instance name (e.g.,
  % primary:micro:unit), double slashes, and the Aida attribute
  % name TACT.
  %
  % beam - string containing a beam code number.
  %
  % status - returned string indicating whether the specified trigger 
  % device is activated or deactivated on the specified beam code. 
  %
aidainit;
import java.util.Vector;

err = Err.getInstance('trigGetDemo');

da = DaObject();

da.setParam('BEAM', beam);

status = da.get(query, 10);  % 10 is the code for DaValue.Type.STRING

da.reset();

return;
