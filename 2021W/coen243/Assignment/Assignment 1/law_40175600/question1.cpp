// Assignment 1, Question 1, Week 3
// Hei Wang Andre Law, 4017 5600
// Program: Sum, product and average of three integers

#include <iostream>
using namespace std;

int main() {
	//defined variables
	int num1, num2, num3; //user input of three integers
	float sum, prod, avg; //calculation variables

	//ask user three integers
	cout << "Enter the first number: ";
	cin >> num1;

	cout << "Enter the second number: ";
	cin >> num2;

	cout << "Enter the third number: ";
	cin >> num3;

	//calculate the sum, product and average
	sum = num1 + num2 + num3;
	prod = num1 * num2 * num3; 
	avg = sum / 3;

	//display results statement
	cout << "\nThe results are: ";

	//sum of those three integers
	cout << "\nSum: " << sum;

	//product of those three integers
	cout << "\nProduct: " << prod;

	//average of those three integers
	cout << "\nAverage: " << avg << endl;

	return 0;
}