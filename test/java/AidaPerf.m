%
% Analyze timing data files
% Robert C. Sass    16-Jul-2004
%
datafile = input ('Enter timing data file to analyze: ', 's'); 
fin = fopen (datafile,'r');
%Read file description and array of:
% time of first data collection, test comment and test data
desc = fgets (fin);
ttime = fgets(fin);
tcomnt = fgets(fin);
ttimes = [];
tcomnts = [];
tarray = [];
while (ttime ~= -1)
    ttimes = [ttimes; ttime];
    tcomnts = [tcomnts; tcomnt];
    [a] = fscanf (fin, '%g\n');
    tarray = [tarray; a'];
    ttime = fgets(fin);
    tcomnt = fgets(fin);
end
ptarray = tarray';
%
% Make subplots (assume even number of tests)
%
as = size(ptarray);
nplots = as(2);
nplots2 = nplots/2;
%Add better calc of subplot params
for i=1:nplots
    pdata = ptarray(:,i);
    subplot (nplots, nplots2, i);
    plot (pdata);
%    semilogy (pdata);
    tnum = sprintf('%i',i);
    tlabel = ['Trial ' tnum ' on ' ttimes(i,:) ' ' tcomnts(i,:)]; 
    xlabel(tlabel);
    ylabel('Time milliseconds');
    smin = sprintf('%2.1f',min(pdata));
    smax = sprintf('%2.1f',max(pdata));
    smean = sprintf('%2.1f',mean(pdata));
    smedian = sprintf('%2.1f',median(pdata));
    sstd = sprintf('%2.1f',std(pdata));
    stats = ['Min ' smin ' Max ' smax ' Mean ' smean ' Median ' smedian ' Std Dev ' sstd];
    title(stats);
end
fighand = gcf;
set (fighand,'Name',desc);
% Add jpeg output
%print -djpeg -r100 test.jpg

