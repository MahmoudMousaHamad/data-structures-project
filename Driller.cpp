#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "DrillingRecord.h"
#include "ResizableArray.h"
#include "Search.h"
#include "Sorter.h"
#include "Comparator.h"
#include "DrillingRecordComparator.h"

// Resizable array that stores the records
ResizableArray<DrillingRecord>* drillingArray;
// Resizable array used to keep track of input file names to ensure no duplicates
ResizableArray<std::string>* file_names_array;
// Constants
const std::string INVALID_DATE = "INVALID_DATE";
const std::string DUPLICATE_TIMESTAMP = "DUPLICATE_TIMESTAMP";
const std::string DUPLICATE_TIMESTAMP_DIFFERENT_FILE = "DUPLICATE_TIMESTAMP_DIFFERENT_FILE";
const std::string INVALID_DATA = "INVALID_DATA";
const std::string VALID_RECORD = "VALID_RECORD";
const std::string MENUE = "Enter (o)utput, (s)ort, (f)ind, (m)erge, (p)urge, (r)ecords, or (q)uit: \n";
// Counters and flags
long file_starting_index_in_array = 0;
unsigned long line_counter = 0;
unsigned long data_lines_read;
unsigned long valid_records;
unsigned long stored_records;
unsigned int sort_column = -1;
// Functions definitions
void input_loop();
void data_manipulation_loop();
std::string print_array();
void read_line(std::string line, std::ifstream& datafile);
std::string check_record(DrillingRecord record);
bool check_data(DrillingRecord record);
void insert_record(DrillingRecord record);
void print(std::string output);
void output();
void quit();
void sort();
void find();
bool duplicate_file_name(std::string filename);

int main()
{
	input_loop();
	if (drillingArray->getSize() > 0)
	{
		DrillingRecordComparator comparator(1);	
		Sorter<DrillingRecord>::sort(*drillingArray, comparator);
		sort_column = 1;
		data_manipulation_loop();
	}
	return 0;
}
/**
 * Manipulates data upon user's input
*/
void data_manipulation_loop()
{
	std::string userinput;
	do
	{
		print(MENU);
		std::cin >> userinput;
		if (userinput.empty())
		{
			continue;
		}
		else if (userinput == "o")
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
		else if (userinput == "m")
		{
			merge();
		}
		else if (userinput == "p")
		{
			purge();
		}
		else if (userinput == "r")
		{
			records();
		}
		else if (userinput == "q")
		{
			quit();
			break;
		}
	} while (true);
}
/**
 * Sorts the resizable array
*/
void sort()
{
	unsigned int columnnum;
	std::cin >> columnnum; 
	DrillingRecordComparator comparator(columnnum);	
	Sorter<DrillingRecord>::sort(*drillingArray, comparator);
	sort_column = columnnum;
}
/**
 * Quits the program
*/
void quit()
{
	print("Thanks for using Driller.\n");
}
/**
 * Outputs the content of the resizable array to a file or cout
*/
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
			std::cout << print_array();
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
			outputfile << print_array();
			outputfile.close();
			break;
		}
	} while (!outputsuccessful);
}
/**
 * Finds a record and its duplicates if they exist
*/
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
	if (sort_column == columnnum)
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
		else if (sort_column == columnnum)
		{
			break;
		}
	}
	print("Drilling records found: " + std::to_string(recordsfoundcounter) + ".\n");
}
/**
 * Reads the input data files and stores valid records
*/
void input_loop()
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
		line_counter = 0;
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
				read_line(line, datafile);
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
/**
 * Reads a single line and, if valid, stores it in the array
 * */
void read_line(std::string line, std::ifstream& datafile)
{
	line_counter++;
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
	data_lines_read++;
	recordvalidity = check_record(currentRecord);
	if (recordvalidity == VALID_RECORD)
	{
		insert_record(currentRecord);
	} 
	else if (recordvalidity == INVALID_DATE)
	{
		datafile.close();
		print("Date mismatch; file closed.\n");
	}
}
/**
 * Returns whether the current file has already been read
*/
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
 * Checks the validity of a DrillingRecord object
*/
std::string check_record(DrillingRecord record)
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
	if (!check_data(record))
	{
		print("Invalid floating-point data at line " + std::to_string(line_counter) + ".\n");
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
		print("Duplicate timestamp " + record.getString(1) + " at line " + std::to_string(line_counter) + ".\n");
		return DUPLICATE_TIMESTAMP;
	}
	valid_records++;
	return VALID_RECORD;
}
/**
 * Returns true if the record float-point data is valid
*/
bool check_data(DrillingRecord record)
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
/**
 * Inserts a record into the array
*/
void insert_record(DrillingRecord record)
{
	drillingArray->add(record);
	stored_records++;	
}
/**
 * Returns a string representation of the records array
*/
std::string print_array()
{
	std::ostringstream oSS;
	for (unsigned long i = 0; i < drillingArray->getSize(); i++)
	{
		DrillingRecord record = drillingArray->get(i);
		oSS << record << "\n";
	}
	oSS << "Data lines read: " << data_lines_read << "; Valid drilling records read: " << valid_records << "; Drilling records in memory: " << stored_records << "\n";
	return oSS.str();
}
/**
 * prints a string to cout
*/
void print(std::string output)
{
	std::cout << output;
}