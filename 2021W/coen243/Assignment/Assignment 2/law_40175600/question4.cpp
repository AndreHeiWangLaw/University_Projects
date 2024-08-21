// Assignment 2, Question 4, Week 5
// Hei Wang Andre Law, 4017 5600
// Program: Call function based on entered character

#include <iostream>
#include <iomanip>
using namespace std;

// function prototype for the three functions
int function1(int, int); // funtion prototype for 'a'
void function2(int, int, int &); // function prototype for 'b'
double function3(double, double); // function prototype for 'c'

// main body of the program
int main() {
	// 1) State the necessary variables
	int lbound, ubound; // lower and upper bound
	string letter; // input that call functions, string because 'char' cannot diferentiate between: 'a', 'abc', 'abbb', etc.
	int result; // variable by reference of function 2

	// 2) Beginning statement and promt user for inputs
	cout << "Please enter two positive integer numbers (Lower bound/Upper bound): ";
	cin >> lbound >> ubound; // lower and upper bound variable

	// 3) Choose which function to call by inputing a specific character
	cout << "\nPlease enter a character (a, b or c): ";
	cin >> letter; // char that calls its function
	
	// 4) filter that calls different functions depending on the letter input 
	if (letter == "a") { // first function, 'a'
		function1(lbound, ubound); 
	}
	else if (letter == "b") { // second function, 'b'
		function2(lbound, ubound, result);
		cout << "\nThe difference between two numbers is " << result; 
	}
	else if (letter == "c") { // third function, 'c'
		cout << "\nThe value of sum is: " << function3(lbound, ubound);
	}
	else { // All invalid input ends program with terminations message
		cout << "\nInvalid input";
	}
	cout << endl;
	return 0;
}

// function 1: Range of number being multiples fof 2 and 7 in bound
int function1(int l, int u) {
	cout << "\nList of numbers in this interval which are multiple of both 2 and 7: ";
	while (l <= u) { // while-loop to test all values between bound 
		if (l % 2 == 0 && l % 7 == 0) { // multiples of 2 and 7 ONLY when remains of factor are zero
			cout << l << " "; // print out result of this iteration loop
		}
		l++;
	}
	return l; // return back to function2 the results
}

// function 2: Difference between upper and lower bound
void function2(int l, int u, int &res) { 
	res = u - l; // substraction betwwen upper bound to lower bound
}

// function 3: Sum calculation with type double
double function3(double l, double u) {
	cout << fixed << setprecision(3); // set 3 decimal places
	double sum = 0;
	for (double temp= l; temp<= u; temp++) { // for-loop from lower to upper bound
		sum += 1 / temp; // equation of the sum
	}
	return sum; // return back to function3 the results
}