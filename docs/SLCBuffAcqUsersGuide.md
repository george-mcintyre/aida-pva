This page provides documentation for users and programmers of AIDA's SLC Buffered Data Service. This data service gives access to the so called "Buffered BPM Data" acquisition facilities of the SLC Control System, in the SLAC accelerator complex. Buffered BPM data, is the name given to the pulse-by-pulse data acquisition system, in which a user names the devices whose values they want to be acquired, for how many beam pulses, and the service then returns a table of results - the x and y position values and the Tmit, given in one row per device, per pulse.

See Also: Basic Users Guide to Aida, REF_AIDASHR; AIDA SLC Peer Programmers Guide

Users Guide
This section describes what an AIDA user should know about accessing SLC Buffered Data through AIDA. For general information on using AIDA see Basic Users Guide to Aida, and the Aida javadoc, in particular the classes DaObject and DaReference, in aida.lib.da which form Aida's programming interface. One can also use Aida through Matlab.

SUMMARY
The SLC Buffered Acquisition Service (dpslcbuff) gets SLC congrol system pulse synchronized "Buffered Data", for the following device types: BPM x (mm), y (mm), tmit (num particles estimated); TORO tmit; KLYS and SBST phase; GAPM data, and status information for these, as specified in the user's query. The acquisition is subject to the so-called measurement definition "BPMD" given, over the given "NRPOS" parameter sequential pulses.
A table is returned. Since the kind of data one gets back from these device types is all a little different, some elements of the table of data you get back should be ignored.

BPMS = x, y, tmit in columns 3, 4 and 5 respectively
TORO = tmit in column 5
KLYS = phase in column 3
SBST = phase in column 3
GAPM = data, in column 3.
For all, name and pulse id are in column 1 and 2, and stat and goodmeas are in columns 6 and 7:

E.g:
[fphysics@facet-srv01 ~ ]$ aidaget NDRFACET//BUFFACQ -DBPMD=57 -DNRPOS=5 -DDEV1=KLYS:LI03:31 \
-DDEV2=SBST:LI03:001 -DDEV3=BPMS:LI02:501 -DDEV4=TORO:LI20:2040

  TORO:LI20:2040  31330          0.0          0.0   3.39403981E9  1  1
  TORO:LI20:2040  31690          0.0          0.0   3.39403981E9  1  1
  TORO:LI20:2040  32050          0.0          0.0   3.73344358E9  1  1
  TORO:LI20:2040  32410          0.0          0.0    3.5071744E9  1  1
  TORO:LI20:2040  32770          0.0          0.0   3.62030899E9  1  1
   BPMS:LI02:501  31330     0.331939  -0.20821595    9.2273582E9  1  1
   BPMS:LI02:501  31690   0.32889742  -0.19808215    9.5858289E9  1  1
   BPMS:LI02:501  32050    0.2943182  -0.18990237  1.02538885E10  1  1
   BPMS:LI02:501  32410   0.43009153  -0.19211863    9.4554767E9  1  1
   BPMS:LI02:501  32770   0.33982033  -0.18054654    9.3577114E9  1  1
     SBST:LI03:1  31330    21.059832          0.0            0.0  1  0
     SBST:LI03:1  31690    20.382906          0.0            0.0  1  0
     SBST:LI03:1  32050    21.436647          0.0            0.0  1  0
     SBST:LI03:1  32410    20.955797          0.0            0.0  1  0
     SBST:LI03:1  32770    21.267794          0.0            0.0  1  0
    KLYS:LI03:31  31330    0.3280487          0.0            0.0  1  0
    KLYS:LI03:31  31690  -0.44467163          0.0            0.0  1  0
    KLYS:LI03:31  32050    0.4645996          0.0            0.0  1  0
    KLYS:LI03:31  32410   0.10968016          0.0            0.0  1  0
    KLYS:LI03:31  32770   0.51013184          0.0            0.0  1  0
