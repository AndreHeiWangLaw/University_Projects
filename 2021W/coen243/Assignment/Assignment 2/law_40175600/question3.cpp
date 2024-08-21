// Assignment 2, Question 3, Week 5
// Hei Wang Andre Law, 4017 5600
// Program: House drawing with a roof, walls and flooring

#include <iostream>
#include <iomanip>
using namespace std;

// main body of program
int main() {
	string name; // user input of their name
	string drawAnswer; // answer if they want to draw or not
	int numHouse = 0; // number of house counter
	int width, height; // width and height of the house
	int halfWidth; // half the widthof the house
	int tries; // number of tries from miss-input of width input

	// 1) ask and store user name with a welcome message
	cout << "What is your name? ";
	cin >> name; 
	cout << "Well " << name << ", welcome to the house drawing program.";

	// do-while loop of entire house drawing
	do {
		// welcome question about wanting to draw a house or not
		cout << "\nDo you want me to draw a simple house for you? (yes/no) ";
		cin >> drawAnswer;
		if (drawAnswer == "no") {
			break; // end program if user doesn't want to draw, keep going otherwise
		}
		
		// 2.1) request house dimensions (height)
		cout << "\nEnter height of the house you want me to draw: ";
		cin >> height; // ask user the height of the house

		// 2.2) request house dimensions (width), for-loop for valid input
		for (tries = 0; tries < 3; tries++) { // allows the user 3 tries before error, end of program
			cout << "Please enter an even number for the width of the house (must be even numbers and bigger than 2): ";
			cin >> width;

			// filter value of width with personalized error message
			if (width > 2 && width % 2 == 0) { // valid input
				break; // stops the loop and continue the program
			}
			else if (width == 0) { // invalid input of zero
				cout << "You enter " << width << " for the width. Nor an even number nor larger than 2!\n\n";
			}
			else if (width <= 2 && width % 2 != 0) { // invalid input below 2 and odd
				cout << "You enter " << width << " for the width. Nor an even number nor larger than 2!\n\n";
			}
			else if (width <= 2) { // invalid input below 2
				cout << "You enter " << width << " for the width. Not larger than 2!\n\n";
			}
			else { // invalid input of odd number
				cout << "You enter " << width << " for the width. Not an even number!\n\n";
			}
		}
		// 2.3) because of too many tries, termination of program and end message
		if (tries == 3) { 
			cout << "It seems you are having troubles entering even numbers larger than 2! Program ends now.\n";
			return 0; 
		}

		// 3) draw the house 
		halfWidth = width / 2; // the roof is half the widthof the house

		// 3.1) for-loop of the roof drawing
		for (int i = 1; i <= halfWidth; i++) { // "i" layer of roofing
			cout << setw(2); // cleanup, space the roof to match the one space distance from the wall
			for (int j = 0; j <= width; j++) { // "j" amount of "*" in one layer
				if (j >= (halfWidth - i) && j < (halfWidth + i)) {
					cout << "*"; // yes roofing
				}
				else {
					cout << " "; // no roofing
				}
			}
			cout << endl; // proceed to the next layer (next iteration loop)
		}
		// 3.2) for-loop of the walls drawing
		for (int w = 0; w < height; w++) {
			cout << "|"; // left side of the wall
			for (int z = 1; z <= width; z++) { // amount of empty space between walls, same as width
				cout << " "; // empty space
			}
			cout << "|"; // right side of the wall
			cout << endl; // proceed to the next layer (next iteration loop)
		}
		// 3.3) for-loop of the floor drawing
		cout << setw(2); // cleanup, space the flooring to match the roof
		for (int r = 1; r <= width; r++) { // amount of flooring, same as width
			cout << "-"; 
		}
		// increase the counter number of house built so far
		numHouse = numHouse + 1; 

	}while (drawAnswer != "no"); // restart loop until user promt "no" for stoping the drawing

	// wrap up message with the amount of house built number displayed
	cout << "\nHope you like your " << numHouse << " house(s)\n";
	return 0;
}