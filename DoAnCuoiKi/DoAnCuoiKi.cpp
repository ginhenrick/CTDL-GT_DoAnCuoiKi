// DoAnCuoiKi.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <algorithm>
#include <limits> // Để sử dụng numeric_limits cho việc nhập liệu
#include <iomanip> // Thêm thư viện iomanip để định dạng output
#include <clocale> // Thêm thư viện clocale
#include <random> // Thêm thư viện random để tạo số ngẫu nhiên
#include <vector> // Thêm thư viện vector


using namespace std;

// === CẤU TRÚC DỮ LIỆU ===

// Cấu trúc dữ liệu cho sách
struct Sach {
    string maSach;
    string tenSach;
    string tacGia;
    string theLoai;
    int namXuatBan;
    int soLuong;
    double giaTien;

    Sach* next; // Con trỏ trỏ đến sách tiếp theo trong danh sách
};

// Cấu trúc dữ liệu cho khách hàng (tương tự như sách)
struct KhachHang {
    string maKH;
    string tenKH;
    string diaChi;
    string soDienThoai;

    KhachHang* next;
};

// === Cấu trúc TreeNode cho Binary Search Tree ===
struct TreeNode {
    Sach data;
    TreeNode* left;
    TreeNode* right;
};

// === Hàm Hỗ Trợ ===

// Hàm xóa bộ nhớ đệm (buffer) - giúp việc nhập liệu từ bàn phím hoạt động đúng
void clearCin() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// === QUẢN LÝ DANH SÁCH LIÊN KẾT (LINKED LIST) ===

// Hàm khởi tạo node mới cho danh sách liên kết
Sach* newNode(string maSach, string tenSach, string tacGia, string theLoai, int namXuatBan, int soLuong, double giaTien) {
    // Cấp phát động bộ nhớ cho node mới
    Sach* sachMoi = new Sach;

    // Gán giá trị cho các trường dữ liệu của node
    sachMoi->maSach = maSach;
    sachMoi->tenSach = tenSach;
    sachMoi->tacGia = tacGia;
    sachMoi->theLoai = theLoai;
    sachMoi->namXuatBan = namXuatBan;
    sachMoi->soLuong = soLuong;
    sachMoi->giaTien = giaTien;

    // Khởi tạo con trỏ 'next' trỏ đến null, vì đây là node cuối cùng (tạm thời)
    sachMoi->next = nullptr;

    return sachMoi; // Trả về node mới đã được khởi tạo
}

// Hàm kiểm tra danh sách rỗng
bool isEmpty(Sach* head) {
    return head == nullptr; // Danh sách rỗng nếu con trỏ đầu danh sách là null
}

// === HÀM HỖ TRỢ CHO BINARY SEARCH TREE ===
TreeNode* insertBST(TreeNode* root, Sach data) {
    if (root == nullptr) {
        return new TreeNode{ data, nullptr, nullptr };
    }

    if (data.maSach < root->data.maSach) {
        root->left = insertBST(root->left, data);
    }
    else {
        root->right = insertBST(root->right, data);
    }
    return root;
}

