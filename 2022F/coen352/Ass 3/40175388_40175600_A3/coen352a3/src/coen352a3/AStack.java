/** Source code example for "A Practical Introduction to Data
Structures and Algorithm Analysis, 3rd Edition (Java)" 
by Clifford A. Shaffer
Copyright 2008-2011 by Clifford A. Shaffer
*/

// PDF page 126

package coen352a3;

/** Array-based stack implementation */
class AStack<E> implements Stack<E> {
	
	private static final int defaultSize = 10;
	
	private int maxSize; // Maximum size of stack
	private int top; // Index for top Object
	private E [] listArray; // Array holding stack
	
	// Constructors
	AStack() { this(defaultSize); }
	@SuppressWarnings("unchecked") // Generic array allocation
	AStack(int size) {
		maxSize = size;
		top = 0;
		listArray = (E[])new Object[size]; // Create listArray
	}
	
	public void clear() // Reinitialize stack
	{ top = 0; }
	
	public void push(E it) { // Push "it" onto stack
		assert top != maxSize : "Stack is full";
		listArray[top++] = it;
	}
	
	public E pop() { // Pop top element
		assert top != 0 : "Stack is empty";
		return listArray[--top];
	}
	
	public E topValue() { // Return top element
		assert top != 0 : "Stack is empty";
		return listArray[top-1];
	}
	
	public int length() { return top; } // Return length
}