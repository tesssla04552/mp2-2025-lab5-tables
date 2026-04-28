#include "logger.h"
#include <iomanip>
#include <iostream>
#include <algorithm>
void Logger::Enable() {
	enabled = true;
	std::cout << "[Logger] Logging is enabled" << std::endl;
}
void Logger::Disable() {
	enabled = false;
	std::cout << "[Logger] Logging is disabled" << std::endl;
}
void Logger::LogComparison(const std::string& tableName, int count) {
    if (!enabled) { return; }
    if (stats.find(tableName) == stats.end()) {
        stats[tableName].TableName = tableName;
    }
    stats[tableName].Comparisons += count;
}
void Logger::LogShift(const std::string& tableName, int count) {
    if (!enabled) { return; }
    if (stats.find(tableName) == stats.end()) {
        stats[tableName].TableName = tableName;
    }
    stats[tableName].Shifts += count;
}
void Logger::LogRotation(const std::string& tableName, int count) {
    if (!enabled) { return; }
    if (stats.find(tableName) == stats.end()) {
        stats[tableName].TableName = tableName;
    }
    stats[tableName].Rotations += count;
}
void Logger::LogChainProbe(const std::string& tableName, int count) {
    if (!enabled) { return; }
    if (stats.find(tableName) == stats.end()) {
        stats[tableName].TableName = tableName;
    }
    stats[tableName].ChainProbes += count;
}
void Logger::LogOperation(const std::string& tableName) {
    if (!enabled) { return; }
    if (stats.find(tableName) == stats.end()) {
        stats[tableName].TableName = tableName;
    }
    stats[tableName].Operations++;
}
void Logger::LogInsert(const std::string& tableName, int comparisons, \
    int shifts, int rotations, int chainProbes) {
    if (!enabled) { return; }
    LogComparison(tableName, comparisons);
    LogShift(tableName, shifts);
    LogRotation(tableName, rotations);
    LogChainProbe(tableName, chainProbes);
    LogOperation(tableName);
}
TableStats Logger::GetStats(const std::string& tableName) const {
    auto it = stats.find(tableName);
    if (it != stats.end()) {
        return it->second;
    }
    TableStats empty;
    empty.TableName = tableName;
    return empty;
}
void Logger::ResetStats(const std::string& tableName) {
    auto it = stats.find(tableName);
    if (it != stats.end()) {
        it->second.Reset();
    }
}
void Logger::ResetAllStats() {
    for (auto& pair : stats) {
        pair.second.Reset();
    }
    std::cout << "[Logger] Stat-s of all tables are reset" << std::endl;
}
void Logger::PrintStats(const std::string& operationName) const {
    if (stats.empty()) {
        std::cout << "\n====== NO DATA FOR STATISTICS ======\n" << std::endl;
        return;
    }
    std::cout << "\n|--------------------------------------------|\n" << std::endl;
    if (!operationName.empty()) {
        std::cout << "|| Statistic after operation: " << std::left << std::setw(46) << operationName << "||" \
            << std::endl;
        std::cout << "|--------------------------------------------|\n" << std::endl;
    }
    else {
        std::cout << "||    CURRENT STATISTICS ON TABLE     ||" << std::endl;
        std::cout << "|--------------------------------------------|\n" << std::endl;
    }
    std::cout << "|| " << std::left << std::setw(20) << "Table" \
        << "|| " << std::setw(12) << "Comparisions" \
        << "|| " << std::setw(6) << "Shifts" \
        << "|| " << std::setw(9) << "Rotations" \
        << "|| " << std::setw(11) << "ChainProbes" \
        << "|| " << std::setw(10) << "Operations" << " ||" << std::endl;
    std::cout << "|---------------------------------------------------------------------------|\n" << std::endl;
    for (const auto& pair : stats) {
        const TableStats& s = pair.second;
        std::cout << "|| " << std::left << std::setw(22) << s.TableName << \
            "|| " << std::setw(12) << s.Comparisons << \
            "|| " << std::setw(6) << s.Shifts << \
            "|| " << std::setw(9) << s.Rotations << \
            "|| " << std::setw(11) << s.ChainProbes << \
            "|| " << std::setw(10) << s.Operations << " ||" << std::endl;
    }
    std::cout << "|---------------------------------------------------------------------------|\n" << std::endl;
}