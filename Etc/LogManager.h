#pragma once
namespace log
{

	class LogManager
	{
		LogManager();
		~LogManager();


		void Init();
		
	};

	class BaseLog
	{
		BaseLog();
		~BaseLog();

		virtual void Init();
	};

	class FileLog : BaseLog
	{
		FileLog();
		~FileLog();
		
		virtual void Init();

	};
	class ConsoleLog : BaseLog
	{
		ConsoleLog();
		~ConsoleLog();
		virtual void Init();
	};


};