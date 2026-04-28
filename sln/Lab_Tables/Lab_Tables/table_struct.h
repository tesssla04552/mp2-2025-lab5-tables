#ifndef TABLE_STRUCT_H
#define TABLE_STRUCT_H
#include <string>
#include "../include/polynomial.h"
template<typename KeyType, typename ValueType>
class Table {
public:
	virtual ~Table() = default;
	virtual void Insert(const KeyType& key, const ValueType& value) = 0;
	virtual ValueType* Search(const KeyType& key) = 0;
	virtual bool Remove(const KeyType& key) = 0;
	virtual int GetComparisonCount() const { return 0; }
	virtual int GetShiftCount() const { return 0; }
	virtual int GetRotationCount() const { return 0; }
	virtual int GetChainProbeCount() const { return 0; }
	virtual void ResetStats() = 0;
	virtual std::string GetName() const = 0;
	virtual int GetSize() const = 0;
	virtual bool IsEmpty() const = 0;
};
#endif
