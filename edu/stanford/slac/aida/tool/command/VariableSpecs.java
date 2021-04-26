package edu.stanford.slac.aida.tool.command;


public class VariableSpecs
{
    // for some reason aida objects expect this instead of an empty array!?
    static final String[] emptyArgs = { "null", "" };

    // name of the aida instance/attribute
    private String target;

    // aida data provder parameters for get() method
    private String[] readArgs = emptyArgs;


    // aida data provider parameters for set() method
    private String[] writeArgs = emptyArgs;


    public VariableSpecs (String target)
    {
        setTarget (target);
        clearReadArgs();
        clearWriteArgs();
    }

    
    public VariableSpecs (String target, String[] readArgs, String[] writeArgs)
    {
        setTarget (target);
        setReadArgs (readArgs);
        setWriteArgs (writeArgs);
    }


    public String getTarget()
    {
        return target;
    }


    public void setTarget (String target)
    {
        this.target = new String (target);
    }


    public String[] getReadArgs()
    {
        return readArgs;
    }


    public void setReadArgs (String[] args)
    {
        if (args.length == 0)
            clearReadArgs();
        
        else readArgs = args;
    }


    public void clearReadArgs ()
    {
        readArgs = emptyArgs;
    }


    public String[] getWriteArgs()
    {
        return writeArgs;
    }


    public void setWriteArgs (String[] args)
    {
        if (args.length == 0)
            clearWriteArgs();
        
        else writeArgs = args;
    }


    public void clearWriteArgs ()
    {
        writeArgs = emptyArgs;
    }
}
