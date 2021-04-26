--==============================================================
--
--  Abs:   Wraps the QA to production db synchronization for the AIDA schema. 
--
--  Name:  aida_qatoo_sync.sql
--
--  Rem:   
--
--  Usage: One required argument: A free form quoted string, giving the 
--         kind of name data AIDA was synchronizing like 'SLC Names', or
--         'XAL Model Names'.
--
--         @aida_qatoo_sync 'SLC Names'
-- 
--  Ret:   0 - success
--         10 - validation failed due to duplicates, exits without synchronization
--         11 - validation failed due to entries in aida_names without matching entry in _dir 
--         21 - validation succeeded, but synchronization not authorized
--         22 - validation succeeded, but synchronization failed with logged error. See
--             DATA_VALIDATION_TAB table for schema aida.
--
--  Auth:  10-Feb-2010, Greg White (greg).
--  Rev:   dd-mmm-yyyy, Whoever    (user):
--
----------------------------------------------------------------
-- Mods: (Latest to oldest)
-- 
--  23-Feb-10 Greg White: Replace MCCO_COPY_TABLE_DATA with PUSH_QUEUE_DATA. 
--    EXECUTE ONLINE_MODEL_PKG.MCCO_COPY_TABLE_DATA('MCCOTOMCCQA_AIDA.SLAC.STANFORD.EDU', 
--    'AIDA_NAMES,AIDA_DIRECTORY');
--
--============================================================== 

var v_ret number; 

define names_kind = '&1';

DECLARE
   V_CURR_RETVAL    NUMBER;
   V_MSG            VARCHAR2(300);
   V_STAGE          VARCHAR2(18); -- Validation or Synchronization
   V_STATUS         VARCHAR2(7);  -- SUCCESS or FAILED
   V_DUPLICATES     INTEGER;      -- Number of duplicate instance/attributes in aida_names
   V_NODIRENTRY     INTEGER;
