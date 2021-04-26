% This MATLAB script demonstrates using Aida to get values out of
% Oracle. See methods of DaValue, for hwo to extract data from one.
%

% Initialize Aida
aidainit; 
err = Err.getInstance('oracleBSAnames');
da = DaObject();

% Get the BSA Names. This will return a vector of 2 vectors.
v = da.getDaValue('LCLS//BSAnames'); 

% Extract the vectors from v. 1st in this case are the BSA
% element names. 2nd is the "sources" ('EPICS' or 'SLC').  
names = v.get(0);
sources = v.get(1);

% Extract the number of BSA element names returned (the number of rows)
Mrows = names.size(); 

% Print the names and sources, one by one.
fprintf( 1, 'BSA Element Name \t Source' );
for i=0:Mrows-1
  fprintf( 1, '%s\t%s\n', names.elementAt(i), sources.elementAt(i));
end


