// Sale.cpp
#include "Sale.h"
#include "Utils.h"

#include <iomanip>
#include <iostream>
#include <sstream>

Sale::Sale() : Entity(), artworkId(0), customerId(0), amount(0.0), date(""), status("Completed") {}

Sale::Sale(int saleId, int artworkId, int customerId, double amount, const std::string& date,
           const std::string& status)
    : Entity(saleId),
      artworkId(artworkId),
      customerId(customerId),
      amount(amount),
      date(date),
      status(status) {}

int Sale::getArtworkId() const { return artworkId; }
int Sale::getCustomerId() const { return customerId; }
double Sale::getAmount() const { return amount; }
const std::string& Sale::getDate() const { return date; }
const std::string& Sale::getStatus() const { return status; }
bool Sale::isCompleted() const { return status == "Completed"; }
void Sale::cancel() { status = "Cancelled"; }

void Sale::printHeader() {
    std::cout << std::left << std::setw(8) << "SaleID" << std::setw(10) << "ArtworkID"
              << std::setw(12) << "CustomerID" << std::setw(18) << "Amount" << std::setw(13)
              << "Date" << "Status\n"
              << std::string(70, '-') << "\n";
}

void Sale::display() const {
    std::cout << std::left << std::setw(8) << id << std::setw(10) << artworkId << std::setw(12)
              << customerId << std::setw(18) << Utils::formatMoney(amount) << std::setw(13) << date
              << status << "\n";
}

std::string Sale::serialize() const {
    std::ostringstream out;
    out << id << '|' << artworkId << '|' << customerId << '|' << std::fixed << std::setprecision(2)
        << amount << '|' << date << '|' << status;
    return out.str();
}

std::string Sale::typeName() const { return "Sale"; }

std::string Sale::searchText() const { return date + " " + status; }
