package test.java; 

// The following librarie must me imported by all AIDA clients 
import edu.stanford.slac.aida.lib.da.*; 
import edu.stanford.slac.aida.lib.util.common.*;  // DaValue

// The following must be imported for AIDA clients that want to use dpChads.
// because that's where the histData helper classes are.
import edu.stanford.slac.aida.dp.dpChads.*;

// For manipulating DaValue objects (Vector and Enumeration)
import java.util.*;
 
/** 
 * The Channel Archiver AIDA Data Provider Test Suite.  
 * 
 * @author  17-Mar-2003, Ron MacKenzie. Based on Greg's DpCaTests.java
 * @version 02-Feb-2011, Bob Hall.  Added a test for the new FACET Aida Channel Archiver server.
 * @version 17-Nov-2008, Bob Hall.  Changed test 1 to specify a recent date/time for the default index.
 * @version 02-Aug-2008, Bob Hall.  Added display of new isdst field.
 * @version 31-Jul-2008, Bob Hall.  Added test 11 for retrieval of LCLS data using INDEX parameter. 
 * @version 10-Sep-2007, Bob Hall.  Added test 10 for waveform example using new count field.
 * @version 17-Aug-2007, Bob Hall.  Added display of new pulseid field.
 * @version 10-Mar-2007, Bob Hall.  Added LCLS test. 
 * @version 25-Jul-2006, Bob Hall.  Added display of new unixtime field. 
 * @version 29-Apr-2005, Bob Hall.  Condensed tests. 
 * @version 14-Apr-2005, Bob Hall.  Added tests 9 and 10 to get data using getDaValue
 *                                  from a "da" object.
 * @version 01-Oct-2004, Greg White Added test 8, to verify you can successfully
 *                                  get Chads data into a DaValue.
 * @version 21-May-2004, Bob Hall.  Changed "attribute" part of PV specification
 *                                  to be "HIST.subsystem" (where subsystem is
 *                                  pepii, nlcta, or pack) instead of "HIST".
 * @version 06-May-2004, Bob Hall.  Added two tests and, as a consequence,
 *                                  renumbered the other tests.  New test 2
 *                                  obtains data for a NLCTA PV.  New test 3
 *                                  obtains data for a 8-Pack PV.  Test 4
 *                                  is what was previously called test 2:
 *                                  it prompts for PV and time.  
 * @version 03-Apr-2003, Ron MacKenzie. Add test 2 which is same as test 1
 *                                      except that it prompts for PV and time.
 */ 
public class DpChadsTests 
{     
		private DaObject da; 
     
		public DpChadsTests(String[] args) 
		{ 
				// DpChads is a C++ server.
				// We don't currently support DaObject in C++, but lets create one
				// here anyway for the future when we do.
				try 
				{ 
						da = new DaObject(args);
				} 
				catch ( Exception e ) 
				{ 
						System.err.println( e.getMessage() ); 
						e.printStackTrace(); 
				} 
		}
 
		/** 
		 * Acquire DOUBLE type EPICS PV, giving type, through DaServer, with reference
		 * for a LCLS PV.
		 */  
		private void simpleTest01() 
		{ 
				System.out.println( "====================\n TEST 01  \n--------------------"); 

				try
				{
						System.out.println("Getting data for LCLS PV VPIO:IN20:111:VRAW//HIST.lcls");
						String query = "VPIO:IN20:111:VRAW//HIST.lcls";
						DaReference r = new DaReference(query, da);


						r.setParam("STARTTIME", "11/17/2008 10:00:00");
						r.setParam("ENDTIME",   "11/17/2008 10:10:00");

						histData f = histDataHelper.extract(r.getAny());
						System.out.println( "# entries returned = " + 
											f.d.length);
             
						System.out.println(query);      
						if (f.d.length > 0) {
								for ( int ii = 0; ii < f.d.length ; ii++ ) {
										System.out.println( 
												"\t" + f.s[ii] + "\t" + f.d[ii]  + "\t" + 
												f.repCnt_l[ii] + "\t" + f.unixtime[ii] + "\t" + f.isdst[ii] + "\t" + f.pulseid[ii] + "\t" + f.count[ii]); 
								}
						}
				}
				catch (Exception e)
				{
						System.err.println( e.getMessage() );
				}

				return; 
		}

