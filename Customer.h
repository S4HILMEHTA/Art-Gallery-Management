// Customer.h
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "Person.h"

class Customer : public Person {
private:
    std::string phone;

public:
    Customer();
    Customer(int customerId, const std::string& name, const std::string& phone);

    const std::string& getPhone() const;
    void setPhone(const std::string& newPhone);

    void display() const override;
    std::string serialize() const override;
    std::string typeName() const override;
    std::string searchText() const override;
    std::string role() const override;

    static void printHeader();
};

#endif
