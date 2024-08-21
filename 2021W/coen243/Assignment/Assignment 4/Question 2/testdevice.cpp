// Assignment 4, Question 2, Week 10
// Hei Wang Andre Law, 4017 5600
// Program: user-defined class housing info of electronic devices
// (cpp file of main function)

#include<iostream>
#include <string>
#include "device.h"
using namespace std;

int main() {
	Edevice e1; // instantiate object e1 of class Edevice
	Edevice e2; // instantiate object e2 of class Edevice
	// declare the variables
	string cat; // 'category'
	int num; // 'model number'
	string col; // 'color'
	bool stat; // 'status'
	int year; // 'year'
	double price; // 'price'

	int checkNum = 0;
	int editNum = 0;

	// for-loop twice for each Edevice 
	for (int i = 1; i <= 2; i++) {
		// ask user to enter information of each attributes
		cout << "\nEnter the information for device number " << i;
		cout << "\n\nCategory: ('Smartphone', 'Tablet', 'Laptop', 'Smartwatch') ";
		cin >> cat; // 'category' input
		// while-loop until input corresponds to one of the option
		while (cat != "Smartphone" && cat != "Tablet" && cat != "Laptop" && cat != "Smartwatch") {
			cout << "Please try again...\nCategory: ('Smartphone', 'Tablet', 'Laptop', 'Smartwatch')";
			cin >> cat;
		}
		cout << "Model Number: ";
		cin >> num; // 'model nuber' input
		cout << "Color: ";
		cin >> col; // 'color' input
		cout << "Status: (0: false, 1:true) ";
		cin >> stat; // 'status' input
		cout << "Year Built: ";
		cin >> year; // 'year' input
		cout << "Price: ";
		cin >> price; // 'price' input

		// first loop means i is 1, thus storing the data in e1
		if (i == 1) {
			e1.category(cat); // set category of the device 
			e1.color(col); // set the color of the device
			e1.modelNumber(num); // set the model number of the device
			e1.status(stat); // set the status of the device
			e1.yearBuilt(year); // set the year of the device
			e1.priceD(price); // set the price of the device
		}
		// second loop means i is 2, thus storing the data in e2
		else {
			e2.category(cat); // set the category of the device
			e2.color(col); // set the color of the device
			e2.modelNumber(num); // set the model number of the device
			e2.status(stat); // set the status of the device
			e2.yearBuilt(year); // set the year of the device
			e2.priceD(price); // set the price of the device
		}
	}
	// loop as long as user doesn\t input '5'
	// this loop allows for user to interact with the member functions
	while (checkNum != 5) {
		cout << "\nEnter a number to perform one of the following commands:";
		cout << "\n1. Display the Information of Edevice One";
		cout << "\n2. Display the Informtion of Edevice Two";
		cout << "\n3. Edit an attribute of Edevice One";
		cout << "\n4. Edit an attribute of edevice Two";
		cout << "\n5. Exit Program\n\n";

		// switch-case performing the selected function
		cin >> checkNum;
		switch (checkNum) {
		case 1: // prints/get function for all attributes
			e1.display(); 
			break;
		case 2: // print/get function for all attributes
			e2.display(); 
			break;
		case 3: // modify/set function for all attributes of Edevice 1
			cout << "Enter a number corresponding an attribute to edit";
			cout << "\n1. Category";
			cout << "\n2. Model number";
			cout << "\n3. Color";
			cout << "\n4. Status";
			cout << "\n5. Year Built";
			cout << "\n6. Price\n";

			// switch-case for editing a specific attribute
			cin >> editNum;
			switch (editNum) {
			case 1:
				cout << "\nCategory: ('Smartphone', 'Tablet', 'Laptop', 'Smartwatch') ";
				cin >> cat;
				// while-loop until input corresponds to one of the option
				while (cat != "Smartphone" && cat != "Tablet" && cat != "Laptop" && cat != "Smartwatch") {
					cout << "Please try again...\nCategory: ('Smartphone', 'Tablet', 'Laptop', 'Smartwatch')";
					cin >> cat;
				}
				e1.category(cat); // set category of the device 
				break;
			case 2:
				cout << "Model Number: ";
				cin >> num;
				e1.modelNumber(num); // set the model number of the device
				break;
			case 3:
				cout << "Color: ";
				cin >> col;
				e1.color(col); // set the color of the device
				break;
			case 4:
				cout << "Status: (0: false, 1:true): ";
				cin >> stat;
				e1.status(stat); // set the status of the device
				break;
			case 5:
				cout << "Year Built: ";
				cin >> year;
				e1.yearBuilt(year); // set the year of the device
				break;
			case 6:
				cout << "Price: ";
				cin >> price;
				e1.priceD(price); // set the year of the device
				break;
			}
			break;
		case 4: // modify/set function for all attributes of Edevice 2
			cout << "Enter a number corresponding an attribute to edit";
			cout << "\n1. Category";
			cout << "\n2. Model number";
			cout << "\n3. Color";
			cout << "\n4. Status";
			cout << "\n5. Year Built";
			cout << "\n6. Price\n";

			// switch-case for editing a specific attribute
			cin >> editNum;
			switch (editNum) {
			case 1:
				cin >> cat;
				// while-loop until input corresponds to one of the option
				while (cat != "Smartphone" && cat != "Tablet" && cat != "Laptop" && cat != "Smartwatch") {
					cout << "Please try again...\nCategory: ('Smartphone', 'Tablet', 'Laptop', 'Smartwatch')";
					cin >> cat;
				}
				e2.category(cat); // set category of the device 
				break;
			case 2:
				cout << "Model Number: ";
				cin >> num;
				e2.modelNumber(num); // set the model number of the device
				break;
			case 3:
				cout << "Color: ";
				cin >> col;
				e2.color(col); // set the color of the device
				break;
			case 4:
				cout << "Status: (0: false, 1:true): ";
				cin >> stat;
				e2.status(stat); // set the status of the device
				break;
			case 5:
				cout << "Year Built: ";
				cin >> year;
				e2.yearBuilt(year); // set the year of the device
				break;
			case 6:
				cout << "Price: ";
				cin >> price;
				e2.priceD(price); // set the year of the device
				break;
			}
			break;
		case 5: // exits program
			cout << "You have exited the program...";
			break;
		}
	}
	return 0;
}