#ifndef EXPENSE_H
#define EXPENSE_H

#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

struct Expense
{
    int expense_id;
    string description;
    int paid_by_member_id;
    double amount;

    Expense (int expense_id, string description, int paid_by_member_id, double amount) {
        this->expense_id = expense_id;
        this->description = description;
        this->paid_by_member_id = paid_by_member_id;
        this->amount = amount;
    }

    void display() const {
        cout << "  - [ID: " << expense_id << "] " << description 
             << " (" << fixed << setprecision(0) << amount << " VND) "
             << "duoc tra boi ID: " << paid_by_member_id << endl;
    }
};

#endif