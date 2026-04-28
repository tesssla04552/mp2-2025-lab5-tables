#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include "table_struct.h"
#include <vector>
#include <list>
#include <functional>
#include <string>
class HashTable : public Table<std::string, Polynomials> {
private:
	std::vector<std::list<std::pair<std::string, Polynomials>>> buckets;
	int TableSize;
	int ComparisonCount;
	int hash(const std::string& key) const;
public:
	HashTable(int size = 107);
	~HashTable() = default;
	void Insert(const std::string& key, const Polynomials& value) override;
	Polynomials* Search(const std::string& key) override;
	bool Remove(const std::string& key) override;
	int GetComparisonCount() const override { return ComparisonCount; }
	void ResetStats() override { ComparisonCount = 0; }
	std::string GetName() const override { return "HashTable"; }
	int GetSize() const;
	bool IsEmpty() const override;
	double GetLoadFactor() const;
	void PrintStats() const;
	void PrintAll() const;
};
#endif
