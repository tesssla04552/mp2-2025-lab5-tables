#ifndef SORTED_TABLE_H
#define SORTED_TABLE_H
#include <vector>
#include <utility>
#include <string>
#include <algorithm>
#include "table_struct.h"
class SortedTable : public Table<std::string, Polynomials> {
private:
	std::vector<std::pair<std::string, Polynomials>> data;
	int ComparisonCount;
	int ShiftCount;
	int BinarySearch(const std::string& key) const;
public:
	SortedTable();
	~SortedTable() = default;
	void Insert(const std::string& key, const Polynomials& value) override;
	Polynomials* Search(const std::string& key) override;
	bool Remove(const std::string& key) override;
	int GetComparisonCount() const override { return ComparisonCount; }
	int GetShiftCount() const override { return ShiftCount; }
	void ResetStats() override { ComparisonCount = 0; ShiftCount = 0; }
	std::string GetName() const override { return "SortedArray"; }
	int GetSize() const override { return data.size(); }
	bool IsEmpty() const override { return data.empty(); }
	void PrintAll() const;
};
#endif
