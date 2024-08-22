package coen352.a1;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Iterator;

import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.xssf.usermodel.XSSFSheet;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;



public class Inventory {

	//class data members
	
	private String sku;
	private String description;
	private String bin_no;
	private String location;
	private double qty;
	private double reorder_qty;
	private double cost;
	private String reorder;
	
	//methods
	
	public String getSku() { //get SKU value for given Inventory object
		return sku;
	}
	
	public double getCost() {
		return cost;
	}
	
	
	
	
	
	public Inventory() { //constructor (initializes all of the data members of Inventory

	
		ClassLoader classloader = Thread.currentThread().getContextClassLoader();
        InputStream is = classloader.getResourceAsStream("tf16400470_win32.xltm"); //open the excel file


		XSSFWorkbook workbook = null;
		try {
			workbook = new XSSFWorkbook(is);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		//select 4th sheet in the workbook
		XSSFSheet sheet = workbook.getSheetAt(3);

		//Iterate through each row
		Iterator<Row> rowIterator = sheet.iterator();{
		while (rowIterator.hasNext()) 
		{
			Row row = rowIterator.next();
			
			//For each row, iterate through all the columns
			Iterator<Cell> cellIterator = row.cellIterator();
			Cell cell = cellIterator.next();
				
				Iterator<Cell> cellIterator1 = row.cellIterator();
				sku = cell.getStringCellValue();
				
				Iterator<Cell> cellIterator2 = row.cellIterator();
				description = cell.getStringCellValue();
				
				Iterator<Cell> cellIterator3 = row.cellIterator();
				bin_no = cell.getStringCellValue();
				
				Iterator<Cell> cellIterator4 = row.cellIterator();
				location = cell.getStringCellValue();
				
				Iterator<Cell> cellIterator5 = row.cellIterator();
				qty = cell.getNumericCellValue();
				
				Iterator<Cell> cellIterator6 = row.cellIterator();
				reorder_qty = cell.getNumericCellValue();
				
				Iterator<Cell> cellIterator7 = row.cellIterator();
				cost = cell.getNumericCellValue();
				
				Iterator<Cell> cellIterator8 = row.cellIterator();
				reorder = cell.getStringCellValue();
				
			}
		}
		is.close();
	}
}
	