BEGIN 
        :v_ret := 0;              -- Default return status to success.

        -- Log that valdidation is proceeding.
        V_STAGE := 'Validation';
        insert into controls_global.data_validation_tab(
        INST, SCHEMA_NM, PROCESS_NM, STAGE, STATUS_CHK, MSG, PROCESS_GRP_NO)
  	values ('MCCQA', 'AIDA', '&names_kind', V_STAGE, NULL, 'Begins', 0);

        -- Check for duplicate instance//attribute pairs in AIDA_NAMES.
        --
        V_STATUS := 'SUCCESS';
        SELECT COUNT(*) 
        INTO V_DUPLICATES FROM ( 
            SELECT X.INSTANCE, X.ATTRIBUTE, X.RN  FROM (  
               SELECT N.INSTANCE, N.ATTRIBUTE, ROW_NUMBER() OVER (
                   PARTITION BY N.INSTANCE,N.ATTRIBUTE ORDER BY N.INSTANCE,N.ATTRIBUTE ) 
               AS RN FROM AIDA_NAMES N  
            ) X WHERE X.RN > 1  
        ); 
        V_MSG := to_char(V_DUPLICATES)||' duplicates found in AIDA_NAMES.';
        DBMS_OUTPUT.PUT_LINE( V_MSG );
        if ( v_duplicates > 0 ) then
           V_STATUS := 'FAILED';
           :v_ret := 11;    -- Set return status to 11 (bad).
        end if;
        insert into controls_global.data_validation_tab(
  	INST, SCHEMA_NM, PROCESS_NM, STAGE, STATUS_CHK, MSG, PROCESS_GRP_NO)
  	values ('MCCQA', 'AIDA', '&names_kind', V_STAGE, V_STATUS, V_MSG, 0);
   
        -- Check that all entries in AIDA_NAMES have an entry in AIDA_DIRECTORY.
        -- 
        V_STATUS := 'SUCCESS';
        select count(*)
        into v_nodirentry from 
        (
           select id from aida_names names minus select name_id from aida_directory
        ); 
        V_MSG := to_char(V_NODIRENTRY)||
           ' AIDA_NAMES elements found without an AIDA_DIRECTORY entry.';
        DBMS_OUTPUT.PUT_LINE( V_MSG );
        if ( v_nodirentry > 0 ) then
           V_STATUS := 'FAILED';
           :v_ret := 12;    -- Set return status to 12 (bad).
        end if;
        insert into controls_global.data_validation_tab(
  	INST, SCHEMA_NM, PROCESS_NM, STAGE, STATUS_CHK, MSG, PROCESS_GRP_NO)
  	values ('MCCQA', 'AIDA', '&names_kind', V_STAGE, V_STATUS, V_MSG, 0);

        -- Confirm that no sync will take place when validation failed.
        --
        if ( v_duplicates > 0 or v_nodirentry > 0 ) then

           V_MSG := 'Validation Completes with Errors, No Synchronization'; 
           insert into controls_global.data_validation_tab(
  	   INST, SCHEMA_NM, PROCESS_NM, STAGE, STATUS_CHK, MSG, PROCESS_GRP_NO)
  	   values ('MCCQA', 'AIDA', '&names_kind',  V_STAGE, 'FAILED', V_MSG, 0);
           DBMS_OUTPUT.PUT_LINE( V_MSG );

        else

           V_MSG := 'Validation Completed Successfully'; 
           insert into controls_global.data_validation_tab(
  	   INST, SCHEMA_NM, PROCESS_NM, STAGE, STATUS_CHK, MSG, PROCESS_GRP_NO)
  	   values ('MCCQA', 'AIDA', '&names_kind', V_STAGE, 'SUCCESS', V_MSG, 0);
           DBMS_OUTPUT.PUT_LINE( V_MSG );

           -- Log that we're beginning the MCCQA->MCCO synchronization to the 
           -- data validation/synch logging table.
           --
           V_STAGE := 'Synchronization';
  	   insert into controls_global.data_validation_tab(
  	   INST, SCHEMA_NM, PROCESS_NM, STAGE, STATUS_CHK, MSG, PROCESS_GRP_NO)
  	   values  ('MCCQA', 'AIDA', '&names_kind', V_STAGE, NULL, 'Begins', 0);

           -- Execute synchronization.
           -- 
 	   MCC_AR_PKG.PUSH_QUEUE_DATA('MCCQA_REPG_AIDA', V_CURR_RETVAL, 0);

           IF ( V_CURR_RETVAL  = 0 ) THEN

              -- Successful
              insert into controls_global.data_validation_tab(
              INST, SCHEMA_NM, PROCESS_NM, STAGE, STATUS_CHK, MSG, PROCESS_GRP_NO)
              values('MCCQA', 'AIDA', '&names_kind', V_STAGE, 'SUCCESS', 'Ends', 0);

           ELSIF ( V_CURR_RETVAL = -1 ) THEN

              -- Unsuccessful: SYNCHRONIZATION FAILED DUE TO LACK OF AUTHORIZATION. 
              V_MSG := '- NOT AUTHORIZED TO PERFORM SYNCHRONIZATION.';
              insert into controls_global.data_validation_tab(
              INST, SCHEMA_NM, PROCESS_NM, STAGE, STATUS_CHK, MSG, PROCESS_GRP_NO)
              values('MCCQA', 'AIDA', '&names_kind', V_STAGE, 'FAILED', V_MSG, 0);

             :v_ret := 21;    -- Set return status to 21 (bad).
             
           ELSIF ( V_CURR_RETVAL NOT IN (0, -1) ) THEN

              -- Unsuccessful: Synchronization failed due to some error.
              V_MSG := SUBSTR(SQLERRM(V_CURR_RETVAL),1,300);
              insert into controls_global.data_validation_tab(
              INST, SCHEMA_NM, PROCESS_NM, STAGE, STATUS_CHK, MSG, PROCESS_GRP_NO)
              values('MCCQA', 'AIDA', '&names_kind', 'FINISH', 'FAILED', V_MSG, 0);
              
              :v_ret := 22;    -- Set return status to 22 (bad).

           END IF;  -- synchronization status 

      END IF; -- Validations

END;
/

-- Commit and Return with (possibly bad) error code
exit :v_ret;  
