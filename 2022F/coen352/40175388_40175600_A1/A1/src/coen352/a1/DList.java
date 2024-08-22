/** Source code example for "A Practical Introduction to Data
Structures and Algorithm Analysis, 3rd Edition (Java)" 
by Clifford A. Shaffer
Copyright 2008-2011 by Clifford A. Shaffer
*/

package coen352.a1;

// Based on PDF P.130

// Double DLinked list implementation
public class DList<E> implements ADTList<E> {
	private DLink<E> head; // Pointer to list header
	private DLink<E> tail; // Pointer to last element
	protected DLink<E> curr; // Access to current element
	int cnt; // Size of list
	
	//Constructors
	public DList(int size) { 
		this(); } // Constructor -- Ignore size
	
	public DList() {
		curr = head = new DLink<E>(null, null); // Create header
		tail = new DLink<E>(head, null);
		head.setNext(tail);
		cnt = 0;
	}
	public void clear() { // Remove all elements
		head.setNext(null); // Drop access to DLinks
		curr = head = new DLink<E>(null, null); // Create header
		tail = new DLink<E>(head, null); 
		head.setNext(tail); 
		cnt = 0;
	} 
	
// Based on PDF P.140
	
	// Insert "it" at current position
	public void insert(E it) {
		curr.setNext(new DLink<E>(it, curr, curr.next()));
		curr.next().next().setPrev(curr.next());
		cnt++;
	}
	public void append(E it) { // Append "it" to list
		tail.setPrev(new DLink<E>(it, tail.prev(), tail));
		tail.prev().prev().setNext(tail.prev());
		cnt++;
	}
	// Remove and return first element in right partition
	public E remove() {
		if (curr.next() == tail) 
			return null; // Nothing to remove
		E it = curr.next().element(); // Remember value
		curr.next().next().setPrev(curr);
		curr.setNext(curr.next().next()); // Remove from list
		cnt--; // Decrement the count
		return it; // Return value removed
	}
	
	// Move curr one step left; no change if at front
	public void prev() {
		if (curr != head) // Can’t back up from list head
			curr = curr.prev();
	}
	
// Based on PDF P.131
	
	public void moveToStart() {// Set curr at list start
		curr = head; } 
	
	public void moveToEnd() {// Set curr at list end
		curr = tail.prev(); } 
	
	// Move curr one step right; no change if already at end
	public void next() {
		if (curr != tail) 
			curr = curr.next(); } 
	
	public int length() { 
		return cnt; } // Return length
	
	// Return the position of the current element
	public int currPos() {
		DLink<E> temp = head;
		int i;
		for (i=0; curr != temp; i++)
			temp = temp.next();
		return i;
	}
	
	// Move down list to "pos" position
	public void moveToPos(int pos) {
		assert (pos>=0) && (pos<=cnt) : "Position out of range";
		curr = head;
		for(int i=0; i<pos; i++) 
			curr = curr.next();
	}
	
	public E getValue() { // Return current element
		if(curr.next() == tail) 
			return null;
		return curr.next().element();
	}
}