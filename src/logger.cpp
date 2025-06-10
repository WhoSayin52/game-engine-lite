#include "logger.hpp"

#include <chrono>
#include <ctime>
#include <iostream>

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

std::vector <LogEntry> Logger::logs{};

static void append_current_date_time(std::string& dt_str);

void Logger::log(const std::string& message) {
	std::string output{ "LOG: [" };
	append_current_date_time(output);
	output.append("] - ").append(message);

	LogEntry entry{ LogType::Info, output };

	std::cout << GREEN << output << RESET << '\n';

	Logger::logs.push_back(entry);
}

void Logger::err(const std::string& message) {
	std::string output{ "ERR: [" };
	append_current_date_time(output);
	output.append("] - ").append(message);

	LogEntry entry{ LogType::Error, output };

	std::cout << RED << output << RESET << '\n';

	logs.push_back(entry);
}

static void append_current_date_time(std::string& dt_str) {
	static constexpr std::size_t date_time_char_max_size = 30;

	const auto now{ std::chrono::system_clock::now() };
	const auto date_time{ std::chrono::system_clock::to_time_t(now) };

	char buffer[date_time_char_max_size];
	std::strftime(buffer, date_time_char_max_size, "%d/%b/%Y %H:%M:%S", std::localtime(&date_time));

	dt_str.append(buffer);
}
