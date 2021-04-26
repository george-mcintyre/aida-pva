% This MATLAB script demonstrates using Aida to get details of LCLS
% BSA aware devices out of Oracle. 
%
% For a simpler example see oracleBSAnames.m. This example shows 
% efficient conversion of the data vectors returned to matlab native types.

% Initialize Aida
aidainit; 
err = Err.getInstance('oracleBSAelements');
da = DaObject();

% Get the BSA Names. For this specific query, this will return a 
% vector of 6 vectors.
v = da.getDaValue('LCLS//BSA.elements.byZ'); 

% Extract the number of BSA element names returned (the number of rows)
Mrows = v.get(0).size(); 

% Extract all the data, Strings to matlab char arrays, the double
% array of Zs'is just to an array of matlab doubles. 
elements = char(v.get(0).getStrings());                      
beamlines = char(v.get(1).getStrings());                      
areas = char(v.get(2).getStrings());                      
Zs = v.get(3).getDoubles();                      
root_names = char(v.get(4).getStrings());                      
sources = char(v.get(5).getStrings());                      

% Print all the data returned as a table.
fprintf( 1, 'BSA Element \tBeamline \tArea \tLinac Z \tRoot Name \tSource\n' );
for i=1:Mrows
  fprintf( 1, '%s\t%s\t%s\t%g\t%s\t%s\n', elements(i,:), ...
	   beamlines(i,:), areas(i,:), Zs(i), ...
	   root_names(i,:), sources(i,:));
end


