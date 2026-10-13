// Utils.h
// Helper functions for string handling, validation and safe console input.
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

namespace Utils {

// ---------- String helpers ----------
std::string trim(const std::string& s);
std::string toLower(const std::string& s);
bool containsIgnoreCase(const std::string& text, const std::string& term);
bool isDigits(const std::string& s);
std::vector<std::string> split(const std::string& s, char delimiter);
std::string formatMoney(double amount);   // e.g. "Rs. 25000.00"
std::string todayDate();                  // system date as dd/mm/yyyy

// ---------- Validation / parsing ----------
bool isValidDate(const std::string& date);       // dd/mm/yyyy with leap-year check
int parseInt(const std::string& s);              // throws GalleryException if invalid
double parseDouble(const std::string& s);        // throws GalleryException if invalid

// ---------- Safe console input (re-prompts until the value is valid) ----------
std::string readLine(const std::string& prompt);
std::string readText(const std::string& label, size_t maxLength);
std::string readOptionalText(const std::string& label, size_t maxLength);
int readInt(const std::string& prompt, int minValue, int maxValue);
double readPrice(const std::string& prompt);
double readOptionalPrice(const std::string& prompt, double currentValue);
std::string readPhone(const std::string& prompt);
std::string readDate(const std::string& prompt);
bool readYesNo(const std::string& prompt);

}  // namespace Utils

#endif
