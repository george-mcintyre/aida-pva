function [value] = masterOscGetDemo()

  % value = masterOscGetDemo()
  %
  % Usage example:
  %   value = masterOscGetDemo();
  %
  % Master Oscillator Get demonstration function.  This function
  % obtains a double value containing the measured setting of the
  % Master Oscillator in KHz. 
  %
  % value - the returned double value containing the measured
  % setting of the Master Oscillator in KHz.
  %
aidainit;
import java.util.Vector;

err = Err.getInstance('masterOscGetDemo');

da = DaObject();
value = da.getDaValue('MASTEROSC//VAL');

da.reset();

return;
