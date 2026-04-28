#include "hash_table.h"
#include <iostream>
#include <iomanip>
HashTable::HashTable(int size) : TableSize(size), ComparisonCount(0) {
	buckets.resize(TableSize);
	std::cout << "[HashTable] HashTable of the following size has been created" << TableSize << std::endl;
}
int HashTable::hash(const std::string& key) const {
	int sum = 0;
	for (char c : key) {
		sum += c;
	}
	return sum % TableSize;
}
void HashTable::Insert(const std::string& key, const Polynomials& value) {
	int index = hash(key);
	int localComparisons = 0;
	for (auto& pair : buckets[index]) {
		localComparisons++;
		if (pair.first == key) {
			pair.second = value;
			ComparisonCount += localComparisons;
			std::cout << "[HashTable] Existing key has been updated: " << key << " (basket: " << index \
				<< ", comparisons: " << localComparisons << ") " << std::endl;
			return;
		}
	}
	buckets[index].push_back({ key, value });
	ComparisonCount += localComparisons;
	std::cout << "[HashTable] New key has been added: " << key << " (basket: " << index << ", comparisons: " << \
		localComparisons << "), size of basket: " << buckets[index].size() << ", loading: " << \
		std::fixed << std::setprecision(2) << GetLoadFactor() << std::endl;
}
Polynomials* HashTable::Search(const std::string& key) {
	int index = hash(key);
	int localComparisons = 0;
	for (auto& pair : buckets[index]) {
		localComparisons++;
		if (pair.first == key) {
			ComparisonCount += localComparisons;
			std::cout << "[HashTable] Key has been found: " << key << " (basket: " << index << ", comparisons: " << \
				localComparisons << ")" << std::endl;
			return &pair.second;
		}
	}
	ComparisonCount += localComparisons;
	std::cout << "[HashTable] Key was not found: " << key << " (basket: " << index << ", comparisons: " << \
		localComparisons << ")" << std::endl;
	return nullptr;
}
bool HashTable::Remove(const std::string& key) {
	int index = hash(key);
	int localComparisons = 0;
	for (auto it = buckets[index].begin(); it != buckets[index].end(); ++it) {
		localComparisons++;
		if (it->first == key) {
			buckets[index].erase(it);
			ComparisonCount += localComparisons;
			std::cout << "[HashTable] Key has been deleted: " << key << " (basket: " << index << ", comparisions: " << \
				localComparisons << "), size of basket: " << buckets[index].size() << std::endl;
			return true;
		}
	}
	ComparisonCount += localComparisons;
	std::cout << "[HashTable] Couldn't delete (key was not found): " << key << " (basket: " << index << ", comparisions: " \
		<< localComparisons << ")" << std::endl;
	return false;
}
int HashTable::GetSize() const {
	int total = 0;
	for (const auto& bucket : buckets) {
		total += bucket.size();
	}
	return total;
}
bool HashTable::IsEmpty() const {
	return GetSize() == 0;
}
double HashTable::GetLoadFactor() const {
	if (TableSize == 0) { return 0.0; }
	return static_cast<double>(GetSize()) / TableSize;
}
void HashTable::PrintStats() const {
	std::cout << "\n===== STATISTICS OF HASH TABLE =====" << std::endl;
	std::cout << "Size of table: " << TableSize << std::endl;
	std::cout << "Number of elements: " << GetSize() << std::endl;
	std::cout << "Factor loading: " << std::fixed << std::setprecision(4) << GetLoadFactor() << std::endl;
	int maxBucketSize = 0;
	int emptyBuckets = 0;
	int totalCollisions = 0;
	for (const auto& bucket : buckets) {
		int size = bucket.size();
		if (size > maxBucketSize) { maxBucketSize = size; }
		if (size == 0) { emptyBuckets++; }
		if (size > 1) { totalCollisions += (size - 1); }
	}
	std::cout << "Maximum size of basket: " << maxBucketSize << std::endl;
	std::cout << "Empty baskets: " << emptyBuckets << " (" << std::fixed << std::setprecision(2) << \
		(100.0 * emptyBuckets / TableSize) << "%) " << std::endl;
	std::cout << "Collisions: " << totalCollisions << std::endl;
	double AvgSearch = 0.0;
	for (const auto& bucket : buckets) {
		AvgSearch += (bucket.size() * (bucket.size() + 1)) / 2.0;
	}
	if (GetSize() > 0) {
		AvgSearch /= GetSize();
		std::cout << "Average number of comparisions with successful search: ~" << std::fixed << std::setprecision(2) << \
			AvgSearch << std::endl;
	}
	std::cout << "===============\n" << std::endl;
}
void HashTable::PrintAll() const {
	if (IsEmpty()) {
		std::cout << "[HashTable] Table is empty" << std::endl;
		return;
	}
	std::cout << "\n===== CONTENT OF HASH TABLE =====" << std::endl;
	for (int i = 0; i < TableSize; ++i) {
		if (!buckets[i].empty()) {
			std::cout << "Basket: " << i << " (" << buckets[i].size() << " elements):" << std::endl;
			for (const auto& pair : buckets[i]) {
				std::cout << "   Key: " << pair.first << "--> Polynom: " << pair.second << std::endl;
			}
		}
	}
	std::cout << "Total elements: " << GetSize() << std::endl;
	PrintStats();
}
	