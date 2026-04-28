#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <map>
#include <iostream>
#include <iomanip>
struct TableStats {
	std::string TableName;
	int Comparisons = 0;
	int Shifts = 0;
	int Rotations = 0;
	int ChainProbes = 0;
	int Operations = 0;
	void Reset() {
		Comparisons = 0;
		Shifts = 0;
		Rotations = 0;
		ChainProbes = 0;
		Operations = 0;
	}
};
class Logger {
private:
	std::map<std::string, TableStats> stats;
	bool enabled;
public:
	Logger() : enabled(true) {}
	void Enable();
	void Disable();
	void LogComparison(const std::string& tableName, int count = 1);
	void LogShift(const std::string& tableName, int count = 1);
	void LogRotation(const std::string& tableName, int count = 1);
	void LogChainProbe(const std::string& tableName, int count = 1);
	void LogOperation(const std::string& tableName);
	TableStats GetStats(const std::string& tableName) const;
	void ResetStats(const std::string& tableName);
	void ResetAllStats();
	void PrintStats(const std::string& operationName = "") const;
	void LogInsert(const std::string& tableName, int comparisons, int shifts = 0, int rotations = 0, int chainProbes = 0);
};
#endif
