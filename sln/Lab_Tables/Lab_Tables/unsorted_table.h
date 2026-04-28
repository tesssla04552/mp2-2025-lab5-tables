#ifndef UNSORTED_TABLE_H
#define UNSORTED_TABLE_H
#include "table_struct.h"
#include "polynomial.h"
#include <vector>
#include <utility>
#include <string>
class UnsortedTable : public Table<std::string, Polynomials> {
private:
	std::vector<std::pair<std::string, Polynomials>> data;
	int ComparisonCount;
public:
	UnsortedTable();
	~UnsortedTable() = default;
	void Insert(const std::string& key, const Polynomials& value) override;
	Polynomials* Search(const std::string& key) override;
	bool Remove(const std::string& key) override;
	int GetComparisonCount() const override { return ComparisonCount; }
	std::string GetName() const override { return "UnsortedArray"; }
	bool IsEmpty() const override { return data.empty(); }
	void PrintAll() const;
	void ResetStats() override { ComparisonCount = 0; }
	int GetSize() const override { return data.size(); }
};
#endif
