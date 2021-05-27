package edu.stanford.slac.aida.tmp;

import org.epics.pvaccess.PVAException;
import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvaccess.server.rpc.RPCServer;
import org.epics.pvdata.factory.FieldFactory;
import org.epics.pvdata.factory.PVDataFactory;
import org.epics.pvdata.pv.FieldCreate;
import org.epics.pvdata.pv.PVDataCreate;
import org.epics.pvdata.pv.PVField;
import org.epics.pvdata.pv.PVStructure;
import org.epics.pvdata.pv.Status.StatusType;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import java.io.File;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.Map;
import java.util.Set;

public class GregsImpl {
/*
    private final static String SERVER_NAME_DEFAULT = "Sample";
    private final static String CHANNELS_FILENAME_DEFAULT = "channels.yml";

    // Factories for creating the data and introspection interfaces of data
    // exchanged by RdbService.
    private static final FieldCreate fieldCreate = FieldFactory.getFieldCreate();
    private static final PVDataCreate pvDataCreate = PVDataFactory.getPVDataCreate();

    public static void main(String[] args) throws PVAException {
        // An XML file (*.dbx) defines the channels of this EPICS V4
        // server; the names of the important XML elements in the file
        // are defined here.
        //
        final String CHANNEL_ELEMENTNAME = "channel";
        final String CHANNELNAME_ELEMENTNAME = "name";
        final String SQLQUERY_ELEMENTNAME = "sqlStatement";

        // Get service name and channel definitions for the server to publish.
        String server_name = System.getProperty("SERVER_NAME", SERVER_NAME_DEFAULT);
        String rpcchannelsxml_fn = System.getProperty("CHANNELS_FILENAME", CHANNELS_FILENAME_DEFAULT);

        // Initialize console logging.
        // logger.info("Server \"" + server_name + "\" is initializing...");

        try {
            // Initialize database connection.
            RdbServiceConnection rdbConnection = new RdbServiceConnection(server_name);

            // Instantiate ChannelRPC service instances of this
            // pvAccess server.
            RPCServer server = new RPCServer();
            // logger.info("Services of \"" + server_name + "\" is/are initializing...");

            // Make hashtable of sql queries keyed by query name.
            Hashtable<String, String> rpcqueries_ht = new Hashtable<String, String>();

            // XML file of channels and their metadata. In case of
            // RDB service the metada are sql queries whose values
            // are fetched by this service.
            //
            File fXmlFile = new File(rpcchannelsxml_fn);
            DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
            DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
            Document doc = dBuilder.parse(fXmlFile);
            doc.getDocumentElement().normalize();

            // Read through xml file, and build the hashtable of queries
            //
            NodeList pv_nodeList = doc.getElementsByTagName(CHANNEL_ELEMENTNAME);
            for (int i = 0; i < pv_nodeList.getLength(); i++) {
                Node pv_node = pv_nodeList.item(i);
                // logger.fine("Adding Channel metadata :" + pv_node.getNodeName());
                if (pv_node.getNodeType() == Node.ELEMENT_NODE) {
                    Element element = (Element) pv_node;
                    String pvName = element
                            .getElementsByTagName(CHANNELNAME_ELEMENTNAME)
                            .item(0)
                            .getTextContent();

                    if (pvName == null) {
                        // logger.severe(
                        //         String.format(
                        //                 Message.MISSINGREQUIREDELEMENT,
                        //                 CHANNELNAME_ELEMENTNAME,
                        //                 rpcchannelsxml_fn));
                    }

                    String sqlStatement = element.getElementsByTagName(SQLQUERY_ELEMENTNAME).item(0).getTextContent().trim();

                    if (sqlStatement == null) {
                        // logger.severe(
                        //         String.format(
                        //                 Message.MISSINGREQUIREDELEMENT,
                        //                 SQLQUERY_ELEMENTNAME,
                        //                 rpcchannelsxml_fn));
                    }

                    // Assign the sql query for the PV,
                    // making any substitutions into the
                    // query string based on the PV name.
                    //
                    sqlStatement = substitute(pvName, sqlStatement);
                    rpcqueries_ht.put(pvName, sqlStatement);
                }
            }

            // Create one implementation of the RPC Service, which in this server
            // we'll use for all the PVs published by this server.
            //
            SampleServiceImpl rdbServiceImpl = new SampleServiceImpl(rdbConnection, rpcqueries_ht);

            // For each element of the hashtable of PVs, register a service
            // implementation on a channel whose name is the PV name.
            Set<String> keys = rpcqueries_ht.keySet();  // Keys are pv names
            for (String pvname : keys) {
                server.registerService(pvname, rdbServiceImpl);
            }

            // Issue successful startup.
            server.printInfo();

            // Start the service.
            server.run(0);

        } catch (Exception e) {
            // Issue unsuccessful startup.
            // logger.severe(e.toString());
            //  logger.severe(String.format(Message.SERVERINIT_FAILED, server_name));
        }
    }

    static Map<String, String> getPvArgs(PVStructure query)
            throws RPCRequestException {
        // logger.setLevel(Level.FINER);
        RPCRequestException iss = null;
        Map<String, String> args_map = new HashMap<String, String>();
        PVField[] args = query.getPVFields();

        // For each argument given to the RPC PV (ie each -a of
        // eget), get the name and value of the argument from the PV
        // data inteface, and insert into hashmap.
        //
        for (PVField arg : args) {
            String argname = arg.getFieldName();
            if (argname == null) {
                throw new RPCRequestException(StatusType.ERROR, "Missing argument");
            }

            String argval = query.getStringField(argname).get();
            if (argval == null) {
                throw new RPCRequestException(StatusType.ERROR, "Missing argument value");
            }
            args_map.put(argname, argval);
        }
        return args_map;
    }

    static String substituteArgsIntoQuery(String sqlQuery, Map<String, String> pvArgs_map) {
        Set<String> keys = pvArgs_map.keySet();  // Keys are argument names

        // For each argument given to service, replace the substring
        // in the sqlQuery literally matching the argument name, with the
        // argument value. Eg if "eget -a person bob" was issued, each occurence
        // of 'person' would be replaced with 'bob'.
        //
        for (String key : keys) {
            // logger.fine("Substitution " + key + ": " + pvArgs_map.get(key));
            sqlQuery = sqlQuery.replace(key, pvArgs_map.get(key));
        }

        // logger.fine("Query following substitution: " + sqlQuery);
        return sqlQuery;
    }

    private static String substitute(String pvname, String originalQuery) {
        // logger.info("pvname: " + pvname);
        // logger.info("original query: " + originalQuery);

        // The query string after replacements if any
        String finalQuery = originalQuery;

        int attributeSeparatorIndex = pvname.lastIndexOf(':');
        if (attributeSeparatorIndex != -1) // -1 == colon not found
        {
            String afterinstsub = entityPattern
                    .matcher(originalQuery)
                    .replaceAll(pvname.substring(0, attributeSeparatorIndex));
            finalQuery = attributePattern
                    .matcher(afterinstsub)
                    .replaceAll(pvname.substring(attributeSeparatorIndex + 1));
        }

        // TODO: You could now, if you wanted, parse final string
        // for regexps matching the components of a device-name, PV
        // name, etc, and do replacements of those too. Eg, replace
        // (\w(2)[0-9](2):) with the text matching from the
        // instance, which is likely to be an "area" aka micro
        // name.

        return finalQuery;
    }*/
}
