#include <iostream>
#include <string>
#include "Group.h"
#include <filesystem>
using namespace std;

void traverseGroup();
void createGroup();
void deleteGroup();
string findGroup();

// Hàm xóa màn hình (cho đẹp he he )
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int main() {
    // Tạo một nhóm duy nhất để demo
    Group *myGroup = new Group("temp");

    // Thêm dữ liệu mẫu (để test nhanh)
    // myGroup.addMember("Tuan");
    // myGroup.addMember("Hung");
    // myGroup.addMember("Lan");

    int groupChoice;
    

    while (true) {
        cout << "\n==========================================\n";
        cout << "SPLITWISE MINI (C++) - Chon hoac tao nhom\n";
        cout << "==========================================\n";
        cout << "1. Xem danh sach nhom\n";
        cout << "2. Them nhom\n";
        cout << "3. Xoa nhom\n";
        cout << "4. Chon nhom\n";
        cout << "0. Thoat\n";
        cout << "Nhap lua chon cua ban: ";
        cin >> groupChoice;
        if (groupChoice == 0) return 0;
        cin.ignore(1, '\n');
        string myGroupName;
        bool to_group = false;
        switch (groupChoice)
        {
            case 0:
                cout << "Cam on vi da dung chuong trinh!" << endl;
                return 0;
            case 1:
                traverseGroup();
                break;
            case 2:
                createGroup();
                break;
            case 3:
                deleteGroup();
                break;
            case 4:
                myGroupName = findGroup();
                if (myGroupName != "") {
                    myGroup = new Group(myGroupName);
                    to_group = true;
                }
                break;
            default:
                cout << "Lua chon khong hop le!" << endl;
                break;
        }
        cout << "\nNhan Enter de tiep tuc...";
        cin.ignore();
        // cin.get();
        clearScreen();
        if (to_group) {
            break;
        }
    }

    myGroup->loadData();
    int choice;
    while (true) {
        cout << "\n====================================\n";
        cout << "   SPLITWISE MINI (C++) - " << myGroup->group_name << "\n";
        cout << "====================================\n";
        cout << "1. Xem danh sach thanh vien & So du\n";
        cout << "2. Them thanh vien moi\n";
        cout << "3. Ghi nhan chi tieu (Add Expense)\n";
        cout << "4. Xem bao cao: Ai no ai? (Settlement)\n";
        cout << "5. Luu du lieu ra file\n";
        cout << "6. Tra tien\n";
        cout << "7. Doi ten thanh vien\n";
        cout << "8. Xoa thanh vien\n";
        cout << "0. Thoat\n";
        cout << "Nhap lua chon cua ban: ";
        cin >> choice;
        cin.ignore(1, '\n');
        if (choice == 0) break;

        switch (choice) {
            case 1:
                myGroup->displayMembers();
                break;
            case 2: {
                string name;
                cout << "Nhap ten thanh vien: ";
                // cin.ignore(); 
                getline(cin, name);
                myGroup->addMember(name);
                break;
            }
            case 3: {
                int payerId;
                double amount;
                string desc;//description

                myGroup->displayMembers();
                while (true) {
                    cout << "Nhap ID nguoi tra tien: ";
                    if (cin >> payerId && payerId > 0) { 
                        if (myGroup->memberExists(payerId)) break; 
                        else cout << "Loi: ID khong ton tai trong nhom!" << endl;
                    } else {
                        cout << "Loi: ID phai la mot so nguyen duong!" << endl;
                        cin.clear(); // Reset error flags
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                    }
                }

                while (true) {
                    cout << "Nhap so tien: ";
                    if (cin >> amount && amount > 0) {
                        break;
                    } else {
                        cout << "Loi: So tien phai la so duong!" << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                }
                cout << "Nhap noi dung chi tieu: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Robust ignore
                getline(cin, desc);

                myGroup->addExpense(desc, amount, payerId);
                break;
            }
            case 4:
                myGroup->showSettlement();
                break;
            case 5:
                // Nhớ tạo folder "data" ngang hàng với folder "src" trước khi chọn cái này
                myGroup->saveData(); 
                break;
            case 6:
                myGroup->displayMembers();
                int id;
                double money;

                while (true) {
                    cout << "Nhap id cua nguoi tra tien: ";
                    if (cin >> id) {
                        if (myGroup->memberExists(id)) {
                            break;
                        } else {
                            cout << "Loi: Thanh vien ko ton tai!\n";
                        }
                    } else {
                        cout << "Loi: ID phai la mot so! " << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                }

                while (true) {
                    cout << "Nhap so tien phai tra: ";
                    if (cin >> money && money >= 0) {
                        break; 
                    } else {
                        cout << "Loi: So tien khong hop le (phai la so va >= 0)!" << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                myGroup->payMoney(id, money);
                break;
            case 7: {
                myGroup->displayMembers();
                int personID;
                while (true) {
                    cout << "Nhap id cua nguoi muon doi ten: ";
                    if (cin >> personID) {
                        if (myGroup->memberExists(personID)) {
                            break;
                        } else {
                            cout << "Loi: Thanh vien ko ton tai!\n";
                        }
                    } else {
                        cout << "Loi: ID phai la mot so! " << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                }

                
                cout << "Nhap ten muon doi: ";
                cin.ignore();
                string changeName;
                getline(cin, changeName);
                myGroup->changeMemberName(personID, changeName);
                break;
            }
            default:
                cout << "Lua chon khong hop le!\n";
        }
        cout << "\nNhan Enter de tiep tuc...";
        cin.ignore();
        // cin.get();
        clearScreen();
    }

    return 0;
}

void traverseGroup() {
    string rootPath = "data/"; // Thư mục hiện tại 

    cout << "Danh sach cac nhom trong " << rootPath << endl;
    cout << "-----------------------------------" << endl;

    try {
        //directory_iterator để duyệt qua tất cả các mục
        for (const auto& entry : filesystem::directory_iterator(rootPath)) {
            
            //dùng is_directory(entry.status()) để tránh lỗi nếu symbolic link bị hỏng
            if (entry.is_directory()) {
                
                //lấy Tên của Thư mục con
                string folderName = entry.path().filename().string();
                
                cout << folderName << endl;
            }
        }
    } catch (const filesystem::filesystem_error& e) {
        cerr << "Loi tim thu muc " << e.what() << endl;
    }
}

void createGroup() {
    cout << "Nhap ten nhom moi: ";
    string folderName;
    getline(cin, folderName);
    string relativePath = "data/" + folderName;
    try {
        if (filesystem::create_directory(relativePath)) {
            cout << "Tao nhom thanh cong: " << folderName << endl;
        } else {
            cout << "Nhom da ton tai: " << folderName << endl;
        }

    } catch (const filesystem::filesystem_error& e) {
        cerr << "loi khi tao nhom: " << e.what() << endl;
    }
}

void deleteGroup() {
    cout << "Chon thu muc can xoa: ";
    string targetPath;
    getline(cin, targetPath);
    string relativePath = "data/" + targetPath;
    try {
        // Sử dụng remove_all để xóa thư mục và file lưu bên trong
        uintmax_t count = filesystem::remove_all(relativePath);
        
        if (count > 0) {
            cout << "Xoa nhom thanh cong!" << endl;
            cout << "Tong cong " << count << " mu da bi xoa." << endl;
        } else {
            cout << "Thu muc ko ton tai." << endl;
        }

    } catch (const filesystem::filesystem_error& e) {
        cerr << "loi khi xoa: " << e.what() << endl;
    }

}

string findGroup() {
    cout << "Dien ten nhom ban muon tim: ";
    string path;
    getline(cin, path);
    string relativePath = "data/" + path;
    if (filesystem::exists(relativePath) && filesystem::is_directory(relativePath)) {
        return path;
    } else {
        cout << "Khong co ton tai nhom " << path << endl;
    }
    return "";
}