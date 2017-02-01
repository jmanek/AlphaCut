#include "Log.h"
#include <cstdio>
#include <fstream>
#include <cstdlib>
const std::string Log::defaultLogFilePath = "log.txt";
std::shared_ptr<spdlog::logger> Log::console = spdlog::stdout_color_mt("console");
std::shared_ptr<spdlog::logger> Log::fileLog = spdlog::basic_logger_mt("basic_logger", Log::defaultLogFilePath, true); 

void Log::SetLogFile(const string &fp)
{
	Log::fileLog = spdlog::basic_logger_mt(to_string(rand()), fp, true); 
}

// trace = 0,
// debug = 1,
// info = 2,
// warn = 3,
// err = 4,
// critical = 5,
// off = 6
void Log::HideDebug()
{
	spdlog::set_level(spdlog::level::info);	
}
void Log::ShowDebug()
{
	spdlog::set_level(spdlog::level::debug);	
}

void Log::HideConsole()
{
	spdlog::set_level(spdlog::level::off);
}
