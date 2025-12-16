#ifndef GROUP_H
#define GROUP_H

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <cmath>
#include "Member.h"
#include "Expense.h"

using namespace std;

class Group {
public:
    string group_name;
    vector<Member> members;
    vector<Expense> expenses;

    Group(string group_name) {
        this->group_name = group_name;
    }

    // Thêm thành viên
    void addMember(string memberName) {
        int newId = members.size() + 1;
        // Khởi tạo balance = 0, debt = 0
        members.push_back(Member(newId, memberName, 0.0, 0.0));
        cout << "Da them thanh vien: " << memberName << endl;
    }

    // Thêm expense => chia tiền
    void addExpense(string desc, double amount, int payerId) {
        if (members.empty()) return; //Nếu ko có thì return ko có j xảy ra

        int expenseId = expenses.size() + 1; //Thêm expense
        expenses.push_back(Expense(expenseId, desc, amount, payerId));

        // Logic chia tiền đều (Split Equally)
        double splitAmount = amount / members.size();

        for (auto &mem : members) {
            if (mem.id == payerId) {
                // Người trả tiền: Được cộng số tiền đã trả, trừ đi phần của chính họ
                mem.balance += (amount - splitAmount);
            } else {
                // Người hưởng thụ: Bị trừ đi phần tiền phải đóng
                mem.balance -= splitAmount;
            }
        }
        cout << "Da ghi nhan khoan chi: " << desc << endl;
    }

    // Chia đều kiểu Water Filling
    void payMoney(int memberId, double money) {
        // Người trả tiền
        Member* payer = nullptr;
        for (auto &mem : members) {
            if (mem.id == memberId) {
                payer = &mem;
                break;
            }
        }

        if (!payer) { cout << "Khong tim thay ID thanh vien nay!\n"; return; }
        if (payer->debt <= 0) { cout << payer->name << " khong co no de tra!\n"; return; }

        double actualPay = (money > payer->debt) ? payer->debt : money;
        payer->debt -= actualPay;

        cout << ">> " << payer->name << " da tra: " << (long)actualPay << " VND.\n";

        // Tìm chủ nợ
        vector<Member*> creditors;
        for (auto &mem : members) {
            if (mem.balance > 0) {
                creditors.push_back(&mem);
            }
        }

        if (creditors.empty()) {
            cout << "Loi: Tien duoc tra nhung khong co ai nhan (Balance he thong sai)!\n";
            return;
        }

        // Thuật toán chia đều 
        sort(creditors.begin(), creditors.end(), [](Member* a, Member* b) {
            return a->balance < b->balance;
        });

        // Vòng lặp phân phối tiền
        while (actualPay > 0.001 && !creditors.empty()) { // > 0.001 để tránh lỗi làm tròn số thực
            double share = actualPay / creditors.size(); // Mức chia đều lý thuyết
            
            // Lấy người cần thu ít nhất hiện tại
            Member* smallestCreditor = creditors.front(); 

            if (share >= smallestCreditor->balance) {
                // TRƯỜNG HỢP 1: Nếu chia đều thì ông nhỏ nhất bị dư tiền
                // -> Chỉ trả đủ số ổng cần thôi
                double paidAmount = smallestCreditor->balance;
                
                smallestCreditor->balance = 0; // Ông này đã nhận đủ
                actualPay -= paidAmount;       // Trừ tiền trong quỹ
                
                cout << "   -> Tra het cho " << smallestCreditor->name 
                     << ": " << (long)paidAmount << " VND (Xong)\n";

                // Loại ông này ra khỏi danh sách nhận tiền, để tiền thừa chia cho người khác
                creditors.erase(creditors.begin());
            } 
            else {
                // TRƯỜNG HỢP 2: Tiền ít quá, chia đều không ai bị dư cả
                // -> Chia hết sạch tiền luôn cho mọi người còn lại
                for (auto* cred : creditors) {
                    cred->balance -= share;
                    cout << "   -> Tra mot phan cho " << cred->name 
                         << ": " << (long)share << " VND (Con du " << (long)cred->balance << ")\n";
                }
                actualPay = 0; // Đã tiêu hết tiền
            }
        }
        cout << "Da phan phoi xong tien tra no.\n";
    }

    // Hiển thị báo cáo "Ai nợ ai" (Thuật toán Greedy)
    void showSettlement() {
        cout << "\n--- BAO CAO THANH TOAN (AI NO AI) ---\n";
        
        // Debtors và chủ nợ Creditors
        struct SettlementEntry { int group_id; string group_name; double amount; };
        vector<SettlementEntry> debtors;
        vector<SettlementEntry> creditors;

        for (const auto &mem : members) {
            if (mem.balance < -1.0) debtors.push_back({mem.id, mem.name, mem.balance}); // < 0
            else if (mem.balance > 1.0) creditors.push_back({mem.id, mem.name, mem.balance}); // > 0
        }

        // Logic Greedy: Người nợ nhiều trả cho người cho vay nhiều trước
        // Sắp xếp theo giá trị tuyệt đối giảm dần (cần include <algorithm>)

        int i = 0; // index cho debtors
        int j = 0; // index cho creditors

        while (i < debtors.size() && j < creditors.size()) {
            double debt = abs(debtors[i].amount);
            double credit = creditors[j].amount;
            double amountToSettle = 0;

            if (debt < credit) {
                amountToSettle = debt;
                creditors[j].amount -= debt; // Chủ nợ vẫn còn được nhận thêm
                debtors[i].amount = 0;
                i++; // Xử lý xong con nợ này
            } else {
                amountToSettle = credit;
                debtors[i].amount += credit; // Con nợ vẫn còn nợ thêm
                creditors[j].amount = 0;
                j++; // Xử lý xong chủ nợ này
            }

            cout << debtors[i].group_name << " tra cho " << creditors[j].group_name 
                 << ": " << fixed << setprecision(0) << amountToSettle << " VND" << endl;
        }
        
        if (debtors.empty() && creditors.empty()) {
            cout << "Tat ca da thanh toan xong! Oach xa lach vo cung, absolute cinema\n";
        }
    }

    
    void displayMembers() {
        cout << "\n--- CHI TIET THANH VIEN ---\n";
        // Header
        cout << "| ID  | Ten                  | Balance (Cho Thu)| Debt (Phai Tra) |\n";
        cout << "|-----|----------------------|------------------|-----------------|\n";
        for (const auto &mem : members) {
            cout << "| " << setw(3) << left << mem.id 
                 << " | " << setw(20) << left << mem.name 
                 << " | " << setw(13) << right << fixed << setprecision(0) << mem.balance << " | " 
                 << setw(13) << right << fixed << setprecision(0) << mem.debt << "   |" << endl;
        }
    }

    //Save data lưu mấy cái member bắt chước file csv
    void saveData() {
        ofstream file("data/splitwise_data.txt");
        if (file.is_open()) {
            file << members.size() << endl;
            for (const auto &mem : members) {
                file << mem.id << "|" << mem.name << "|" << mem.balance << endl;
            }
            cout << "Da luu du lieu thanh cong!\n";
            file.close();
        } else {
            cout << "Loi: Khong the mo file de luu!\n";
        }
    }
};

#endif