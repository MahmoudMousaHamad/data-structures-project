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
#include "OULink.h"
#include "OULinkedList.h"
#include "OULinkedListEnumerator.h"

// Resizable array that stores the records
ResizableArray<DrillingRecord>* drillingArray;
// LinkedList that stores the records
OULinkedList<DrillingRecord>* drillingLinkedList;
// Constants
const std::string INVALID_DATE = "INVALID_DATE";
const std::string DUPLICATE_TIMESTAMP = "DUPLICATE_TIMESTAMP";
const std::string DUPLICATE_TIMESTAMP_DIFFERENT_FILE = "DUPLICATE_TIMESTAMP_DIFFERENT_FILE";
const std::string INVALID_DATA = "INVALID_DATA";
const std::string VALID_RECORD = "VALID_RECORD";
const std::string MENU = "Enter (o)utput, (s)ort, (f)ind, (m)erge, (p)urge, (r)ecords, or (q)uit: ";
// Counters and flags
unsigned long last_file_ending_index_in_array = 0; // Marks the end of the previous file in the array
unsigned long line_counter = 0;
unsigned long data_lines_read;
unsigned long valid_records;
unsigned long stored_records;
unsigned int sort_column = -1;
// Functions definitions
void user_option_loop();
std::string print_array();
void read_line(std::string line, std::ifstream& datafile, OULinkedList<DrillingRecord>& linkedList);
std::string check_record(DrillingRecord record);
bool check_data(DrillingRecord record);
void insert_record(DrillingRecord record);
void print(std::string output);
void sort_array(int column_num);
DrillingRecord get_record_to_find(int column_num);
OULinkedList<DrillingRecord>* read_file();
void output(std::string option);
void quit();
void sort();
void find();
void merge();
void purge();
std::string print_linked_list();

int main()
{
	user_option_loop();
	return 0;
}

void user_option_loop()
{
	try
	{
		DrillingRecordComparator* comparator = new DrillingRecordComparator(1);
		drillingLinkedList = new OULinkedList<DrillingRecord>(comparator);
	}
	catch(ExceptionMemoryNotAvailable* e)
	{
		print("Not enough memory.\n");
		delete e;
	}
	drillingLinkedList = read_file();
	if (drillingLinkedList->getSize() == 0) return;
	try
	{
		drillingArray = new ResizableArray<DrillingRecord>(*drillingLinkedList);
	}
	catch(ExceptionMemoryNotAvailable* e)
	{
		print("Not enough memory.\n");
		delete e;
	}
	do
	{
		std::string userinput = "";
		print(MENU);
		std::cin >> userinput;
		if (userinput.empty()) continue;
		else if (userinput == "o") output(userinput);
		else if (userinput == "s") sort();
		else if (userinput == "f") find();
		else if (userinput == "m") merge();
		else if (userinput == "p") purge();
		else if (userinput == "r") output(userinput);
		else if (userinput == "q") { quit(); break; }
	} while (true);
}

void merge()
{
	OULinkedList<DrillingRecord>* list_to_merege = read_file();
	OULinkedListEnumerator<DrillingRecord> enumerator_to_merge = list_to_merege->enumerator();
	while (enumerator_to_merge.hasNext())
	{
		drillingLinkedList->insert(enumerator_to_merge.next());
	}
	drillingArray = new ResizableArray<DrillingRecord>(*drillingLinkedList);
	delete list_to_merege;
}