		/** 
		 * Acquire DOUBLE type EPICS PV, giving type, through DaServer, with reference
		 * for a FACET PV.
		 */  
		private void simpleTest02() 
		{ 
				System.out.println( "====================\n TEST 02  \n--------------------"); 

				try
				{
						System.out.println("Getting data for FACET PV LI20:ASTS:VP10//HIST.facet");
						String query = "LI20:ASTS:VP10//HIST.facet";
						DaReference r = new DaReference(query, da);


						r.setParam("STARTTIME", "01/31/2011 10:00:00");
						r.setParam("ENDTIME",   "01/31/2011 10:10:00");

						histData f = histDataHelper.extract(r.getAny());
						System.out.println( "# entries returned = " + 
											f.d.length);
             
						System.out.println(query);      
						if (f.d.length > 0) {
								for ( int ii = 0; ii < f.d.length ; ii++ ) {
										System.out.println( 
												"\t" + f.s[ii] + "\t" + f.d[ii]  + "\t" + 
												f.repCnt_l[ii] + "\t" + f.unixtime[ii] + "\t" + f.isdst[ii] + "\t" + f.pulseid[ii] + "\t" + f.count[ii]); 
								}
						}
				}
				catch (Exception e)
				{
						System.err.println( e.getMessage() );
				}

				return; 
		}
     
		/** 
		 * Acquire DOUBLE type EPICS PV, giving type, through DaServer, with reference
		 * for a PEPII PV.
		 */  
		private void simpleTest03() 
		{ 
				System.out.println( "====================\n TEST 03  \n--------------------"); 

				try
				{
						System.out.println("Getting data for PEPII PV HR81:CAV2:VACM//HIST.pepii");
						String query = "HR81:CAV2:VACM//HIST.pepii";
						DaReference r = new DaReference(query, da);


						r.setParam("STARTTIME", "08/27/2007 18:00:00");
						r.setParam("ENDTIME",   "08/27/2007 18:10:00");

						histData f = histDataHelper.extract(r.getAny());
						System.out.println( "# entries returned = " + 
											f.d.length);
             
						System.out.println(query);      
						if (f.d.length > 0) {
								for ( int ii = 0; ii < f.d.length ; ii++ ) {
										System.out.println( 
												"\t" + f.s[ii] + "\t" + f.d[ii]  + "\t" + 
												f.repCnt_l[ii] + "\t" + f.unixtime[ii] + "\t" + f.isdst[ii] + "\t" + f.pulseid[ii] + "\t" + f.count[ii]); 
								}
						}
				}
				catch (Exception e)
				{
						System.err.println( e.getMessage() );
				}

				return; 
		}

		/** 
		 * Acquire DOUBLE type EPICS PV, giving type, through DaServer, with reference
		 * for a NLCTA PV.
		 */  
		private void simpleTest04() 
		{ 
				System.out.println( "====================\n TEST 04  \n--------------------"); 

				try
				{
						System.out.println("Getting data for NLCTA PV TRS1:AS2:LOST:PAVE//HIST.nlcta");
						String query = "TRS1:AS2:LOST:PAVE//HIST.nlcta";
						DaReference r = new DaReference(query, da);


						r.setParam("STARTTIME", "08/27/2007 18:00:00");
						r.setParam("ENDTIME",   "08/27/2007 18:20:00");

						histData f = histDataHelper.extract(r.getAny());
						System.out.println( "# entries returned = " + 
											f.d.length);
             
						System.out.println(query);      
						if (f.d.length > 0) {
								for ( int ii = 0; ii < f.d.length ; ii++ ) {
										System.out.println( 
												"\t" + f.s[ii] + "\t" + f.d[ii]  + "\t" + 
												f.repCnt_l[ii] + "\t" + f.unixtime[ii] + "\t" + f.isdst[ii] + "\t" + f.pulseid[ii] + "\t" + f.count[ii]);
								}
						}
				}
				catch (Exception e)
				{
						System.err.println( e.getMessage() );
				}

				return; 
		}

