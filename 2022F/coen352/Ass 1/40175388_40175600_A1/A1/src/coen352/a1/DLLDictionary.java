/** Source code example for "A Practical Introduction to Data
Structures and Algorithm Analysis, 3rd Edition (Java)" 
by Clifford A. Shaffer
Copyright 2008-2011 by Clifford A. Shaffer
*/

package coen352.a1;

// Based on PDF P.164

/** Dictionary implemented by double linked list-based. */
public class DLLDictionary<Key, E> implements ADTDictionary<Key, E> {

	// to store key-value pair dictionary
	private DList<KVpair <Key, E>> pair;
	
	// Constructor
	DLLDictionary() {
		pair = new DList<KVpair <Key, E>> ();
	}
	
	// Reinitialize
	public void clear()	{
		pair.clear();
	}
	
	// Insert an element: append to list
	public void insert(Key k, E e) {
		// ensure the key is unique
		KVpair<Key, E> temp1 = new KVpair<Key, E>(k, e);
		if ( find( k ) == null ) {
			pair.append(temp1);
		}
	}

	// Use sequential search to find the element to remove 
	public E remove(Key k) {
		
		E temp2 = find(k);
        if (temp2 != null)
            pair.remove();
        return temp2;
	}

	// Remove the last element 
	public E removeAny() {
		if ( size() != 0 ) {
			pair.moveToEnd();
			pair.prev();
			KVpair<Key, E> e = pair.remove();
			return e.value();
		}
		else
			return null;
	}

	// Find "k" using sequential search
	public E find(Key k) {
		pair.moveToStart(); // start at beginning of list
		for(int i = 0; i < pair.length(); i++) {
			if(pair.getValue().key() == k) {
				return pair.getValue().value(); // return key value found
			}
			pair.next(); // next element
		}
		return null; // key not found
	}

	// Return list size
	public int size() {
		return pair.length();
	}
}