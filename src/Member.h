#ifndef MEMBER_H
#define MEMBER_H

#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

struct Member {
    int id;
    string name;
    double balance; // dương thì ngta nợ mình, âm thì mình nợ ng khác
    double debt;

    Member(int id, string name, double balance = 0.0, double debt = 0.0) {
        this->id = id;
        this->name = name;
        this->balance = balance;
        this->debt = debt;
    }


    void display() const {
        cout << "| ID: " << setw(3) << id 
             << " | Ten: " << setw(20) << left << name //ten ko qua 20 ki tu de can cho dep
             << " | So Tien Tra: " << setw(10) << right << fixed << setprecision(0) << balance << " VND"
             << " | So No: " << setw(10) << right << fixed << setprecision(0) << balance << " VND" << endl;
    }
};

#endif