TreeNode* searchBST(TreeNode* root, string keyword, bool timTheoMaSach = false) {
    if (root == nullptr) {
        return nullptr;
    }

    // Chuyển đổi thông tin sách và keyword thành chữ thường để so sánh không phân biệt hoa thường
    string maSachLowerCase = root->data.maSach;
    transform(maSachLowerCase.begin(), maSachLowerCase.end(), maSachLowerCase.begin(), ::tolower);

    string tenSachLowerCase = root->data.tenSach;
    transform(tenSachLowerCase.begin(), tenSachLowerCase.end(), tenSachLowerCase.begin(), ::tolower);

    string tacGiaLowerCase = root->data.tacGia;
    transform(tacGiaLowerCase.begin(), tacGiaLowerCase.end(), tacGiaLowerCase.begin(), ::tolower);

    string theLoaiLowerCase = root->data.theLoai;
    transform(theLoaiLowerCase.begin(), theLoaiLowerCase.end(), theLoaiLowerCase.begin(), ::tolower);

    string keywordLowerCase = keyword;
    transform(keywordLowerCase.begin(), keywordLowerCase.end(), keywordLowerCase.begin(), ::tolower);

    // Nếu tìm kiếm theo mã sách, chỉ so sánh mã sách
    if (timTheoMaSach) {
        if (maSachLowerCase == keywordLowerCase) {
            return root;
        }
        else if (keywordLowerCase < maSachLowerCase) {
            return searchBST(root->left, keyword, timTheoMaSach);
        }
        else {
            return searchBST(root->right, keyword, timTheoMaSach);
        }
    }
    else {
        // Nếu tìm kiếm theo các thông tin khác, so sánh với tất cả các trường
        if (maSachLowerCase.find(keywordLowerCase) != string::npos ||
            tenSachLowerCase.find(keywordLowerCase) != string::npos ||
            tacGiaLowerCase.find(keywordLowerCase) != string::npos ||
            theLoaiLowerCase.find(keywordLowerCase) != string::npos) {
            return root;
        }

        // Tìm kiếm đệ quy trong cây con trái và phải
        TreeNode* leftResult = searchBST(root->left, keyword, timTheoMaSach);
        if (leftResult != nullptr) {
            return leftResult;
        }

        TreeNode* rightResult = searchBST(root->right, keyword, timTheoMaSach);
        return rightResult;
    }
}

// === Biến toàn cục cho cây tìm kiếm ===
TreeNode* rootBST = nullptr; // Ban đầu cây rỗng

void taoRandomSach(Sach** head, int soLuongSach) {
    random_device rd;
    mt19937 generator(rd());

    uniform_int_distribution<> distNam(1900, 2023); // Năm xuất bản từ 1900 đến 2023
    uniform_int_distribution<> distSoLuong(1, 100); // Số lượng từ 1 đến 100
    uniform_real_distribution<> distGiaTien(10.0, 500.0); // Giá tiền từ 10.0 đến 500.0

    string tenSach[] = { "Doraemon", "Conan", "Naruto", "One Piece", "Attack on Titan", "Your Name", "Spirited Away", "Howl's Moving Castle", "The Wind Rises", "Ponyo" };
    string tacGia[] = { "Fujiko F. Fujio", "Gosho Aoyama", "Masashi Kishimoto", "Eiichiro Oda", "Hajime Isayama", "Makoto Shinkai", "Hayao Miyazaki" };
    string theLoai[] = { "Truyen tranh", "Tieu thuyet", "Khoa hoc vien tuong", "Lich su", "Kinh di", "Lang man" };

    uniform_int_distribution<> distTenSach(0, size(tenSach) - 1);
    uniform_int_distribution<> distTacGia(0, size(tacGia) - 1);
    uniform_int_distribution<> distTheLoai(0, size(theLoai) - 1);

    for (int i = 0; i < soLuongSach; ++i) {
        string maSach = "S" + to_string(i + 1); // Mã sách dạng "S1", "S2", ...
        string tenSachRandom = tenSach[distTenSach(generator)];
        string tacGiaRandom = tacGia[distTacGia(generator)];
        string theLoaiRandom = theLoai[distTheLoai(generator)];
        int namXuatBanRandom = distNam(generator);
        int soLuongRandom = distSoLuong(generator);
        double giaTienRandom = distGiaTien(generator);

        Sach* sachMoi = newNode(maSach, tenSachRandom, tacGiaRandom, theLoaiRandom, namXuatBanRandom, soLuongRandom, giaTienRandom);
        sachMoi->next = *head;
        *head = sachMoi;

        // Thêm sách vào Binary Search Tree
        rootBST = insertBST(rootBST, *sachMoi);
    }
}


// === QUẢN LÝ SÁCH ===

