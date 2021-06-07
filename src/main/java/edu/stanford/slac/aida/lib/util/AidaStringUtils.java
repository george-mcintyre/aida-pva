package edu.stanford.slac.aida.lib.util;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

public class AidaStringUtils {

    /**
     * Show first 10 strings at most
     *
     * @return 10 or less strings
     */
    public static List<String> lessStrings(Set<String> allStringSet) {
        return lessStrings(new ArrayList<String>(allStringSet));
    }

    /**
     * Show first 10 strings at most
     *
     * @return 10 or less strings
     */
    public static List<String> lessStrings(List<String> allStringList) {
        List<String> lessStrings;
        try {
            lessStrings = allStringList.subList(0, 9);
            lessStrings.add("...");
        } catch (Exception e) {
            lessStrings = allStringList;
        }
        return lessStrings;
    }

}
