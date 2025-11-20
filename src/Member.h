#ifndef MEMBER_H
#define MEMBER_H

#include <string>
#include <iostream>
#include <iomanip>
using namespace std;

struct Member
{
    string name;
    int id;
    double balance;

    Member(string name, int id, double balance) {
        this->name = name;
        this->id = id;
        this->balance = balance;
    }

    // Tương tự với hàm display, viết luôn code ở đây
    void display() {
        cout << fixed << setprecision(2);
        cout << "| ID: " << id << " | Ten: " << name
             << " | So Du: " << balance << " VND" << endl;
    }
};

#endif
