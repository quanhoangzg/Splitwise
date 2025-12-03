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

    Member(int id, string name, double balance = 0.0) {
        this->id = id;
        this->name = name;
        this->balance = balance;
    }


    void display() const {
        cout << "| ID: " << setw(3) << id 
             << " | Ten: " << setw(20) << left << name //ten ko qua 20 ki tu de can cho dep
             << " | So Du: " << setw(10) << right << fixed << setprecision(0) << balance << " VND" << endl;
    }
};

#endif