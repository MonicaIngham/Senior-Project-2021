//Written by Monica Ingham

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <chrono>
#include "CHF.h"
using namespace std;

//Function definitions
string chf(string input, bool debug);
int multiTest(int testNum);
string randInput(int length);
vector<vector<int>> Analyzer();
float avalancheTest(int testLength);



/*
This program tests a cryptographic hash function (CHF.cpp).
*/
int main()
{
    
	//Initialize variables
	string input;	//Holds input
	int testMode;	//Holds the testing mode

	//First, get test mode from the user
	cout << "Welcome to the cryptographic hash function tester!\n" << 
			"Testing modes:\n\n" <<
			"1. Single test\n" <<
			"2. Multi test\n" <<
			"3. Multi test analyzer\n" <<
			"4. Avalanche effect test\n" <<
			"\nPlease enter a number: ";
	cin >> input;
	testMode = stoi(input);



	//Next, decide what to do based on the testing mode
	if (testMode == 1) {	//Single test

		//Get an input from the user
		cout << "\nSingle test:\n" <<
				"Please enter a string: ";
		cin >> input;

		//Start timer
		auto start = chrono::high_resolution_clock::now();

		//Then run the chf with that string
		string output = chf(input, false);

		//Stop timer
		auto finish = chrono::high_resolution_clock::now();
		auto time = chrono::duration_cast<chrono::microseconds>(finish - start);

		//Output the digest
		cout << "Output: " << output << endl;

		//Ouput run time
		cout << "Time elapsed: " << time.count() << " microseconds\n";

	}
	else if (testMode == 2) {	//Multi test

		//Get the number of tests to run through
		cout << "\nMulti test:\n" <<
				"Please enter the number of tests to run: ";
		cin >> input;

		//Convert to int
		int testNum = stoi(input);

		//Do the tests
		int average = multiTest(testNum);

		//Tell the user it's finished
		cout << "The results have been saved to the file Output.txt\n" <<
				"Average time elapsed: " << average << " microseconds\n";

	}
	else if (testMode == 3) {	//Analyzer

		//Analyze the data
		vector<vector<int>> data = Analyzer();

		//Output the results
		cout << "\nMulti test analyzer:\n";
		cout << "Characters\t0\t1\t2\t3\t4\t5\t6\t7\t8\t9\tA\tB\tC\tD\tE\tF\n";

		//Loop through every line
		for (int a = 0; a < 32; a++) {

			cout << "Char " << a + 1 << ": ";

			//Loop through every entry
			for (int b = 0; b < 16; b++) {

				cout << "\t" << data.at(a).at(b) << "%";

			}

			cout << endl;

		}

	}
	else if (testMode == 4) {

		//Get the length of the test
		cout << "\nAvalanche effect analyzer:\n" <<
				"Please enter the length of the test: ";
		cin >> input;

		//Convert to int
		int testLength = stoi(input);

		//Do the test
		float result = avalancheTest(testLength);

		//Output the results
		cout << "Avalanche effectiveness: " << result << "%\n";

	}



	//Exit program
	system("Pause");
	return 0;

}



//This function conducts a test for the avalanche effect, and returns the
//total percent chance that a character will change if a single part of the
//input is changed
float avalancheTest(int testLength) {

	float result = 0;	//Holds the results of the test

	//Get a starting random input with length 10
	string input = randInput(10);

	//Loop through all the tests
	for (int a = 0; a < testLength; a++) {

		//Get the first output
		string outputOne = chf(input, false);

		//Generate a random number to either swap 2 characters, delete a character, or add a character
		int choice = rand() % 3;

		//Keep the input length from 5-15 characters long
		if (choice == 1 && input.length() == 15) {

			choice = 2;

		}
		else if (choice == 2 && input.length() == 5) {

			choice = 1;

		}

		if (choice == 0) {	//Swap 2 characters

			//Get 2 character locations
			int char1 = rand() % input.length();
			int char2 = rand() % (input.length() - 1);

			//Make sure they're different
			if (char1 == char2) {

				char2++;

			}

			//Swap the characters
			char c = input.at(char1);
			input.at(char1) = input.at(char2);
			input.at(char2) = c;

		}
		else if (choice == 1) {	//Add a character

			//Get a character location
			int loc = rand() % input.length();

			//Get a random character
			char c = rand() % 95 + 32;

			//Insert the character
			input.insert(input.begin() + loc, c);

		}
		else if (choice == 2) {	//Delete a character
			
			//Get a character location
			int loc = rand() % input.length();

			//Delete the character
			input.erase(input.begin() + loc);

		}

		//Get the second output
		string outputTwo = chf(input, false);

		int mismatchCount = 0;	//Counts the number of times a character changed

		//Compare the 2 outputs
		for (int b = 0; b < 32; b++) {

			//If it's a mismatch
			if (outputOne.at(b) != outputTwo.at(b)) {

				//Count
				mismatchCount++;

			}

		}

		//Convert the count to a percentage and add it to the result
		result = ((result * a) + (((float)mismatchCount * 100) / 32)) / (a + 1);

	}

	//Return the results
	return result;

}



