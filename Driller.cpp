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
long file_starting_index_in_array = 0;
unsigned long linecounter = 0;
unsigned long datalinesread;
unsigned long validrecords;
unsigned long storedrecords;

void inputloop();
void datamanipulationloop();
std::string printarray();
void readline(std::string line, const std::ifstream& datafile);
std::string checkrecord(DrillingRecord record);
bool checkdata(DrillingRecord record);
void insertrecord(DrillingRecord record);
void print(std::string output);
void output();
void quit();

int main()
{
	inputloop();
	datamanipulationloop();
	return 0;
}

void datamanipulationloop()
{
	std::string userinput;
	do
	{
		print("Enter (o)utput, (s)ort, (f)ind, or (q)uit: \n");
		std::cin >> userinput;
		if (userinput.empty())
		{
			continue;
		}
		else
		if (userinput == "o")
		{
			output();
		}
		else if (userinput == "s")
		{
			// sort();
		}
		else if (userinput == "f")
		{
			// find();
		}
		else if (userinput == "q")
		{
			quit();
			break;
		}
	} while (true);
}

void quit()
{
	print("Thanks for using Driller.\n");
}

void output()
{
	std::string outputfilename;
	std::ofstream outputdatafile;
	bool sendoutputtocout = false;
	bool fileaccessable = false;
	do
	{
		print("Enter output file name: ");
		if (std::cin.get() == '\n')
		{
			sendoutputtocout = true;
			std::cout << printarray();
			break;
		}
		else
		{
			std::cin >> outputfilename;
			outputdatafile.open(outputfilename);
			if (!outputdatafile.is_open())
			{
				print("File is not available.");
				continue;
			}
			else
			{
				fileaccessable = true;
			}
		}
	} while (!fileaccessable || sendoutputtocout);
	if (fileaccessable)
	{
		outputdatafile << printarray();
		outputdatafile.close();
	}
}

std::string printarray()
{
	std::ostringstream oSS;
	for (unsigned long i = 0; i < drillingArray->getSize(); i++)
	{
		DrillingRecord record = drillingArray->get(i);
		oSS << record << "\n";
	}
	oSS << "Data lines read: " << datalinesread << "; Valid drilling records read: " << validrecords << "; Drilling records in memory: " << storedrecords << "\n";
	return oSS.str();
}


void readline(std::string line, std::ifstream& datafile)
{
	linecounter++;
	// parse input
	std::stringstream ss(line);
	// column counter
	int columnCounter = 0;
	// record validity flag
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
		} 
		else 
		{
			currentRecord.addNum(std::stod(field));
		}
		columnCounter++;
	}
	datalinesread++;
	recordvalidity = checkrecord(currentRecord);
	if (recordvalidity == VALID_RECORD)
	{
		insertrecord(currentRecord);
		DrillingRecordComparator comparator(1);	
		Sorter<DrillingRecord>::sort(*drillingArray, comparator);
	} 
	else if (recordvalidity == INVALID_DATE)
	{
		datafile.close();
		print("Date mismatch; file closed.\n");
	}
}

void inputloop()
{
	drillingArray = new ResizableArray<DrillingRecord>();
	while (true)
	{
		std::string filename;
		std::string line;
		linecounter = 0;
		// prompt user for the name of a data file
		print("Enter data file name: ");
		// get file name from user
		std::getline(std::cin, filename);
		// exit input loop if user inputs nothin
		if (filename.empty()) break;
		std::ifstream datafile(filename);
		if (datafile.is_open())
		{
			file_starting_index_in_array = drillingArray->getSize();
			std::getline(datafile, line);
			while (std::getline(datafile, line))
			{
				readline(line, datafile);
			}
			datafile.close();
		}
		else 
		{
			print("File is not available.");
			break;
		}
	}
}

std::string checkrecord(DrillingRecord record)
{ 
	if (drillingArray->getSize() > 0)
	{
		// check date using
		if (drillingArray->get(0).getString(0) != record.getString(0))
		{
			return INVALID_DATE;
		}
		else 
		{
			// check timestamp
			DrillingRecordComparator comparator(1);
			long result = linearSearch(record, *drillingArray, comparator);
			if (result >= 0)
			{
				// if (result >= file_starting_index_in_array)
				// {
				// 	drillingArray->replaceAt(record, result);
				// 	return DUPLICATE_TIMESTAMP_DIFFERENT_FILE;
				// }
				print("Duplicate timestamp " + record.getString(1) + " at line " + std::to_string(linecounter) + ".\n");
				return DUPLICATE_TIMESTAMP;
			} 
		}
	}
	// check data validity
	if (!checkdata(record))
	{
		print("Invalid floating-point data at line " + std::to_string(linecounter) + ".\n");
		return INVALID_DATA;
	}
	validrecords++;
	return VALID_RECORD;
}

/**
 * @param record
 * @returns true if the record float-point data is valid.
*/
bool checkdata(DrillingRecord record)
{
	for (unsigned int i = 0; i < 16; i++) 
	{
		if (record.getNum(i) <= 0) 
		{
			return false;
		}
	}
	return true;
}

void insertrecord(DrillingRecord record)
{
	drillingArray->add(record);
	storedrecords++;	
}

void print(std::string output)
{
	std::cout << output;
}