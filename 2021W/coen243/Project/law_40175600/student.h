// COEN 243 Project, Week 10
// Hei Wang Andre Law, 4017 5600
// Program: System that Handles the Records of Students at Concordia
// File: student.h

#include <string>
using namespace std;

// clas definition for 'student'
class Student {
private: // data member
	string fName; // first name
	string lName; // last name
	int id; // student id
	int dateBirth; // date of birth
	float gpa; // student gpa
	int startYear; // start year
	float compCredit; // completed credit
	string program; // program

public: // member function
	Student(); // constructor
	// setters
	void setfName(string);
	void setlName(string);
	void setId(int);
	void setDateBirth(int);
	void setGpa(float);
	void setStartYear(int);
	void setCompCredit(float);
	void setProgram(string);

	// getters
	string getfName();
	string getlName();
	int getId();
	int getDateBirth();
	float getGpa();
	int getStartYear();
	float getCompCredit();
	string getProgram();

	// 'student' functions
	bool completeprogram();
	string studentStatus();
	void printStdInfo();
	float compareGpa(float, float);
};
