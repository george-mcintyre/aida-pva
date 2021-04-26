function [names] = aidalist( varargin )

% NAMES = AIDALIST(PATTERN) returns a 1-dimensional cellArray of NAMES
% retrievable by Aida, whose names match the given PATTERN.  PATTERN
% may contain both an instance and attribute (see below), separated by
% a space (eg 'XCOR:PR10:9042 X%'), in which case the returned
% cellArray is arranged in blocks of 2, that is, every odd numbered
% element is an instance name, and every even numbered element is the
% name of an attribute of its preceding instance.
%
% NAMES = AIDALIST(INSTANCE,ATTRIBUTE) returns a 2-dimensional cellArray of
% NAMES retrievable by Aida, whose names match the given INSTANCE
% and ATTRIBUTE patterns. Eg aidalist('XCOR:LI%:502','twiss')
%
% Aida names are composed of two parts, the Instance and the
% Attribute. Roughly, the instance is the name of a thing, and the
% Attribute is the name of some value of the thing. For instance,
% EPICS Channel Access records are equivalent to an Instance, and the
% most often used Attribute of is the VAL channel of that record. For
% SLC items, prim:micro:unit are the instance, and secn is the
% attribute. Eg:
%
% aidalist('XCOR:PR10:9042')       - Confirms XCOR:PR10:9042 instance is known
% aidalist('XCOR:PR10:9%')         - All "9 something" XCOR instances in PR10
% aidalist('XCOR:PR10:9042 X%')    - All X attributes of
%                                    XCOR:PR10:9042, returned in a
%                                    1-d cellArray.
% aidalist('XCOR:LI%:502','twiss') - Which linac 502 units have twiss
% aidalist('HB60:DCCT%','%')       - All complete instance names
%                                    and attributes of HB60:DCCT,
%                                    returned in a Nx2 cellArray 
% aidalist('PB60:LUM%','HIST%')    - What LUM related things are historied.
%
% Both instance and attribute are case sensitive. 
%
% You form a name Aida can acquire as <instance>//<attribute>, eg
% PB60:LUMCOR//HIST.pepii. Since aidalist returns a cellArray, you
% often have to convert to standard char array to use it, for
% instance, getting just the first item returned by aidalist:
%  
%  >> names = aidalist('PB60:LUMCO%','VAL')
%
% names = 
%
%    'PB60:LUMCOR'         'VAL'
%    'PB60:LUMCOR:COLL'    'VAL'
%    'PB60:LUMCOR:DAQ'     'VAL'
%
% >> lumi = sprintf('%s//%s',char(names(1,1)),char(names(1,2)))
%
% lumi =
%
% PB60:LUMCOR//VAL
%
% >> da.get(lumi,4)                                            
%
% ans =
% 
%   45.5398
%
msg = nargchk(1,2,nargin);
error(msg);
if nargin == 1 
  query = sprintf('\"aidalist %s\"',varargin{1});
else
  query = sprintf('\"aidalist %s %s\"',varargin{1},varargin{2});
end
[s, l] = system(query);

i=0;
rem = l;
while true
    [str, rem] = strtok(rem);
    if isempty(str), break; end
    i=i+1; 
    if nargin == 1
      names(i) = {str};
    else
      names(i,1) = {str};
      [str, rem] = strtok(rem);
      names(i,2) = {str};
    end
end

return;

