#include "sorted_table.h"
#include <iostream>
#include <algorithm>
SortedTable::SortedTable() : ComparisonCount(0), ShiftCount(0) {}
int SortedTable::BinarySearch(const std::string& key) const {
	int ComparisionCount = 0;
	int left = 0;
	int right = data.size() - 1;
	while (left <= right) {
		ComparisionCount++;
		int mid = left + (right - left) / 2;
		if (data[mid].first == key) {
			return mid;
		}
		else if (data[mid].first > key) {
			right = mid - 1;
		}
		else {
			left = mid + 1;
		}
	}
	return -1;
}
void SortedTable::Insert(const std::string& key, const Polynomials& value) {
	int localComparisions = 0;
	int localShifts = 0;
	int left = 0;
	int right = data.size() - 1;
	int position = 0;
	while (left <= right) {
		localComparisions++;
		int mid = left + (right - left) / 2;
		if (data[mid].first == key) {
			data[mid].second = value;
			ComparisonCount += localComparisions;
			ShiftCount += localShifts;
			std::cout << "[SortedTable] Existing key has been updated: " << key << " (comparisions: " \
				<< localComparisions << ") " << std::endl;
			return;
		}
		else if (data[mid].first > key) {
			right = mid - 1;
		}
		else {
			left = mid + 1;
		}
	}
	position = left;
	data.resize(data.size() + 1);
	for (size_t i = data.size() - 1; i > position; --i) {
		data[i] = data[i - 1];
		localShifts++;
	}
	data[position] = { key, value };
	ComparisonCount += localComparisions;
	ShiftCount += localShifts;
	std::cout << "[SortedTable] New key has been added: " << key << " (comparisions: " \
		<< localComparisions << ", shifts: " << localShifts << "), table size: " << data.size() << std::endl;
}
Polynomials* SortedTable::Search(const std::string& key) {
	int index = BinarySearch(key);
	if (index != -1) {
		std::cout << "[SortedTable] Key has been found: " << key << " (comparisions: " \
			<< ComparisonCount << ") " << std::endl;
		return &data[index].second;
	}
	std::cout << "[SortedTable] Key was not found: " << key << " (comparisions: " \
		<< ComparisonCount << ") " << std::endl;
	return nullptr;
}
bool SortedTable::Remove(const std::string& key) {
	int oldComparisions = ComparisonCount;
	int oldShifts = ShiftCount;
	int index = BinarySearch(key);
	if (index == -1) {
		std::cout << "[SortedTable] Couldn't delete (key was not found): " << key << " (comparisions: " \
			<< (ComparisonCount - oldComparisions) << ") " << std::endl;
		return false;
	}
	int localShifts = 0;
	for (size_t i = index; i < data.size() - 1; ++i) {
		data[i] = data[i + 1];
		localShifts++;
	}
	data.pop_back();
	ShiftCount += localShifts;
	std::cout << "[SortedTable] Key was deleted: " << key << " (comparisions: " \
		<< (ComparisonCount - oldComparisions) << ", shifts: " << localShifts << ")" << std::endl;
	return true;
}
void SortedTable::PrintAll() const {
	if (data.empty()) {
		std::cout << "[SortedTable] Table is empty" << std::endl;
		return;
	}
	std::cout << "[SortedTable] Table Contents (sorted by key):" << std::endl;
	for (const auto& pair : data) {
		std::cout << "  Key: " << pair.first << " -> Polynom: " << pair.second << std::endl;
	}
	std::cout << "Total elements: " << data.size() << std::endl;
}