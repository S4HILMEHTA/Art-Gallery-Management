// Customer.cpp
#include "Customer.h"

#include <iomanip>
#include <iostream>
#include <sstream>

Customer::Customer() : Person(), phone("") {}

Customer::Customer(int customerId, const std::string& name, const std::string& phone)
    : Person(customerId, name), phone(phone) {}

const std::string& Customer::getPhone() const { return phone; }

void Customer::setPhone(const std::string& newPhone) { phone = newPhone; }

void Customer::printHeader() {
    std::cout << std::left << std::setw(8) << "ID" << std::setw(32) << "Name" << "Phone\n"
              << std::string(60, '-') << "\n";
}

void Customer::display() const {
    std::cout << std::left << std::setw(8) << id << std::setw(32) << name << phone << "\n";
}

std::string Customer::serialize() const {
    std::ostringstream out;
    out << id << '|' << name << '|' << phone;
    return out.str();
}

std::string Customer::typeName() const { return "Customer"; }

std::string Customer::searchText() const { return name + " " + phone; }

std::string Customer::role() const { return "Customer"; }
