# 💰 Splitwise Mini (C++) - Ứng dụng Quản lý Chi tiêu Nhóm

> Một ứng dụng Console (CLI) giúp quản lý chi phí, chia tiền nhóm và theo dõi nợ nần một cách minh bạch, chính xác.

## 📖 Giới thiệu

**Splitwise Mini** là dự án xây dựng chương trình cho phép người dùng ghi nhận các khoản chi tiêu trong nhóm, tự động tính toán và chia phần tiền mà mỗi người đã trả hoặc còn nợ. Hệ thống giúp theo dõi tổng chi tiêu, số dư của từng thành viên và xuất báo cáo thanh toán cuối cùng.

Dự án được viết hoàn toàn bằng **C++** (Standard C++17), sử dụng kiến thức về lập trình hướng đối tượng (OOP), quản lý file và cấu trúc dữ liệu.

## 🚀 Chức năng chính

Dựa trên yêu cầu dự án và mã nguồn hiện tại, ứng dụng bao gồm các tính năng sau:

### 1. Quản lý Nhóm (Group Management)
* **Tạo nhóm mới:** Tạo thư mục riêng biệt cho từng nhóm chi tiêu (ví dụ: du lịch, ăn uống, nhà trọ).
* **Chọn nhóm làm việc:** Chuyển đổi linh hoạt giữa các nhóm dữ liệu khác nhau.
* **Xóa nhóm:** Xóa toàn bộ dữ liệu và thư mục của một nhóm khi không còn sử dụng.
* **Duyệt danh sách:** Hiển thị danh sách các nhóm hiện có trong hệ thống.

### 2. Quản lý Thành viên (Member Management)
* **Thêm thành viên:** Nhập tên thành viên mới vào nhóm.
* **Đổi tên thành viên:** Cập nhật tên hiển thị nếu nhập sai.
* **Xóa thành viên:** Chỉ cho phép xóa khi thành viên đó **không còn nợ**, **không còn số dư** và **không liên quan đến giao dịch trả tiền nào** (Logic an toàn dữ liệu).
* **Hiển thị trạng thái:** Xem danh sách ID, Tên, Số dư (Balance) và Nợ (Debt).

### 3. Quản lý Chi tiêu (Expense Tracking)
* **Ghi nhận chi tiêu (Add Expense):**
    * Nhập ID người trả tiền, số tiền và nội dung.
    * Hệ thống tự động chia đều (Split Equally) số tiền cho tất cả thành viên trong nhóm.
    * Tự động cấn trừ vào nợ cũ (nếu có) hoặc cập nhật số dư mới.
* **Trả tiền (Settlement):**
    * Ghi nhận một thành viên trả nợ cho nhóm.
    * Sử dụng thuật toán "Water Filling" (Rót nước) để phân phối tiền trả nợ cho những người đang có số dư dương (Creditors) một cách công bằng.

### 4. Báo cáo & Lưu trữ (Reporting & Persistence)
* **Xuất báo cáo (Report):**
    * Hiển thị chi tiết: Danh sách các khoản đã chi, Ai đang nợ ai, Ai cần thu tiền.
    * Xuất file `.txt` báo cáo vào thư mục `bills/` (Ví dụ: `bills/DuLich_bao_cao.txt`).
* **Lưu/Tải dữ liệu (Save/Load):**
    * Dữ liệu (Thành viên, Giao dịch) được lưu vào file `save.txt` trong thư mục của từng nhóm.
    * Tự động nạp lại dữ liệu khi mở lại nhóm.

# 🧮 Giải Thích Thuật Toán (Algorithms Explained)
Tài liệu này mô tả chi tiết hai thuật toán cốt lõi được sử dụng trong dự án Splitwise Mini: **Ghi nhận chi tiêu (Add Expense)** và **Trả nợ (Pay Money / Settlement)**.

---

## 1. Thuật toán Ghi nhận Chi tiêu (Add Expense)

Mục tiêu của thuật toán này là chia đều khoản chi cho tất cả thành viên và tự động cấn trừ vào các khoản nợ cũ (nếu có) để đảm bảo số dư luôn phản ánh đúng thực tế tài chính.

### 📝 Nguyên lý hoạt động
Thay vì lưu trữ từng giao dịch nợ lẻ tẻ (Ví dụ: A nợ B, A nợ C), hệ thống duy trì hai chỉ số cho mỗi thành viên:
* **Balance (Dương):** Số tiền thành viên đã chi giùm nhóm (Nhóm đang nợ thành viên này).
* **Debt (Dương):** Số tiền thành viên phải trả lại cho nhóm.

**Quy trình xử lý trong code:**
1.  **Tính thị phần (Share):** Lấy tổng số tiền chi (`amount`) chia cho tổng số thành viên trong nhóm.
2.  **Cộng tiền cho người trả (Payer):** Tăng `balance` của người trả tiền lên đúng bằng `amount`, vì họ vừa ứng tiền ra cho cả nhóm.
3.  **Trừ trách nhiệm của từng người:** Duyệt qua tất cả thành viên, trừ đi phần `Share` của họ vào `balance`.
    * Nếu `balance` trở thành số âm, hệ thống chuyển phần âm đó sang `debt` và đặt `balance` về 0.
