// COEN 243 Project, Week 10
// Hei Wang Andre Law, 4017 5600
// Program: System that Handles the Records of Students at Concordia
// File: student.cpp

#include <iostream>
#include <string>
#include <fstream>
#include "student.h"
using namespace std;

// class implementaion for 'student'
Student::Student() {
	fName = "";
	lName = "";
	id = 0;
	dateBirth = 0;
	gpa = 0.0;
	startYear = 0;
	compCredit = 0.0;
	program = "";
}
// All set functions opens an ofstream 'outFile' and check if it exists, if it does
// write specific variable into the file, then close it. (Same for all 8 setters)
void Student::setfName(string fname) {
	fName = fname;
	ofstream outFile;
	outFile.open("student.txt", ios::app);
	if (!outFile) {
		cout << "Error";
	}
	else { 
		outFile << fName << endl;
	}
	outFile.close();
}
void Student::setlName(string lname) {
	lName = lname;
	ofstream outFile;
	outFile.open("student.txt", ios::app);
	if (!outFile) {
		cout << "Error";
	}
	else {
		outFile << lName << endl;
	}
	outFile.close();
}
void Student::setId(int Id) {
	id = Id;
	ofstream outFile;
	outFile.open("student.txt", ios::app);
	if (!outFile) {
		cout << "Error";
	}
	else {
		outFile << id << endl;
	}
	outFile.close();
}
void Student::setDateBirth(int datebirth) {
	dateBirth = datebirth;
	ofstream outFile;
	outFile.open("student.txt", ios::app);
	if (!outFile) {
		cout << "Error";
	}
	else {
		outFile << dateBirth << endl;
	}
	outFile.close();
}
void Student::setGpa(float Gpa) {
	gpa = Gpa;
	ofstream outFile;
	outFile.open("student.txt", ios::app);
	if (!outFile) {
		cout << "Error";
	}
	else {
		outFile << gpa << endl;
	}
	outFile.close();
}
void Student::setStartYear(int startyear) {
	startYear = startyear;
	ofstream outFile;
	outFile.open("student.txt", ios::app);
	if (!outFile) {
		cout << "Error";
	}
	else {
		outFile << startYear << endl;
	}
	outFile.close();
}
void Student::setCompCredit(float compcredit) {
	compCredit = compcredit;
	ofstream outFile;
	outFile.open("student.txt", ios::app);
	if (!outFile) {
		cout << "Error";
	}
	else {
		outFile << compCredit << endl;
	}
	outFile.close();
}
void Student::setProgram(string prog) {
	program = prog;
	ofstream outFile;
	outFile.open("student.txt", ios::app);
	if (!outFile) {
		cout << "Error";
	}
	else {
		outFile << program << endl;
	}
	outFile.close();
}

// A;; get functions simply return specific variable of the called function
string Student::getfName() {
	return fName;
}
string Student::getlName() {
	return lName;
}
int Student::getId() {
	return id;
}
int Student::getDateBirth() {
	return dateBirth;
}
float Student::getGpa() {
	return gpa;
}
int Student::getStartYear() {
	return startYear;
}
float Student::getCompCredit() {
	return compCredit;
}
string Student::getProgram() {
	return program;
}

// complete program boolean function
bool Student::completeprogram() {
	// check which program student is enrolled. then
	// compare completed credits to said progaram
	// return true(1) when completed, otherwise false(0)
	if (program == "B") {
		if (compCredit >= 140)
			return true;
		else
			return false;
	}
	else if (program == "M") {
		if (compCredit >= 16)
			return true;
		else
			return false;
	}
	else if (program == "P") {
		if (compCredit >= 12)
			return true;
		else
			return false;
	}
	else {
		cout << "Invalid.";
		return 0;
	}
}

// student status function, checks gpa range and returns
// appropriate letter grade of the student
string Student::studentStatus() {
	if (gpa >= 3.5)
		return "A+";
	else if (gpa >= 3.0)
		return "A";
	else if (gpa >= 2.5)
		return "B";
	else if (gpa >= 2.0)
		return "C";
	else 
		return "D";
}

// print student info function, 'cout' all 8 attributes
void Student::printStdInfo() {
	cout << fName << endl;
	cout << lName << endl;
	cout << id << endl;
	cout << dateBirth << endl;
	cout << gpa << endl;
	cout << startYear << endl;
	cout << compCredit << endl;
	cout << program << endl;
}

// compare gpa function, take in two floats and compares them
// return the higher gpa
float Student::compareGpa(float gpa1, float gpa2) {
	if (gpa1 > gpa2)
		return gpa1;
	else
		return gpa2;
}