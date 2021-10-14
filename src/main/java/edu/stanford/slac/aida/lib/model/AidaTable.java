package edu.stanford.slac.aida.lib.model;

import lombok.ToString;

import java.util.ArrayList;
import java.util.List;

/**
 * This class encapsulates an {@link AidaConfigGroup}.
 * <p>
 * It is used to represent the configuration that will apply to a single group of {@link AidaChannel}s.
 * <p>
 * The {@link AidaConfigGroup} class controls how the AIDA-PVA Channel Provider responds to a channel request.
 * <p>
 * There is a {@link AidaConfigGroup#getterConfig} for the `get` requests,
 * and a {@link AidaConfigGroup#setterConfig} for the `set` requests.
 * <p>
 * The List of {@link AidaConfigGroup#channels} are the names of the channels that will use the specified config
 * <p>
 * Note that it uses the {@link lombok.Data} annotation to provide all the getters and setters.
 * a constructor with all required arguments,
 * and an equals(), hashcode() and toString()  method.
 * <p>
 * It also uses the {@link lombok.NoArgsConstructor} annotation to provide a constructor
 * with no arguments.
 * <p>
 */
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