Status and limitations	
Presently the service acquires only named devices in the query, each given by a "DEVn" or "BPMn" (it doesn't matter which you use) parameter; there is no support for simply getting all the BPMs in the DGRP implied by the AIDA entity - such as all those in NDRFACET.

Plan	No active plans for extension, but could add the ability to acquire all devices in the DGRP implied by the AIDA entity name.
Names	AIDA entities for making buffered acquisitions, are composed of DGRP name, with attribute == "BUFFACQ" (see INSTANCES and ATTRIBUTES below). So, see aidalist % BUFFACQ for an up-to-date list. The following names exist at the time of writing:
       $ aidalist % BUFFACQ

       E163BMLN                                           BUFFACQ
       ELECEP01                                           BUFFACQ
       INJ_ELEC                                           BUFFACQ
       INJ_POSI                                           BUFFACQ
       NDRFACET                                           BUFFACQ
       SCAVPOSI                                           BUFFACQ
       TAXXALL                                            BUFFACQ
       SDRFACET                                           BUFFACQ

EXAMPLES
Schematic	Schematically, the syntax of an AIDA Buffered acquisition is an AIDA entity name (required), plus parameters giving a BPMD number (required), number of pulses (1 pulse is the default), and device names given in unique parameter names. The parameter names must begin "DEV" or "BPM" and at least 1 must be given.
      aidaget <DGRP-name>//BUFFACQ BPMD=n [NRPOS=m] {(BPM<i>|DEV<i>)=<device-name>}+1
An example using aidaget at the command line:
[flora03]:u/cd/greg> aidaget NDRFACET//BUFFACQ -DBPMD=57 -DNRPOS=10 -DDEV1=GAPM:LI18:930 -DDEV2=BPMS:LI11:501 
  BPMS:LI11:501  51778  0.14952832    0.04818424  1.84745226E10   1  1
  BPMS:LI11:501  51850  0.15105376  -0.007751522   1.8179285E10   1  1
  BPMS:LI11:501  51886  0.15066777   0.026444921  1.82530949E10   1  1
            . . . <17 rows snipped>
Java	Java example (see $CD_SOFT/ref/package/aida/test/java/DpSlcBuffTests.java for more examples. See in particular the method printffDaValue( DaValue v ) for how to extract each of the result vectors from the DaValue object returned (v in the example below):
	da = new DaObject();
	String query  = "NDRFACET//BUFFACQ";
	da.setParam("BPMD","57");             /* BPMD is a required parameter */
	da.setParam("NRPOS","180");           /* Number of pulses */ 
	da.setParam("BPM1","BPMS:LI11:501");  /* 3 bpms to acquire given */
	da.setParam("BPM2","BPMS:LI11:601");
	da.setParam("BPM3","BPMS:LI11:701");
	DaValue v = da.getDaValue(query);     /* Acquire Buffered data */
	System.out.println( v.toString() );   /* Print the results */
      
Having set up the DaObject (da in this example), you can execute da.getDaValue(query) repeatedly to make the same acquisition. Reset the da object using da.reset().

Matlab	
	% The following is matlab code to use AIDA to acquire 2 bpms, 
        % for the same 10 pulses, on measurement definition (BPMD) 57.
	aidainit;

        % Set up the acquisition
	d=DaObject;
	d.setParam('BPMD','57');
	d.setParam('NRPOS','10');
	d.setParam('BPM1','BPMS:LI02:501');
	d.setParam('BPM2','BPMS:DR12:334');

	% Do the acquisition
	buffdata = d.getDaValue('NDRFACET//BUFFACQ');  

	% Extract the results from the returned buffdata DaValue java vector.
	pts = buffdata.get(0).size();
	StringArray = javaArray('java.lang.String',pts);
	namesStrings = char(buffdata.get(0).toArray(StringArray));
	% Then each bpm name is a 2 char matrix - namesString(1,:) is the whole name of the 1st BPM.
	pulses=[];                            
	pulses=[pulses; cell2mat(cell(buffdata.get(1).toArray()))]
	xs=[];
	xs=[xs; cell2mat(cell(buffdata.get(2).toArray()))]  
	ys=[];                                                    
	ys=[ys; cell2mat(cell(buffdata.get(3).toArray()))]
	tmits=[];
	tmits=[tmits; cell2mat(cell(buffdata.get(4).toArray()))]
	stats=[];                                               
	stats=[stats; cell2mat(cell(buffdata.get(5).toArray()))]
	goodmeas=[];                                            
	goodmeas=[goodmeas; cell2mat(cell(buffdata.get(6).toArray()))]
      

AIDA ENTITIES
AIDA Entities are the names, or queries, known to AIDA. An entity is composed of an instance part, followed by "//", followed by the attribute part. The instance should describe an object, the attribute should describe a property of that object. This table formally describes the entities associated with the SLC Buffered Acquisition service, and the data that can be acquired with them.

INSTANCES and ATTRIBUTES
Instance Type	Description
Buffered BPM Data	Syntax	<dgrp-mnemonic-name>//BUFFACQ
Examples	
    NDRFACET//BUFFACQ
    INJ_ELEC//BUFFACQ
    INJ_POSI//BUFFACQ
    TAXXALL//BUFFACQ
      
See aidalist % BUFFACQ for a complete list.
Attributes
Attribute	Description
BUFFACQ	Makes a buffered acq reading of each given device, in the dgrp specified in the "instance" part of the query, under the BPM Measurement Definition specified by the BPMD (required) parameter, at each beam crossing for the number of pulses given by NRPOS.
Methods	Name*	Returns
getDaValue(q)	Returns a DaValue containing a heterogenous vector of 7 homogenous congruent vectors. Ie, a table. Below is definition of the columns for a BPM device. See the summary above for the column interpretations for KLYS, SBST and TORO.
name of each BPM (string)
pulse id (integer)
x offsets (floating pt mm)
y offsets (floating pt mm)
tmits (floating pt coulomb)
STATs (32 bit field)
goodmeas (boolean 1 for good, or 0 for bad)
Parameters	Name	
Req/
Opt

Syntax	Semantics
BPMD	
req

Integer Bpm Measurement Definition number.	This specifies the timing profile of the acquisition. Each BPMD corresponds to exactly one DGRP. To find likely BPMDs, go to the BPM Device panel of a SCP and hit the 'HELP' button, then select the button corresponding to the bpm orbit you would have wanted to see. The button name for that acquisition, as displayed in the help, is the BPMD for the selected orbit. Use that number for this parameter. Eg BPMD=57 is the primary BPMD for the DGRP NDRFACET, and so will be the main one used for FACET.
NRPOS	opt	1=<n<=2800. Default is 1.	In theory, this parameter can range up to 2800, but some things must be borne in mind. Firstly, roughly speaking the minimum time it could take a buffered acquisition to complete, is NRPOS/machine-rate-in-Hz. But, the SCP in fact adds time to this theroetical minimum heuristically, to give the micros time to prep the acquisition. Secondly, AIDA's network timeout ranges between 60 and 90 seconds depending on platform, so if your acquisition takes too long, the client will timeout. So, in practice, limit your NRPOS to numbers that will result in relatively short acquistions, plan for about 30 seconds max.
BPMi or DEVi	req	BPM, TORO, GAPM, KLYS or SBST device name	Parameters whose name begin "BPM" or "DEV" are used to pass the names of the devices from which a user would like the Buffered service to acquire data. The parameter name must begin BPM or DEV and be followed by an integer, and be unique in the parameter list used. See the examples above.
