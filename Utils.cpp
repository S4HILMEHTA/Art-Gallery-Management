// Utils.cpp
#include "Utils.h"
#include "GalleryException.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace Utils {

// ---------------------------------------------------------------- strings
std::string trim(const std::string& s) {
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) ++start;
    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) --end;
    return s.substr(start, end - start);
}

std::string toLower(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return result;
}

bool containsIgnoreCase(const std::string& text, const std::string& term) {
    return toLower(text).find(toLower(term)) != std::string::npos;
}

bool isDigits(const std::string& s) {
    if (s.empty()) return false;
    for (size_t i = 0; i < s.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) return false;
    }
    return true;
}

// Unlike getline-based splitting this keeps empty fields (needed for file records).
std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> parts;
    size_t start = 0;
    while (true) {
        size_t pos = s.find(delimiter, start);
        if (pos == std::string::npos) {
            parts.push_back(s.substr(start));
            break;
        }
        parts.push_back(s.substr(start, pos - start));
        start = pos + 1;
    }
    return parts;
}

std::string formatMoney(double amount) {
    std::ostringstream out;
    out << "Rs. " << std::fixed << std::setprecision(2) << amount;
    return out.str();
}

std::string todayDate() {
    std::time_t now = std::time(nullptr);
    std::tm* local = std::localtime(&now);
    std::ostringstream out;
    out << std::setfill('0') << std::setw(2) << local->tm_mday << '/'
        << std::setw(2) << (local->tm_mon + 1) << '/' << (local->tm_year + 1900);
    return out.str();
}

// ------------------------------------------------------------- validation
bool isValidDate(const std::string& date) {
    if (date.size() != 10 || date[2] != '/' || date[5] != '/') return false;
    std::string d = date.substr(0, 2), m = date.substr(3, 2), y = date.substr(6, 4);
    if (!isDigits(d) || !isDigits(m) || !isDigits(y)) return false;

    int day = std::atoi(d.c_str());
    int month = std::atoi(m.c_str());
    int year = std::atoi(y.c_str());
    if (year < 1900 || year > 2100 || month < 1 || month > 12 || day < 1) return false;

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (leap) daysInMonth[1] = 29;
    return day <= daysInMonth[month - 1];
}

int parseInt(const std::string& s) {
    if (!isDigits(s) || s.size() > 9) throw GalleryException("Invalid number: '" + s + "'");
    return std::atoi(s.c_str());
}

double parseDouble(const std::string& s) {
    char* end = nullptr;
    double value = std::strtod(s.c_str(), &end);
    if (s.empty() || *end != '\0') throw GalleryException("Invalid decimal number: '" + s + "'");
    return value;
}

// Accepts only digits and at most one '.', so "-5", "abc", "nan", "1e9" are rejected.
static bool looksLikePositiveDecimal(const std::string& s) {
    if (s.empty()) return false;
    int dots = 0;
    int digits = 0;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '.') {
            ++dots;
        } else if (std::isdigit(static_cast<unsigned char>(s[i]))) {
            ++digits;
        } else {
            return false;
        }
    }
    return dots <= 1 && digits > 0;
}

// ------------------------------------------------------------ console input
std::string readLine(const std::string& prompt) {
    std::cout << prompt << std::flush;
    std::string line;
    if (!std::getline(std::cin, line)) throw InputEndedException();
    return trim(line);
}

std::string readText(const std::string& label, size_t maxLength) {
    while (true) {
        std::string value = readLine(label + ": ");
        if (value.empty()) {
            std::cout << "  Invalid input: " << label << " cannot be empty.\n";
        } else if (value.find('|') != std::string::npos) {
            std::cout << "  Invalid input: " << label << " cannot contain the '|' character.\n";
        } else if (value.size() > maxLength) {
            std::cout << "  Invalid input: " << label << " cannot exceed " << maxLength
                      << " characters.\n";
        } else {
            return value;
        }
    }
}

// Blank input is allowed and means "keep the current value" (returns "").
std::string readOptionalText(const std::string& label, size_t maxLength) {
    while (true) {
        std::string value = readLine(label + ": ");
        if (value.empty()) return "";
        if (value.find('|') != std::string::npos) {
            std::cout << "  Invalid input: " << label << " cannot contain the '|' character.\n";
        } else if (value.size() > maxLength) {
            std::cout << "  Invalid input: " << label << " cannot exceed " << maxLength
                      << " characters.\n";
        } else {
            return value;
        }
    }
}

int readInt(const std::string& prompt, int minValue, int maxValue) {
    while (true) {
        std::string value = readLine(prompt);
        if (isDigits(value) && value.size() <= 9) {
            int number = std::atoi(value.c_str());
            if (number >= minValue && number <= maxValue) return number;
        }
        std::cout << "  Invalid input: enter a whole number between " << minValue << " and "
                  << maxValue << ".\n";
    }
}

double readPrice(const std::string& prompt) {
    while (true) {
        std::string value = readLine(prompt + ": ");
        if (looksLikePositiveDecimal(value)) {
            double price = std::atof(value.c_str());
            if (price > 0 && price <= 1000000000.0) return price;
        }
        std::cout << "  Invalid input: price must be a positive number (e.g. 25000 or 25000.50).\n";
    }
}

double readOptionalPrice(const std::string& prompt, double currentValue) {
    while (true) {
        std::string value = readLine(prompt + ": ");
        if (value.empty()) return currentValue;
        if (looksLikePositiveDecimal(value)) {
            double price = std::atof(value.c_str());
            if (price > 0 && price <= 1000000000.0) return price;
        }
        std::cout << "  Invalid input: price must be a positive number (e.g. 25000 or 25000.50).\n";
    }
}

std::string readPhone(const std::string& prompt) {
    while (true) {
        std::string value = readLine(prompt + ": ");
        if (value.size() == 10 && isDigits(value)) return value;
        std::cout << "  Invalid input: phone number must be exactly 10 digits.\n";
    }
}

std::string readDate(const std::string& prompt) {
    while (true) {
        std::string value = readLine(prompt + " (dd/mm/yyyy): ");
        if (isValidDate(value)) return value;
        std::cout << "  Invalid input: enter a real calendar date in dd/mm/yyyy format.\n";
    }
}

bool readYesNo(const std::string& prompt) {
    while (true) {
        std::string value = toLower(readLine(prompt + " (y/n): "));
        if (value == "y" || value == "yes") return true;
        if (value == "n" || value == "no") return false;
        std::cout << "  Invalid input: please enter y or n.\n";
    }
}

}  // namespace Utils
