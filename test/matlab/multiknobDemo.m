function [device, value] = multiknobDemo(filename, relativedelta)

  % [device, value] = multiknobDemo(filename, relativedelta)
  %
  % Usage example:
  %   [device, value] = multiknobDemo('mkb:li02b_xb.mkb', 1.0);
  %
  % Matlab Aida Multiknob demonstration function.  This function
  % sets the devices referenced in a specified multiknob file by
  % knob rotation using a specified relative delta value.  Three
  % loops are made using the specified relative delta value,
  % pausing 5 seconds after every loop.  Then three loops are
  % made using the negative of the specified relative delta value,
  % again pausing 5 seconds after every loop. 
  %
  % filename - filename path specification (including the prefix
  % "mkb:", if file is in the standard multiknob file directory)
  % of a multiknob file.  For example, mkb:li02b_xb.mkb. 
  %
  % relativedelta - relative delta knob rotation (e.g., 1.0).
  %
  % device - name of each device referenced in the specified multiknob 
  % file. 
  %
  % value - secondary value of each device after the knob rotation set
  % operation.
  %
  
aidainit;
import java.util.Vector;

err = Err.getInstance('multiknobDemo');

da = DaObject();
da.setParam('MKB',filename);                   % Required parameter 

inData = DaValue(java.lang.Float(relativedelta));

for i = 1:3,

    v  = da.setDaValue('MKB//VAL', inData);

    devices = Vector(v.get(0));

    disp 'device names =';
    disp (devices);

    values = Vector(v.get(1));

    disp 'secn values =';
    disp (values);

    numDevices = devices.size();

    for j = 1:numDevices,
        device(i) = {devices.elementAt(i-1)};
        value(i) = values.elementAt(i-1);
    end

    disp 'sleeping for 5 seconds';

    pause(5);

    disp ' ';

end

disp '*** reversing sign of relative delta';
disp ' ';

inData2 = DaValue(java.lang.Float(-relativedelta));

for i = 1:3,

    v2  = da.setDaValue('MKB//VAL', inData2);

    devices = Vector(v2.get(0));

    disp 'device names =';
    disp (devices);

    values = Vector(v2.get(1));

    disp 'secn values =';
    disp (values);

    numDevices = devices.size();

    for j = 1:numDevices,
        device(i) = {devices.elementAt(i-1)};
        value(i) = values.elementAt(i-1);
    end

    disp 'sleeping for 5 seconds';

    pause(5);

    disp ' ';

end

da.reset();

return;
