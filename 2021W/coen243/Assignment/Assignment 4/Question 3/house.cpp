// Assignment 4, Question 3, Week 10
// Hei Wang Andre Law, 4017 5600
// Program: user-defined class "House" defined by attributes
// (cpp file for implementing the member functions of the class)

#include<iostream>
#include <string>
#include "house.h"
using namespace std;

// class implementation for 'House'
House::House() {
	age = 0;
	type = "";
	rooms = 0;
	cost = 0.0;
}

// function estimating the price of a future house
double House::estimatePrice(string typeH, int year) {
	double appRateI; // appreciation rate within 5 years (Initial)
	double appRateF; // apprecition rate after 5 years (Final)
	int currentPri; // current house price
	double estPri; // final value of calculated price estimate

	// checks which type of house it is
	if (typeH == "Detached") {
		appRateI = 0.02;
		appRateF = 0.02;
		currentPri = 200000;
	}
	else if (typeH == "Semi-Attached") {
		appRateI = 0.02;
		appRateF = 0.03;
		currentPri = 150000;
	}
	else {
		appRateI = 0.01;
		appRateF = 0.02;
		currentPri = 100000;
	}

	// checks if we looking further than 5 years
	if (year > 5) { // formula when more than 5 years
		estPri = currentPri * ((pow(1 + appRateI, 5)) + (pow(1 + appRateI, year - 5)));
	}
	else { // formula when less than 5 years
		estPri = currentPri * (pow(1 + appRateI, year));
	}
	return estPri;
}

void House::ageH(int ageH) {
	age = ageH; // set the 'age' attribute
}

void House::typeH(string typeH) {
	type = typeH; // set the 'type' attribute
}

void House::roomsH(int roomsH) {
	rooms = roomsH; // set the 'rooms' attribute
}

void House::costH(double costH) {
	cost = costH; // set the 'cost' attribute
}

void House::display() { // get/prints all attributes of 'house'
	cout << "The attributes for this house are:";
	cout << "\nAge: " << age;
	cout << "\nType: " << type;
	cout << "\nRooms: " << rooms;
	cout << "\nCost: " << cost;
	cout << endl;
}