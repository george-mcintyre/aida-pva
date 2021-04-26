%-*-matlab-*-
function xalModelDemo
% Demonstrates getting XAL Model data through AIDA, using AIDA 
% directly rather than using aidaget.
%
% The AIDA XAL model server recognizes 5 parameters (see test 3). 
% All are optional. Check the help for their default settings.

% Set up for access to AIDA, and create a (required) error issuing 
% instance (helps put messages in cmlog). Create a basic AIDA 
% access object.
aidainit;               
err = Err.getInstance('xalModelDemo');
d = DaObject();

% TEST 1) Simplest example. Get the EXTANT (as calculated from the present
% machine - also known as DATABASE) Twiss parameters for a simple device.
% EXTANT is the default, so no parmater is required.
%
twiss = double(d.geta('BPMS:IN20:781//twiss', 54)) % 54 is the AIDA typecode for
                                                   % "array of double".


% TEST 2) Get the DESIGN R-matrix of a device, from the latest loaded design model.
%
d.setParam('TYPE','DESIGN');
R = d.geta('BPMS:IN20:781//R', 54);
rmatBPM=reshape(double(R),6,6)'                  % Rreshape array to matrix. Note
                                               % transpose
d.reset(); % Clear parameters


% TEST 3) Get DESIGN R-matrix from the MIDDLE of a QUAD. The POSition
% is only meaningful to use on sliced elements, and may be assigned
% 'BEGIN','MID','END'. END is the default. 
%
d.setParam('TYPE','DESIGN');
d.setParam('POS','MID');
R = d.geta('QUAD:IN20:361//R', 54);
rmatQUAD=reshape(double(R),6,6)'                   
d.reset(); % Clear parameters


% TEST 4) Get the (extant) Rmatrix from device A, to device B. This uses 'B'
% parameter. For sliced devices you can specify the position at A with 'POS'
% parameter, and postion in B with POSB parameter.
%
A = 'QUAD:IN20:361//R';
B = 'BPMS:IN20:781';
d.setParam('B',B); 
d.setParam('POS','MID');
% d.setParam('POSB','BEG'); - use if device B is also sliced.
Ratob = reshape(double(d.geta(A, 54)),6,6)'
d.reset(); % Clear parameters

% TEST 5) Get the (extant) Rmatrix from device A, to device B, where B is 
% upstream of A! The returned matrix is inv(rmatB*inv(rmatA))
A = 'BPMS:IN20:781//R';
B = 'QUAD:IN20:361';
d.setParam('B',B); 
d.setParam('POS','MID'); % In this case A is a BPM, POS can be given but is ignored.
d.setParam('POSB','MID'); 
Ratob_bisupstreamofa = reshape(double(d.geta(A, 54)),6,6)'
d.reset(); % Clear parameters

% TEST 6) As test 2 above, but instead of getting the Rmat from the latest
% design model, get it specifically from the model Run Id given. When a RUNID
% is specified, the type of the run given must match the TYPE given - RUNID
% does not specifically override a wrongly supplied TYPE. Eg, if runid 601 is
% a DESIGN run, then TYPE=DESIGN must be supplied.
%
d.setParam('TYPE','DESIGN');
d.setParam('RUNID','601');    
rmatBPM_runId601 = reshape(double(d.geta('BPMS:IN20:781//R', 54)),6,6)'
d.reset(); % Clear parameters