		/** 
		 * Acquire DOUBLE type EPICS PV, giving type, through DaServer, with reference
		 * for a 8-Pack PV.
		 */  
		private void simpleTest05() 
		{ 
				System.out.println( "====================\n TEST 05  \n--------------------"); 

				try
				{
						System.out.println("Getting data for 8-Pack PV TRS8:TWT56:OUTFWD:VOLT//HIST.pack");
						String query = "TRS8:TWT56:OUTFWD:VOLT//HIST.pack";
						DaReference r = new DaReference(query, da);


						r.setParam("STARTTIME", "04/30/2004 06:40:00");
						r.setParam("ENDTIME",   "04/30/2004 06:50:00");

						histData f = histDataHelper.extract(r.getAny());
						System.out.println( "# entries returned = " + 
											f.d.length);
             
						System.out.println(query);      
						if (f.d.length > 0) {
								for ( int ii = 0; ii < f.d.length ; ii++ ) {
										System.out.println( 
												"\t" + f.s[ii] + "\t" + f.d[ii]  + "\t" + 
												f.repCnt_l[ii] + "\t" + f.unixtime[ii] + "\t" + f.isdst[ii] + "\t" + f.pulseid[ii] + "\t" + f.count[ii]);
								}
						}
				}
				catch (Exception e)
				{
						System.err.println( e.getMessage() );
				}

				return; 
		}
     
		//******************************************************************
		// TODO ADD TEST LIKE DaCa TEST WHICH IS WITHOUT REFERENCE.
		// LOOK at other DaCa tests to see what types of tests to do below.
		//******************************************************************

		/** 
		 * Acquire DOUBLE type EPICS PV, giving type, through DaServer, with reference. 
		 */  
		private void simpleTest06() 
		{ 
				System.out.println( "====================\n TEST 06 \n--------------------");     

				// Prompt for PV, Start, and End Time.
				// Allow the user to enter Carrage Return for default values.

				System.out.print(
						"Enter PV name [enter C/R for HR81:CAV2:VACM//HIST.pepii ]:");

				try {
						java.io.BufferedReader br;
						br = new java.io.BufferedReader(
								new java.io.InputStreamReader(System.in));
						String query = br.readLine();
						if (query.length() == 0) query = "HR81:CAV2:VACM//HIST.pepii";

						// String query = "HR81:CAV2:VACM//HIST.pepii";
						DaReference r = new DaReference(query, da);

						System.out.print(
								"Enter START time [enter C/R to use 02/01/2003 12:20:00 ]: ");
						String query2 = br.readLine();
						if (query2.length() == 0) query2 = "02/01/2003 12:20:00";
						r.setParam("STARTTIME", query2);

						System.out.print(
								"Enter   END time [enter C/R to use 02/01/2003 12:40:00 ]: ");
						String query3 = br.readLine();
						if (query3.length() == 0) query3 = "02/01/2003 12:40:00";
						r.setParam("ENDTIME", query3);

						System.out.print("Getting data for " + query + "\n" + query2 + "\n" +
										 query3 + "\n\n");
	    
						// r.setParam("STARTTIME", "02/01/2003 12:20:00");
						// r.setParam("ENDTIME",   "02/01/2003 12:40:00");

						histData f = histDataHelper.extract(r.getAny());

						System.out.println( "Done! # entries returned is: " + 
											f.d.length);
						System.out.println("for PV = " + query);      
						if (f.d.length > 0) {
								for ( int ii = 0; ii < f.d.length ; ii++ ) {
										System.out.println( 
												"\t" + f.s[ii] + "\t" + f.d[ii]  + "\t" + 
												f.repCnt_l[ii] + "\t" + f.unixtime[ii] + "\t" + f.isdst[ii] + "\t" + f.pulseid[ii] + "\t" + f.count[ii]);
								}
						}
				}
				catch (Exception ex) {
						System.err.println( ex.getMessage() );
				}
				return; 
		} 
     
