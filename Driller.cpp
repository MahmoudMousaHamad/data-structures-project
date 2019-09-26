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
#include "Sorter.h"
#include "Comparator.h"
#include "DrillingRecordComparator.h"

ResizableArray<DrillingRecord>* drillingArray;
const std::string INVALID_DATE = "INVALID_DATE";
const std::string DUPLICATE_TIMESTAMP = "DUPLICATE_TIMESTAMP";
const std::string DUPLICATE_TIMESTAMP_DIFFERENT_FILE = "DUPLICATE_TIMESTAMP_DIFFERENT_FILE";
const std::string INVALID_DATA = "INVALID_DATA";
const std::string VALID_RECORD = "VALID_RECORD";
unsigned long file_starting_index_in_array = 0;
unsigned long validrecordscounter;
unsigned long storedrecordscounter;

int main() 
{
	inputloop();
}

void inputloop()
{
	drillingArray = new ResizableArray<DrillingRecord>();
	while (true)
	{
		std::string filename;
		std::string line;
		unsigned long linecounter = 1;
		// prompt user for the name of a data file
		println("Enter data file name: ");
		// get file name from user
		std::getline(std::cin, filename);
		// exit input loop if user inputs nothin
		if (filename.empty()) break;
		std::ifstream datafile(filename);
		if (datafile.is_open())
		{
			file_starting_index_in_array = drillingArray.getSize();
			while (std::getline(datafile, line))
			{
				readline(line, linecounter, datafile);
			}
			datafile.close();
		}
		else 
		{
			println("File is not available.");
			break;
		}
	}
}

void readline(std::string line, unsigned int& counter, const std::ifstream& datafile)
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
	// record validity level
	std::string recordvalidity;
	// current record
	DrillingRecord currentRecord;
	// parse current row
	while (ss.good()) 
	{
		std::string field;
		getline(ss, field, ',');
		if (columnCounter == 0 || columnCounter == 1) 
		{
			// string column
			currentRecord.addString(field);
		} else 
		{
			currentRecord.addNum(std::stod(field));
		}
		columnCounter++;
	}
	recordvalidity = checkrecord(drillingArray, currentRecord);
	if (recordvalidity == VALID_RECORD)
	{
		insertrecord(array, currentRecord);
	} 
	else if (recordvalidity == INVALID_DATE)
	{
		datafile.close();
		println("Date mismatch; file closed.");
	}
	counter++;
}

std::string checkrecord(ResizableArray<DrillingRecord>& drillingArray, DrillingRecord record)
{ 
	// check date using
	if (drillingArray.get(0).getString(0) != record.getString(0))
	{
		return INVALID_DATE;
	}
	// check timestamp
	DrillingRecordComparator comparator(1);	
	Sorter.sort(drillingArray, comparator);
	unsigned long result = binarySearch(record, drillingArray, comparator);
	else if (result >= 0)
	{
		if (result >= file_starting_index_in_array)
		{
			drillingArray.replaceAt(record, (result - 1) * -1);
			return DUPLICATE_TIMESTAMP_DIFFERENT_FILE;
		}
		return DUPLICATE_TIMESTAMP;
	}
	// check data validity
	else if (checkdata(record))
	{
		return INVALID_DATA;
	}
	validrecordscounter++;
	return VALID_RECORD;
}

bool checkdata(DrillingRecord record)
{
	for (unsigned long i = 0; i < 16; i++) 
	{
		if (record.getNum(j) <= 0.0) 
		{
			return false;
		} 
	}
	return true;
}

void insertrecord(ResizableArray<DrillingRecord>& drillingArray, DrillingRecord record)
{
	drillingArray->add(currentRecord);
	storedrecordscounter++;	
}

void println(std::string output)
{
	std::out << output << "\n";
}