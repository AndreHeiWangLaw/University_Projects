// Assignment 1, Question 5, Week 3
// Hei Wang Andre Law, 4017 5600
// Program: Canada's income tax calculator

#include <iostream>
using namespace std;

int main() {
	//defined variable
	double income, tax, net; //shown values
	double bracket1, bracket2, bracket3, bracket4; //calculations variables for each tax bracket

	//ask user the income
	cout << "Enter your income: ";
	cin >> income;

	//logic -> calculate the tax of each bracket in advance
	bracket1 = 0.15 * 48535; //income below 48535$
	bracket2 = 0.205 * 48534; //income between 48535$ - 97069$
	bracket3 = 0.26 * 53404; //income between 97069$ - 150473$
	bracket4 = 0.29 * 63895; //income between 150473$ - 214368$

	//filter income to know which bracket it belongs to
	if (income > 214368) {
		tax = bracket1 + bracket2 + bracket3 + bracket4 + 0.33 * (income - 214368);
	}
	else if (income > 150473) {
		tax = bracket1 + bracket2 + bracket3 + 0.29 * (income - 150473);
	}
	else if (income > 97069) {
		tax = bracket1 + bracket2 + 0.26 * (income - 97069);
	}
	else if (income > 48535) {
		tax = bracket1 + 0.205 * (income - 48535);
	}
	else if (income <= 48535) {
		tax = 0.15 * income;
	}
	net = income - tax;
		cout << "\nTax income: " << tax << "$";
	cout << "\nNet income: " << net << "$\n";
	return 0;
}