		/** 
		 * Get history data into DaValue by DaReference
		 */  
		private void simpleTest07() 
		{ 
				System.out.println( "====================\n TEST 07  \n--------------------"); 

				try
				{
						System.out.println("Getting data for PEPII PV HR81:CAV2:VACM//HIST.pepii");
						String query = "HR81:CAV2:VACM//HIST.pepii";
						DaReference r = new DaReference(query, da);

						r.setParam("STARTTIME", "02/01/2003 12:20:00");
						r.setParam("ENDTIME",   "02/01/2003 12:40:00");

						r.compile(da);
						DaValue v = r.getDaValue();
						System.out.println(v.toString());
				}
				catch (Exception e)
				{
						System.err.println( e.getMessage() );
				}

				return; 
		}

		/** 
		 *  Get data using getDaValue from the "da" object.  Also demostrate
                 *  extracting the values and timestamp vectors from the DaValue object.
		 */  
		private void simpleTest08() 
		{ 
				System.out.println( "====================\n TEST 08  \n--------------------"); 

				try
				{
						System.out.println("Getting data for PEPII PV HR81:CAV1:VACM//HIST.dev");
						String query = "HR81:CAV1:VACM//HIST.dev";

						da.setParam("STARTTIME", "02/01/2003 12:20:00");
						da.setParam("ENDTIME",   "02/01/2003 12:40:00");

                                                DaValue v = da.getDaValue(query);

                                                Vector valuesvec = (Vector)v.get(0);
                                                Vector timesvec = (Vector)v.get(1);

						System.out.println("history values vector = " +
                                                    valuesvec.toString());
						System.out.println("history times vector = " +
                                                    timesvec.toString());

                                                // Convert the subvectors to
                                                int vectorSize = valuesvec.size();

                                                Double[] values = new Double[vectorSize];
                                                values = (Double[]) valuesvec.toArray(values);

                                                String[] times = new String[vectorSize];
                                                times = (String[]) timesvec.toArray(times);

                                                for (int i = 0; i < vectorSize; i++)
                                                {
                                                    System.out.println("history values [" + i + "]" +
                                                        values[i]);

                                                    System.out.println("history times [" + i + "]" +
                                                        times[i]);
                                                }

                                                da.reset();
				}
				catch (Exception e)
				{
						System.err.println( e.getMessage() );
				}

				return; 
		}

		/** 
		 *  Get data using getDaValue from the "da" object.  Also demostrate
                 *  extracting vectors in DaValue to individual vectors and using
                 *  them with an enumeration.
		 */  
		private void simpleTest09() 
		{ 
				System.out.println( "====================\n TEST 09  \n--------------------"); 

				try
				{
						System.out.println("Getting data for PEPII PV HR23:CAV2:VACM//HIST.dev");

						String query = "HR23:CAV2:VACM//HIST.dev";

						da.setParam("STARTTIME", "02/01/2003 12:20:00");
						da.setParam("ENDTIME",   "02/01/2003 12:40:00");

                                                DaValue v = da.getDaValue(query);

                                                Vector values = (Vector) v.elementAt(0);
                                                for (Enumeration e = values.elements(); e.hasMoreElements(); )
                                                {
                                                    System.out.println(e.nextElement());
                                                }

                                                Vector times = (Vector) v.elementAt(1);
                                                for (Enumeration e = times.elements(); e.hasMoreElements(); )
                                                {
                                                    System.out.println(e.nextElement());
                                                }

                                                da.reset();
				}
				catch (Exception e)
				{
						System.err.println( e.getMessage() );
				}

				return; 
		}

