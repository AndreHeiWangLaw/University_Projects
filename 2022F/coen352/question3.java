class DLink<E> { // Doubly linked list node
	private E element; // Value for this node
	private DLink<E> next; // Pointer to next node in list
	private DLink<E> prev; // Pointer to previous node
	
	// Constructors
	DLink(E it, DLink<E> p, DLink<E> n) {
		element = it; prev = p; next = n; 
	}
	DLink(DLink<E> p, DLink<E> n) {
		prev = p; next = n; 
	}
	DLink<E> next() { 
		return next; 
	}
	DLink<E> setNext(DLink<E> nextval) {
		return next = nextval;
	}
	DLink<E> prev() { 
		return prev; 
	}
	DLink<E> setPrev(DLink<E> prevval) { 
		return prev = prevval;
	}
	E element() { 
		return element;
	}
	E setElement(E it) { 
		return element = it; 
	}
} // class DLink

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
	if (curr.next() == tail) return null; // Nothing to remove
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
