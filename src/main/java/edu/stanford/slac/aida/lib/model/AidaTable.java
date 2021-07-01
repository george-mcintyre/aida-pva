package edu.stanford.slac.aida.lib.model;

import lombok.ToString;

import java.util.ArrayList;
import java.util.List;

/**
 * Interface object to pass tables between JNI/C and Java
 */
@ToString
public class AidaTable {
    private final List<List<Object>> data;

    public AidaTable() {
        this.data = new ArrayList<List<Object>>();
    }

    /**
     * Add an element to the specified numbered collection
     *
     * @param collectionID the number representing the number of the collection to add to
     * @param object       the object to add to that collection
     * @return true if added correctly
     */
    public boolean add(int collectionID, Object object) {
        synchronized (this.data) {
            List<Object> collection;
            int currentCollectionCount = this.data.size();
            if (currentCollectionCount <= collectionID) {
                for (int collectionCounter = currentCollectionCount - 1; collectionCounter < collectionID; collectionCounter++) {
                    collection = new ArrayList<Object>();
                    this.data.add(collection);
                }
            }
            collection = this.data.get(collectionID);
            if (collection == null) {
                return false;
            }

            if (object == null) {
                return collection.add("<null>");
            }

            return collection.add(object);
        }
    }

    /**
     * Get the list underpinning this AidaTable object
     *
     * @return the list
     */
    public List<List<Object>> asList() {
        return this.data;
    }
}
