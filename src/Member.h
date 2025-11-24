#ifndef MEMBER_H
#define MEMBER_H

#include <string>
#include <iostream>
#include <iomanip>
#include <unordered_map>
using namespace std;

struct Member
{
    string name;
    int id;
    double balance;
    //unordered_map<int, double> owes; //store Id of the ones this member owes money

    Member(string name, int id, double balance) {
        this->name = name;
        this->id = id;
        this->balance = balance;
    }

    // Tương tự với hàm display, viết luôn code ở đây
    void display() {
        cout << fixed << setprecision(2);
        cout << "| ID: " << id << " | Name: " << name
             << " | Balance: " << balance << " VND" << endl;
    }
};

#endif
