#ifndef GROUP_H
#define GROUP_H

#include <vector>
#include <string>
#include "Member.h"
#include "Expense.h"
using namespace std;

struct Group
{
    int group_id;
    string group_name;
    vector<Member> members;
    vector<Expense> expenses;
    
    Group(int group_id, string group_name) {
        this->group_id = group_id;
        this->group_name = group_name;
    }
};

#endif