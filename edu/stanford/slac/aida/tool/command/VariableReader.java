package edu.stanford.slac.aida.tool.command;

import edu.stanford.slac.aida.lib.util.common.*;
import edu.stanford.slac.except.*;


public interface VariableReader
{
    abstract DaValue read ()
        throws UnableToGetDataException,
        UndefinedNameException;
}
