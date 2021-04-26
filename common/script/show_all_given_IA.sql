--==============================================================
--
--  Abs:   Gets basically all teh information for a given Aida Instance
--         Attribute pair presently in the Aida Directory database.
--
--  Name:  show_all_given_IA.sql                        
--
--  Rem:   This is intended to echo what the NameServer's getTargetRef()
--         method find out for a given Instance Attribute.
--
--  Usage: 
--   SQL> @show_all_given_IA '<LIKE-expression>' '<LIKE-expression>'  
--   SQL> @show_all_given_IA 'BPMS:PR02:8%' 'TWISS' 
--   > sqlplus -S AIDADEV/<passwd> @show_all_given_IA.sql BPMS:PR02:8% TWISS
--
--  Auth:  28-Nov-2004, Greg White (greg) 
--  Rev:   
--
----------------------------------------------------------------
-- Mods: (Latest to oldest)
--
--
--============================================================== 
set head on;
set pagesize 10000;
-- set wrap off;
set verify off;
DEFINE groupId=1
DEFINE inst=&1
DEFINE attr=&2
SELECT N.TRANSFORM,  N.TYPE_ID,  N.COUNT,  SI.SERVICE_ID,  I.SOR 
       FROM  AIDA_NAMES N,  AIDA_DIRECTORY D, 
             AIDA_SERVICE_INTERFACE SI, AIDA_INTERFACES I
       WHERE N.ID = D.NAME_ID AND 
             D.SERVICE_ID = SI.SERVICE_ID AND 
             SI.GROUP_ID = &groupId 
	      AND SI.INTERFACE_ID = I.ID  
	      AND N.INSTANCE LIKE '&inst' 
	      AND N.ATTRIBUTE LIKE '&attr'
 ORDER BY "D"."ORDER_NO";
