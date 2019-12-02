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
#include "DrillingRecordHasher.h"
#include "HashTable.h"
#include "HashTableEnumerator.h"
#include "AVLTree.h"
#include "AVLTreeEnumerator.h"
#include "AVLTreeOrder.h"

// Resizable array that stores the records
ResizableArray<DrillingRecord>* drillingArray;
// AVL Tree that stores the records
AVLTree<DrillingRecord>* drillingAVLTree;
// HashTable that stores the records
HashTable<DrillingRecord>* drillingHashTable;
// Drilling Hasher
DrillingRecordHasher* hasher;
// Constants
const std::string INVALID_DATE = "INVALID_DATE";
const std::string DUPLICATE_TIMESTAMP = "DUPLICATE_TIMESTAMP";
const std::string DUPLICATE_TIMESTAMP_DIFFERENT_FILE = "DUPLICATE_TIMESTAMP_DIFFERENT_FILE";
const std::string INVALID_DATA = "INVALID_DATA";
const std::string VALID_RECORD = "VALID_RECORD";
const std::string MENU = "Enter (o)utput, (s)ort, (f)ind, (m)erge, (p)urge, (r)ecords, (h)ash table, or (q)uit: ";
// Counters and flags
unsigned long last_file_ending_index_in_array = 0; // Marks the end of the previous file in the array
unsigned long line_counter = 0;
unsigned long data_lines_read;
unsigned long valid_records;
unsigned long stored_records;
unsigned int sort_column = -1;
DrillingRecordComparator* timestamp_comparator;
// Functions definitions
void user_option_loop();
std::string array_to_string();
void read_line(std::string line, std::ifstream& datafile, AVLTree<DrillingRecord>& avlTree);
std::string check_record(DrillingRecord record, AVLTree<DrillingRecord>& avlTree);
bool check_data(DrillingRecord record);
void print(std::string output);
void sort_array(int column_num);
DrillingRecord get_record_to_find(int column_num);
AVLTree<DrillingRecord>* read_file();
void output(std::string option);
void quit();
void sort();
void find();
void merge();
void purge();
std::string avlTree_to_string();
std::string hashtable_to_string();
std::string counters_to_string();

int main()
{
	// Create master drilling AVLTree in heap
	timestamp_comparator = new DrillingRecordComparator(1);
    if (timestamp_comparator == nullptr) throw new ExceptionMemoryNotAvailable();
	drillingAVLTree = new AVLTree<DrillingRecord>(timestamp_comparator);
    if (drillingAVLTree == nullptr) throw new ExceptionMemoryNotAvailable();
	// Read first file
	drillingAVLTree = read_file();
	// Make sure that we actually read something
	if (drillingAVLTree->getSize() == 0) return 0;
	// Create drilling resizable array
	drillingArray = new ResizableArray<DrillingRecord>(*drillingAVLTree);
    if (drillingArray == nullptr) throw new ExceptionMemoryNotAvailable();
	// Create drilling HashTable and hasher in heap
	hasher = new DrillingRecordHasher();
	if (hasher == nullptr) throw new ExceptionMemoryNotAvailable();
	drillingHashTable = new HashTable<DrillingRecord>(timestamp_comparator, hasher, drillingAVLTree->getSize());
	if (drillingHashTable == nullptr) throw new ExceptionMemoryNotAvailable();
	AVLTreeEnumerator<DrillingRecord> enumerator(drillingAVLTree);
	while (enumerator.hasNext())
	{
		drillingHashTable->insert(enumerator.next());
	}
	// Show menu
	user_option_loop();
	return 0;
}

/**
 * Contains user menu
 * */
void user_option_loop()
{
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
		else if (userinput == "h") output(userinput);
		else if (userinput == "q") { quit(); break; }
	} while (true);
}
/**
 * Reads file to merge.
 * */
