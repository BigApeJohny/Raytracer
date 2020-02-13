#pragma once

#include <fstream>
#include <iostream>

using namespace std;

class InputReader
{
public: 

	string _fileName;

	InputReader(
		const string &fileName
	) :
		_fileName(fileName) {}

	void read() {
		ifstream inputFile;
		inputFile.open(_fileName);
		if (!inputFile) {
			cerr << "Unable to open file datafile.txt";
			exit(1);
		}
		char line[255];

		while (inputFile) {
			inputFile.getline(line, 255);  // delim defaults to '\n'
			if (inputFile) {
				cout << line << endl;
			}
		}
		inputFile.close();
	}


};