void purge()
{
	OULinkedList<DrillingRecord>* list_to_purge = read_file();
	OULinkedListEnumerator<DrillingRecord> enumerator_to_purge = list_to_purge->enumerator();
	while (enumerator_to_purge.hasNext())
	{
		drillingLinkedList->remove(enumerator_to_purge.next());
	}
	drillingArray = new ResizableArray<DrillingRecord>(*drillingLinkedList);
	delete list_to_purge;
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
 * Outputs the content of the resizable array or linked list to a file or cout
*/
void output(std::string option)
{
	std::string outputfilename;
	std::ofstream outputfile;
	std::string output_string = "";
	if (option == "o")
	{
		output_string = print_array();
	}
	else if (option == "r")
	{
		output_string = print_linked_list();
	}
	print("Enter output file name: ");
	do
	{
		std::cin.ignore();
		if (std::cin.peek() == '\n')
		{
			std::cout << output_string;
			break;			
		}
		std::cin >> outputfilename;
		outputfile.open(outputfilename);
		if (outputfile.is_open())
		{
			outputfile << output_string;
			outputfile.close();
			break;
		}
	} while (true);
}
/**
 * Finds a record and its duplicates if they exist
*/
void find()
{
	unsigned int columnnum;
	long firstindex;
	int recordsfoundcounter = 0;
	print("Enter search field (0-17): ");
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
 * Reads a file, checks its records and then returns a linked list
 * */
OULinkedList<DrillingRecord>* read_file()
{
	OULinkedList<DrillingRecord>* tempLinkedList;
	try
	{
		DrillingRecordComparator* comparator = new DrillingRecordComparator(1);
		tempLinkedList = new OULinkedList<DrillingRecord>(comparator);
	}
	catch(ExceptionMemoryNotAvailable* e)
	{
		print("Not enough memory.\n");
		delete e;
	}
	do
	{
		std::string filename = "";
		std::string line = "";
		line_counter = 0;
		// prompt user for the name of a data file
		print("Enter data file name: ");
		std::cin >> filename;
		if (filename.length() == 0) break;
		std::ifstream datafile(filename);
		if (datafile.good())
		{
			std::getline(datafile, line);
			while (std::getline(datafile, line))
			{
				read_line(line, datafile, *tempLinkedList);
			}
			datafile.close();
			// check if any records were read.
			if (tempLinkedList->getSize() == last_file_ending_index_in_array)
			{
				print("No valid records found.\n");
			}
			// update the file ending index to use later.
			last_file_ending_index_in_array = tempLinkedList->getSize();
			break;
		}
		else 
		{
			print("File is not available.\n");
		}
	}  while (true);
	return tempLinkedList;
}
/**
 * Reads a single line and, if valid, stores it in the LinkedList
 * */
void read_line(std::string line, std::ifstream& datafile, OULinkedList<DrillingRecord>& linkedList)
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
		linkedList.insert(currentRecord);
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
	if (drillingLinkedList->getSize() > 0)
	{
		// check date
		if (drillingLinkedList->getFirst().getString(0) != record.getString(0))
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
	OULinkedListEnumerator<DrillingRecord> enumerator = drillingLinkedList->enumerator();
	unsigned long counter = 0;
	while (enumerator.hasNext())
	{
		if (enumerator.next().getString(1).compare(record.getString(1)) == 0)
		{
			if (counter < last_file_ending_index_in_array)
			{
				drillingLinkedList->replace(record);
				break;
			}
			print("Duplicate timestamp " + record.getString(1) + " at line " + std::to_string(line_counter) + ".\n");
			return DUPLICATE_TIMESTAMP;
		}
		counter++;
	}
	valid_records++;
	return VALID_RECORD;
}
/**
 * Returns true if the record's float-point data is valid
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
 * Halper method that returns a DrillingRecord object that needs to be found in the array
*/
DrillingRecord get_record_to_find(int column_num)
{
	double floatvalue;
	std::string strvalue;
	DrillingRecord record;
	if (column_num >= 2)
	{
		print("Enter number on which to search: ");
		std::cin >> floatvalue;
		record.setNum(floatvalue, column_num - 2);
	}
	else
	{
		print("Enter exact text on which to search: ");
		std::cin >> strvalue;
		record.setString(strvalue, column_num);
	}
	return record;
}
/**
 * Inserts a record into the LinkedList
*/
void insert_record(DrillingRecord record)
{
	drillingLinkedList->insert(record);
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
	oSS << "Data lines read: " << data_lines_read << "; Valid Drilling records read: " << valid_records << "; Drilling records in memory: " << drillingArray->getSize() << "\n";
	return oSS.str();
}
/**
 * Returns a string representation of the records linked list
*/
std::string print_linked_list()
{
	std::ostringstream oSS;
	OULinkedListEnumerator<DrillingRecord> enumerator = drillingLinkedList->enumerator();
	while (enumerator.hasNext())
	{
		DrillingRecord record = enumerator.next();
		oSS << record << "\n";
	}
	oSS << "Data lines read: " << data_lines_read << "; Valid Drilling records read: " << valid_records << "; Drilling records in memory: " << drillingLinkedList->getSize() << "\n";
	return oSS.str();
}
/**
 * prints a string to cout
*/
void print(std::string output)
{
	std::cout << output;
}