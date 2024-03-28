// COEN 243 Project, Week 10
// Hei Wang Andre Law, 4017 5600
// Program: System that Handles the Records of Students at Concordia
// File: driver.cpp

#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <algorithm>
#include "student.h"
#include "faculty.h"
using namespace std;

int main() { // main function to test the whole system
	//------------------------Faculty Start-----------------------------//
	Faculty fa; // assign object "Faculty" to fa
	int count;
	int numSize1 = 41; // lines of 1 txt file
	int numSize2 = 82; // lines of 2 txt files
	int numSize3 = 123; // lines of 3 txt files
	const int arraySize = 123;
	string numMax;
	int counterGpa = 0;

	// reads studentCivil.txt and create a dynamic array
	string* lineCivil = fa.getFile("studentCivil.txt", numSize1, &count);
	
	// reads studentMech.txt and create a dynamic array
	string* lineMech = fa.getFile("studentMech.txt", numSize1, &count);
	
	// reads studentElecComp.txt and create a dynamic array
	string* lineElecComp = fa.getFile("studentElecComp.txt", numSize1, &count);

	// obtain highest number for each txt file
	for (int i = 5; i < numSize1-8; i = i + 8) {
		if (lineCivil[5] < lineCivil[i])
			lineCivil[5] = lineCivil[i];
	}
	for (int i = 5; i < numSize1- 8; i = i + 8) {
		if (lineMech[5] < lineMech[i])
			lineMech[5] = lineMech[i];
	}
	for (int i = 5; i < numSize1- 8; i = i + 8) {
		if (lineElecComp[5] < lineElecComp[i])
			lineElecComp[5] = lineElecComp[i];
	}
	
	fa.highestGpa(lineCivil[5], lineMech[5], lineElecComp[5]);

	/*Pseudo-Code: 
	* 1-Combine lineCivil, lineMech and lineElecComp into a single dynamic array
	* 2-Member function for numOfUnderGrad:
	*	a-Use linear search array => Find amount of M (masters) and P (Ph.D)
	*	b-Use linear search array => Find amount of B (bachelor) with completed program
	*	c-Total num of students - (part a + part b) = numOfUnderGrad
	* 3-Member function for numOfGrad:
	*	a-Add part a and part b from point 2
	* 4-Member function for AvgunderGrad:
	*	a-For every underGrad found previously, determine their info and locate their gpa
	*	b-Add specific array pointer, then divide by numOfUnderGrad for the average
	* 5-Member function for avgGrad:
	*	a-For all graduate found earlier, determine their info, locate their gpa
	*	b-Add this array pointer together and divide by total amount of graduates
	*/

	//fa.numUnderGrad();
	//fa.numGrad();
	//fa.avgunderGrad();

	//------------------------Faculty End-------------------------------//
	//------------------------Student Start-----------------------------//
	Student st1; // assign object "Student" to st1
	Student st2; // assign object "Student" to st2
	Student st3; // assign object "Student" to st3
	Student st4; // assign object "Student" to st4
	Student st5; // assign object "Student" to st5

	// input variables, used in filling the txt file
	string fName; // first name
	string lName; // last name
	int id; // student id
	int birth; // date of birth
	float gpa; // student gpa
	int startYear; // start year date
	float comCre; // completed credit
	string program; // program (bachelor, master, Ph.D)

	int numFunc = 0; // switch, number input performing the selected function
	int numId; // switch, number input selecting the right student
	float numCompareGpa1 = 0; // student 1, compare gpa function
	float numCompareGpa2 = 0; // student 2, compare gpa function
	int numId1; // switch, number input selecting the first student
	int numId2; // switch, number input selecting the second student

	// Request input to fill in the student.txt file, repeat five times
	// and assign set functions for each.
	cout << "Fill in the information of five students.";
	cout << "\nThe following will be for student number ONE";
	cout << "\nFirst Name: "; cin >> fName; st1.setfName(fName);
	cout << "Last Name : "; cin >> lName; st1.setlName(lName);
	cout << "Id: "; cin >> id; st1.setId(id);
	cout << "Date of Birth: "; cin >> birth; st1.setDateBirth(birth);
	cout << "GPA (0 - 4.4): "; cin >> gpa; st1.setGpa(gpa);
	cout << "Start Year: "; cin >> startYear; st1.setStartYear(startYear);
	cout << "Completed Credit: "; cin >> comCre; st1.setCompCredit(comCre);
	cout << "Program (B, M, P): "; cin >> program; st1.setProgram(program);

	cout << "\nThe following will be for student number TWO";
	cout << "\nFirst Name: "; cin >> fName; st2.setfName(fName);
	cout << "Last Name : "; cin >> lName; st2.setlName(lName);
	cout << "Id: "; cin >> id; st2.setId(id);
	cout << "Date of Birth: "; cin >> birth; st2.setDateBirth(birth);
	cout << "GPA (0 - 4.4): "; cin >> gpa; st2.setGpa(gpa);
	cout << "Start Year: "; cin >> startYear; st2.setStartYear(startYear);
	cout << "Completed Credit: "; cin >> comCre; st2.setCompCredit(comCre);
	cout << "Program (B, M, P): "; cin >> program; st2.setProgram(program);

	cout << "\nThe following will be for student number THREE";
	cout << "\nFirst Name: "; cin >> fName; st3.setfName(fName);
	cout << "Last Name : "; cin >> lName; st3.setlName(lName);
	cout << "Id: "; cin >> id; st3.setId(id);
	cout << "Date of Birth: "; cin >> birth; st3.setDateBirth(birth);
	cout << "GPA (0 - 4.4): "; cin >> gpa; st3.setGpa(gpa);
	cout << "Start Year: "; cin >> startYear; st3.setStartYear(startYear);
	cout << "Completed Credit: "; cin >> comCre; st3.setCompCredit(comCre);
	cout << "Program (B, M, P): "; cin >> program; st3.setProgram(program);

	cout << "\nThe following will be for student number FOUR";
	cout << "\nFirst Name: "; cin >> fName; st4.setfName(fName);
	cout << "Last Name : "; cin >> lName; st4.setlName(lName);
	cout << "Id: "; cin >> id; st4.setId(id);
	cout << "Date of Birth: "; cin >> birth; st4.setDateBirth(birth);
	cout << "GPA (0 - 4.4): "; cin >> gpa; st4.setGpa(gpa);
	cout << "Start Year: "; cin >> startYear; st4.setStartYear(startYear);
	cout << "Completed Credit: "; cin >> comCre; st4.setCompCredit(comCre);
	cout << "Program (B, M, P): "; cin >> program; st4.setProgram(program);

	cout << "\nThe following will be for student number FIVE";
	cout << "\nFirst Name: "; cin >> fName; st5.setfName(fName);
	cout << "Last Name : "; cin >> lName; st5.setlName(lName);
	cout << "Id: "; cin >> id; st5.setId(id);
	cout << "Date of Birth: "; cin >> birth; st5.setDateBirth(birth);
	cout << "GPA (0 - 4.4): "; cin >> gpa; st5.setGpa(gpa);
	cout << "Start Year: "; cin >> startYear; st5.setStartYear(startYear);
	cout << "Completed Credit: "; cin >> comCre; st5.setCompCredit(comCre);
	cout << "Program (B, M, P): "; cin >> program; st5.setProgram(program);
	cout << endl;

	// while loop for set/get and member functions, continues until -1
	// -1: stops this loop, 1 to 8: get functions, 9 to 12: member functions
	while (numFunc != -1) {
		cout << "\n\nNumber input which performs the following member functions: ";
		cout << "\n-1. Terminate this section";
		cout << "\n1. Get First Name";
		cout << "\n2. Get Last Name  ";
		cout << "\n3. Get Id ";
		cout << "\n4. Get Date of Birth ";
		cout << "\n5. Get GPA (0 - 4.4) ";
		cout << "\n6. Get Start Year ";
		cout << "\n7. Get Completed Credit ";
		cout << "\n8. Get Program ";
		cout << "\n9. Complete Program (0:False, 1:True";
		cout << "\n10. Student Status";
		cout << "\n11. Print Student Info";
		cout << "\n12. Compare GPA\n";

		// user select which function to perform with a switch case
		cin >> numFunc;
		switch (numFunc) {
		case 1: // get first name case
			cout << "\nWhich student? (Enter Number): ";
			cout << "\n1. " << st1.getId();
			cout << "\n2. " << st2.getId();
			cout << "\n3. " << st3.getId();
			cout << "\n4. " << st4.getId();
			cout << "\n5. " << st5.getId() << endl;
			// user determine specific student through Id
			cin >> numId;
			switch (numId) {
			case 1:
				cout << st1.getfName();
				break;
			case 2:
				cout << st2.getfName();
				break;
			case 3:
				cout << st3.getfName();
				break;
			case 4:
				cout << st4.getfName();
				break;
			case 5:
				cout << st5.getfName();
				break;
			default:
				cout << "This isn't an option";
			}
			break;
		case 2: // get last name case
			cout << "\nWhich student? (Enter Number): ";
			cout << "\n1. " << st1.getId();
			cout << "\n2. " << st2.getId();
			cout << "\n3. " << st3.getId();
			cout << "\n4. " << st4.getId();
			cout << "\n5. " << st5.getId() << endl;
			// user determine specific student through Id
			cin >> numId;
			switch (numId) {
			case 1:
				cout << st1.getlName();
				break;
			case 2:
				cout << st2.getlName();
				break;
			case 3:
				cout << st3.getlName();
				break;
			case 4:
				cout << st4.getlName();
				break;
			case 5:
				cout << st5.getlName();
				break;
			default:
				cout << "This isn't an option";
			}
			break;
		case 3: // get id case
			cout << "\nWhich student? (Enter Number): ";
			cout << "\n1. " << st1.getId();
			cout << "\n2. " << st2.getId();
			cout << "\n3. " << st3.getId();
			cout << "\n4. " << st4.getId();
			cout << "\n5. " << st5.getId() << endl;
			// user determine specific student through Id
			cin >> numId;
			switch (numId) {
			case 1:
				cout << st1.getId();
				break;
			case 2:
				cout << st2.getId();
				break;
			case 3:
				cout << st3.getId();
				break;
			case 4:
				cout << st4.getId();
				break;
			case 5:
				cout << st5.getId();
				break;
			default:
				cout << "This isn't an option";
			}
			break;
		case 4: // get date of birth id case
			cout << "\nWhich student? (Enter Number): ";
			cout << "\n1. " << st1.getId();
			cout << "\n2. " << st2.getId();
			cout << "\n3. " << st3.getId();
			cout << "\n4. " << st4.getId();
			cout << "\n5. " << st5.getId() << endl;
			// user determine specific student through Id
			cin >> numId;
			switch (numId) {
			case 1:
				cout << st1.getDateBirth();
				break;
			case 2:
				cout << st2.getDateBirth();
				break;
			case 3:
				cout << st3.getDateBirth();
				break;
			case 4:
				cout << st4.getDateBirth();
				break;
			case 5:
				cout << st5.getDateBirth();
				break;
			default:
				cout << "This isn't an option";
			}
			break;
		case 5: // get gpa case
			cout << "\nWhich student? (Enter Number): ";
			cout << "\n1. " << st1.getId();
			cout << "\n2. " << st2.getId();
			cout << "\n3. " << st3.getId();
			cout << "\n4. " << st4.getId();
			cout << "\n5. " << st5.getId() << endl;
			// user determine specific student through Id
			cin >> numId;
			switch (numId) {
			case 1:
				cout << st1.getGpa();
				break;
			case 2:
				cout << st2.getGpa();
				break;
			case 3:
				cout << st3.getGpa();
				break;
			case 4:
				cout << st4.getGpa();
				break;
			case 5:
				cout << st5.getGpa();
				break;
			default:
				cout << "This isn't an option";
			}
			break;
		case 6: // get start year case
			cout << "\nWhich student? (Enter Number): ";
			cout << "\n1. " << st1.getId();
			cout << "\n2. " << st2.getId();
			cout << "\n3. " << st3.getId();
			cout << "\n4. " << st4.getId();
			cout << "\n5. " << st5.getId() << endl;
			// user determine specific student through Id
			cin >> numId;
			switch (numId) {
			case 1:
				cout << st1.getStartYear();
				break;
			case 2:
				cout << st2.getStartYear();
				break;
			case 3:
				cout << st3.getStartYear();
				break;
			case 4:
				cout << st4.getStartYear();
				break;
			case 5:
				cout << st5.getStartYear();
				break;
			default:
				cout << "This isn't an option";
			}
			break;
		case 7: // get completed credit case
			cout << "\nWhich student? (Enter Number): ";
			cout << "\n1. " << st1.getId();
			cout << "\n2. " << st2.getId();
			cout << "\n3. " << st3.getId();
			cout << "\n4. " << st4.getId();
			cout << "\n5. " << st5.getId() << endl;
			// user determine specific student through Id
			cin >> numId;
			switch (numId) {
			case 1:
				cout << st1.getCompCredit();
				break;
			case 2:
				cout << st2.getCompCredit();
				break;
			case 3:
				cout << st3.getCompCredit();
				break;
			case 4:
				cout << st4.getCompCredit();
				break;
			case 5:
				cout << st5.getCompCredit();
				break;
			default:
				cout << "This isn't an option";
			}
			break;
		case 8: // get program case
			cout << "\nWhich student? (Enter Number): ";
			cout << "\n1. " << st1.getId();
			cout << "\n2. " << st2.getId();
			cout << "\n3. " << st3.getId();
			cout << "\n4. " << st4.getId();
			cout << "\n5. " << st5.getId() << endl;
			// user determine specific student through Id
			cin >> numId;
			switch (numId) {
			case 1:
				cout << st1.getProgram();
				break;
			case 2:
				cout << st2.getProgram();
				break;
			case 3:
				cout << st3.getProgram();
				break;
			case 4:
				cout << st4.getProgram();
				break;
			case 5:
				cout << st5.getProgram();
				break;
			default:
				cout << "This isn't an option";
			}
			break;
		case 9: // complete program function case
			cout << "\nWhich student? (Enter Number): ";
			cout << "\n1. " << st1.getId();
			cout << "\n2. " << st2.getId();
			cout << "\n3. " << st3.getId();
			cout << "\n4. " << st4.getId();
			cout << "\n5. " << st5.getId() << endl;
			// user determine specific student through Id
			cin >> numId;
			switch (numId) {
			case 1:
				cout << st1.completeprogram();
				break;
			case 2:
				cout << st2.completeprogram();
				break;
			case 3:
				cout << st3.completeprogram();
				break;
			case 4:
				cout << st4.completeprogram();
				break;
			case 5:
				cout << st5.completeprogram();
				break;
			default:
				cout << "This isn't an option";
			}
			break;
		case 10: // student status function case
			cout << "\nWhich student? (Enter Number): ";
			cout << "\n1. " << st1.getId();
			cout << "\n2. " << st2.getId();
			cout << "\n3. " << st3.getId();
			cout << "\n4. " << st4.getId();
			cout << "\n5. " << st5.getId() << endl;
			// user determine specific student through Id
			cin >> numId;
			switch (numId) {
			case 1:
				cout << st1.studentStatus();
				break;
			case 2:
				cout << st2.studentStatus();
				break;
			case 3:
				cout << st3.studentStatus();
				break;
			case 4:
				cout << st4.studentStatus();
				break;
			case 5:
				cout << st5.studentStatus();
				break;
			default:
				cout << "This isn't an option";
			}
			break;
		case 11: // print student info function case
			cout << "\nWhich student? (Enter Number): ";
			cout << "\n1. " << st1.getId();
			cout << "\n2. " << st2.getId();
			cout << "\n3. " << st3.getId();
			cout << "\n4. " << st4.getId();
			cout << "\n5. " << st5.getId() << endl;		
			// user determine specific student through Id
			cin >> numId;
			switch (numId) {
			case 1:
				st1.printStdInfo();
				break;
			case 2:
				st2.printStdInfo();
				break;
			case 3:
				st3.printStdInfo();
				break;
			case 4:
				st4.printStdInfo();
				break;
			case 5:
				st5.printStdInfo();
				break;
			default:
				cout << "This isn't an option";
			}
			break;
		case 12: // compare gpa function case
			cout << "Choose two student to compare their GPA.";
			cout << "\n1. " << st1.getId();
			cout << "\n2. " << st2.getId();
			cout << "\n3. " << st3.getId();
			cout << "\n4. " << st4.getId();
			cout << "\n5. " << st5.getId() << endl;
			// user determine specific student through Id
			cout << "\nStudent n.1: ";
			cin >> numId1;
			cout << "\nStudent m.2:";
			cin >> numId2;
			switch (numId1) { // select first student
			case 1: 
				numCompareGpa1 = st1.getGpa();
				break;
			case 2:
				numCompareGpa1 = st2.getGpa();
				break;
			case 3:
				numCompareGpa1 = st3.getGpa();
				break;
			case 4:
				numCompareGpa1 = st4.getGpa();
				break;
			case 5:
				numCompareGpa1 = st5.getGpa();
				break;
			default:
				cout << "This isn't an option";
			}
			switch (numId2) { // select second student
			case 1: 
				numCompareGpa2 = st1.getGpa();
				break;
			case 2:
				numCompareGpa2 = st2.getGpa();
				break;
			case 3:
				numCompareGpa2 = st3.getGpa();
				break;
			case 4:
				numCompareGpa2 = st4.getGpa();
				break;
			case 5:
				numCompareGpa2 = st5.getGpa();
				break;
			default:
				cout << "This isn't an option";
			}
			// compare gpa function
			cout << st1.compareGpa(numCompareGpa1, numCompareGpa2);
			break;
		default:
			break;
		}
	}
	//------------------------Student End-----------------------------//
	return 0;
}



/*ofstream outs;
	outs.open("student.txt");

	if (!outs) {
		cout << "erroe";
	}
	else {
		int grade;
		cout << "enter grade, -1 exit: ";
		cin >> grade;

		while (grade != -1) {
			outs << grade << endl;
			cout << "enter grade, -1 exit: ";
			cin >> grade;
		}
		outs.close();
	}*/