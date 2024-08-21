// Assignment 4, Question 3, Week 10
// Hei Wang Andre Law, 4017 5600
// Program: user-defined class "House" defined by attributes
// (cpp file of main function)

#include<iostream>
#include <string>
#include "house.h"
using namespace std;

int main() {
	House h1; // calls the constructor, linking it to house 2
	House h2; // calls the constructor, linking it to house 1
	House h; // calls the constructor, linking it to the house estimate

	// declare variables
	int age; // 'age' of the house
	string type; // 'type' of house
	int rooms; // 'rooms number' of the house
	double cost; // 'cost' of the house
	int checkNum = 0; // check number for the while-loop
	int editNum = 0; // switch-case, attribute to edit
	int yearEstPri; // price estimate year number

	// for-loop twice for each 'House' 
	for (int i = 1; i <= 2; i++) {
		// ask user to enter information of each attributes
		cout << "\nEnter the information for 'house' number " << i;
		cout << "\n\nAge: ";
		cin >> age; // 'age' input
		cout << "Type:  ('Detached', 'Semi-Attached', 'Attached') ";
		cin >> type; // 'type' input
		// repeat until the type corresponds to one of the option
		while (type != "Detached" && type != "Semi-Attached" && type != "Attached") {
			cout << "Try again...\nType: ";
			cin >> type;
		}
		cout << "Rooms Number: ";
		cin >> rooms; // 'rooms' input
		cout << "Cost: ";
		cin >> cost; // 'cost' input
		
		// first loop means i is 1, thus storing the data in h1
		if (i == 1) {
			h1.ageH(age); // set age of the 'house' 
			h1.typeH(type); // set the type of the 'house'
			h1.roomsH(rooms); // set the rooms of the 'house'
			h1.costH(cost); // set the cost of the 'house'
		}
		// second loop means i is 2, thus storing the data in h2
		else {
			h2.ageH(age); // set the age of the 'house'
			h2.typeH(type); // set the type of the 'house'
			h2.roomsH(rooms); // set the rooms of the 'house'
			h2.costH(cost); // set the cost of the 'house'
		}
	}

	// loop as long as user doesn\t input '5'
	// this loop allows for user to interact with the member functions
	while (checkNum != 6) {
		cout << "\nEnter a number to perform one of the following commands:";
		cout << "\n1. Display the Information of House One";
		cout << "\n2. Display the Informtion of House Two";
		cout << "\n3. Edit an attribute of House One";
		cout << "\n4. Edit an attribute of House Two";
		cout << "\n5. Display the price estimate of a house in the future";
		cout << "\n6. Exit Program\n\n";

		// switch-case performing the selected function
		cin >> checkNum;
		switch (checkNum) {
		case 1: // prints/get function for all attributes
			h1.display();
			break;
		case 2: // print/get function for all attributes
			h2.display();
			break;
		case 3: // modify/set function for all attributes of House 1
			cout << "Enter a number corresponding an attribute to edit";
			cout << "\n1. Age";
			cout << "\n2. Type";
			cout << "\n3. Rooms";
			cout << "\n4. Cost\n";

			// switch-case for editing a specific attribute
			cin >> editNum;
			switch (editNum) {
			case 1:
				cout << "Age: ";
				cin >> age;
				h1.ageH(age); // set category of the device 
				break;
			case 2:
				cin >> type;
				cout << "Type: ('Detached', 'Semi-Attached', 'Attached') ";
				cin >> type; // 'type' input
				// repeat until the type corresponds to one of the option
				while (type != "Detached" && type != "Semi-Attached" && type != "Attached") {
					cout << "Try again...\nType: ";
					cin >> type;
				}
				h1.typeH(type); // set the color of the device
				break;
			case 3:
				cout << "Rooms: ";
				cin >> rooms;
				h1.roomsH(rooms); // set the model number of the device
				break;
			case 4:
				cout << "Cost: ";
				cin >> cost;
				h1.costH(cost); // set the status of the device
				break;
			}
			break;
		case 4: // modify/set function for all attributes of House 2
			cout << "Enter a number corresponding an attribute to edit";
			cout << "\n1. Age";
			cout << "\n2. Type";
			cout << "\n3. Rooms";
			cout << "\n4. Cost\n";

			// switch-case for editing a specific attribute
			cin >> editNum;
			switch (editNum) {
			case 1:
				cout << "Age: ";
				cin >> age;
				h2.ageH(age); // set category of the device 
				break;
			case 2:
				cout << "Type: ('Detached', 'Semi-Attached', 'Attached') ";
				cin >> type; // 'type' input
				// repeat until the type corresponds to one of the option
				while (type != "Detached" && type != "Semi-Attached" && type != "Attached") {
					cout << "Try again...\nType: ";
					cin >> type;
				}
				h2.typeH(type); // set the color of the device
				break;
			case 3:
				cout << "Rooms: ";
				cin >> rooms;
				h2.roomsH(rooms); // set the model number of the device
				break;
			case 4:
				cout << "Cost: ";
				cin >> cost;
				h2.costH(cost); // set the status of the device
				break;
			}
			break;
		case 5: // 'estimatePrice()' function call
			cout << "Type: ('Detached', 'Semi-Attached', 'Attached') ";
			cin >> type; // 'type' input
			// repeat until the type corresponds to one of the option
			while (type != "Detached" && type != "Semi-Attached" && type != "Attached") {
				cout << "Try again...\nType: ";
				cin >> type;
			}
			cout << "Enter the years from now: ";
			cin >> yearEstPri; // number of years looking into the future
			cout << "The cost estimate of this house is ";
			cout << h.estimatePrice(type, yearEstPri) << endl; // function call
			break;
		case 6: // exits program
			cout << "You have exited the program...";
			break;
		}
	}
	return 0;
}