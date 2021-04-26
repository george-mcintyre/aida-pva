package edu.stanford.slac.aida.tool.command;

import edu.stanford.slac.aida.lib.da.*;
import edu.stanford.slac.aida.lib.util.common.*;
import edu.stanford.slac.except.*;


public class DaVariable
    extends VariableSpecs
    implements VariableReader, VariableWriter
{
    private DaObject da;

    
    public DaVariable (String target)
         throws AidaInitException
    {
        super (target);
        da = new DaObject();
    }

    
    public DaValue read ()
        throws UnableToGetDataException,
        UndefinedNameException
    {
        return da.getDaValue (getTarget());
    }

    
    public void write (DaValue dv)
        throws UnableToSetDataException,
        UndefinedNameException
    {
        da.setDaValue (getTarget(), dv);
    }
}
