#pragma once
#include "spdlog/spdlog.h"
#include "fmt/ostream.h"
#include <iostream>
#include <memory>
#include <string>
	using namespace std;
	class Log
	{
		public:
			static shared_ptr<spdlog::logger> console;
			static shared_ptr<spdlog::logger> fileLog;
			static const string defaultLogFilePath; 
			template<typename... Args>
			static void Error(const char* fmt, const Args&... args)
			{
				console->error(fmt, args...);
			}

			template<typename... Args>
			static void Debug(const char* fmt, const Args&... args)
			{
				console->debug(fmt, args...);
			}
			template<typename... Args>
			static void Info(const char* fmt, const Args&... args)
			{
				console->info(fmt, args...);
			}	

			template<typename... Args>
			static void Warn(const char* fmt, const Args&... args)
			{
				console->warn(fmt, args...);
			}	

			template<typename... Args>
			static void Critical(const char* fmt, const Args&... args)
			{
				console->critical(fmt, args...);
			}

			template<typename... Args>
			static void File(const char* fmt, const Args&... args)
			{
				fileLog->info(fmt, args...);
			}

			static void ClearLogFile();
			static void SetLogFile(const string &fp);
			static void HideDebug();
			static void ShowDebug();
			static void HideConsole();
	};

