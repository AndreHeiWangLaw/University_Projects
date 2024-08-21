// Assignment 4, Question 2, Week 10
// Hei Wang Andre Law, 4017 5600
// Program: user-defined class housing info of electronic devices
// (header file containing the specification of the class)

#include <string>
using namespace std;

// class definition for Edevice
class Edevice {
private: // data members
	string cat; // category of the Edevice
	int mod; // model number of the Edevice
	string col; // color of the Edevice
	bool stat; // starus of the Edevice
	int year; // year of the Edevice
	double price; // price of the Edevice

public: // member function
	Edevice(); // constructor
	void category(string cat); // set category of the device 
	void modelNumber(int); // set model number of the device 
	void color(string col); // set color of the device 
	void status(bool stat); // set status of the device 
	void yearBuilt(int year); // set year of the device 
	void priceD(double price); // set price of the device 
	void display(); // print/get all attributes
}; // end of class Edevice 