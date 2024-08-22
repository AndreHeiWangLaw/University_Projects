package coen352.ch4.dictionary;

import org.junit.runner.JUnitCore;
import org.junit.runner.Result;
import org.junit.runner.notification.Failure;


public class DictionaryManualTest {

		public static void main(String[] args) {
		   
			DictionaryManualTest.runManualTest();
		   
	   }
	   
	   public static void runManualTest() {
		   DictionaryJUnitTest t = new DictionaryJUnitTest();
		   DictionaryJUnitTest.setUp();
		   t.testClear();
		   t.testFind();
		   t.testInsert();
		   t.testRemove();
		   t.testRemoveAny();
		   t.testSize();
	   }
	   
	
}