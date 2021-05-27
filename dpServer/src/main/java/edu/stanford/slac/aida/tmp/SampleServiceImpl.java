package edu.stanford.slac.aida.tmp;

import org.epics.pvaccess.server.rpc.RPCRequestException;
import org.epics.pvaccess.server.rpc.RPCService;
import org.epics.pvdata.pv.PVString;
import org.epics.pvdata.pv.PVStructure;
import org.epics.pvdata.pv.Status;

import java.util.Map;

class SampleServiceImpl /* implements RPCService*/ {
/*
    public PVStructure request(PVStructure pvUri) throws RPCRequestException {
        String msg = null; // Server messages logged and returned.

        String type = pvUri.getStructure().getID();
        if (!type.equals(MemeNormativeTypes.NTURI_ID)) {
            msg = "Unable to get data. Bad argument to server: " + String.format(Message.NOTEXPECTEDNTID, MemeNormativeTypes.NTURI_ID, type);
            throw new RPCRequestException(Status.StatusType.ERROR, msg);
        }

        // Retrieve the PV name, assuming pvUri is
        // is a PVStructure conforming to NTURI.
        //
        PVString pvPathField = pvUri.getStringField("path");
        if (pvPathField == null) {
            throw new RPCRequestException(Status.StatusType.ERROR, String.format(Message.MISSINGREQUIREDARGLVAL, "path"));
        }
        String pvname = pvPathField.get();
        if (pvname == null || pvname.length() == 0) {
            throw new RPCRequestException(Status.StatusType.ERROR, String.format(Message.MISSINGREQUIREDARGRVAL, "path"));
        }

        // Retrieve arguments, if any given to this RPC PV channel.
        //
        Map<String, String> pvArgs_map = null;
        PVStructure pvUriQuery = pvUri.getStructureField("query");
        if (pvUriQuery != null) {
            pvArgs_map = GregsImpl.getPvArgs(pvUriQuery);
        }

        // Look up the SQL query keyed by the pv name used to make
        // the channel connection, substitute any argument names and
        // values if any given to (RPC) channel, then process the
        // resulting SQL query on the SQL database.
        //
        PVStructure pvTop = null;
        try {
            // Look up sql query for queryname given.
            String sqlQuery = queries_ht.get(pvname);

            // Substitute arguments into SQL query if any
            if (pvArgs_map != null) {
                sqlQuery = GregsImpl.substituteArgsIntoQuery( sqlQuery, pvArgs_map);
            }

            // Execute sql query and serialize to pvTop PVStructure
            pvTop = connection.getData(sqlQuery);

            // Return the data from Oracle, in pvTop, to the client.
            return pvTop;

        } catch (UnableToGetDataException ex) {
            msg = Message.NORETURNEDDATA + ": " + ex.getMessage();
            throw new RPCRequestException(Status.StatusType.ERROR, msg);
        }
    }*/
}
