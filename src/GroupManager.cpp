#include "GroupManager.h"
#include "Group.h"
#include "Member.h"
#include "Expense.h"
#include <iostream>
#include <limits>
#include <algorithm>
using namespace std;

//Helper Fuctions
Member* GroupManager::containMember(string name) {
    for (Member &member : group.members) {
        if (name == member.name) {
            return &member;
        }
    }
    return nullptr;
}


double inputdouble() {
    double num;
    while (!(cin >> num)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input" << endl;
    }
    cin.ignore();
    return num;
}


//Main methods
GroupManager::GroupManager(Group &group) : group(group) {
    this->ex_id = 1;
    this->mem_id = 1;
}

bool GroupManager::addExpense() {
    cout << "Enter description (or name) of the expense: ";
    string description;
    getline(cin, description);
    cout << "Amount (this will be divided equally): ";
    double amount = inputdouble();
    cout << "Who paid it?: ";
    string name;
    getline(cin, name);

    Member *member = containMember(name);
    if (member != nullptr) {
        group.expenses.push_back(Expense(ex_id, description, member->id, amount));
        cout << "----------Add Expense Successful!----------" << endl;
        ex_id++;
        return true;
    } else {
        cout << "------------Add Expense Failed!------------" << endl;
        return false;
    }
    

}

bool GroupManager::addMember() {
    // cin.ignore(1, '\n');
    cout << "Enter member's name: ";
    string name;
    getline(cin, name);
    cout << "Enter member's balance: ";
    double balance = inputdouble();

    group.members.push_back(Member(name, mem_id, balance));
    mem_id++;
    return true;
}

bool GroupManager::removeMember(string name) {
    Member *member = containMember(name);

    auto it = remove_if(group.members.begin(), group.members.end(), 
        [&name](const Member& m) {
            return m.name == name;
        });

    if (it != group.members.end()) {
        group.members.erase(it, group.members.end());
        return true;
    }
    return false;
}

void GroupManager::printMember() {
    cout << "----------Print members:----------" << endl;
    for (int i = 0; i < group.members.size(); i++) {
        group.members[i].display();
    }
}

void GroupManager::printExpense() {
    cout << "---------Print expenses:----------" << endl;
    for (int i = 0; i < group.expenses.size(); i++) {
        group.expenses[i].display();
    }
}