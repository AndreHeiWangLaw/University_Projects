// Assignment 4, Question 1, Week 10
// Hei Wang Andre Law, 4017 5600
// Program: Multiplication of Row Element of Two Dimentional Array

#include<iostream>
#include<array>
using namespace std;

// declare array size and function prototype for the multiplication 
const size_t cols{ 6 };
const size_t rows{ 5 };
void mult(array<array<int, cols>, rows>);

int main() {
	array<array<int, cols>, rows> arrayMult; // declare the array
	int num; // declare the integer input

	cout << "Fill in the two-dimentional array A[5][6].\n\n";

	// for-loop filling in the array elements from user inputs
	for (size_t i{ 0 }; i < rows; i++) { // loop 5 times
		for (size_t j{ 0 }; j < cols; j++) { // loop 6 times
			cin >> num; // ask user a number
			arrayMult[i][j] = num; // store that number in corresponding element
		}
	}
	mult(arrayMult); // 'mult' function call
	return 0;
}

// 'mult' function printing the multiplication of each row
void mult(array<array<int, cols>, rows> arrayMult){
	// for-loop multiplying elements of each row
	for (size_t i{ 0 }; i < rows; i++) { // loop 5 times
		int result = 1; // set initial result to 1 
		for (size_t j{ 0 }; j < cols; j++) { // loop 6 times
			// multiply 'result' by array element in [i][j] position
			result = result * arrayMult[i][j]; 
		}
		// print final result for each row
		cout << "\nThe multiplication of row " << i<< " is "<< result;
	}
	cout << endl;
}