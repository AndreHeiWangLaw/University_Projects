// Assignment 1, Question 4, Week3
// Hei Wang Andre Law, 4017 5600
// Program: Final grade of a student in letter form

#include <iostream>
using namespace std;

int main() {
	//defined variable
	int grade;

	//ask student to input the final grade
	cout << "Enter your final grade: ";
	cin >> grade;

	//Display results followup
	cout << "\nYour final grade: ";

	//check the range where grade belongs
	if (90 <= grade) {
		cout << "A+"; }
	else if (85 <= grade) {
		cout << "A"; }
	else if (80 <= grade) {
		cout << "A-"; }
	else if (75 <= grade) {
		cout << "B+"; }
	else if (70 <= grade) {
		cout << "B"; }
	else if (65 <= grade) {
		cout << "C+"; }
	else if (60 <= grade) {
		cout << "C"; }
	else if (55 <= grade) {
		cout << "C-"; }
	else if (50 <= grade) {
		cout << "D+"; }
	else if (45 <= grade) {
		cout << "D"; }
	else if (40 <= grade) {
		cout << "D-"; }
	else if (40 > grade) {
		cout << "F"; }
	cout << endl;
	return 0;
}