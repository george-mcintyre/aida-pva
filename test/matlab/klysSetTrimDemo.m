function [phas] = klysSetTrimDemo(query, value)

  % [phas] = klysSetTrimDemo(query, value)
  %
  % Usage example:
  %   [phas] = klysSetTrimDemo('KLYS:LI31:31//PDES', 90.0);
  %
  % Aida Klystron Set Trim demonstration function.  This function
  % sets the PDES value of a specified klystron or subbooster and
  % trims its phase.  It also returns the PHAS readback value
  % after the set PDES and trim phase operation.
  %
  % query - string consisting of a Aida instance name (e.g.,
  % primary:micro:unit), double slashes, and the Aida attribute
  % name PDES.
  %
  % value - new PDES float value.
  %
  % phas - the PHAS readback value after the set PDES and trim
  % phase operation.
  %

aidainit;
import java.util.Vector;

err = Err.getInstance('klysSetTrimDemo');

da = DaObject();

inData = DaValue(java.lang.Float(value));

phas = da.setDaValue(query, inData);

da.reset();

return;
