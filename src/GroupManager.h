#ifndef GROUP_MANAGER
#define GROUP_MANAGER
#include "Group.h"
#include "Member.h"
#include "Expense.h"


class GroupManager {
    private:
        int ex_id;
        int mem_id;
        Group group;
    public:
        GroupManager(Group &group);
        //GroupManager(string group_name);
        bool addMember();
        bool removeMember(string name);
        bool addExpense();
        

        //Helper function
        Member* containMember(string name);
        void printMember();
        void printExpense();

};

#endif