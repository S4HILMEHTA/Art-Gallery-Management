// Sale.h
// Supporting class: one record per artwork sale (the "main transaction").
#ifndef SALE_H
#define SALE_H

#include "Entity.h"

class Sale : public Entity {
private:
    int artworkId;
    int customerId;
    double amount;
    std::string date;       // dd/mm/yyyy
    std::string status;     // "Completed" or "Cancelled"

public:
    Sale();
    Sale(int saleId, int artworkId, int customerId, double amount, const std::string& date,
         const std::string& status = "Completed");

    int getArtworkId() const;
    int getCustomerId() const;
    double getAmount() const;
    const std::string& getDate() const;
    const std::string& getStatus() const;
    bool isCompleted() const;
    void cancel();

    void display() const override;
    std::string serialize() const override;
    std::string typeName() const override;
    std::string searchText() const override;

    static void printHeader();
};

#endif
