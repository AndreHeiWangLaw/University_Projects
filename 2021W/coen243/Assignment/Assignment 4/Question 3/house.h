// Assignment 4, Question 3, Week 10
// Hei Wang Andre Law, 4017 5600
// Program: user-defined class "House" defined by attributes
// (header file containing the specification of the class)

#include <string>
using namespace std;

// class definition for Edevice
class House {
private: // data members
	int age;
	string type;
	int rooms;
	double cost;
	
public: // member function
	House(); // constructor
	double estimatePrice(string typeH, int year); // estimation function call
	void ageH(int age); // attributes
	void typeH(string type); // attributes
	void roomsH(int rooms); // attributes
	void costH(double cost); // attributes
	void display(); // prints/get/display the current house attributes
}; // end of class Edevice 