void merge()
{
	AVLTree<DrillingRecord>* list_to_merege = read_file();
	AVLTreeEnumerator<DrillingRecord> enumerator_to_merge = list_to_merege->enumerator();
	while (enumerator_to_merge.hasNext())
	{
		try
		{
			drillingAVLTree->insert(enumerator_to_merge.next());
		}
		catch(const ExceptionMemoryNotAvailable& e)
		{
			print("No memory available");
		}
	}
	drillingArray = new ResizableArray<DrillingRecord>(*drillingAVLTree);
    if (drillingArray == nullptr) throw new ExceptionMemoryNotAvailable();
	drillingHashTable = new HashTable<DrillingRecord>(timestamp_comparator, hasher, drillingAVLTree->getSize());
	if (drillingHashTable == nullptr) throw new ExceptionMemoryNotAvailable();
	AVLTreeEnumerator<DrillingRecord> enumerator = drillingAVLTree->enumerator();
	while (enumerator.hasNext())
	{
		drillingHashTable->insert(enumerator.next());
	}
	if (drillingHashTable == nullptr) throw new ExceptionMemoryNotAvailable();
	delete list_to_merege;
	list_to_merege = nullptr;
}
/**
 * Reads file to purge.
 * */
void purge()
{
	AVLTree<DrillingRecord>* list_to_purge = read_file();
	AVLTreeEnumerator<DrillingRecord> enumerator_to_purge = list_to_purge->enumerator();
	while (enumerator_to_purge.hasNext())
	{
		drillingAVLTree->remove(enumerator_to_purge.next());
	}
	drillingArray = new ResizableArray<DrillingRecord>(*drillingAVLTree);
    if (drillingArray == nullptr) throw new ExceptionMemoryNotAvailable();
	drillingHashTable = new HashTable<DrillingRecord>(timestamp_comparator, hasher, drillingAVLTree->getSize());
	if (drillingHashTable == nullptr) throw new ExceptionMemoryNotAvailable();
	AVLTreeEnumerator<DrillingRecord> enumerator = drillingAVLTree->enumerator();
	while (enumerator.hasNext())
	{
		drillingHashTable->insert(enumerator.next());
	}
	if (drillingHashTable == nullptr) throw new ExceptionMemoryNotAvailable();
	delete list_to_purge;
	list_to_purge = nullptr;
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
 * Outputs the content of the resizable array or AVL Tree to a file or cout
*/
void output(std::string option)
{
	std::string outputfilename;
	std::ofstream outputfile;
	std::string output_string = "";
	if (option == "o")
	{
		output_string = array_to_string();
	}
	else if (option == "r")
	{
		output_string = avlTree_to_string();
	}
	else if (option == "h")
	{
		output_string = hashtable_to_string();
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
	DrillingRecord record = get_record_to_find(columnnum);
	// if search based on timestamp, use hashtable
	if (columnnum == 1)
	{
		try
		{
			DrillingRecord item = drillingHashTable->find(record);
			std::cout << item;
			print("Drilling records found: 1.\n");
			return;			
		}
		catch(const ExceptionHashTableAccess e)
		{
			print("Drilling records found: 0.\n");
		}
	}
	DrillingRecordComparator comparator(columnnum);
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
 * Reads a file, checks its records and then returns an AVL Tree
 * */
AVLTree<DrillingRecord>* read_file()
{
	DrillingRecordComparator* comparator = new DrillingRecordComparator(1);
    if (comparator == nullptr) throw new ExceptionMemoryNotAvailable();
	AVLTree<DrillingRecord>* tempAVLTree = new AVLTree<DrillingRecord>(comparator);
    if (tempAVLTree == nullptr) throw new ExceptionMemoryNotAvailable();
	do
	{
		std::string filename = "";
		std::string line = "";
		line_counter = 0;
		// prompt user for the name of a data file
		print("Enter data file name: ");
		// do not execute cin.ignore() if first file
		if (last_file_ending_index_in_array != 0)
		{
			std::cin.ignore();
		}
		if (std::cin.peek() == '\n')
		{
			break;			
		}
		std::cin >> filename;
		if (filename.length() == 0) break;
		std::ifstream datafile(filename);
		if (datafile.good())
		{
			std::getline(datafile, line);
			while (std::getline(datafile, line))
			{
				read_line(line, datafile, *tempAVLTree);
			}
			datafile.close();
			// check if any records were read.
			if (tempAVLTree->getSize() == last_file_ending_index_in_array)
			{
				print("No valid records found.\n");
			}
			// update the last_file_ending_index_in_array for later use.
			last_file_ending_index_in_array = tempAVLTree->getSize();
			break;
		}
		else 
		{
			print("File is not available.\n");
		}
	}  while (true);
	return tempAVLTree;
}
/**
 * Reads a single line and, if valid, stores it in the AVLTree
 * */
void read_line(std::string line, std::ifstream& datafile, AVLTree<DrillingRecord>& avlTree)
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
	recordvalidity = check_record(currentRecord, avlTree);
	if (recordvalidity == VALID_RECORD)
	{
		try
		{
			avlTree.insert(currentRecord);			
		}
		catch(const ExceptionMemoryNotAvailable& e)
		{
			print("No memory available");
		}
	}
	else if (recordvalidity == INVALID_DATE)
	{
		datafile.close();
		print("Date mismatch; file closed.\n");
	}
}
/**
 * Checks the validity of a DrillingRecord object within an AVL Tree
*/
std::string check_record(DrillingRecord record, AVLTree<DrillingRecord>& avlTree)
{ 
	if (avlTree.getSize() > 0)
	{
		// check date
		if (avlTree.getFirst().getString(0) != record.getString(0))
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
	AVLTreeEnumerator<DrillingRecord> enumerator = avlTree.enumerator();
	unsigned long counter = 0;
	while (enumerator.hasNext())
	{
		if (enumerator.next().getString(1).compare(record.getString(1)) == 0)
		{
			if (counter < last_file_ending_index_in_array)
			{
				avlTree.replace(record);
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
		print("Enter positive field value: ");
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
 * Returns a string representation of the records array
*/
std::string array_to_string()
{
	std::ostringstream oSS;
	for (unsigned long i = 0; i < drillingArray->getSize(); i++)
	{
		DrillingRecord record = drillingArray->get(i);
		oSS << record << "\n";
	}
	oSS << counters_to_string();
	return oSS.str();
}
/**
 * Returns a string representation of the records AVL Tree
*/
std::string avlTree_to_string(AVLTreeOrder order)
{
	std::ostringstream oSS;
	AVLTreeEnumerator<DrillingRecord> enumerator(drillingAVLTree, order);
	while (enumerator.hasNext())
	{
		DrillingRecord record = enumerator.next();
		oSS << record << "\n";
	}
	oSS << counters_to_string();
	return oSS.str();
}
/**
 * Returns a string representation of the records hash table
*/
std::string hashtable_to_string()
{
	std::ostringstream oSS;
	HashTableEnumerator<DrillingRecord>* enumerator = new HashTableEnumerator<DrillingRecord>(drillingHashTable);
	// variables to discerne whether a certain record is in the overflow or not.
    int current_bucket = 0;
	int previous_bucket = -1;
	while (enumerator->hasNext())
	{
		DrillingRecord record = enumerator->next();
		if (current_bucket == previous_bucket)
		{
			oSS << "OVERFLOW: ";
		}
		else
		{
			oSS << current_bucket << ": ";
		}
		oSS << record << "\n\n";
		previous_bucket = current_bucket;
		current_bucket = ((int) hasher->hash(record)) % ((int) drillingHashTable->getBaseCapacity());
	}
	oSS << "Base Capacity: " << drillingHashTable->getBaseCapacity() << "; Total Capacity: " << drillingHashTable->getTotalCapacity() << "; Load Factor: " << drillingHashTable->getLoadFactor() << "\n";
	oSS << counters_to_string();
	return oSS.str();
}

/**
 * Returns a string representation of the data lines read, valid recrods read, and records in memory.
*/
std::string counters_to_string()
{
	std::ostringstream oSS;
	oSS << "Data lines read: " << data_lines_read << "; Valid Drilling records read: " << valid_records << "; Drilling records in memory: " << drillingAVLTree->getSize() << "\n";
	return oSS.str();
}
/**
 * prints a string to cout
*/
void print(std::string output)
{
	std::cout << output;
}
