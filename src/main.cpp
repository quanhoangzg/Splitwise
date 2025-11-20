#include <iostream>
#include "Member.h"
#include "Expense.h"
#include "Group.h"
using namespace std;
int main() {
    Group group(1, "Test 1");
    
    //member1
    Member Quan("Quan", 1, 100);

    //member2
    Member Quang("Quang", 3, 400);

    //EXPENSE
    Expense ex(1, "Dinner", 1, 50);

    group.members.push_back(Quan);
    group.members.push_back(Quang);
    group.expenses.push_back(ex);

    //Print members
    cout << "---------Print members:----------" << endl;
    for (int i = 0; i < group.members.size(); i++) {
        group.members[i].display();
    }

    //Print expenses
    cout << "---------Print expenses:----------" << endl;
    for (int i = 0; i < group.expenses.size(); i++) {
        group.expenses[i].display();
    }


    return 0;
}