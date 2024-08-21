// Assignment 1, Question 3, Week 3
// Hei Wang Andre Law, 4017 5600
// Program: Ascending order of three numbers

#include <iostream>
using namespace std;

int main() {
	int num1, num2, num3; //user inputs variables

	//ask user three integers
	cout << "Enter the first number: ";
	cin >> num1;
	cout << "Enter the second number: ";
	cin >> num2;
	cout << "Enter the third number: ";
	cin >> num3;

	cout << "\nThe ascending order is: ";
	if (num1 > num2 && num1 > num3) {
		if (num2 > num3) {
			cout << num3 << ", " << num2 << ", " << num1; //results where (num1>num2>num3)
		}
		else {
			cout << num2 << ", " << num3 << ", " << num1; //results where (num1>num3>num2)
		}
	}
	else if (num2 > num1 && num2 > num3) {
		if (num1 > num3) {
			cout << num3 << ", " << num1 << ", " << num2; //results wehre (num2>num1>num3)
		}
		else {
			cout << num1 << ", " << num3 << ", " << num2; //results where (num2>num3>num1)
		}
	}
	else if (num3 > num2 && num3 > num1) {
		if (num2 > num1) {
			cout << num1 << ", " << num2 << ", " << num3; //results where (num3>num2>num1)
		}
		else {
			cout << num2 << ", " << num1 << ", " << num3; //results where (num3>num1>num2)
		}
	}
	cout << endl;
	return 0;
}