//This function analyzes a file of CHF outputs and returns an analysis
//Of how likely any character is likely to show up
vector<vector<int>> Analyzer() {

	vector<vector<int>> data;	//Holds all the data about the analysis
	vector<int> line;			//Used to initialize the data vector

	//Initialize the data vector
	for (int a = 0; a < 16; a++) {	//16 possible characters

		line.push_back(0);

	}
	for (int a = 0; a < 32; a++) {	//32 characters per output

		data.push_back(line);

	}

	//Open the file
	fstream file;
	file.open("Output.txt", ios::in);

	//If the file is open
	if (file.is_open()) {

		//Read each line of the file
		string input;
		int count = 0;	//Counts the number of lines
		while (getline(file, input)) {

			count++;	//Increment the count

			//Loop through each character of the line
			for (int a = 0; a < input.length(); a++) {

				//Add a count depending on what the character is
				if (input.at(a) == '0') {
					data.at(a).at(0)++;
				}
				else if (input.at(a) == '1') {
					data.at(a).at(1)++;
				}
				else if (input.at(a) == '2') {
					data.at(a).at(2)++;
				}
				else if (input.at(a) == '3') {
					data.at(a).at(3)++;
				}
				else if (input.at(a) == '4') {
					data.at(a).at(4)++;
				}
				else if (input.at(a) == '5') {
					data.at(a).at(5)++;
				}
				else if (input.at(a) == '6') {
					data.at(a).at(6)++;
				}
				else if (input.at(a) == '7') {
					data.at(a).at(7)++;
				}
				else if (input.at(a) == '8') {
					data.at(a).at(8)++;
				}
				else if (input.at(a) == '9') {
					data.at(a).at(9)++;
				}
				else if (input.at(a) == 'A') {
					data.at(a).at(10)++;
				}
				else if (input.at(a) == 'B') {
					data.at(a).at(11)++;
				}
				else if (input.at(a) == 'C') {
					data.at(a).at(12)++;
				}
				else if (input.at(a) == 'D') {
					data.at(a).at(13)++;
				}
				else if (input.at(a) == 'E') {
					data.at(a).at(14)++;
				}
				else if (input.at(a) == 'F') {
					data.at(a).at(15)++;
				}

			}

		}

		//Change each number in data to a percentage
		for (int a = 0; a < 32; a++) {

			for (int b = 0; b < 16; b++) {

				data.at(a).at(b) = (data.at(a).at(b) * 100) / count;

			}

		}

		//Close the file
		file.close();

	}

	//Return the data file
	return data;

}



//This function runs the CHF a certain number of times with random inputs, then saves the outputs to a file
int multiTest(int testNum) {

	int average = 0;	//Holds the average runtime for the algorithm
	int randLength;		//Holds the length for the random input
	string input;		//Holds a random input to be used for the CHF
	string output;		//Holds an output from the CHF

	//Open the file to save to
	fstream file;
	file.open("Output.txt", ios::out);

	//Loop through every test
	for (int a = 0; a < testNum; a++) {

		//Get a random input length from 5 to 20
		randLength = rand() % 16 + 5;

		//Get a random input
		input = randInput(randLength);

		//Start the timer
		auto start = chrono::high_resolution_clock::now();

		//Run the algorithm
		output = chf(input, false);

		//Stop the timer
		auto finish = chrono::high_resolution_clock::now();
		auto time = chrono::duration_cast<chrono::microseconds>(finish - start);

		//Save the output to the file
		file << output << endl;
		
		//Calculate the average
		average = ((average * a) + time.count()) / (a + 1);

	}

	//Close the file and return the average runtime
	file.close();
	return average;

}



//This function creates a random input to use for the CHF
string randInput(int length) {

	string input = "";	//Holds the input that's being created
	char c;				//Holds a random character

	//Loop through every character
	for (int a = 0; a < length; a++) {

		//Get a random character
		c = rand() % 95 + 32;	//Any ascii value from 32 to 126

		//Add the character to the string
		input += c;

	}

	//Return the input
	return input;

}