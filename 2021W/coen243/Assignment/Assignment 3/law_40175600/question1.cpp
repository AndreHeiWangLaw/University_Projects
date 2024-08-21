// Assignment 3, Question 1, Week 7
// Hei Wang Andre Law, 4017 5600
// Program: Function Implementations of Grades Stored Inside an Array

#include<iostream>
#include <iomanip>
#include <array>
using namespace std;

const int arraySize = 100; // large array size, assuming a single class doesn't surpass 100 students 
int hold; // temp variable for exchanging positions

// function prototype of each tasks
float maxGrade(array<int, arraySize>maxG, int count); // maximum grade function
float minGrade(array<int, arraySize>minG, int count); // minimum grade function
float avgGrade(array<int, arraySize>avgG, int count); // average grade function
float medGrade(array<int, arraySize>medG, int count); // median grade function
int gradeA(array<int, arraySize> gradeA, int count); // grades A function
int gradeB(array<int, arraySize> gradeB, int count); // grades B function
int gradeC(array<int, arraySize> gradeC, int count); // grades C function
int gradeD(array<int, arraySize> gradeD, int count); // grades D function
int gradeF(array<int, arraySize> gradeF, int count); // grades Ffunction

int main() {
	int numGrade; // grades if a student
	int zeroCounter = 0; // number of empty zeros in array (unused array cells)

	// create an empty array that will contain the grades
	array <int, arraySize> grade{};

	// ask user the number of students
	size_t userSize;
	cout << "Number of grades to enter: ";
	cin >> userSize;

	cout << fixed << setprecision(2); // set cedimal place to 2

	// loop replacing empty array of zeros with inputed grades
	for (size_t i{ 0 }; i < userSize; i++) {
		cout << "Enter grade: ";
		cin >> numGrade;
		grade[i] = numGrade;
	}

	// loop counting amount of zeros in the array 'grade'
	for (size_t x{ 0 }; x < grade.size(); x++) {
		if (grade[x] == 0) {
			zeroCounter++;
		}
	}

	// loop to rearrange the elements from smallest to largest
	for (size_t i{ 0 }; i < grade.size() - (1 + zeroCounter); ++i) {
		// loop that tests the amount of comparisons per inputed grades
		for (size_t j{ 0 }; j < grade.size() - (1 + zeroCounter); ++j) {
			// compare and swap only if first element < second element
			if (grade[j] < grade[j + 1]) {
				hold = grade[j]; // temp holder for first element
				grade[j] = grade[j + 1]; // assign first element as second 
				grade[j + 1] = hold; // assign second element as first
			}
		}
	}
	// function call of each tasks
	cout << "\nMaximum Grade: " << maxGrade(grade, zeroCounter); // maximum grade function
	cout << "\nMinimum Grade: " << minGrade(grade, zeroCounter); // minimum grade function
	cout << "\nAverage Grade: " << avgGrade(grade, zeroCounter); // average grade function
	cout << "\nMedian Grade: " << medGrade(grade, zeroCounter); // median grade function
	cout << "\nNumber of A Grade: " << gradeA(grade, zeroCounter); // grades A function
	cout << "\nNumber of B Grade: " << gradeB(grade, zeroCounter); // grades B function
	cout << "\nNumber of C Grade: " << gradeC(grade, zeroCounter); // grades C function
	cout << "\nNumber of D Grade: " << gradeD(grade, zeroCounter); // grades D function
	cout << "\nNumber of F Grade: " << gradeF(grade, zeroCounter); // grades F function
	cout << endl;
	return 0;
}

// 1. Maximum grade function
float maxGrade(array<int, arraySize> maxG, int count) {
	return maxG[0]; // return the biggest integer of the ordered array
}

// 2. Minimum grade function
float minGrade(array<int, arraySize> minG, int count) {
	return minG[(minG.size()-(1+count))]; // return the smallest integer
}

// 3. Average grade function
float avgGrade(array<int, arraySize> avgG, int count) {
	float sum = 0; // set the initial sum of all grade as zero
	// for loop that sums all inputed grades
	for (size_t i{ 0 }; i < avgG.size() - count; i++) {
		sum += avgG[i];
	}
	// the average is calculated by dividing the total sum by the amount of inputs
	return sum / (avgG.size() - count); // return the average
}

// 4. Median grade function
float medGrade(array<int, arraySize> medG, int count) {
	float median = 0.0; // set initial value of median to zero

	// check if array size is odd or even
	if ((medG.size() - count) % 2 == 0) {
		// compute median equation for even number
		median = ((medG[(medG.size()-count)/2]) + (medG[(medG.size() - (1 + count)) / 2]))/2.0;
	}
	else {
		// compute mdian equation for odd number
		median = medG[(medG.size() - (1 + count)) / 2];
	}
	return median; // return the value of the median
}

// 5. Number of A grades
int gradeA(array<int, arraySize> gradeA, int count) {
	int gradeCounter = 0; // set initial counter to zero
	// loop every element of the array
	for (size_t i{ 0 }; i < (gradeA.size() - count); i++) {
		// check if it is within 800 to 100
		if ((gradeA[i] > 80) && (gradeA[i] <= 100)) {
			gradeCounter++; // add 1 to the counter
		}
	}
	return gradeCounter;
}

// 6. Number of B grades
int gradeB(array<int, arraySize> gradeB, int count) {
	int gradeCounter = 0; // set initial counter to zero
	// loop every element of the array
	for (size_t i{ 0 }; i < (gradeB.size() - count); i++) {
		// check if it is within 70 to 80
		if ((gradeB[i] > 70) && (gradeB[i] <= 80)) {
			gradeCounter++; // add 1 to the counter
		}
	}
	return gradeCounter;
}

// 7. Number of C grades
int gradeC(array<int, arraySize> gradeC, int count) {
	int gradeCounter = 0; // set initial counter to zero
	// loop every element of the array
	for (size_t i{ 0 }; i < (gradeC.size() - count); i++) {
		// check if it is within 55 to 70
		if ((gradeC[i] > 55) && (gradeC[i] <= 70)) {
			gradeCounter++; // add 1 to the counter
		}
	}
	return gradeCounter;
}

// 8. Number of D grades
int gradeD(array<int, arraySize> gradeD, int count) {
	int gradeCounter = 0; // set initial counter to zero
	// loop every element of the array
	for (size_t i{ 0 }; i < (gradeD.size() - count); i++) {
		// check if it is within 40 and 55
		if ((gradeD[i] > 40) && (gradeD[i] <= 55)) {
			gradeCounter++; // add 1 to the counter
		}
	}
	return gradeCounter;
}

// 9. Number of F grades
int gradeF(array<int, arraySize> gradeF, int count) {
	int gradeCounter = 0; // set initial counter to zero
	// loop every element of the array
	for (size_t i{ 0 }; i < (gradeF.size() - count); i++) {
		// check if it is within 0 and 40
		if ((gradeF[i] > 0) && (gradeF[i] <= 40)) {
			gradeCounter++; // add 1 to the counter
		}
	}
	return gradeCounter;
}