4.  **Tự động cấn trừ (Netting):** Nếu một thành viên vừa có tiền trong `balance` (do các lần trả trước) vừa phát sinh `debt` mới, hệ thống sẽ tự động trừ hai khoản này cho nhau để tối giản số liệu.

### 💡 Ví dụ minh họa
**Tình huống:** Nhóm có 3 người: **A, B, C**. Ban đầu tất cả Balance = 0, Debt = 0.
**Sự kiện:** **A** trả **300k** cho bữa ăn.

1.  **Tính toán:** Mỗi người chịu: 300k / 3 = **100k**.
2.  **Xử lý A (Người trả):**
    * Balance của A tăng lên 300k (Nhóm nợ A).
    * Trừ trách nhiệm bản thân: A cũng phải chịu 100k → Balance cuối cùng của A: 300k - 100k = **+200k**.
3.  **Xử lý B (Thành viên):**
    * Balance ban đầu 0.
    * Trừ trách nhiệm: 0 - 100k = -100k.
    * Chuyển đổi: Balance = 0, **Debt = 100k**.
4.  **Xử lý C (Thành viên):**
    * Tương tự B → Balance = 0, **Debt = 100k**.

**Kết quả:** A (+200k), B (Nợ 100k), C (Nợ 100k). Tổng nợ khớp với tổng số dư.

---

## 2. Thuật toán Trả tiền (Pay Money - Water Filling Strategy)

Đây là thuật toán dùng để phân phối số tiền một người nợ (`debt`) trả vào quỹ chung cho những chủ nợ (`creditors`) đang chờ tiền.

### 📝 Nguyên lý hoạt động: "Water Filling" (Rót đầy từ thấp đến cao)
Thuật toán ưu tiên trả dứt điểm cho những chủ nợ có số dư (`balance`) **thấp nhất** trước. Cách này giúp giảm số lượng chủ nợ nhanh chóng, làm gọn danh sách tài chính.

**Quy trình xử lý trong code:**
1.  **Trừ nợ người trả:** Giảm `debt` của người trả tiền (`payer`).
2.  **Lập danh sách chủ nợ:** Tìm tất cả thành viên có `balance > 0`.
3.  **Sắp xếp (Sorting):** Sắp xếp danh sách chủ nợ theo thứ tự `balance` tăng dần (ít tiền đến nhiều tiền).
4.  **Vòng lặp phân phối:**
    * Tính `share` (mức chia lý thuyết) = Tổng tiền đang có / Số lượng chủ nợ còn lại.
    * So sánh `share` với số tiền mà chủ nợ nhỏ nhất (`smallestCreditor`) cần.
    * **Trường hợp 1 (Đủ tiền trả hết cho người nhỏ nhất):** Nếu `share >= balance` của người đó.
        * Trả đủ tiền cho người này (Balance về 0).
        * Trừ số tiền vừa trả khỏi quỹ.
        * Loại người này khỏi danh sách và tiếp tục chia cho những người còn lại.
    * **Trường hợp 2 (Không đủ trả hết):** Nếu `share < balance` của người nhỏ nhất (nghĩa là cũng nhỏ hơn tất cả những người khác vì đã sắp xếp).
        * Chia đều số tiền còn lại cho tất cả mọi người trong danh sách.
        * Kết thúc giao dịch.

### 💡 Ví dụ minh họa
**Tình huống:**
* **X** đang nợ **100k**.
* Các chủ nợ đang chờ tiền: **Y** (cần thu 20k), **Z** (cần thu 80k).
* **Sự kiện:** **X** trả **100k** vào nhóm.

**Các bước chạy:**
1.  **Chuẩn bị:** X hết nợ. Tiền quỹ có **100k**. Danh sách chủ nợ sắp xếp: `[Y(20k), Z(80k)]`.
2.  **Vòng lặp 1:**
    * Số chủ nợ: 2.
    * Mức chia đều (`share`): 100k / 2 = **50k**.
    * Xét **Y** (20k): Thấy `50k > 20k` (Đủ tiền trả dứt điểm).
    * -> **Trả cho Y 20k**. Y xong nhiệm vụ.
    * Tiền còn lại: 100k - 20k = **80k**. Danh sách còn: `[Z]`.
3.  **Vòng lặp 2:**
    * Số chủ nợ: 1.
    * Mức chia đều (`share`): 80k / 1 = **80k**.
    * Xét **Z** (80k): Thấy `80k == 80k` (Đủ tiền trả dứt điểm).
    * -> **Trả cho Z 80k**. Z xong nhiệm vụ.
    * Tiền còn lại: 0.

**Tại sao thuật toán này tối ưu?**
Nếu chia đều ngay từ đầu (mỗi người 50k):
* Y cần 20k mà nhận 50k -> **Dư 30k** (Sai logic).
* Z cần 80k mà nhận 50k -> **Thiếu 30k**.
-> Cách tiếp cận "Water Filling" giải quyết triệt để vấn đề phân phối bất đối xứng này.

## 🛠️ Cài đặt & Hướng dẫn sử dụng

### Yêu cầu hệ thống
* Trình biên dịch C++ hỗ trợ chuẩn **C++17** trở lên (do sử dụng thư viện `<filesystem>`).
* Hệ điều hành: Windows, Linux, hoặc macOS.

### Cách biên dịch (Compile)

Sử dụng `g++`:

```bash khi ở folder src
g++ main.cpp -o Splitwise.exe

