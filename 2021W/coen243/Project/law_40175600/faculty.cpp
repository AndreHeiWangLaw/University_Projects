// COEN 243 Project, Week 10
// Hei Wang Andre Law, 4017 5600
// Program: System that Handles the Records of Students at Concordia
// File: faculty.cpp

#include <iostream>
#include <string>
#include <fstream>
#include "faculty.h"
using namespace std;

// class implementaion for 'faculty'
Faculty::Faculty() { // constructor
	highNumCivil = 0.0;
	highNumMech = 0.0;
	highNumElecComp = 0.0;
};

string* Faculty::getFile(string name, const int max, int* count) {
	string* data = new string[max];
	ifstream is(name.c_str());
	*count = 0;
	while (*count < max && getline(is, data[*count])) {
		++* count;
	}
	return data;
}


// display highest gpa member function
void Faculty::highestGpa(string gpa1, string gpa2, string gpa3) {
	cout << "The highest gpa in this faculty is ";
	const int numLine = 41;
	highNumCivil = stof(gpa1); // convert string to float
	highNumMech = stof(gpa2); // convert string to float
	highNumElecComp = stof(gpa3); // convert string to float
	// compare the highest gpa, then print it
	if (highNumCivil > highNumElecComp && highNumCivil > highNumMech)
		cout << highNumCivil;
	else if (highNumElecComp > highNumCivil && highNumElecComp > highNumMech)
		cout << highNumElecComp;
	else if (highNumMech > highNumElecComp && highNumMech > highNumCivil)
		cout << highNumMech;
	else
		cout << "Error";
	cout << endl << endl;
}

void Faculty::numUnderGrad() {

}
void Faculty::numGrad() {

}
void Faculty::avgunderGrad() {

}