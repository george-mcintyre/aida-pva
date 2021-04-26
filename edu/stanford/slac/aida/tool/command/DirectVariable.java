package edu.stanford.slac.aida.tool.command;

import edu.stanford.slac.aida.lib.da.*;
import edu.stanford.slac.aida.lib.dp.*;
import edu.stanford.slac.aida.lib.util.common.*;
import edu.stanford.slac.except.*;
import org.omg.CORBA.*;


public class DirectVariable
    extends VariableSpecs
    implements VariableReader, VariableWriter
{
    private static DaObject da = null;
    private AidaObjectI dp;

    
    public DirectVariable (String dpName, String target)
        throws AidaInitException
    {
        super (target);

        if (da == null)
            da = new DaObject();

        // ack! for the time being, play along with improper casting
        dp = AidaObjectIHelper.unchecked_narrow (
            da.getORB().string_to_object (
                da.getNameServerI().GetObjRef (dpName)));
    }


    public AidaObjectI getAidaObject ()
    {
        return dp;
    }


    public DaValue read ()
        throws UnableToGetDataException,
        UndefinedNameException
    {
        return new DaValue (dp.get (getTarget(), getReadArgs()));
    }

    
    public void write (DaValue dv)
        throws UnableToSetDataException,
        UndefinedNameException
    {
        dp.set (getTarget(), getWriteArgs(), new AnyHolder (dv.toAny()));
    }
}
