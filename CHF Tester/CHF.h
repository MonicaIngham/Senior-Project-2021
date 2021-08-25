#pragma once
#ifndef CHF_H
#define CHF_H

#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

//Function definitions
vector<vector<int>> createMatrix(string input);
vector<vector<int>> swapMatrix(vector<vector<int>> rngMatrix);
void outputMatrix(vector<vector<int>> rngMatrix);
string createDigest(vector<vector<int>> rngMatrix, string input);

#endif
