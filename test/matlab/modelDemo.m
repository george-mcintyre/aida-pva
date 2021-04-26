%-*-matlab-*-
function modelDemo
% Demonstrates getting SLC Model data through AIDA.

% Set up for access to AIDA, and create a (required) error issuing 
% instance (helps put messages in cmlog). Create a basic AIDA 
% access object.
aidasetup               
err = Err.getInstance('gregsMatlab');
da = DaObject();

% Get Twiss parameters for one BPM, default DGRP
da.reset();
da.setParam('MODE=7');          % MODE is a required param
tps = da.geta('BPMS:PR02:8032//twiss', 55);                
Twiss = double(tps)

% Get R-matrix (computed) of the BPM, same Mode.
R = double(da.geta('BPMS:PR02:8032//R', 55));
rmatrix=reshape(R,6,6)'         % Note transpose

