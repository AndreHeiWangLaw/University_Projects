// COEN 243 Project, Week 10
// Hei Wang Andre Law, 4017 5600
// Program: System that Handles the Records of Students at Concordia
// File: faculty.h

#include <string>
using namespace std;

// clas definition for 'faculty'
class Faculty {
private:
	float highNumCivil; // num being the highest gpa in civil
	float highNumMech; // num being the highest gpa in mech
	float highNumElecComp; // num being the highest gpa in elec and comp
public:
	Faculty(); // constructor
	string *getFile(string, const int, int*);
	void highestGpa(string, string, string);
	void numUnderGrad();
	void numGrad();
	void avgunderGrad();
};
