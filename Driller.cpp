#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <iomanip>
#include "DrillingRecord.h"
#include "ResizableArray.h"
#include "Search.h"
#include "Comparator.h"
#include "DrillingRecordComparator.h"

const std::string DUPLICATE_DATE = "DUPLICATE_DATE";
const std::string DUPLICATE_TIMESTAMP = "DUPLICATE_TIMESTAMP";
const std::string INVALID_DATA = "INVALID_DATA";
const std::string VALID_RECORD = "VALID_RECORD";


int main() {
	ResizableArray<DrillingRecord>* drillingArray = new ResizableArray<DrillingRecord>();
	ResizableArray<std::string>* timestamps = new ResizableArray<std::string>();
	std::string filename = "";
	std::string line = "";
	unsigned int filecounter;
	unsigned int linecounter;
	// Promote user for the name of a data file
	std::cout << "Enter data file name: \n";
	// Break from loop if no file name is given
	if (filename == "")
	{
		break;
	}
	// Get file name from user
	std::cin >> filename;
	std::ifstream datafile(filename);
	if (datafile.is_open())
	{
		filecounter++;
		linecounter = 1;
		while (std::getline(datafile, line))
		{
			readline(line, linecounter);
		}
		datafile.close();
	}
	else 
	{
		std::cout << "File is not available.\n";
		break;
	}
}

void readline(std::string line, unsigned int& counter)
{
	if (counter == 1)
	{
		counter++;
		return;
	}
	// parse input
	std::stringstream ss(line);
	// column counter
	int columnCounter = 0;
	// current record
	DrillingRecord currentRecord;
	// parse current row
	while (ss.good()) {
		std::string field;
		getline(ss, field, ',');
		if (columnCounter == 0 || columnCounter == 1) {
			// string column
			currentRecord.addString(field);
		} else {
			currentRecord.addNum(std::stod(field));
		}
		columnCounter++;
	}
	if (checkrecord(drillingArray, currentRecord) == VALID_RECORD)
	{
		drillingArray->add(currentRecord);
		counter++;
	}
}

std::string checkrecord(ResizableArray<DrillingRecord>& drillingArray, DrillingRecord record)
{
	// check date using binary search
	
	// check timestamp using binary search

	// check data validity

	return "";
}


	std::string line;
	// line counter
	unsigned int counter = 1;
	// ignore header row
	std::getline(std::cin, line);
	// init drilling array
	ResizableArray<DrillingRecord>* myDrillingArray = new ResizableArray<DrillingRecord>();
	// store data
	while (std::getline(std::cin, line)) {
		// parse input
		std::stringstream ss(line);
		// column counter
		int columnCounter = 0;
		// current record
		DrillingRecord currentRecord;
		// parse current row
		while (ss.good()) {
			std::string field;
			getline(ss, field, ',');
			if (columnCounter == 0 || columnCounter == 1) {
				// string column
				currentRecord.addString(field);
			} else {
				currentRecord.addNum(std::stod(field));
			}
			columnCounter++;
		}
		myDrillingArray->add(currentRecord);
		counter++;
	}
	std::string firstdatestamp;
	std::vector<std::string> timestamps;
	// valid data store
	std::vector<DrillingRecord> validData;
	// process data and print errors
	for (unsigned int i = 1; i < counter; i++) {
		// error flags
		bool duplicateTimestamp = false;
		bool invalidFloatingPoint = false;
		DrillingRecord currentRecord = myDrillingArray->get(i - 1);
		// process datestamp
		std::string datestamp = currentRecord.getString(0);
		if (i == 1) {
			firstdatestamp = datestamp;
		}
		if (datestamp != firstdatestamp && i > 1) {
			std::cout << "Non-matching date stamp " << datestamp << " at line " << std::to_string(i) << "." << "\n";
		} 
		// process time stamp
		else if (i > 1) {
		    if (std::find(timestamps.begin(), timestamps.end(), currentRecord.getString(1)) != timestamps.end()){
		        std::cout << "Duplicate timestamp " << currentRecord.getString(1) << " at line " << std::to_string(i) << "." << "\n";
		        duplicateTimestamp = true;
		    }
		} 
		// process floating-point data
		if (!duplicateTimestamp) {
			for (unsigned int j = 0; j < 16; j++) {
				if (currentRecord.getNum(j) <= 0.0) {
					std::cout << std::fixed << std::setprecision(2) << "Invalid floating-point data at line " << i << "." << "\n";
					invalidFloatingPoint = true;
					break;
				} 
			}
			if (!invalidFloatingPoint) {
				validData.push_back(currentRecord);
			}
		}
		timestamps.push_back(currentRecord.getString(1));
	}
	// print valid data
	for (unsigned int i = 0; i < validData.size(); ++i) {
		std::cout << validData[validData.size() - i - 1] << std::endl;
	}
	return 0;