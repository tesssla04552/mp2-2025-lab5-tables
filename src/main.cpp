#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <chrono>
#include <sstream>
#include "unsorted_table.h"
#include "sorted_table.h"
#include "avl_tree_table.h"
#include "hash_table.h"
#include "logger.h"
Logger logger;
void clearInputBuffer() {
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
void printSeparator(char ch = '=', int length = 60) {
	std::cout << std::string(length, ch) << std::endl;
}
void printHeader(const std::string& title) {
	printSeparator();
	std::cout << "   " << title << std::endl;
	printSeparator();
}
void printOperationsStats(const std::string& operation, UnsortedTable& uTable, SortedTable& sTable, AVLTreeTable& avlTable,
	HashTable& hTable) {
	printSeparator('-', 60);
	std::cout << "STATISTICS AFTER OPERATION: " << operation << std::endl;
	printSeparator('-', 60);
	std::cout << std::left << std::setw(26) << "Type of table" \
		<< std::setw(15) << "Comparisions" << std::setw(12) << "Shifts" \
		<< std::setw(12) << "Rotations" << std::setw(12) << "ChainProbes" << "Operations" << std::endl;
	printSeparator('-', 60);
	std::cout << std::left << std::setw(26) << uTable.GetName() \
		<< std::setw(15) << uTable.GetComparisonCount() << std::setw(12) << "----" \
		<< std::setw(12) << "----" << std::setw(12) << "----" << uTable.GetSize() << std::endl;
	std::cout << std::left << std::setw(26) << sTable.GetName() \
		<< std::setw(15) << sTable.GetComparisonCount() << std::setw(12) << sTable.GetShiftCount() \
		<< std::setw(12) << "----" << std::setw(12) << "----" << sTable.GetSize() << std::endl;
	std::cout << std::left << std::setw(26) << avlTable.GetName() \
		<< std::setw(15) << avlTable.GetComparisonCount() << std::setw(12) << "----" \
		<< std::setw(12) << avlTable.GetRotationCount() << std::setw(12) << avlTable.GetChainProbeCount() \
		<< avlTable.GetSize() << std::endl;
	std::cout << std::left << std::setw(26) << hTable.GetName() \
		<< std::setw(15) << hTable.GetComparisonCount() << std::setw(12) << "----" \
		<< std::setw(12) << "----" << std::setw(12) << "----" << hTable.GetSize() << std::endl;
	printSeparator('-', 60);
}
void resetAllStats(UnsortedTable& uTable, SortedTable& sTable, AVLTreeTable& avlTable, HashTable& hTable) {
	uTable.ResetStats();
	sTable.ResetStats();
	avlTable.ResetStats();
	hTable.ResetStats();
	logger.ResetAllStats();
	std::cout << "Statistics of all tables have been reset!" << std::endl;
}
void showMenu() {
	printHeader("Laboratory work 5. Tables of polynomials");
	std::cout << "1. Add a polynomial to the table" << std::endl;
	std::cout << "2. Find the polynomial by the key" << std::endl;
	std::cout << "3. Delete a polynomial by key" << std::endl;
	std::cout << "4. Show the contents of all tables" << std::endl;
	std::cout << "5. Show detailed statistics" << std::endl;
	std::cout << "6. Reset statistics" << std::endl;
	std::cout << "7. Perform arithmetic operations with polynomials" << std::endl;
	std::cout << "0. Exit" << std::endl;
	printSeparator();
	std::cout << "Choose the action: ";
}
void arithmeticOperations() {
	printHeader("ARITHMETICAL OPERATIONS WITH POLYNOMIALS");
	Polynomials p1, p2, result;
	std::cout << "Enter first polynomial: " << std::endl;
	std::cin >> p1;
	std::cout << "Enter second polynomial: " << std::endl;
	std::cin >> p2;
	printSeparator('-', 30);
	result = p1 + p2;
	std::cout << "Amount: |" << p1 << "| + |" << p2 << "| = |" << result << std::endl;
	result = p1 - p2;
	std::cout << "Subtraction: |" << p1 << "| - |" << p2 << "| = |" << result << std::endl;
	double constant;
	std::cout << "Enter constant by multiplication: ";
	std::cin >> constant;
	result = p1 * constant;
	std::cout << "Multiplication by " << constant << ": |" << p1 << "| * " << constant << " = |" << result << std::endl;
	try {
		result = p1 * p2;
		std::cout << "Multiplication polynomials: |" << p1 << "| * |" << p2 << "| = |" << result << std::endl;
	}
	catch (const std::exception& e) {
		std::cout << "Multiplication error!" << e.what() << std::endl;
	}
}
void addPolynomial(UnsortedTable& uTable, SortedTable& sTable, AVLTreeTable& avlTable, HashTable& hTable) {
	printHeader("ADD POLYNOMIAL");
	std::string key;
	Polynomials poly;
	std::cout << "Enter name(key) polynomial: ";
	std::cin >> key;
	std::cin.ignore();
	std::cout << "Enter polynomial (examples: 3x^2y + 5z^3):" << std::endl;
	std::cin >> poly;
	printSeparator('-', 40);
	resetAllStats(uTable, sTable, avlTable, hTable);
	uTable.Insert(key, poly);
	sTable.Insert(key, poly);
	avlTable.Insert(key, poly);
	hTable.Insert(key, poly);
	printOperationsStats("Addendum polynomial '" + key + "'", uTable, sTable, avlTable, hTable);
	std::cout << "Polynomial was successfully added!" << std::endl;
}
void searchPolynomial(UnsortedTable& uTable, SortedTable& sTable, AVLTreeTable& avlTable, HashTable& hTable) {
	printHeader("SEARCHING POLYNOMIAL");
	std::string key;
	std::cout << "Enter key for searching: ";
	std::cin >> key;
	printSeparator('-', 40);
	resetAllStats(uTable, sTable, avlTable, hTable);
	Polynomials* result = uTable.Search(key);
	std::cout << "Unsorted table: ";
	if (result) { std::cout << "Found -> " << *result << std::endl; }
	else { std::cout << "Not found" << std::endl; }
	result = sTable.Search(key);
	std::cout << "Sorted table: ";
	if (result) { std::cout << "Found -> " << *result << std::endl; } 
	else { std::cout << "Not found" << std::endl; }
	result = avlTable.Search(key);
	std::cout << "AVLTreeTable with chains: ";
	if (result) { std::cout << "Found -> " << *result << std::endl; } 
	else { std::cout << "Not found" << std::endl; }
	result = hTable.Search(key);
	std::cout << "HashTable: ";
	if (result) { std::cout << "Found -> " << *result << std::endl; }
	else { std::cout << "Not found" << std::endl; }
	printSeparator('-', 40);
	printOperationsStats("Key searching '" + key + "'", uTable, sTable, avlTable, hTable);
}
void removePolynomial(UnsortedTable& uTable, SortedTable& sTable, AVLTreeTable& avlTable, HashTable& hTable) {
	printHeader("DELETING POLYNOMIAL");
	std::string key;
	std::cout << "Enter key for deleting: ";
	std::cin >> key;
	printSeparator('-', 40);
	resetAllStats(uTable, sTable, avlTable, hTable);
	bool uResult = uTable.Remove(key);
	bool sResult = sTable.Remove(key);
	bool avlResult = avlTable.Remove(key);
	bool hResult = hTable.Remove(key);
	std::cout << "Unsorted table: " << (uResult ? "Deleted" : "Not found") << std::endl;
	std::cout << "Sorted table: " << (sResult ? "Deleted" : "Not found") << std::endl;
	std::cout << "AVLTreeTable with chains: " << (avlResult ? "Deleted" : "Not found") << std::endl;
	std::cout << "HashTable: " << (hResult ? "Deleted" : "Not found") << std::endl;
	printSeparator('-', 40);
	printOperationsStats("Deleting a key '" + key + "'", uTable, sTable, avlTable, hTable);
}
void showAllTables(UnsortedTable& uTable, SortedTable& sTable, AVLTreeTable& avlTable, HashTable& hTable) {
	printHeader("TABLE CONTENTS");
	uTable.PrintAll();
	std::cout << std::endl;
	sTable.PrintAll();
	std::cout << std::endl;
	avlTable.PrintAll();
	std::cout << std::endl;
	hTable.PrintAll();
	std::cout << std::endl;
}
void showDetailedStats(UnsortedTable& uTable, SortedTable& sTable, AVLTreeTable& avlTable, HashTable& hTable) {
	printHeader("DETAILED STATISTICS");
	std::cout << "\n===== UNSORTED TABLE =====" << std::endl;
	std::cout << "Elements: " << uTable.GetSize() << std::endl;
	std::cout << "Comparisions: " << uTable.GetComparisonCount() << std::endl;
	std::cout << "\n===== SORTED TABLE =====" << std::endl;
	std::cout << "Elements: " << sTable.GetSize() << std::endl;
	std::cout << "Comparisions: " << sTable.GetComparisonCount() << std::endl;
	std::cout << "Shifts: " << sTable.GetShiftCount() << std::endl;
	std::cout << "\n===== AVLTREE TABLE WITH CHAINS =====" << std::endl;
	std::cout << "Elements: " << avlTable.GetSize() << std::endl;
	std::cout << "Comparisions: " << avlTable.GetComparisonCount() << std::endl;
	std::cout << "Turns when balancing: " << avlTable.GetRotationCount() << std::endl;
	std::cout << "Requests to the chains: " << avlTable.GetChainProbeCount() << std::endl;
	std::cout << "Height of tree: " << avlTable.GetHeight() << std::endl;
	std::cout << "\n===== HASH-TABLE =====" << std::endl;
	hTable.PrintStats();
}
int main() {
	UnsortedTable unsortedTable;
	SortedTable sortedTable;
	AVLTreeTable avlTreeTable;
	HashTable hashTable(101);
	std::cout << "Welcome to the spreadsheet program!" << std::endl;
	std::cout << "Available table types:" << std::endl;
	std::cout << "  1. UnsortedArray" << std::endl;
	std::cout << "  2. SortedArray" << std::endl;
	std::cout << "  3. AVLTreeWithChains" << std::endl;
	std::cout << "  4. HashTable" << std::endl;
	logger.Enable();
	int choice;
	do {
		std::cout << std::endl;
		showMenu();
		std::cin >> choice;
		if (std::cin.fail()) {
			clearInputBuffer();
			std::cout << "Error! Enter number" << std::endl;
			continue;
		}
		switch (choice) {
		case 1:
			addPolynomial(unsortedTable, sortedTable, avlTreeTable, hashTable);
			break;
		case 2:
			searchPolynomial(unsortedTable, sortedTable, avlTreeTable, hashTable);
			break;
		case 3:
			removePolynomial(unsortedTable, sortedTable, avlTreeTable, hashTable);
			break;
		case 4:
			showAllTables(unsortedTable, sortedTable, avlTreeTable, hashTable);
			break;
		case 5:
			showDetailedStats(unsortedTable, sortedTable, avlTreeTable, hashTable);
			break;
		case 6:
			resetAllStats(unsortedTable, sortedTable, avlTreeTable, hashTable);
			break;
		case 7:
			arithmeticOperations();
			break;
		case 0:
			std::cout << "\nGood bye!" << std::endl;
			break;
		default:
			std::cout << "Uncorrect choice! Try again." << std::endl;
			break;
		}
	} while (choice != 0);
	return 0;
}
