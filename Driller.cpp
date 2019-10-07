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
ResizableArray<std::string>* file_names_array;
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
unsigned int lastsortcolumn = -1;

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
void sort();
void find();
bool duplicate_file_name(std::string filename);

int main()
{
	inputloop();
	if (drillingArray->getSize() > 0)
	{
		DrillingRecordComparator comparator(1);	
		Sorter<DrillingRecord>::sort(*drillingArray, comparator);
		lastsortcolumn = 1;
		datamanipulationloop();
	}
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
			sort();
		}
		else if (userinput == "f")
		{
			find();
		}
		else if (userinput == "q")
		{
			quit();
			break;
		}
	} while (true);
}

void sort()
{
	unsigned int columnnum;
	std::cin >> columnnum; 
	DrillingRecordComparator comparator(columnnum);	
	Sorter<DrillingRecord>::sort(*drillingArray, comparator);
	lastsortcolumn = columnnum;
}

void quit()
{
	print("Thanks for using Driller.\n");
}

void output()
{
	std::string outputfilename;
	std::ofstream outputfile;
	bool outputsuccessful = false;
	print("Enter output file name: ");
	do
	{
		std::cin.ignore();
		if (std::cin.peek() == '\n')
		{
			std::cout << printarray();
			break;
		}
		std::getline(std::cin, outputfilename);
		outputfile.open(outputfilename);
		if (!outputfile.is_open())
		{
			continue;
		}
		else
		{
			outputsuccessful = true;
			outputfile << printarray();
			outputfile.close();
			break;
		}
	} while (!outputsuccessful);
}

void find()
{
	unsigned int columnnum;
	double floatvalue;
	std::string strvalue;
	long firstindex;
	int recordsfoundcounter = 0;
	std::cin >> columnnum;
	DrillingRecordComparator comparator(columnnum);
	DrillingRecord record;
	if (columnnum >= 2)
	{
		std::cin >> floatvalue;
		record.setNum(floatvalue, columnnum - 2);
	}
	else
	{
		std::cin >> strvalue;
		record.setString(strvalue, columnnum);
	}
	if (lastsortcolumn == columnnum)
	{
		firstindex = binarySearch(record, *drillingArray, comparator);		
	}
	else
	{
		firstindex = linearSearch(record, *drillingArray, comparator);
	}
	if (firstindex < 0)
	{
		print("Drilling records found: 0.\n");
		return;
	}
	for (unsigned long i = firstindex; i < drillingArray->getSize(); ++i)
	{
		DrillingRecord recordtocompare = drillingArray->get(i);
		if (comparator.compare(record, recordtocompare) == 0)
		{
			std::cout << recordtocompare << "\n";
			recordsfoundcounter++;
		}
		else
		{
			if (lastsortcolumn == columnnum)
			{
				break;
			}
		}
	}
	print("Drilling records found: " + std::to_string(recordsfoundcounter) + ".\n");
}

void inputloop()
{
	drillingArray = new ResizableArray<DrillingRecord>();
	if (drillingArray == nullptr)
	{
		throw new ExceptionMemoryNotAvailable();
	}
	file_names_array = new ResizableArray<std::string>(); 
	if (file_names_array == nullptr)
	{
		throw new ExceptionMemoryNotAvailable();
	}
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
		if (duplicate_file_name(filename))
		{
			print("Duplicate data file.\n");
			break;
		}
		std::ifstream datafile(filename);
		if (datafile.good())
		{
			file_starting_index_in_array = drillingArray->getSize();
			std::getline(datafile, line);
			while (std::getline(datafile, line))
			{
				readline(line, datafile);
			}
			file_names_array->add(filename);
			datafile.close();
		}
		else 
		{
			print("File is not available.\n");
		}
	}
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
	} 
	else if (recordvalidity == INVALID_DATE)
	{
		datafile.close();
		print("Date mismatch; file closed.\n");
	}
}

std::string checkrecord(DrillingRecord record)
{ 
	if (drillingArray->getSize() > 0)
	{
		// check date
		if (drillingArray->get(0).getString(0) != record.getString(0))
		{
			return INVALID_DATE;
		}
	}
	// check data validity
	if (!checkdata(record))
	{
		print("Invalid floating-point data at line " + std::to_string(linecounter) + ".\n");
		return INVALID_DATA;
	}
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
	validrecords++;
	return VALID_RECORD;
}

bool duplicate_file_name(std::string filename)
{
	if (file_names_array->getSize() == 0)
	{
		return false;
	}
	for (unsigned long i = 0; i < file_names_array->getSize() - 1; ++i)
	{
		if (filename.compare(file_names_array->get(i)) == 0)
		{
			return true;
		}
	}
	return false;
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


void print(std::string output)
{
	std::cout << output;
}