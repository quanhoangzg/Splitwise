#ifndef EXPENSE_H
#define EXPENSE_H

#include <string>
#include <iostream>
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

    void display() {
        cout << fixed << setprecision(2);
        cout << "| ID: " << expense_id << " | Mo ta: " << description
             << " | So tien: " << amount << " VND"
             << " | Nguoi tra: " << paid_by_member_id << endl;
    }
};

#endif