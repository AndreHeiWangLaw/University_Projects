// Assignment 4, Question 2, Week 10
// Hei Wang Andre Law, 4017 5600
// Program: user-defined class housing info of electronic devices
// (cpp file for implementing the member functions of the class)

#include<iostream>
#include <string>
#include "device.h"
using namespace std;

// class implementation of Edevice
Edevice::Edevice() {
	// declare the variables for each atttributes
	cat = "";
	mod = 0;
	col = "";
	stat = true;
	year = 0;
	price = 0.0;
}
// 'category' attribute
void Edevice::category(string category) {
	cat = category;
}
// 'model number' attribute
void Edevice::modelNumber(int number) {
	mod = number;
}
// 'color' attribute
void Edevice::color(string color) {
	col = color;
}
// 'status' attribute
void Edevice::status(bool status) {
	stat = status;
}
// 'year' attribute
void Edevice::yearBuilt(int yearBuilt) {
	year = yearBuilt;
}
// 'price' attribute
void Edevice::priceD(double pri) {
	price = pri;
}
// print all atributes
void Edevice::display() {
	cout << "The attributes for this device are:";
	cout << "\nCategory: " << cat;
	cout << "\nModel Number: " << mod;
	cout << "\nColor: " << col;
	cout << "\nStatus: " << stat;
	cout << "\nYear Built: " << year;
	cout << "\nPrice: " << price;
	cout << endl;
}