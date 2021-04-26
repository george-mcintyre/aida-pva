function [values, times ] = histDemo(name, starttime, endtime)

% Acquires and plots history of some Aida name between given
% starttime and endtime.
% 
% [values, times] = histDemo(name, starttime, endtime)
%
% name - is the name of an AIDA acquireable history variable. These
% typically end in //HIST, or sometimes //HIST.pepii if from
% EPICS. See aidalist unix script.
% starttime - Beginning time in format 'mm/dd/yyyy hh:mm:ss'
% endtime - Ending time in format as starttime above.
%
% Example:
%
% >> histDemo('PB60:LUMCOR//HIST.pepii','05/06/2005 00:00:00',...
% '05/07/2005 00:00:00');

aidainit; 

err=Err.getInstance();
da = DaObject();
disp 'Acquisition begins ...'
da.setParam('STARTTIME',starttime);  
da.setParam('ENDTIME',endtime);   
hist = da.getDaValue(name);                          
disp 'Acquisition ends successfully'

pts = hist.get(0).size();                        
dblArray = javaArray('java.lang.Double',pts);   
values = double(hist.get(0).toArray(dblArray));                      
StringArray = javaArray('java.lang.String',pts);
times = char(hist.get(1).toArray(StringArray));                      

disp 'Plotting...'
plot(datenum(times),values,'-+');
datetick('x');
xlabel(sprintf('%s - %s',times(1,:),times(end,:)))
title(name);
 
return;

