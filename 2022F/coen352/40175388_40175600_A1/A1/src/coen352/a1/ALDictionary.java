/** Source code example for "A Practical Introduction to Data
Structures and Algorithm Analysis, 3rd Edition (Java)" 
by Clifford A. Shaffer
Copyright 2008-2011 by Clifford A. Shaffer
*/

package coen352.a1;

import org.junit.jupiter.api.Test;

// Based on PDF P.164

/** Dictionary implemented by unsorted array-based list. */
class ALDictionary<Key, E> implements ADTDictionary<Key, E> {
	private static final int defaultSize = 10; // Default size
	private ADTList<KVpair<Key,E>> list; // To store dictionary
	
	// Constructors
	ALDictionary() { 
		this(defaultSize); 
	}
	ALDictionary(int sz) { 
		list = new AList<KVpair<Key, E>>(sz);
	}
	public void clear() { 
		list.clear(); 
	} // Reinitialize
	
	
	/** Insert an element: append to list */
	public void insert(Key k, E e) {
		KVpair<Key,E> temp = new KVpair<Key,E>(k, e);
		list.append(temp);
	}
	
	/** Use sequential search to find the element to remove */
	public E remove(Key k) {
		E temp = find(k);
		if (temp != null) 
			list.remove();
		return temp;
	}
	
	/** Remove the last element */
	public E removeAny() {
		if (size() != 0) {
			list.moveToEnd();
			list.prev();
			KVpair<Key,E> e = list.remove();
			return e.value();
		}
		else 
			return null;
	}
	
	// Find "k" using sequential search
	public E find(Key k) {
		for(list.moveToStart(); list.currPos() < list.length();
				list.next()) {
			KVpair<Key,E> temp = list.getValue();
			if (k == temp.key())
				return temp.value();
		}
		return null; // "k" does not appear in dictionary
	} 
	
	// Return list size
	public int size() { 
		return list.length();
	}
}
