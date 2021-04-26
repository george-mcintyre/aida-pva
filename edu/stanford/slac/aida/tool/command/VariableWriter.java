package edu.stanford.slac.aida.tool.command;

import edu.stanford.slac.aida.lib.util.common.*;
import edu.stanford.slac.except.*;


public interface VariableWriter
{
    void write (DaValue dv)
        throws UnableToSetDataException,
        UndefinedNameException;
}
