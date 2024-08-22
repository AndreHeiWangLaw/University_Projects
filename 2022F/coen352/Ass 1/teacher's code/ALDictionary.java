package coen352.ch4.dictionary;

import org.junit.jupiter.api.Test;

import coen352.ch4.list.ADTList;
import coen352.ch4.list.AList;

/** Source code example for "A Practical Introduction to Data
Structures and Algorithm Analysis, 3rd Edition (Java)" 
by Clifford A. Shaffer
Copyright 2008-2011 by Clifford A. Shaffer
*/



/** Dictionary implemented by unsorted array-based list. */
public class ALDictionary<Key, E> implements ADTDictionary<Key, E> {
	
	private static final int defaultSize = 100; // Default size
	

	
	private AList<Key> klist; 
	private AList<E> vlist; 
	
	private int[] index = null; 

	/** Constructors */
	ALDictionary() { 
		this(defaultSize); 
	}
	
	ALDictionary(int sz){ 
		klist = new AList<Key>(sz); 
		vlist = new AList<E>(sz);
	}
	
	/** Reinitialize */
	public void clear() { 
		klist.clear(); 
		vlist.clear();
	}
	
	/** Find k using sequential search
	  @return Record with key value k */
	public E find(Key k) {
		
		int pos = klist.find(k);
		if(pos<klist.length()&&pos>=0) {
			return vlist.getValue(pos);
		}
		return null;
	}
	
	
	/** Insert an element: append to list */
	public void insert(Key k, E e) {
		if(find(k)==null) {
			klist.append(k);
			vlist.append(e);
		}
			
	}
	
	/** Use sequential search to find the element to remove */
	public E remove(Key k) {
		E temp = find(k);
		int origin = klist.currPos();
		if (temp != null) {
			int pos = klist.find(k);
			klist.moveToPos(pos);
			vlist.moveToPos(pos);
			klist.remove();
			vlist.remove();
		}
		klist.moveToPos(origin);
		vlist.moveToPos(origin);
		return temp;
	}
	
	/** Remove the current element */
	public E removeAny() {
		
		if (size() != 0) {
		  klist.remove();
		  E temp = vlist.getValue();
		  vlist.remove();
		  return temp;
		}
		else 
			return null;
	}
	

	
	
	/** @return List size */
	public int size() { 
		return klist.length(); 
	}
	
	
	
	
	public E[] toArray() {
		return vlist.toArray();
	}
	
	
	public String toString() {
		int origin = klist.currPos();
		StringBuffer out = new StringBuffer();
		assert (vlist.length()==klist.length()): "the dict is inconsistent";
		for(int i=0;i<klist.length();i++) {
			out.append(klist.getValue().toString());
			klist.next();
			out.append(":");
			out.append(vlist.getValue().toString());
			out.append(" , ");
			vlist.next();
		}
		
		klist.moveToPos(origin);
		vlist.moveToPos(origin);
		return out.toString().trim();
		
	}
	
	 
	  public int find(AList<Key> klist, Key k) {
		  
	   int orgCurr = klist.currPos();
	   int pos = 0;
	   for(;pos<klist.length();pos++) {
		  klist.moveToPos(pos);
		  if (k == klist.getValue()) {
			  break;
		  }

			  
	   }
	   klist.moveToPos(orgCurr);
	   return pos;
	                             // k not found
	}
	  
	  /**
	  public int[] createDesendingIndex() {
			index = new int [klist.length()];
			// revise sorting algorithm to solve the right position of each record
			// according to if it is a ascending or descending order. 
			// the original record order must be reserved without any swap. 
			
			
			return index; 
			
			
		}
		*/
	
}