// Thêm sách vào đầu danh sách
void themSach(Sach** head) {
    string maSach, tenSach, tacGia, theLoai;
    int namXuatBan, soLuong;
    double giaTien;

    cout << "\n===== THEM SACH MOI =====" << endl;

    cout << "Nhap ma sach: ";
    getline(cin, maSach);
    cout << "Nhap ten sach: ";
    getline(cin, tenSach);
    cout << "Nhap the loai: ";
    getline(cin, theLoai);
    cout << "Nhap tac gia: ";
    getline(cin, tacGia);
  
    
    // Nhập năm xuất bản
    cout << "Nhap nam xuat ban: ";
    while (!(cin >> namXuatBan)) { // Kiểm tra xem người dùng có nhập số nguyên hợp lệ hay không
        cout << "Nam xuat ban khong hop le. Vui long nhap lai: ";
        cin.clear(); // Xóa cờ lỗi
        clearCin(); // Xóa bộ nhớ đệm
    }

    // Nhập số lượng
    cout << "Nhap so luong: ";
    while (!(cin >> soLuong) || soLuong <= 0) { // Kiểm tra số lượng phải là số nguyên dương
        cout << "So luong khong hop le. Vui long nhap lai: ";
        cin.clear();
        clearCin();
    }

    // Nhập giá tiền
    cout << "Nhap gia tien: ";
    while (!(cin >> giaTien) || giaTien <= 0) { // Kiểm tra giá tiền phải là số dương
        cout << "Gia tien khong hop le. Vui long nhap lai: ";
        cin.clear();
        clearCin();
    }

    // Tạo một node mới cho sách
    Sach* newNode = new Sach;
    newNode->maSach = maSach;
    newNode->tenSach = tenSach;
    newNode->tacGia = tacGia;
    newNode->theLoai = theLoai;
    newNode->namXuatBan = namXuatBan;
    newNode->soLuong = soLuong;
    newNode->giaTien = giaTien;

    // Thêm node mới vào đầu danh sách
    newNode->next = *head; // Node mới trỏ đến node đầu tiên hiện tại (hoặc null nếu danh sách rỗng)
    *head = newNode; // Cập nhật con trỏ head để trỏ đến node mới, biến nó thành node đầu tiên

    // Thêm sách vào Binary Search Tree
    rootBST = insertBST(rootBST, *newNode);

    cout << "Them sach thanh cong!" << endl;
}


// Xóa sách theo mã sách
void xoaSach(Sach** head) {
    string maSachCanXoa; // Biến lưu trữ mã sách cần xóa
    cout << "\n===== XÓA SÁCH =====" << endl;
    cout << "Nhap ma sach can xoa: ";
    cin >> maSachCanXoa; // Nhập mã sách từ người dùng

    Sach* temp = *head;     // Con trỏ temp dùng để duyệt qua danh sách, ban đầu trỏ đến head
    Sach* prev = nullptr;   // Con trỏ prev lưu trữ node trước đó, ban đầu là null vì chưa duyệt

    // Vòng lặp tìm kiếm node cần xóa
    while (temp != nullptr && temp->maSach != maSachCanXoa) {
        // Vòng lặp tiếp tục cho đến khi:
        //      - temp trỏ đến null (đã duyệt hết danh sách)
        //      - temp->maSach trùng với maSachCanXoa (đã tìm thấy node cần xóa)

        prev = temp;       // Lưu node hiện tại vào prev trước khi di chuyển temp
        temp = temp->next;  // Di chuyển temp đến node tiếp theo trong danh sách
    }

    // Xử lý sau khi kết thúc vòng lặp tìm kiếm

    // Trường hợp 1: Không tìm thấy sách
    if (temp == nullptr) {
        cout << "Khong tim thay sach co ma " << maSachCanXoa << endl;
        return; // Kết thúc hàm nếu không tìm thấy
    }

    // Trường hợp 2: Đã tìm thấy sách
    // Yêu cầu người dùng xác nhận trước khi xóa
    char confirm;
    cout << "Ban co chac chan muon xoa sach nay? (Y/N): ";
    cin >> confirm;

    // Nếu người dùng không xác nhận xóa (nhập khác 'Y')
    if (toupper(confirm) != 'Y') {
        cout << "Khong xoa sach." << endl;
        return; // Kết thúc hàm, không xóa sách
    }

    // Xử lý xóa node khi người dùng đã xác nhận

    // Trường hợp 2.1: Node cần xóa là node đầu tiên (prev == nullptr)
    if (prev == nullptr) {
        *head = temp->next; // Cập nhật head trỏ đến node tiếp theo sau node bị xóa
    }
    else {
        // Trường hợp 2.2: Node cần xóa không phải node đầu tiên
        prev->next = temp->next; // Nối node trước đó (prev) với node sau node bị xóa 
        // (bỏ qua node cần xóa khỏi chuỗi liên kết)
    }

    delete temp; // Giải phóng vùng nhớ của node bị xóa
    cout << "Xoa sach thanh cong!" << endl;
}

