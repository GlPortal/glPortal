#include "MapToWavefront.hpp"
#include <iostream>
#include <fstream>
#include <cstring>

namespace glPortal {
  namespace map{
      const char* MapToWavefront::DATA_DIRECTORY = "data";
      MapToWavefront::MapToWavefront(const string & filename){
	char * inputFilename  = new char [filename.length()+1];
	char * resultFilename = new char [filename.length()+5];
	strcpy (inputFilename, filename.c_str());

	ifstream inputFileStream(inputFilename);
	ofstream outputFileStream;
	
	strcpy(resultFilename, inputFilename);
	strcat(resultFilename, ".obj");

      	outputFileStream.open(resultFilename, ios::trunc);
	string line;

	while(getline(inputFileStream, line)) {
	  cout << line << endl;
	  outputFileStream << line << endl;
	}
 
	inputFileStream.close();
	outputFileStream.close();
      }
    
  }
}
