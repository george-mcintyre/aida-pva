package edu.stanford.slac.aida.lib.util.common;

import org.omg.CORBA.*;
import org.omg.PortableServer.*;

import edu.stanford.slac.except.*;

/**
 * Creates Portable Object Adaptor for Aida NameServer. 
 *
 * This object is a sub-class of _DaObjectBase directly and exists
 * seperately to _PersistentDaObject because _PersistentDaObject
 * extends _DaObject, which in turn extends _DaObjectBase. Hence, the
 * user of this class (NameServer) can avoid having methods defined
 * for accessing the NameServer (which are defined in _DaObject),
 * which wouldn't make much sense for NameServer itself. Implemented
 * in this fashion due to unavailability of multiple inheritance in
 * java. Sadly corespondence between methods in _PersistentDaObject
 * and _PersistentDaObjectBase must be maintained manually.
 * 
 * @author 20-Mar-2002, Greg
 * @version 31-Jul-2005, Greg White (greg): Append "perisistentPOA" to m_child
 * peristentPOANAme, and add NO_IMPLICIT_ACTIVATION policy; to correct init for 
 * "perisistent" servers.
 */
public class _PersistentDaObjectBase extends _DaObjectBase
{
        /**
         * Default name if cannot be determined from either command line or
         * properties Also used by to determine the Persistent name for the Name
         * Server.
         */
        public static final String PDO_SERVER_NAME = "NameServer";

        /**
         * Persistent POA
         */
        public POA m_childPersistentPOA;

        /**
         * Persistent POA Name
         */
        public String m_childPersistentPOAName;

        /**
         * Persistence policies
         */
        private Policy[] m_Policy;

	// Messages
	private static final String BADINIT = 
	"Failure to cleanly terminate server after failed initialization";
 
        /**
         * Constructs a _PersistentDaObject without EventService
         */
        public _PersistentDaObjectBase() throws AidaInitException
        {
                super();
        }

        /**
         * Constructs a PersistentUtil without EventService
         * 
         * @param args
         *        a list of CORBA properties name value pairs, see primary
         *        constructor
         * @param childPersistentPOAName
         *        a text string with which to name the child POA created. This
         *        might just be the name of the server like "DaServer", and if
         *        given as null the server name will be used.
         */
        public _PersistentDaObjectBase(String[] args, String childPersistentPOAName)
                throws AidaInitException
        {
                super(args, childPersistentPOAName);
        }

        /**
         * Constructs a <code>_PersistentDaObject</code> including EventService
         * initialization if desired
         * 
         * @param args
         *        a list of CORBA properties name value pairs, see primary
         *        constructor
         * @param childPersistentPOAName
         *        a text string with which to name the child POA created. This
         *        might just be the name of the server like "DaServer", and if
         *        given as null the server name will be used.
         * @param events
         *        whether to initialize EventService. If this is true a
         *        push_consumerImpl must be in the classpath
         */
        public _PersistentDaObjectBase(String[] args, String childPersistentPOAName,
                boolean events) throws AidaInitException
        {
                super(args, childPersistentPOAName, events);
        }

        /**
         * Initialises a <code>_PersistentDaObject</code>. Notably, creates a policy to govern
         * its existence based on the user id associated with the calling
         * process
         */
        protected void init(String serverName)
                throws AidaInitException
        {
                m_childPersistentPOAName = serverName + "persistentPOA";
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
                                m_err.logl(new IllegalStateException(BADINIT).initCause(e));
                        }
                        m_orb = null;
                        m_err.logl(ex, " while initializing server");
                        ex.printStackTrace();
                }
        }

        /**
         * Shuts down the server via a call to orb.destroy()
         * 
         * @throws TerminateException
         *         Thrown if the server cannot be terminated.
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
