
package coen352.a1;


import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.xssf.usermodel.XSSFSheet;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.util.Iterator;


public class WarehouseInventory {

	static int count = 0; //counter of number of inventories
	
	
	public static void main(String[] args) { //main function
		
		
		dict = new LLDictionary<String, Inventory>(); //create new dictionary named dict to store inventories
		
		for(int i=0; i<=10; i++) { //iterate creating Inventory classes (of rows) until Excel table reaches last row
			
			Inventory curr = new Inventory(); //creates new inventory
			
			insert(curr.getSku(), curr); //insert current inventory record to dictionary
			
			count = i; //update count
		}
		
		//create linked list dictionary 
		
		//WarehouseInventory.open();
		
	}
	
	
	private static ADTDictionary<String, Inventory> dict;
	
	
	
	//methods of WarehouseInventory
	public void clear() { //3. Clear the database
		dict.clear();
	}
	
	public static void insert(String sku, Inventory curr) { //1.Insert a record
		dict.insert(sku, curr); 
	}
	
	public Inventory remove(String sku) { //2. Remove a record given a key
		return dict.remove(sku);
	}
	
	public Inventory removeAny() {
		return dict.removeAny();
	}
	
	public Inventory find(String sku) { //4.find a record given a key
		return dict.find(sku);
	}

	public int getNoInventory() {
		int n = count;
		return n;
	}
	
	


	
	public static void open() {
		
	}
	
	
	
}
	
	
	
