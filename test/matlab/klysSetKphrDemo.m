function klysSetKphrDemo(query, value)

  % klysSetKphrDemo(query, value)
  %
  % Usage example:
  %   klysSetKphrDemo('KLYS:LI31:31//KPHR', 60.0);
  %
  % Aida Klystron Set KPHR demonstration function.  This function
  % sets the KPHR value of a specified klystron or subbooster.
  %
  % query - string consisting of a Aida instance name (e.g.,
  % primary:micro:unit), double slashes, and the Aida attribute
  % name KPHR.
  %
  % value - new KPHR float value.
  %
  %

aidainit;
import java.util.Vector;

err = Err.getInstance('klysSetKphrDemo');

da = DaObject();

inData = DaValue(java.lang.Float(value));

da.setDaValue(query, inData);

da.reset();

return;
