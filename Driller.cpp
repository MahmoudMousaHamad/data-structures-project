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
// Constants
const std::string INVALID_DATE = "INVALID_DATE";
const std::string DUPLICATE_TIMESTAMP = "DUPLICATE_TIMESTAMP";
const std::string DUPLICATE_TIMESTAMP_DIFFERENT_FILE = "DUPLICATE_TIMESTAMP_DIFFERENT_FILE";
const std::string INVALID_DATA = "INVALID_DATA";
const std::string VALID_RECORD = "VALID_RECORD";
// Counters and flags
long file_ending_index_in_array = 0; // Marks the end of the previous file in the array
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
void sort_array(int column_num);
DrillingRecord get_record_to_find(int column_num);
void output();
void quit();
void sort();
void find();

int main()
{
	input_loop();
	data_manipulation_loop();
	return 0;
}
/**
 * Manipulates data upon user's input
*/
void data_manipulation_loop()
{
	if (drillingArray->getSize() == 0) return;
	sort_array(1);
	do
	{
		std::string userinput;
		print("Enter (o)utput, (s)ort, (f)ind, or (q)uit: \n");
		std::cin >> userinput;
		if (userinput.empty()) continue;
		else if (userinput == "o") output();
		else if (userinput == "s") sort();
		else if (userinput == "f") find();
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
	sort_array(columnnum);
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
	long firstindex;
	int recordsfoundcounter = 0;
	std::cin >> columnnum;
	DrillingRecordComparator comparator(columnnum);
	DrillingRecord record = get_record_to_find(columnnum);
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
	try
	{
		drillingArray = new ResizableArray<DrillingRecord>();
	}
	catch(ExceptionMemoryNotAvailable* e)
	{
		print("Not enough memory.\n");
		delete e;
	}
	
	
	if (drillingArray == nullptr)
	{
		
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
		std::ifstream datafile(filename);
		if (datafile.good())
		{
			std::getline(datafile, line);
			while (std::getline(datafile, line))
			{
				read_line(line, datafile);
			}
			datafile.close();
			file_ending_index_in_array = drillingArray->getSize() - 1;
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
		if (result <= file_ending_index_in_array && file_ending_index_in_array != 0)
		{
			valid_records++;
			drillingArray->replaceAt(record, result);
			return DUPLICATE_TIMESTAMP_DIFFERENT_FILE;
		}
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
 * Sorts array according to column number
*/
void sort_array(int column_num)
{
	DrillingRecordComparator comparator(column_num);	
	Sorter<DrillingRecord>::sort(*drillingArray, comparator);
	sort_column = column_num;
}
/**
 * Returns a DrillingRecord object that needs to be found in the array
*/
DrillingRecord get_record_to_find(int column_num)
{
	double floatvalue;
	std::string strvalue;
	DrillingRecord record;
	if (column_num >= 2)
	{
		std::cin >> floatvalue;
		record.setNum(floatvalue, column_num - 2);
	}
	else
	{
		std::cin >> strvalue;
		record.setString(strvalue, column_num);
	}
	return record;
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