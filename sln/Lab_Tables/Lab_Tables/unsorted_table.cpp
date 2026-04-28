#include "unsorted_table.h"
#include <iostream>
#include <algorithm>
UnsortedTable::UnsortedTable() : ComparisonCount(0) {}
void UnsortedTable::Insert(const std::string& key, const Polynomials& value) {
	for (size_t i = 0; i < data.size(); ++i) {
		ComparisonCount++;
		if (data[i].first == key) {
			data[i].second = value;
			std::cout << "[UnsortedTable] Existing key has been updated: " << key << std::endl;
			return;
		}
	}
	data.push_back({ key, value });
	std::cout << "[UnsortedTable] New key has been added: " << key << ", table size: " << data.size() << std::endl;
}
Polynomials* UnsortedTable::Search(const std::string& key) {
	for (size_t i = 0; i < data.size(); ++i) {
		ComparisonCount++;
		if (data[i].first == key) {
			std::cout << "[UnsortedTable] Key has been found: " << key << " (comparisons: " \
				<< ComparisonCount << ") " << std::endl;
			return &data[i].second;
		}
	}
	std::cout << "[UnsortedTable] Key was not found: " << key << " (comparisons: " << ComparisonCount << ") " << std::endl;
	return nullptr;
}
bool UnsortedTable::Remove(const std::string& key) {
	int localComparisons = 0;
	for (auto it = data.begin(); it != data.end(); ++it) {
		localComparisons++;
		if (it->first == key) {
			data.erase(it);
			std::cout << "[UnsortedTable] Key was deleted: " << key << " (comparisons: " \
				<< localComparisons << ") " << std::endl;
			return true;
		}
	}
	std::cout << "[UnsortedTable] Couldn't delete (key was not found): " << key << std::endl;
	return false;
}
void UnsortedTable::PrintAll() const {
	if (data.empty()) {
		std::cout << "[UnsortedTable] Table is empty" << std::endl;
		return;
	}
	std::cout << "[UnsortedTable] Table Contents:" << std::endl;
	for (const auto& pair : data) {
		std::cout << " Key: " << pair.first << " -> Polynom: " << pair.second << std::endl;
	}
	std::cout << "Total elements: " << data.size() << std::endl;
}