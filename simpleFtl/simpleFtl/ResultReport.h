#pragma once

#include "Common.h"

struct PathConfigure
{
    string PROJ_PATH;
    string CONFIG_PATH;
    string RESULT_PATH;
    string LOG_PATH;
};

struct ConfigToken
{
    string out;
    string date;
    string hw;
    string sw;
    string wkd;
    string trace;
    string block;
    string page;
};
class Configurer
{
public:
    Configurer(PathConfigure pathConfig, ConfigToken confToken);
    PathConfigure GetPathConfig() { return pathConfig; };
    ConfigToken GetConfigToken() { return confToken; };
    string GetCurrentTime();

private:
    PathConfigure pathConfig;
    ConfigToken confToken;
};

class ResultReport
{
public:
    ResultReport(Configurer& config);
    ResultReport(Configurer& config, string addToken);
    void BufferItemValue(const string& item, const string& value);
    void ReportItemValue();
    void ChkOpenedFile(FILE* csvFile, const char* csvFilePath);
    void fclose_with_chk(FILE* csvFile, const char* csvFilePath);
    string GetCurrentTime();
    
private:
    Configurer& config;
    string csvFilePath;
    string item;
    string value;

};