// Sửa thông tin sách
void suaSach(Sach* head) {
    string maSach;
    cout << "\n===== CAP NHAT THONG TIN SACH =====" << endl;
    cout << "Nhap ma sach can sua: ";
    cin >> maSach;

    Sach* temp = head;

    while (temp != nullptr && temp->maSach != maSach) {
        temp = temp->next;
    }

    if (temp == nullptr) {
        cout << "Khong tim thay sach co ma " << maSach << endl;
        return;
    }

    cout << "\nThong tin sach:\n";
    cout << "----------------------\n";
    cout << "Ma sach: " << temp->maSach << endl;
    cout << "Ten sach: " << temp->tenSach << endl;
    cout << "Tac gia: " << temp->tacGia << endl;
    cout << "The loai: " << temp->theLoai << endl;
    cout << "Nam xuat ban: " << temp->namXuatBan << endl;
    cout << "So luong: " << temp->soLuong << endl;
    cout << "Gia tien: " << temp->giaTien << endl;
    cout << "----------------------\n";

    char confirm;
    cout << "Ban co muon cap nhat thong tin sach nay? (Y/N): ";
    cin >> confirm;

    if (toupper(confirm) == 'Y') {
        cout << "Nhap thong tin sach moi (bo qua neu giu nguyen gia tri cu): " << endl;

        // --- Cập nhật tên sách ---
        cout << "Ten sach (" << temp->tenSach << "): ";
        clearCin(); // Xóa bộ nhớ đệm
        string tenSachMoi;
        getline(cin, tenSachMoi);
        if (!tenSachMoi.empty()) {
            temp->tenSach = tenSachMoi;
        }

        // --- Cập nhật tác giả ---
        cout << "Tac gia (" << temp->tacGia << "): ";
        string tacGiaMoi;
        getline(cin, tacGiaMoi);
        if (!tacGiaMoi.empty()) {
            temp->tacGia = tacGiaMoi;
        }

        // --- Cập nhật thể loại ---
        cout << "The loai (" << temp->theLoai << "): ";
        string theLoaiMoi;
        getline(cin, theLoaiMoi);
        if (!theLoaiMoi.empty()) {
            temp->theLoai = theLoaiMoi;
        }

        // --- Cập nhật năm xuất bản ---
        cout << "Nam xuat ban (" << temp->namXuatBan << "): ";
        string namXuatBanMoiStr;
        getline(cin, namXuatBanMoiStr);
        if (!namXuatBanMoiStr.empty()) {
            // Kiểm tra xem người dùng có nhập số hay không
            try {
                int namXuatBanMoi = stoi(namXuatBanMoiStr);
                temp->namXuatBan = namXuatBanMoi;
            }
            catch (const std::exception&) {
                cout << "Nam xuat ban khong hop le. Giu nguyen gia tri cu.\n";
            }
        }

        // --- Cập nhật số lượng ---
        cout << "So luong (" << temp->soLuong << "): ";
        string soLuongMoiStr;
        getline(cin, soLuongMoiStr);
        if (!soLuongMoiStr.empty()) {
            try {
                int soLuongMoi = stoi(soLuongMoiStr);
                temp->soLuong = soLuongMoi;
            }
            catch (const std::exception&) {
                cout << "So luong khong hop le. Giu nguyen gia tri cu.\n";
            }
        }

        // --- Cập nhật giá tiền ---
        cout << "Gia tien (" << temp->giaTien << "): ";
        string giaTienMoiStr;
        getline(cin, giaTienMoiStr);
        if (!giaTienMoiStr.empty()) {
            try {
                double giaTienMoi = stod(giaTienMoiStr);
                temp->giaTien = giaTienMoi;
            }
            catch (const std::exception&) {
                cout << "Gia tien khong hop le. Giu nguyen gia tri cu.\n";
            }
        }

        cout << "Cap nhat thong tin sach thanh cong!" << endl;
    }
    else {
        cout << "Khong co bat ki thay doi nao ve ma sach " << maSach << endl;
    }
}

