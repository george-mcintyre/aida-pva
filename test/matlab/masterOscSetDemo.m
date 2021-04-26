function [value] = masterOscSetDemo(relativesetting)

  % value = masterOscSetDemo(relativesetting)
  %
  % Usage example:
  %   value = masterOscSetDemo(0.328);
  %
  % Master Oscillator Set demonstration function.  This function
  % sets the Master Oscillator value to a specified setting in 
  % units of relative frequency.  It returns the new measured
  % setting of the Master Oscillator in KHz.
  %
  % relativesetting - a float value containing the specified
  % setting in units of relative frequency.
  %
  % value - the returned double value containing the measured
  % setting of the Master Oscillator in KHz.
  %
aidainit;
import java.util.Vector;

err = Err.getInstance('masterOscSetDemo');

da = DaObject();

inData = DaValue(java.lang.Float(relativesetting));

value = da.setDaValue('MASTEROSC//VAL', inData);

da.reset();

return;
