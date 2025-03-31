#include "./headers.hpp"
#ifndef PARTESISTEMA_H
#define PARTESISTEMA_H

using string = std::string;
void saveExecutionTimeToJson(int id, const string& functionName, double duration);
void loadAndPrintExecutionTimes();
void loadAndPrintResourceUsage();
void printAndSaveResourceUsage(const string& functionName);
void printDatabaseSize(const std::string& fileName);


#endif