/** Source code example for "A Practical Introduction to Data
Structures and Algorithm Analysis, 3rd Edition (Java)" 
by Clifford A. Shaffer
Copyright 2008-2011 by Clifford A. Shaffer
*/

package coen352.a1;

// Based on PDF P.127

public class DLink<E> { // Doubly Linked list node
	private E element; // Value for this node
	private DLink<E> next; // Pointer to next node in list
	private DLink<E> prev; // Pointer to previous node in list

	// Constructors
	DLink(E it, DLink<E> prevval, DLink<E> nextval) { 
		element = it; 
		prev = prevval;
		next = nextval; 
	}
	DLink(DLink<E> prevval, DLink<E> nextval) { 
		next = nextval; 
		prev = prevval;
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

