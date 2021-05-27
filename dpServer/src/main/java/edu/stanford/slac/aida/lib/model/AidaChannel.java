package edu.stanford.slac.aida.lib.model;

import edu.stanford.slac.aida.lib.PVTopStructure;

public class AidaChannel {
    private String instance = "";
    private String attribute;
    private PVTopStructure structure;

    public AidaChannel(String instance, String attribute) {
        this.instance = instance;
        this.attribute = attribute;
    }

    public AidaChannel() {
    }

    public String getInstance() {
        return instance;
    }

    public void setInstance(String instance) {
        this.instance = instance;
    }

    public String getAttribute() {
        return attribute;
    }

    public void setAttribute(String attribute) {
        this.attribute = attribute;
    }

    public PVTopStructure getStructure() {
        return structure;
    }

    public void setStructure(PVTopStructure structure) {
        this.structure = structure;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof AidaChannel)) return false;

        AidaChannel that = (AidaChannel) o;

        if (!instance.equals(that.instance)) return false;
        return attribute != null ? attribute.equals(that.attribute) : that.attribute == null;
    }

    @Override
    public int hashCode() {
        int result = instance.hashCode();
        result = 31 * result + (attribute != null ? attribute.hashCode() : 0);
        return result;
    }

    @Override
    public String toString() {
        return "AidaChannel{" +
                "instance='" + instance + '\'' +
                ", attribute='" + attribute + '\'' +
                '}';
    }
}
