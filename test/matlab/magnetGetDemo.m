function [name, value] = magnetGetDemo(query)

  % [name, value] = magnetGetDemo(query)
  %
  % Usage example:
  %   [name, value] = magnetGetDemo('DEV_DGRP//XCOR:BDES');
  %
  % Matlab Aida Magnet Get demonstration function.  This function
  % obtains primary/micro/unit strings and BDES/VDES values for a
  % specified display group, primary, and secondary name specified
  % in a query string.
  %
  % query - string consisting of a display group, primary, and
  % secondary name.
  %
  % name - primary/micro/unit name string for each device associated
  % with the specified query string.
  %
  % value - secondary value (BDES or VDES) for each primary/micro/unit
  % name string returned in array "name".
  %
aidainit;
import java.util.Vector;

err = Err.getInstance('magnetGetDemo');

da = DaObject();
v = da.getDaValue(query);

names = Vector(v.get(0));

values = Vector(v.get(1));

numPairs = names.size();

for i = 1:numPairs,
    name(i) = {names.elementAt(i-1)};
    value(i) = values.elementAt(i-1);
end

da.reset();

return;