// Tìm kiếm sách 
void timKiemSach(Sach* head, TreeNode* rootBST) {
    string keyword;
    cout << "\n===== TIM KIEM SACH =====" << endl;

    cout << "\nChon phuong thuc tim kiem:\n";
    cout << "1. Tim theo ma sach (su dung Binary Search Tree)\n";
    cout << "2. Tim theo ten sach, tac gia, the loai (duyet danh sach lien ket)\n";
    cout << "Nhap lua chon: ";
    int luaChonTimKiem;
    cin >> luaChonTimKiem;
    clearCin();

    if (luaChonTimKiem == 1) {
        cout << "Nhap ma sach can tim: ";
        getline(cin, keyword);
    }
    else if (luaChonTimKiem == 2) {
        cout << "Nhap ten sach, tac gia hoac the loai can tim: ";
        getline(cin, keyword);
    }
    else {
        cout << "Lua chon khong hop le.\n";
        return; // Thoát khỏi hàm nếu lựa chọn không hợp lệ
    }

    bool found = false;

    if (luaChonTimKiem == 1) {
        // Tìm kiếm theo mã sách bằng Binary Search Tree
        TreeNode* result = searchBST(rootBST, keyword, true);
        if (result != nullptr) {
            cout << "\nThong tin sach:\n";
            cout << "----------------------\n";
            cout << "Ma sach: " << result->data.maSach << endl;
            cout << "Ten sach: " << result->data.tenSach << endl;
            cout << "Tac gia: " << result->data.tacGia << endl;
            cout << "The loai: " << result->data.theLoai << endl;
            cout << "Nam xuat ban: " << result->data.namXuatBan << endl;
            cout << "So luong: " << result->data.soLuong << endl;
            cout << "Gia tien: " << result->data.giaTien << endl;
            cout << "----------------------\n";
            found = true;
        }
    }
    else if (luaChonTimKiem == 2) {
        // Tìm kiếm theo tên sách, tác giả, thể loại (duyệt danh sách liên kết)
        Sach* temp = head;
        cout << "\nKet qua tim kiem:\n";
        cout << "---------------------------------------------------------------------------------------" << endl;
        while (temp != nullptr) {
            // Chuyển đổi tất cả thông tin cần tìm kiếm thành chữ thường
            string maSachLowerCase = temp->maSach;
            transform(maSachLowerCase.begin(), maSachLowerCase.end(), maSachLowerCase.begin(), ::tolower);

            string tenSachLowerCase = temp->tenSach;
            transform(tenSachLowerCase.begin(), tenSachLowerCase.end(), tenSachLowerCase.begin(), ::tolower);

            string tacGiaLowerCase = temp->tacGia;
            transform(tacGiaLowerCase.begin(), tacGiaLowerCase.end(), tacGiaLowerCase.begin(), ::tolower);

            string theLoaiLowerCase = temp->theLoai;
            transform(theLoaiLowerCase.begin(), theLoaiLowerCase.end(), theLoaiLowerCase.begin(), ::tolower);

            string keywordLowerCase = keyword;
            transform(keywordLowerCase.begin(), keywordLowerCase.end(), keywordLowerCase.begin(), ::tolower);

            // So sánh từ khóa với mã sách, tên sách, tác giả, thể loại
            if (maSachLowerCase.find(keywordLowerCase) != string::npos ||
                tenSachLowerCase.find(keywordLowerCase) != string::npos ||
                tacGiaLowerCase.find(keywordLowerCase) != string::npos ||
                theLoaiLowerCase.find(keywordLowerCase) != string::npos) {

                cout << "Ma sach: " << temp->maSach << endl;
                cout << "Ten sach: " << temp->tenSach << endl;
                cout << "Tac gia: " << temp->tacGia << endl;
                cout << "The loai: " << temp->theLoai << endl;
                cout << "Nam xuat ban: " << temp->namXuatBan << endl;
                cout << "So luong: " << temp->soLuong << endl;
                cout << "Gia tien: " << temp->giaTien << endl;
                cout << "---------------------------------------------------------------------------------------" << endl;

                found = true;
            }

            temp = temp->next;
        }
    }

    if (!found) {
        cout << "Khong tim thay sach phu hop.\n";
    }
}