		private void simpleTest10() 
		{ 
				System.out.println( "====================\n TEST 10  \n--------------------"); 

				try
				{
						//System.out.println("Getting data for PEPII PV PB60:LUMCOR//HIST.dev");
						System.out.println("Getting data for PEPII PV PB60:LUMCOR//HIST.pepii");

						//String query = "PB60:LUMCOR//HIST.dev";
						String query = "PB60:LUMCOR//HIST.pepii";

						da.setParam("STARTTIME", "05/01/2007 00:00:00");
						da.setParam("ENDTIME",   "05/01/2007 03:00:00");

                                                Calendar firstTime = Calendar.getInstance();

                                                System.out.println("Before retrieval time: " +
                                                   firstTime.get(Calendar.HOUR) + ":" +
                                                   firstTime.get(Calendar.MINUTE) + ":" +
                                                   firstTime.get(Calendar.SECOND));
  
                                                int i;
                                                for (i = 0; i < 100; i++)
                                                {  
                                                   DaValue v = da.getDaValue(query);
                                                }

                                                Calendar secondTime = Calendar.getInstance();

                                                System.out.println("Before retrieval time: " +
                                                   secondTime.get(Calendar.HOUR) + ":" +
                                                   secondTime.get(Calendar.MINUTE) + ":" +
                                                   secondTime.get(Calendar.SECOND));

                                                //Vector values = (Vector) v.elementAt(0);
                                                //for (Enumeration e = values.elements(); e.hasMoreElements(); )
                                                //{
                                                //    System.out.println(e.nextElement());
                                                //}

                                                //Vector times = (Vector) v.elementAt(1);
                                                //for (Enumeration e = times.elements(); e.hasMoreElements(); )
                                                //{
                                                //    System.out.println(e.nextElement());
                                                //}

                                                da.reset();
				}
				catch (Exception e)
				{
						System.err.println( e.getMessage() );
				}

				return; 
		}

		/**
                 * Waveform test.
		 * Acquire DOUBLE type EPICS PV, giving type, through DaServer, with reference
		 * for a NLCTA waveform PV.
		 */  
		private void simpleTest11() 
		{ 
				System.out.println( "====================\n TEST 11  \n--------------------"); 

				try
				{
                                                int i;
                                                int j;
                                                int k;
                                                int offset;

						System.out.println("Getting data for NLCTA waveform PV TR00:SISDTZ:C0:RMS0//HIST.nlcta");
						String query = "TR00:SISDTZ:C0:RMS0//HIST.nlcta";
						DaReference r = new DaReference(query, da);


						r.setParam("STARTTIME", "01/31/2004 23:00:00");
						r.setParam("ENDTIME",   "01/31/2004 23:59:00");

						histData f = histDataHelper.extract(r.getAny());
						System.out.println( "# samples returned = " + 
											f.count.length);
             
						System.out.println(query);
                                                System.out.println();
						if (f.count.length > 0)
                                                {
                                                    for (i = 0; i < f.count.length ; i++)
                                                    {
                                                        System.out.println( 
                                                            "\t" + f.s[i] + "\t" + f.repCnt_l[i]  + "\t" + 
                                                        f.unixtime[i] + "\t" +
                                                        f.isdst[i] + "\t" + 
                                                        f.pulseid[i] + "\t" + f.count[i]);
                                                    }

                                                    System.out.println();
                                                    System.out.println("Waveform values");
                                                    System.out.println();

                                                    offset = 0;
                                                    for (j = 0; j < f.count.length ; j++)
                                                    {
                                                        System.out.println("Timestamp " + "\t" + f.s[j] +
                                                            "\t" + f.count[j] + " values:"); 

                                                        for (k = 0; k < f.count[j]; k++)
                                                        { 
                                                            System.out.println("\t" + f.d[offset]);
                                                            offset++;
                                                        }

                                                        System.out.println(); 
                                                    }
						}
				}
				catch (Exception e)
				{
						System.err.println( e.getMessage() );
				}

				return; 
		}

