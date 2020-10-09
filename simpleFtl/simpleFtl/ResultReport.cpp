#include "ResultReport.h"

Configurer::Configurer(PathConfigure newPathConfig, ConfigToken newConfToken)
{
	pathConfig = newPathConfig;
	confToken = newConfToken;
	confToken.date = GetCurrentTime();
	confToken.block = to_string(TOTAL_SUPER_BLOCK_CNT);
	confToken.page = to_string(SECTOR_PER_SUPER_BLOCK);

};

string Configurer::GetCurrentTime()
{
	char curTimeChr[30];
	string curTimeStr = "";
	time_t curTime = time(NULL);
	struct tm* pLocal = NULL;
#if defined(_WIN32) || defined(_WIN64) 
	pLocal = localtime(&curTime);
#else 
	localtime_r(&curTime, pLocal);
#endif
	if (pLocal == NULL)
	{
		return "Getting Current Time Failure";
	}
	sprintf(curTimeChr, "%04d-%02d-%02d %02d:%02d:%02d",
		pLocal->tm_year + 1900, pLocal->tm_mon + 1, pLocal->tm_mday,
		pLocal->tm_hour, pLocal->tm_min, pLocal->tm_sec);
	curTimeStr += curTimeChr;
	return curTimeStr;
}

ResultReport::ResultReport(Configurer& config)
	:config(config)
{
	csvFilePath = config.GetPathConfig().RESULT_PATH + config.GetConfigToken().out + "csv";

	if (_access(csvFilePath.c_str(), 0) != 0)
	{
		FILE* csvFile = fopen(csvFilePath.c_str(), "wt");
		ChkOpenedFile(csvFile, csvFilePath.c_str());
		fclose_with_chk(csvFile, csvFilePath.c_str());
	}
	
	item = "Date,HW,SW,WKLD,Block,Page";
	value = config.GetConfigToken().date + "," + config.GetConfigToken().hw + "," + config.GetConfigToken().sw + "," + config.GetConfigToken().wkd 
		+ "," + config.GetConfigToken().block + "," + config.GetConfigToken().page;

}
ResultReport::ResultReport(Configurer& config, string addToken)
	:config(config)
{

}

void ResultReport::BufferItemValue(const string& item, const string& value)
{
	if (!csvFilePath.empty())
	{
		this->item += "," + item;
		this->value += "," + value;
	}
}
void ResultReport::ReportItemValue()
{
	if (!csvFilePath.empty())
	{
		FILE* csvFile = fopen(csvFilePath.c_str(), "a+t");
		ChkOpenedFile(csvFile, csvFilePath.c_str());

		fscanf(csvFile, " ");
		auto eof = feof(csvFile);
		rewind(csvFile);

		if (eof != 0)
		{
			fprintf(csvFile, "%s", item.c_str());
		}
		fprintf(csvFile, "\n%s", value.c_str());
		printf("Write CSV File\n");
		fclose_with_chk(csvFile, csvFilePath.c_str());
	}
}

void ResultReport::ChkOpenedFile(FILE* csvFile, const char* csvFilePath)
{
	if (csvFile == nullptr)
	{
		printf("ERROR open file: %s\n", csvFilePath);
		assert(0);
	}
}
void ResultReport::fclose_with_chk(FILE* csvFile, const char* csvFilePath)
{
	if (fclose(csvFile) != 0)
	{
		printf("ERROR close file: %s\n", csvFilePath);
		assert(0);
	}
}

string ResultReport::GetCurrentTime()
{
	char curTimeChr[30];
	string curTimeStr = "";
	time_t curTime = time(NULL);
	struct tm* pLocal = NULL;
#if defined(_WIN32) || defined(_WIN64) 
	pLocal = localtime(&curTime);
#else 
	localtime_r(&curTime, pLocal);
#endif
	if (pLocal == NULL)
	{
		return "Getting Current Time Failure";
	}
	/*
	sprintf(curTimeStr, "%04d-%02d-%02dT%02d:%02d:%02d",
		pLocal->tm_year + 1900, pLocal->tm_mon + 1, pLocal->tm_mday,
		pLocal->tm_hour, pLocal->tm_min, pLocal->tm_sec);
		*/
	sprintf(curTimeChr, "%04d-%02d-%02d %02d:%02d:%02d",
		pLocal->tm_year + 1900, pLocal->tm_mon + 1, pLocal->tm_mday,
		pLocal->tm_hour, pLocal->tm_min, pLocal->tm_sec);
	curTimeStr += curTimeChr;
	return curTimeStr;
}