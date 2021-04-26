package edu.stanford.slac.aida.lib.util.common;

import org.omg.CORBA.*;
import org.omg.PortableServer.*;

import edu.stanford.slac.except.*;

/**
 * Data Access Object for persistent servers. This object is a sub- class of
 * <code>_DaObject</code> and is not related to
 * <code>_PersistentDaObjectBase</code>. Implemented in this fashion due to
 * unavailability of multiple inheritance in java. Sadly corespondence between
 * methods in _PersistentDaObject and _PersistentDaObjectBase must be maintained
 * manually.
 * 
 * @version 24-Jun-2002, George McIntyre
 * @version 31-Jul-2005, Greg White (greg): added
 * NO_IMPLICIT_ACTIVATION policy to correct init for "perisistent"
 * servers.
 */
public class _PersistentDaObject extends _DaObject
{
        /**
         * Default name if cannot be determined from either command line or
         * properties Also used by DaObject to determine the Persistent name for
         * the DaServer
         */
        public static final String PDO_SERVER_NAME = "daServer";

        /**
         * Persistent POA
         */
        public POA m_childPersistentPOA;


    public POA getChildPersistentPOA ()
    {
        return m_childPersistentPOA;
    }

        /**
         * Persistent POA Name
         */
        public String m_childPersistentPOAName;

        /**
         * Persistence policies
         */
        private Policy[] m_Policy;

	private static final String BADINIT = 
	"Failure to cleanly terminate server after failed initialization";

        /**
         * Constructs a _PersistentDaObject without EventService
         */
        public _PersistentDaObject() throws AidaInitException
        {
                super();
        }

        /**
         * Constructs a _PersistentDaObject without EventService
         * 
         * @param args
         *        a list of CORBA properties name value pairs
         * @param childPersistentPOAName
         *        a text string with which to name the child POA created.
         */
        public _PersistentDaObject(String[] args, String childPersistentPOAName)
                throws AidaInitException
        {
                super(args, childPersistentPOAName);
        }

        /**
         * Constructs a _PersistentDaObject including EventService
         * initialization if desired
         * 
         * @param args
         *        a list of CORBA properties name value pairs
         * @param childPersistentPOAName
         *        a text string with which to name the child POA created.
         * @param events
         *        whether to initialize EventService.
         */
        public _PersistentDaObject(String[] args, String childPersistentPOAName, boolean events)
                throws AidaInitException
        {
                super(args, childPersistentPOAName, events);
        }

        /**
         * Initialises a _PersistentDaObject. Notably, creates a policy to govern
         * its existence based on the user id associated with the calling
         * process
         */
        protected void init(String childPersistentPOAName)
                throws AidaInitException
        {
		// TODO: Append "peristentPOA" to childPeristentPOAName's value. 
                m_childPersistentPOAName = childPersistentPOAName;
                try
                {
                        super.init();

                        // FUDGE: hard code server, should come from m_props
                        // String server = PDO_SERVER_NAME;

                        // Create a child POA using the PERSISTENT and USER_ID
                        // policies
                        //
			Any any = m_orb.create_any();
                        m_Policy = new Policy[3];

			LifespanPolicyValueHelper.insert(
				any, org.omg.PortableServer.LifespanPolicyValue.PERSISTENT);
                        m_Policy[0] = m_orb.create_policy(
                                org.omg.PortableServer.LIFESPAN_POLICY_ID.value, any);

			IdAssignmentPolicyValueHelper.insert(
				any, org.omg.PortableServer.IdAssignmentPolicyValue.USER_ID);
                        m_Policy[1] = m_orb.create_policy(
				org.omg.PortableServer.ID_ASSIGNMENT_POLICY_ID.value, any);

	                ImplicitActivationPolicyValueHelper.insert(
				any, org.omg.PortableServer.ImplicitActivationPolicyValue.
				NO_IMPLICIT_ACTIVATION);
			m_Policy[2] = m_orb.create_policy(
				org.omg.PortableServer.IMPLICIT_ACTIVATION_POLICY_ID.value, any);
                        m_childPersistentPOA = m_rootPOA.create_POA(m_childPersistentPOAName,
                                m_POA_manager, m_Policy);
                }
                catch (Exception ex)
                {
                        try
                        {
                                terminate();
                        }
                        catch (Exception e)
                        {
                                m_err.log(new IllegalStateException(BADINIT).initCause(e));
                        }
                        m_orb = null;
                        m_err.log(ex, "while initializing server");
                        ex.printStackTrace();
                }
        }

        /**
         * Shuts down the server via a call to orb.destroy()
         * 
         * @throws TerminateException
         *         error terminating
         */
        public void terminate()
                throws TerminateException
        {
                if (m_orb != null)
                {
                        try
                        {
                                m_orb.destroy();
                                m_orb = null;
                        }
                        catch (Exception ex)
                        {
                                m_err.log(ex, "while attempting to cleanly terminate server");
                                throw new TerminateException("Terminating AIDA CORBA server");
                        }
                }
                return;
        }
}