		/** 
		 * Acquire DOUBLE type EPICS PV, giving type, through DaServer, with reference
		 * for a LCLS PV using INDEX parameter to retrieve data using old index.
                 * The "isdst" flag value is 1 for all returned samples, indicating the
                 * associated timestamps are Daylight Savings Time times.
		 */  
		private void simpleTest12() 
		{ 
				System.out.println( "====================\n TEST 12  \n--------------------"); 

				try
				{
						System.out.println("Getting data for LCLS PV VPIO:IN20:111:VRAW//HIST.lcls");
						String query = "VPIO:IN20:111:VRAW//HIST.lcls";
						DaReference r = new DaReference(query, da);


						r.setParam("STARTTIME", "11/02/2007 01:00:00");
						r.setParam("ENDTIME",   "11/02/2007 01:10:00");
                                                r.setParam("INDEX", "2007");

						histData f = histDataHelper.extract(r.getAny());
						System.out.println( "# entries returned = " + 
											f.d.length);
             
						System.out.println(query);      
						if (f.d.length > 0) {
								for ( int ii = 0; ii < f.d.length ; ii++ ) {
										System.out.println( 
												"\t" + f.s[ii] + "\t" + f.d[ii]  + "\t" + 
												f.repCnt_l[ii] + "\t" + f.unixtime[ii] + "\t" + f.isdst[ii] + "\t" + f.pulseid[ii] + "\t" + f.count[ii]); 
								}
						}
				}
				catch (Exception e)
				{
						System.err.println( e.getMessage() );
				}

				return; 
		}

		/** 
		 * Acquire DOUBLE type EPICS PV, giving type, through DaServer, with reference
		 * for a LCLS PV using INDEX parameter to retrieve data using old index.
                 * The "isdst" flag value is 0 for all returned samples, indicating the
                 * associated timestamps are Standard Time times.
		 */  
		private void simpleTest13() 
		{ 
				System.out.println( "====================\n TEST 13  \n--------------------"); 

				try
				{
						System.out.println("Getting data for LCLS PV VPIO:IN20:111:VRAW//HIST.lcls");
						String query = "VPIO:IN20:111:VRAW//HIST.lcls";
						DaReference r = new DaReference(query, da);


						r.setParam("STARTTIME", "11/18/2007 01:00:00");
						r.setParam("ENDTIME",   "11/18/2007 01:10:00");
                                                r.setParam("INDEX", "2007");

						histData f = histDataHelper.extract(r.getAny());
						System.out.println( "# entries returned = " + 
											f.d.length);
             
						System.out.println(query);      
						if (f.d.length > 0) {
								for ( int ii = 0; ii < f.d.length ; ii++ ) {
										System.out.println( 
												"\t" + f.s[ii] + "\t" + f.d[ii]  + "\t" + 
												f.repCnt_l[ii] + "\t" + f.unixtime[ii] + "\t" + f.isdst[ii] + "\t" + f.pulseid[ii] + "\t" + f.count[ii]); 
								}
						}
				}
				catch (Exception e)
				{
						System.err.println( e.getMessage() );
				}

				return; 
		}


		public void run(int testNumber ) 
		{ 
				try 
				{ 
						switch ( testNumber ) 
						{ 
						case 1: 
								simpleTest01(); 
								break; 
                     
						case 2: 
								simpleTest02(); 
								break; 
                     
						case 3: 
								simpleTest03(); 
								break; 
                     
						case 4: 
								simpleTest04(); 
								break; 
                     
						case 5: 
								simpleTest05(); 
								break;

						case 6: 
								simpleTest06(); 
								break; 
                     
						case 7: 
								simpleTest07(); 
								break;

						case 8: 
								simpleTest08(); 
								break;

						case 9: 
								simpleTest09(); 
								break;

						case 10: 
								simpleTest10(); 
								break;

						case 11: 
								simpleTest11(); 
								break;

						case 12: 
								simpleTest12(); 
								break;

						case 13: 
								simpleTest13(); 
								break;

						default: 
								System.out.println( "No such Chads test number parameter " + 
													testNumber + "=\"" + testNumber + "\"" ); 
								System.out.println( "usage: java daChadsTest <test num> ..." );
						}
                     
				} 
				catch (Exception e) 
				{ 
						System.out.println( "Exception: Executing test " + testNumber); 
						System.err.println( e.getMessage() + " caused by " + e ); 
				} 
				return; 
		} 
} 
 