// Hiển thị danh sách sách
void hienThiDanhSachSach(Sach* head) {
    Sach* temp = head;

    if (isEmpty(temp)) {
        cout << "Danh sach sach rong." << endl;
        return;
    }

    // In tiêu đề bảng
    cout << "\n===== DANH SACH SACH =====" << endl;
    cout << "+------+----------+---------------------+---------------------+------------+------+----------+" << endl;
    cout << "| STT  | Ma Sach  | Ten Sach            | Tac Gia             | The Loai   | Nam | Gia Tien |" << endl;
    cout << "+------+----------+---------------------+---------------------+------------+------+----------+" << endl;

    // Duyệt qua danh sách và in thông tin từng sách
    int stt = 1; // Khởi tạo số thứ tự
    while (temp != nullptr) {
        cout << "| " << setw(4) << left << stt << " | ";
        cout << setw(8) << left << temp->maSach << " | ";
        cout << setw(19) << left << temp->tenSach << " | ";
        cout << setw(19) << left << temp->tacGia << " | ";
        cout << setw(10) << left << temp->theLoai << " | ";
        cout << setw(4) << right << temp->namXuatBan << " | ";
        cout << setw(8) << right << fixed << setprecision(0) << temp->giaTien << " |" << endl; // Định dạng giá tiền

        stt++;
        temp = temp->next;
    }
    cout << "+------+----------+---------------------+---------------------+------------+------+----------+" << endl;
}

// === QUẢN LÝ KHÁCH HÀNG ===
// (Các hàm cho quản lý khách hàng tương tự như quản lý sách)
// ... 



