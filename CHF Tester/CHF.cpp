//Written by Monica Ingham

#include "CHF.h"

/*
This file contains a cryptographic hash function that I designed, based very loosely on RC4.
It has 3 main steps:
1. Create an rng matrix, a 32x32 vector of pseudorandom numbers based on the input
2. Swap every value in the rng matrix with another location
3. Generate a message digest from the rng matrix
*/
string chf(string input, bool debug) {

	//Step 1: create initial rng matrix values
	vector<vector<int>> rngMatrix = createMatrix(input);

	/*/If in debug mode, output the matrix
	if (debug == true) {

		outputMatrix(rngMatrix);

	}*/

	//Step 2: swap values to get rid of any patterns
	rngMatrix = swapMatrix(rngMatrix);

	//If in debug mode, output the matrix
	if (debug == true) {

		outputMatrix(rngMatrix);

	}

	//Step 3
	string digest = createDigest(rngMatrix, input);

	//Return the digest
	return digest;

}



//This function creates the hash function digest from the rng matrix.
//It does this by splitting up the matrix into 16 8x8 submatrices, and
//then getting 2 digits of the digest from each of those with some math.
string createDigest(vector<vector<int>> rngMatrix, string input) {

	string digest = "";	//Stores the finished digest
	int x;				//Holds x coordinates
	int y;				//Holds y coordinates
	int chunkx = 0;		//Holds the starting x position for the 8x8 matrices
	int chunky;			//Holds the starting y position for the 8x8 matrices

	//Get starting matrix position
	for (int a = 0; a < input.length(); a++) {

		chunkx = (chunkx + input.at(a)) % 4;

	}
	chunky = input.length() % 4;

	//Loop through each 8x8 submatrix
	for (int a = 0; a < 4; a++) {

		//Go to next matrix
		chunkx = (chunkx + 1) % 4;

		for (int b = 0; b < 4; b++) {

			//Go to next matrix
			chunky = (chunky + 1) % 4;

			//m stores a value that will create 2 digits of the digest
			int m = 0;

			//Loop through every line of the submatrix
			for (int c = 0; c < 8; c++) {

				//Get x coordinate
				x = (chunkx * 8) + c;

				//n stores every value of a row xor-ed by each other
				int n;

				//Loop through every entry of the row
				for (int d = 0; d < 8; d++) {

					//Get y coordinate
					y = (chunky * 8) + d;

					//If it's the first iteration, initialize n
					if (d == 0) {

						n = rngMatrix.at(x).at(y);

					}
					else {	//Every other time

						//Xor the values
						n = n ^ rngMatrix.at(x).at(y);

					}

				}

				//Add the xor-ed values to m
				m = (m + n) % 256;

			}

			//Convert the finished m value to hexadecimal and add to the digest
			stringstream stream;	//For converting decimal to hexadecimal

			stream << hex << m;
			string str = stream.str();

			//Force the string to be 2 digits long
			if (str.length() == 1) {

				str = "0" + str;

			}

			//Add to the digest
			digest += str;

		}

	}

	//Convert the digest to uppercase
	for (int a = 0; a < digest.length(); a++) {

		digest.at(a) = toupper(digest.at(a));

	}

	//Return the digest
	return digest;

}



//This function swaps values depending on the location and value of the number
//Based on the KSA from RC4
vector<vector<int>> swapMatrix(vector<vector<int>> rngMatrix) {

	int temp;	//Used to store temporary values when swapping numbers
	int x;		//Used to hold the x location of the value to be swapped
	int y;		//Used to hold the y location of the value to be swapped

	//Number of swaps controller
	for (int c = 0; c < 1; c++) {

		//Loop through the matrix
		for (int a = 0; a < 32; a++) {

			for (int b = 0; b < 32; b++) {

				//Store the temporary values
				temp = rngMatrix.at(a).at(b);

				//Get the x location of the swap value
				x = ((a + 1) * temp) % 32;

				//Get the y location of the swap value
				y = ((b + 1) * temp) % 32;

				//Swap the values
				rngMatrix.at(a).at(b) = rngMatrix.at(x).at(y);
				rngMatrix.at(x).at(y) = temp;


			}

		}

	}

	//Return the rng matrix
	return rngMatrix;

}



//This function outputs the rng matrix (but only if in debug mode)
void outputMatrix(vector<vector<int>> rngMatrix) {

	//Loop through the matrix
	for (int a = 0; a < 32; a++) {

		//Loop through the line and output each values
		for (int b = 0; b < 32; b++) {

			cout << rngMatrix.at(a).at(b) << "\t";

		}

		//Go to the next line for the next line
		cout << endl;

	}

	return;

}



//This function create the initial values of the rng matrix, depending on the input
vector<vector<int>> createMatrix(string input) {

	vector<vector<int>> rngMatrix;	//Holds the rng matrix
	vector<int> matrixLine;			//Used for initializing the matrix

	//If the input is empty, append a value so it does something
	if (input.length() == 0) {

		char c = 0;
		input += c;

	}

	//Start out the matrix filled with 1s
	for (int a = 0; a < 32; a++) {

		matrixLine.push_back(1);

	}
	for (int a = 0; a < 32; a++) {

		rngMatrix.push_back(matrixLine);

	}


	//Next, loop through every character
	for (int a = 0; a < input.length(); a++) {

		//Value n starts as the ascii value of a character from the input,
		//and changes over time to create pseudorandom numbers
		unsigned int n = input.at(a);

		//Pick the starting points based on the location of the character in the input
		//This way, we get much different outputs even if we switch 2 characters, creating an avalanche effect
		int x = (a * 5) % 32;
		int y = (a * 11) % 32;

		//For every character, loop through the whole matrix
		for (int b = 0; b < 32; b++) {

			//Move to next x value
			x = (x + 1) % 32;

			for (int c = 0; c < 32; c++) {

				//Move to next y value
				y = (y + 1) % 32;

				//Update n using the seed function:
				//This function is a simple math equation that helps create
				//pseudorandom numbers as it grows over time

				//If it's over 1,000,000,000, take away the 1B so it doesn't get out of control
				while (n > 1000000000) {

					n -= 1000000000;

				}
				//3n + 1
				n = (n * 3) + 1;

				//Do bitwise XOR for n and the value at the location in the matrix
				//(modulo 256 to keep it in bounds for ascii values)
				rngMatrix.at(x).at(y) = (rngMatrix.at(x).at(y) ^ n) % 256;

			}

			//Slightly change value n every row to break up patterns that usually form around lengths of 100 iterations
			n = n / 7;

		}

	}

	//Return the rng matrix
	return rngMatrix;

}