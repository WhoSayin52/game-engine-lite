#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <vector>

enum class LogType {
	Info,
	Warning,
	Error
};

struct LogEntry {
	LogType type{};
	std::string log{};
};

class Logger {
public:
	static std::vector <LogEntry> logs;

	static void log(const std::string& message);
	static void err(const std::string& message);
};

#endif //LOGGER_HPP
