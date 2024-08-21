// Assignment 3, Question 3, Week 7
// Hei Wang Andre Law, 4017 5600
// Program: Robot Movement Control

#include <iostream>
#include <array>

using namespace std;
const size_t arraySize{ 20 }; // set the array size to have 20 elements

// function prototype
int checkR(int, int); // check if robot goes out of bound (right side)
int checkL(int, int); // check if robot goes out of bound (left side)
void right(int, int&, array<string, arraySize>&); // right function
void left(int, int&, array<string, arraySize>&); // left function
void reboot(int&, array<string, arraySize>&); // reboot function
void current(array<string, arraySize>); // current position function
void grid(array<string, arraySize>pos); // show grid of the array
void cancel(int, bool, int&, array<string, arraySize>&); // cancel last valid operation function
void replay(int, bool, int&, array<string, arraySize>&); // replay last cancellation function

int main() {
	// declare the array with the robot's position and the grid numbers
	array<string, arraySize> position{ "^", " ", " ", " ", " ", " ", " ", " ", " ", " ",
									   "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
	int curPos = 0; // set initial position to element 'zero'
	int command = 0; // sentinel-controlled iteration default value
	bool rightLeft = true; // true means last operation was 'right' function and vice versa
	int shiftBy = 0; // stores amount moved to left or right, used in cancel/replay
	bool cancelCheck = true; // check if user already cancelled, true means it did cancel 
	bool rebootCheck = false; // check if program rebooted, false mean did not reboot

	// display the avalable commands
	cout << "Controller Menu:\n" << "1. Right\n" << "2. Left\n" << "3. Display\n" << "4. Reboot\n";
	cout << "5. Show Array\n" << "6. Cancel\n" << "7. Replay\n" << "8. Exit\n\n";

	// sentinel-controlled iteration with '8' ending the program
	while (command != 8) {
		int input; // number of cells, amount of left/right shift 

		// requests which command to perform
		cout << "Perform command number: "; 
		cin >> command; 

		// switch-case to run specific command
		switch (command) { 
		case 1: // 'right' function case
			cout << "Shift to the right by: "; 
			cin >> input; // robot moves to the right by 'input' amount
			// check if movement is valid
			if (checkR(input, curPos)) {
				right(input, curPos, position); // 'right' function call
				rightLeft = true; // toggle stating last operation was 'right'
				shiftBy = input; // store amount moved
				cancelCheck = false; // reset 'cance' check operation
				rebootCheck = false; // reset 'reboot' check operation

			}
			else // not a valid movement
				cout << "Robot is out of bound, please retry...\n";
			break;
		case 2: // 'left' function case
			cout << "Shift to the left by: ";
			cin >> input; // robot moves to the left by 'input' amount
			// check if movement is valid
			if (checkL(input, curPos)) {
				left(input, curPos, position); // 'left' function call
				rightLeft = false; // toggle stating last operation was 'left'
				shiftBy = input; // stores amount moved
				cancelCheck = false; // reset 'cancel' check operation
				rebootCheck = false; // reset 'reboot' check operation
			}
			else // not a valid movement
				cout << "Robot is out of bound, please retry...\n";
			break;
		case 3: // 'display' function case
			current(position); // 'display' function call
			cout << endl;
			break; 
		case 4: // 'reboot' function case
			reboot(curPos, position); // 'reboot' functio call
			rebootCheck = true; // toggle stating that a reboot was performed
			cout << "Robot return to the start.\n";
			break; 
		case 5: // 'show array' function case
			grid(position); // 'show array' function call
			cout << endl;
			break; 
		case 6:
			if (rebootCheck) { // check if last operation was 'reboot'
				cout << "Program rebooted, nothing to cancel...\n";
			}
			else if (cancelCheck) { // check if a valid operation wa performed last
				cout << "No operation to cancel...\n";
			}
			else { // proceed to cancel when all conditions above is false
				cancel(shiftBy, rightLeft, curPos, position); // 'cancel' function call
				cout << "Last operation cancelled successfully\n";
				cancelCheck = true; // toggle stating that a cancel operation was performed
			}
			break;
		case 7:
			if (rebootCheck) { // check if last operation was 'reboot'
				cout << "Program rebooted, nothing to replay...\n";
			}
			else if (cancelCheck && (shiftBy != 0)) { // check if a cancellation happend and the robot did move
				replay(shiftBy, rightLeft, curPos, position); // 'replay' function call
				cout << "Replayed last cancellation operation successfully\n";
				cancelCheck = false; // toggle stating that last operation is not 'cancel'
			}
			else { // if all above false, it means nothing to replay back to
				cout << "No cancelled operation to replay...\n";
			}
			break;
		default: break; // default case
		}
		cout << endl;
	}
	return 0;
}

// function checking if the robot is out of bound (right side)
int checkR(int i, int curPos) {
	// if final position over 10, out of bound and return false
	if ((curPos + i) >= arraySize/2) {
		return false; 
	}
	// if final position within range, return true
	else {
		return true;
	}
}

// function checking if the robot is out of bound (left side)
int checkL(int i, int curPos) {
	// if final position below 0, out of bound and return false
	if ((curPos - i) < 0) {
		return false;
	}
	// if final position within range, return true
	else {
		return true;
	}
}

// 'right' function with input, ref-current-position and ref-array
void right(int i, int &curPos, array<string, arraySize>&pos) {
	pos[curPos] = " "; // set initial position empty
	pos[curPos + i] = "^"; // set robot into the final position
	curPos = curPos + i; // change current position to the new final position
}

// 'left' function with input, ref-current-position and ref-array
void left(int i, int &curPos, array<string, arraySize>& pos) {
	pos[curPos] = " "; // set initial position empty
	pos[curPos - i] = "^"; // set robot into the final position
	curPos = curPos - i; // change current position to the new final position
}

// 'display' function, prints current position of the robot
void current(array<string, arraySize>pos) {
	// for-loop printing the current robot position
	for (size_t i{ 0 }; i < arraySize/2; i++) {
		cout << pos[i] << " ";
	}
}

// 'reboot' function, resets the robot to the initial position
void reboot(int &curPos, array<string, arraySize>& pos) {
	pos[curPos] = " "; // set initial position empty
	pos[0] = "^"; // set robot into the first position
	curPos = 0; // change current position to the new final position
}

// 'show array' function, prints the numbered grids
void grid(array<string, arraySize>pos) {
	// for loop printing the numbers of the array
	for (size_t i{ 10 }; i < arraySize; i++) {
		cout << pos[i] << " ";
	}
}

// 'cancel' function, return to last state
void cancel(int shift, bool rightLeft, int&curPos, array<string, arraySize>& pos) {
	if (rightLeft) { // if last operation was 'right', move back to 'left'
		left(shift, curPos, pos); // 'left' function call
	}
	else { // if last operation was 'left', move forward to 'right'
		right(shift, curPos, pos); // 'right' function call
	}
}

// 'replay' function, replay before last cancelled operation
void replay(int shift, bool rightLeft, int& curPos, array<string, arraySize>& pos) {
	if (rightLeft) { // if last operation was 'left', move forward to 'right'
		right(shift, curPos, pos); // 'right' function call
	}
	else { // if last operation was 'right', move forward to 'left'
		left(shift, curPos, pos); // 'left' function call
	}
}