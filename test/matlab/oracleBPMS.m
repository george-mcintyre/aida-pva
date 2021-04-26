% This MATLAB script demonstrates using Aida to get the names of
% BSA BPMS using AIDA. Since that data is in Oracle, it also
% demonstrates and example of using an Aida query that accesses oracle.
%

% Initialize Aida
aidainit; 
err = Err.getInstance('oracleBSAnames');
da = DaObject();

% Get the BSA Names. This will return a vector of 2 vectors.
v = da.getDaValue('LCLS//BPMSEPICSnames'); 

% Extract the vectors from v. 1st in this case are the BSA
% element names. 2nd is the "sources" ('EPICS' or 'SLC').  
names = char(v.get(0).getStrings());
sources = char(v.get(1).getStrings());

% Extract the number of BSA element names returned (the number of rows)
Mrows = length(names); 

% Print the names and sources, one by one.
fprintf( 1, 'BSA Element Name \t Source' );
for i=1:Mrows
  fprintf( 1, '%s\t%s\n', names(i,:), sources(i,:));
end


