/*
 * @file
 */
package edu.stanford.slac.aida.lib.model;

import lombok.ToString;

import java.util.ArrayList;
import java.util.List;

/**
 * This class encapsulates an {@link AidaTable}.
 * <p>
 * It is the class that is returned from the Native Providers for requests that return a TABLE.
 * <p>
 * It contains a single property {@link AidaTable#data} that stores a List of column Lists.
 * <p>
 * Note that it uses the {@link lombok.ToString} annotation to provide the toString() method.
 */
@ToString
public class AidaTable {
    /**
     * The property that is used to return the table data.  A simple List of column Lists.
     * The inner, column, Lists are simply {@link List<Object>} to be able to represent any type of data
     * for a column.
     */
    private final List<List<Object>> data = new ArrayList<List<Object>>();

    /**
     * Add an element to the specified column in this {@link AidaTable#data}.
     * <p>
     * Each new column is added by successive calls to {@link AidaTable#add}
     * and so the columnId specified will either have to be inserted, or added at the end.
     * <p>
     * This is called by the Native Channel Provider code in C so be careful if refactoring the signature or name.
     *
     * @param columnId the number representing the number of the column to add to
     * @param object   the object to add to that column
     * @return true if added correctly
     */
    public boolean add(int columnId, Object object) {
        // Only allow this to be run by one thread at a time
        synchronized (this.data) {
            // Get the current number of columns in  {@link AidaTable#data}
            int currentColumnCount = this.data.size();

            // If we don't have enough columns then we need to add new ones from the end of the current list
            // up to the new columnId given
            if (currentColumnCount <= columnId) {
                // Loop from end of current list to new size = columnId
                for (int collectionCounter = currentColumnCount - 1; collectionCounter < columnId; collectionCounter++) {
                    // Add new columns
                    this.data.add(new ArrayList<Object>());
                }
            }

            // Now get the column identified by columnId as it will now definitely exist
            List<Object> column = this.data.get(columnId);
            // If by some crazy alignment of the stars it does not then return failure
            if (column == null) {
                return false;
            }

            // If there is no column data to add then add a single string value of null.
            // This should never happen
            if (object == null) {
                return column.add("<null>");
            }

            // Add the column of data to the correct column
            return column.add(object);
        }
    }

    /**
     * Get the list underpinning this {@link AidaTable} object
     *
     * @return the list
     */
    public List<List<Object>> asList() {
        return this.data;
    }
}