// === HÀM MAIN ===
int main() {
    setlocale(LC_ALL, ""); // Thiet lap locale cho tieng Viet

    Sach* headSach = nullptr;
    KhachHang* headKhachHang = nullptr;

    taoRandomSach(&headSach, 10000);


    int luaChon;

    do {
        cout << "\n" << setfill('=') << setw(40) << "" << setfill(' ') << endl;
        cout << "===== HE THONG QUAN LY BAN SACH =====" << endl;
        cout << setfill('=') << setw(40) << "" << setfill(' ') << endl;
        cout << "1. Quan Ly Sach" << endl;
        cout << "2. Quan Ly Khach Hang" << endl;
        cout << "3. Quan Ly Don Hang" << endl;
        cout << "0. Thoat" << endl;
        cout << setfill('-') << setw(40) << "" << setfill(' ') << endl;
        cout << "Nhap lua chon cua ban: ";
        cin >> luaChon;
        clearCin();

        switch (luaChon) {
        case 1: // Quan ly sach
        {
            int luaChonSach;
            do {
                cout << "\n" << setfill('=') << setw(40) << "" << setfill(' ') << endl;
                cout << "===== QUAN LY SACH =====" << endl;
                cout << setfill('=') << setw(40) << "" << setfill(' ') << endl;
                cout << "1. Them sach" << endl;
                cout << "2. Xoa sach" << endl;
                cout << "3. Sua sach" << endl;
                cout << "4. Tim kiem sach" << endl;
                cout << "5. Hien thi danh sach sach" << endl;
                cout << "0. Quay lai" << endl;
                cout << setfill('-') << setw(40) << "" << setfill(' ') << endl;
                cout << "Nhap lua chon cua ban: ";
                cin >> luaChonSach;
                clearCin();

                switch (luaChonSach) {
                case 1:
                    themSach(&headSach);
                    break;
                case 2:
                    xoaSach(&headSach);
                    break;
                case 3:
                    suaSach(headSach);
                    break;
                case 4:
                    timKiemSach(headSach, rootBST);
                    break;
                case 5:
                    hienThiDanhSachSach(headSach);
                    break;
                case 0:
                    break;
                default:
                    cout << "Lua chon khong hop le." << endl;
                }
            } while (luaChonSach != 0);
            break;
        }
        case 2:
            int luaChonKhach;
            do {
                cout << "\n" << setfill('=') << setw(40) << "" << setfill(' ') << endl;
                cout << "===== QUAN LY KHACH HANG =====" << endl;
                cout << setfill('=') << setw(40) << "" << setfill(' ') << endl;
                cout << "1. Them khach hang" << endl;
                cout << "2. Xoa khach hang" << endl;
                cout << "3. Sua khach hang" << endl;
                cout << "4. Tim kiem khach hang" << endl;
                cout << "5. Lich su mua hang" << endl;
                cout << "6. Hien thi danh sach khach hang" << endl;
                cout << "0. Quay lai" << endl;
                cout << setfill('-') << setw(40) << "" << setfill(' ') << endl;
                cout << "Nhap lua chon cua ban: ";
                cin >> luaChonKhach;
                clearCin();

                switch (luaChonKhach) {
                case 1:
                    
                    break;
                case 2:
                    
                    break;
                case 3:
                    
                    break;
                case 4:
                    
                    break;
                case 5:
                    
                    break;
                case 6:
                    break;
                case 0:
                    break;
                default:
                    cout << "Lua chon khong hop le." << endl;
                }
            } while (luaChonKhach != 0);
            break;
        case 3: 
            int luaChonDonHang;
            do {
                cout << "\n" << setfill('=') << setw(40) << "" << setfill(' ') << endl;
                cout << "===== QUAN LY DON HANG =====" << endl;
                cout << setfill('=') << setw(40) << "" << setfill(' ') << endl;
                cout << "1. Tao don hang" << endl;
                cout << "2. Xem danh sach hang cho" << endl;
                cout << "3. Xu ly don hang" << endl;
                cout << "4. Xem lich su don hang" << endl;
                cout << "0. Quay lai" << endl;
                cout << setfill('-') << setw(40) << "" << setfill(' ') << endl;
                cout << "Nhap lua chon cua ban: ";
                cin >> luaChonDonHang;
                clearCin();

                switch (luaChonDonHang) {
                case 1:
                    
                    break;
                case 2:
                    
                    break;
                case 3:
                   
                    break;
                case 4:
                    
                    break;
                case 0:
                    break;
                default:
                    cout << "Lua chon khong hop le." << endl;
                }
            } while (luaChonDonHang != 0);
            break;
        case 0:
            cout << "Thoat chuong trinh." << endl;
            break;
        default:
            cout << "Lua chon khong hop le." << endl;
        }
    } while (luaChon != 0);

    return 0;
}
