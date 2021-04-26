% This MATLAB script demonstrates using Aida to get values out of
% Oracle. This version, like oracleBSAname.m, gets the names of 
% Beam Acquisition System devices. This version includes loads of
% explanation.
%

% To use Aida, initialize it, then tell the error handling mechanism who you are, and
% get then get a DaObject. This is the basic connection to
% Aida. These 3 lines need only be done once. da is reusable.
%
aidainit; 
err = Err.getInstance('AidaOracleDemo');
da = DaObject();

% Get the BSA Names. The specific
% query 'LCLS//BSAnames' results in a vector of 2
% vectors. Specifically, these vectors are a special Aida type
% DaValue [1]. Each subvector corresponds to a column of Oracle SQL
% query that was exected on the Oracle Database by Aida in response
% the Aida query 'LCLS//BSAnames'. You can use APEX [2]
% to see what the Oracle query was, and hence how many columns will be returned. In APEX go to
% Control System Names -> Aida, then set Instance to 'LCLS' and
% attribute to 'BSAnames' and hit GO. You can easily test the result of
% running the query with AidaWeb [3]. 
%
v = da.getDaValue('LCLS//BSAnames'); 


% Extract the columns of v. 1st column in this case are the BSA
% element names. 2nd column is the "sources" ('EPICS' or 'SLC') 
% v is a DaValue, a kind of Java "vector" [4]. In fact, it is a 2D 
% vector of vectors. We can use the methods of vector [4] to extract data from
% v itself, and each subvector. get(0) is getting the vector of BSA
% names. get(1) gets the vector of sources.
%
names = v.get(0);
sources = v.get(1);

% Extract the number of BSA element names returned (the number of
% rows). 
%
Mrows = names.size(); 

% Print the names and sources, one by one. Each element of names
% and sources is a Java String, but matlab takes care of type
% converting between java String and matlab strings.
%
fprintf( 1, 'BSA Element Name \t Source' );
for i=0:Mrows-1
  fprintf( 1, '%s\t%s\n', names.elementAt(i), sources.elementAt(i));
end


