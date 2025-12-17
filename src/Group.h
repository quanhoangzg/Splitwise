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

    //Check thanh vien
    bool memberExists(int id) {
        for (const auto& mem : members) {
            if (mem.id == id) return true;
        }
        return false;
    }

    // Thêm thành viên
    void addMember(string memberName) {
        int newId = members.size() + 1;
        // Khởi tạo balance = 0, debt = 0
        members.push_back(Member(newId, memberName, 0.0, 0.0));
        cout << "Da them thanh vien: " << memberName << endl;
    }

    // Thêm expense => chia tiền và tự động cấn trừ nợ cũ
    void addExpense(string desc, double amount, int payerId) {
        if (members.empty()) return;

        int expenseId = expenses.size() + 1;
        expenses.push_back(Expense(expenseId, desc, payerId, amount));

        double splitAmount = amount / members.size();

        for (auto &mem : members) {
            // 1. Cộng tiền cho người trả
            if (mem.id == payerId) {
                mem.balance += amount; 
            }

            // 2. Trừ phần trách nhiệm của mỗi người
            if (mem.balance >= splitAmount) {
                mem.balance -= splitAmount;
            } else {
                double remaining = splitAmount - mem.balance;
                mem.balance = 0;
                mem.debt += remaining;
            }

            
            if (mem.balance > 0 && mem.debt > 0) {
                // Khi có đủ tiền trả nợ
                if (mem.balance >= mem.debt) {
                    mem.balance -= mem.debt; 
                    mem.debt = 0;            
                } else {
                    // Có tiền nhưng chưa đủ trả hết nợ cũ
                    mem.debt -= mem.balance; 
                    mem.balance = 0;         
                }
            }
        }
        cout << "Da ghi nhan: " << desc << " | Moi nguoi chiu: " << (long)splitAmount << endl;
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

        while (actualPay > 0.001 && !creditors.empty()) { // > 0.001 để tránh lỗi làm tròn số thực
            double share = actualPay / creditors.size(); // Mức chia đều theo lý thuyết
            
            // Lấy người cần thu ít nhất hiện tại
            Member* smallestCreditor = creditors.front(); 

            if (share >= smallestCreditor->balance) {
                // Case 1: người ít nhất bị trả dư tiền
                double paidAmount = smallestCreditor->balance;
                
                smallestCreditor->balance = 0; // nhận đủ
                actualPay -= paidAmount;       // Trừ tiền trong quỹ
                
                cout << "   -> Tra het cho " << smallestCreditor->name 
                     << ": " << (long)paidAmount << " VND (Xong)\n";

                // Loại người này ra để chia cho ng khác
                creditors.erase(creditors.begin());
            } 
            else {
                // Case 2: Ko bị vượt thì chia sạch
                for (auto* cred : creditors) {
                    cred->balance -= share;
                    cout << "   -> Tra mot phan cho " << cred->name 
                         << ": " << (long)share << " VND (Con du " << (long)cred->balance << ")\n";
                }
                actualPay = 0; // Đã tiêu hết tiền => phá loop
            }
        }
        cout << "Da phan phoi xong tien tra no.\n";
    }


    string findPayer(Expense &expense) {
        for (auto &mem : members) {
            if (mem.id == expense.paid_by_member_id) {
                return mem.name;
            }
        }
        return "";
    }

    // Xuất và báo cáo ra file
    void showSettlement() {
        string fileName = "bills/" + group_name + "_bao_cao.txt";
        ofstream file(fileName);
        
        bool isFileOpen = file.is_open();
        if (!isFileOpen) {
            cout << "[Canh bao] Khong tim thay thu muc 'bills' hoac khong the tao file!\n";
            cout << "Chi hien thi tren man hinh console.\n";
        }

        // 2. Bắt đầu in và ghi file
        // Để tránh viết code lặp lại, mình dùng 1 biến stringstream (cần #include <sstream>)
        // HOẶC cách đơn giản nhất cho bạn dễ hiểu là viết 2 dòng song song
        
        cout << "\n================ TRANG THAI TAI CHINH ================\n";
        if (isFileOpen) file << "================ TRANG THAI TAI CHINH ================\n";

        // In và viết vào file báo cáo danh sách expense
        cout << "[ DANH SACH EXPENSE ]\n";
        if (isFileOpen) file << "[ DANH SACH EXPENSE ]\n";
        for (Expense expense : expenses) {
            cout << "  (&)" << setw(15) << left << expense.description << " Nguoi tra: "
                 << setw(15) << right << findPayer(expense) << "| So tien:"
                 << setw(12) << right << (long) expense.amount << "VND\n";
            
            if (isFileOpen) {
                file << "  (&)" << setw(15) << left << expense.description << " Nguoi tra: "
                 << setw(15) << right << findPayer(expense) << "| So tien:"
                 << setw(12) << right << (long) expense.amount << "VND\n";
            }
            
        }
        cout << '\n';
        file << '\n';

        bool allClear = true;

        // Danh sách người nợ
        cout << "[ DANH SACH NGUOI NO ]\n";
        if (isFileOpen) file << "[ DANH SACH NGUOI NO ]\n";

        for (const auto &mem : members) {
            if (mem.debt > 0) {
                // In ra màn hình
                cout << "  (!) " << setw(15) << left << mem.name << " no: " 
                     << setw(10) << right << (long)mem.debt << " VND\n";
                
                // Ghi vào file
                if (isFileOpen) {
                    file << "  (!) " << setw(15) << left << mem.name << " no: " 
                         << setw(10) << right << (long)mem.debt << " VND\n";
                }
                allClear = false;
            }
        }

        // Danh sách của chủ nợ
        cout << "\n[ DANH SACH CHU NO ]\n";
        if (isFileOpen) file << "\n[ DANH SACH CHU NO ]\n";

        for (const auto &mem : members) {
            if (mem.balance > 0) {
                cout << "  ($) " << setw(15) << left << mem.name << " cho thu: " 
                     << setw(10) << right << (long)mem.balance << " VND\n";
                
                if (isFileOpen) {
                    file << "  ($) " << setw(15) << left << mem.name << " cho thu: " 
                         << setw(10) << right << (long)mem.balance << " VND\n";
                }
                allClear = false;
            }
        }

        // --- TỔNG KẾT ---
        if (allClear) {
            string msg = "\n>> TUYET VOI! Tat ca da thanh toan xong. Khong ai no ai.\n";
            cout << msg;
            if (isFileOpen) file << msg;
        }

        string footer = "======================================================\n";
        cout << footer;
        if (isFileOpen) {
            file << footer;
            cout << "\n(Da xuat bao cao thanh cong vao file: " << fileName << ")\n";
            file.close();
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
                 << " | " << setw(16) << right << fixed << setprecision(0) << mem.balance << " | " 
                 << setw(13) << right << fixed << setprecision(0) << mem.debt << "   |" << endl;
        }
    }

    //Save data 
    void saveData() {
        /*
            n m (số n sẽ là số expense, m sẽ là số member)
            expense
            members
        
        */

        string groupFileName = "data/" + group_name + "/save.txt";
        ofstream file(groupFileName);
        if (file.is_open()) {
            file << expenses.size() << endl;
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