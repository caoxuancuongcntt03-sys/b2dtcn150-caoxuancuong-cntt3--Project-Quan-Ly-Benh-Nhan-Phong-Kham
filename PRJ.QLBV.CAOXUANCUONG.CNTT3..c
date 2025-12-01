//Cao Xuan Cuong -CNTT03-Project Quan Ly Benh Nhan Phong Kham .
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_PATIENTS 100
#define MAX_RECORDS 500
#define PATIENTS_PER_PAGE 2
#define DEFAULT_FEE 100000.0

typedef struct {
    char cardId[10];
    char name[50];
    char phone[15];
    double debt;
    int visitDays;
} Patient;

typedef struct {
    char recId[20];
    char cardId[10];
    char date[20];
    char status[20];
} Record;

Patient patients[MAX_PATIENTS];
Record records[MAX_RECORDS];
int pCount = 0;
int rCount = 0;

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void removeNewline(char* str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

bool is_leap(int year) {
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

bool is_valid_date(int day, int month, int year) {
    if (year < 1 || month < 1 || month > 12 || day < 1) {
        return false;
    }

    int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (is_leap(year)) {
        days_in_month[2] = 29;
    }

    if (month > 12 || day > days_in_month[month]) {
        return false;
    }

    return true;
}

void parse_date(const char *date_str, int *d, int *m, int *y) {
    sscanf(date_str, "%d/%d/%d", d, m, y);
}

int findPatientIndex(char* id) {
    for (int i = 0; i < pCount; i++) {
        if (strcmp(patients[i].cardId, id) == 0) {
            return i;
        }
    }
    return -1;
}

int checkVisitDateExists(char* id, char* date) {
    for (int i = 0; i < rCount; i++) {
        if (strcmp(records[i].cardId, id) == 0 && strcmp(records[i].date, date) == 0) {
            return 1;
        }
    }
    return 0;
}


int isValidPhoneNumber(char* phone) {
    int len = strlen(phone);
    
    
    if (phone[0] == '+') {
        
        if (len < 11 || len > 12) return 0; 
    } 
    else {
        
        if (len < 10 || len > 11) return 0;
    }

    
    for (int i = 0; i < len; i++) {
        if (i == 0 && phone[i] == '+') continue;
        
        if (!isdigit((unsigned char)phone[i])) {
            return 0;
        }
    }
    return 1;
}

void listPatientRecords(char* id, char* name) {
    int found = 0;
    printf("\n--- LICH SU KHAM BENH CUA BN: %s (%s) ---\n", name, id);
    printf("+-------+------------+----------+\n");
    printf("| %-5s | %-10s | %-8s |\n", "ID", "Ngay Kham", "Trang Thai");
    printf("+-------+------------+----------+\n");

    for (int i = 0; i < rCount; i++) {
        if (strcmp(records[i].cardId, id) == 0) {
            printf("| %-5s | %-10s | %-8s |\n",
                records[i].recId + 3,
                records[i].date,
                records[i].status);
            found = 1;
        }
    }
    printf("+-------+------------+----------+\n");
    if (!found) {
        printf("Chua co luot kham nao duoc ghi nhan.\n");
    }
}

void deleteRelatedRecords(char* cardId) {
    int newRCount = 0;
    for (int i = 0; i < rCount; i++) {
        if (strcmp(records[i].cardId, cardId) != 0) {
            records[newRCount++] = records[i];
        }
    }
    int deletedCount = rCount - newRCount;
    rCount = newRCount;
    printf(">> Da xoa %d luot kham lien quan.\n", deletedCount);
}

void initializeDefaultData() {
    strcpy(patients[pCount].cardId,"BN01");
    strcpy(patients[pCount].name, "Nguyen Nhu Quynh");
    strcpy(patients[pCount].phone, "0912345678");
    patients[pCount].debt = 50000.0;
    patients[pCount].visitDays = 2;
    pCount++;

    strcpy(patients[pCount].cardId, "BN02");
    strcpy(patients[pCount].name, "Nguyen Khanh Huyen");
    strcpy(patients[pCount].phone, "+84987654321");
    patients[pCount].debt = 100000.0; 
    patients[pCount].visitDays = 1;
    pCount++;

    strcpy(patients[pCount].cardId,"BN03");
    strcpy(patients[pCount].name, "Tran Minh Cuong");
    strcpy(patients[pCount].phone, "0901112223");
    patients[pCount].debt = 125000.0;
    patients[pCount].visitDays = 0;
    pCount++;

    strcpy(patients[pCount].cardId, "BN04");
    strcpy(patients[pCount].name, "Pham Thi Dung");
    strcpy(patients[pCount].phone, "09345678910");
    patients[pCount].debt = 20000.0;
    patients[pCount].visitDays = 1;
    pCount++;

    strcpy(records[rCount].recId, "REC001");
    strcpy(records[rCount].cardId, "BN01");
    strcpy(records[rCount].date, "10/11/2025");
    strcpy(records[rCount].status, "Tai Kham");
    rCount++;

    strcpy(records[rCount].recId, "REC002");
    strcpy(records[rCount].cardId, "BN02");
    strcpy(records[rCount].date, "20/11/2025");
    strcpy(records[rCount].status, "Theo Doi");
    rCount++;

    strcpy(records[rCount].recId, "REC003");
    strcpy(records[rCount].cardId, "BN01");
    strcpy(records[rCount].date, "25/11/2025");
    strcpy(records[rCount].status, "Tai Kham");
    rCount++;

    strcpy(records[rCount].recId, "REC004");
    strcpy(records[rCount].cardId, "BN04");
    strcpy(records[rCount].date, "01/10/2025");
    strcpy(records[rCount].status, "Theo Doi");
    rCount++;
}

void f01_addPatient() {
    if (pCount >= MAX_PATIENTS) {
        printf("Loi: Danh sach benh nhan da day.\n");
        return;
    }

    Patient p;
    printf("\n--- F01: THEM BENH NHAN MOI ---\n");

    while (1) {
        printf("Nhap cardId (toi da 9 ky tu, khong dau cach): ");
        fgets(p.cardId, 10, stdin);
        removeNewline(p.cardId);

        if (strlen(p.cardId) == 0 || strchr(p.cardId, ' ') != NULL) {
            printf("Loi: cardId khong duoc rong hoac co dau cach.\n");
            continue;
        }
        if (findPatientIndex(p.cardId) != -1) {
            printf("Loi: cardId da ton tai.\n");
            continue;
        }
        break;
    }

    do {
        printf("Nhap Ho Ten: ");
        fgets(p.name, 50, stdin); removeNewline(p.name);
        if (strlen(p.name) == 0) {
            printf("Loi: Ho ten khong duoc de trong.\n");
        }
    } while (strlen(p.name) == 0);

    char phone_buffer[15]; 
    do {
        printf("Nhap SDT ( +84 & 10 den 11 chu so): ");
        fgets(phone_buffer, 15, stdin); 
        removeNewline(phone_buffer);
        
        if (strlen(phone_buffer) == 0) {
            printf("Loi: SDT khong duoc rong.\n");
        } else if (!isValidPhoneNumber(phone_buffer)) {
            printf("Loi: SDT khong hop le. Vui long chi nhap 10 hoac 11 chu so (co the kem dau + o dau).\n");
        }
    } while (strlen(phone_buffer) == 0 || !isValidPhoneNumber(phone_buffer));
    strcpy(p.phone, phone_buffer);

    do {
        printf("Nhap Cong no hien tai (> 0): "); 
        if (scanf("%lf", &p.debt) != 1) {
            printf("Loi: Gia tri cong no khong hop le. Vui long nhap so.\n");
            clearInputBuffer();
            p.debt = 0.0; 
        } else if (p.debt <= 0.0) { 
            printf("Loi: Cong no phai lon hon 0. Vui long nhap lai.\n");
        }
    } while (p.debt <= 0.0); 
    clearInputBuffer();

    p.visitDays = 0;
    patients[pCount++] = p;
    printf("Them benh nhan thanh cong!\n");
}

void f02_updatePatient() {
    char id[10];
    printf("\n--- F02: CAP NHAT THONG TIN ---\n");
    printf("Nhap cardId can sua: ");
    fgets(id, 10, stdin); removeNewline(id);

    int idx = findPatientIndex(id);
    if (idx == -1) {
        printf("Loi: Khong tim thay benh nhan.\n");
        return;
    }

    printf("Dang sua thong tin cho: %s\n", patients[idx].name);
    char buffer[50];
    char phone_buffer[15];

    while (1) {
        printf("Nhap Ten moi (Enter de giu nguyen: %s): ", patients[idx].name);
        fgets(buffer, 50, stdin); removeNewline(buffer);
        if (strlen(buffer) == 0) {
            break;
        }
        if (strlen(buffer) > 0) {
            strcpy(patients[idx].name, buffer);
            break;
        }
    }

    while(1) {
        printf("Nhap SDT moi (Enter de giu nguyen: %s): ", patients[idx].phone);
        fgets(phone_buffer, 15, stdin); removeNewline(phone_buffer);
        
        if (strlen(phone_buffer) == 0) {
            break; 
        }

        if (isValidPhoneNumber(phone_buffer)) {
            strcpy(patients[idx].phone, phone_buffer);
            break;
        } else {
            printf("Loi: SDT khong hop le. Vui long chi nhap 10 hoac 11 chu so (co the kem dau + o dau).\n");
        }
    }

    double newDebt;
    bool validDebtInput = false;

    do {
        printf("Nhap Cong no moi (nhap -1 de giu nguyen: %.3lf). Cong no moi phai > 0 (neu cap nhat): ", patients[idx].debt);
        if (scanf("%lf", &newDebt) != 1) {
            printf("Loi: Dau vao Cong no khong hop le. Giu nguyen.\n");
            clearInputBuffer();
            break; 
        }
        clearInputBuffer();

        if (newDebt == -1.0) {
            break; 
        } else if (newDebt > 0.0) { 
            patients[idx].debt = newDebt;
            validDebtInput = true;
        } else {
            printf("Loi: Cong no moi phai lon hon 0. Nhap -1 de giu nguyen cong no cu.\n");
        }
    } while (!validDebtInput);


    printf("Cap nhat thanh cong!\n");
}

void f03_dischargePatient() {
    char id[10];
    printf("\n--- F03: XUAT VIEN (XOA BENH NHAN) ---\n");
    printf("Nhap cardId de xuat vien: ");
    fgets(id, 10, stdin); removeNewline(id);

    int idx = findPatientIndex(id);
    if (idx == -1) {
        printf("Loi: Khong tim thay benh nhan.\n");
        return;
    }

    printf("Xuat vien cho BN: %s (CardID: %s)\n", patients[idx].name, patients[idx].cardId);

    if (patients[idx].debt > 0) {
        printf("CANH BAO: Benh nhan con no %.3lfVND. Tiep tuc xuat vien? (y/n): ", patients[idx].debt);
        char confirm[5];
        if (fgets(confirm, 5, stdin) == NULL) confirm[0] = 'n';
        removeNewline(confirm);

        if (confirm[0] != 'y' && confirm[0] != 'Y') {
            printf("Huy xuat vien.\n");
            return;
        }
    }

    deleteRelatedRecords(id);

    for (int i = idx; i < pCount - 1; i++) {
        patients[i] = patients[i+1];
    }
    pCount--;
    printf("Da xoa ho so benh nhan khoi he thong.\n");
}

void f04_listPatients() {
    if (pCount == 0) {
        printf("Danh sach trong.\n");
        return;
    }

    int totalPages = (pCount + PATIENTS_PER_PAGE - 1) / PATIENTS_PER_PAGE;
    int currentPage = 1;
    char input_str[10];
    int new_page = -1;

    do {
        if (currentPage < 1) currentPage = 1;
        if (currentPage > totalPages) currentPage = totalPages;

        int startIndex = (currentPage - 1) * PATIENTS_PER_PAGE;
        int endIndex = startIndex + PATIENTS_PER_PAGE;
        if (endIndex > pCount) {
            endIndex = pCount;
        }

        printf("\nTRANG %d/%d (Hien thi %d - %d/%d BN)\n",
                currentPage, totalPages, startIndex + 1, endIndex, pCount);

        printf("\n========================== DANH SACH BENH NHAN ===============================\n");
        printf("| %-8s | %-20s | %-13s | %-13s | %-8s |\n", "CardID", "Ho Ten", "SDT", "Cong No", "Luot Kham");
        printf("+----------+----------------------+---------------+---------------+-----------+\n");

        for (int i = startIndex; i < endIndex; i++) {
            printf("| %-8s | %-20s | %-13s | %-10.3lfVND | %-9d |\n",
                patients[i].cardId,
                patients[i].name,
                patients[i].phone,
                patients[i].debt,
                patients[i].visitDays);
        }

        printf("+----------+----------------------+---------------+---------------+-----------+\n");

        printf("\nLenh: Nhap so trang (1-%d), hoac (0) de thoat: ", totalPages);

        if (fgets(input_str, 10, stdin) == NULL) {
            strcpy(input_str, "0");
        }
        removeNewline(input_str);

        if (strcmp(input_str, "0") == 0 || strcmp(input_str, "q") == 0) {
            break;
        } else if (strcmp(input_str, "n") == 0 || strcmp(input_str, "next") == 0) {
            if (currentPage < totalPages) {
                currentPage++;
            } else {
                printf("Da la trang cuoi cung.\n");
            }
        } else if (strcmp(input_str, "p") == 0 || strcmp(input_str, "prev") == 0) {
            if (currentPage > 1) {
                currentPage--;
            } else {
                printf("Da la trang dau tien.\n");
            }
        } else {
            new_page = atoi(input_str);
            if (new_page >= 1 && new_page <= totalPages) {
                currentPage = new_page;
            } else {
                printf("Loi: So trang khong hop le (phai tu 1 den %d). Vui long nhap lai.\n", totalPages);
            }
        }

    } while (1);

    printf("Ket thuc chuc nang danh sach benh nhan.\n");
}

void f05_searchPatient() {
    char query[50];
    printf("\n--- F05: TIM KIEM BENH NHAN ---\n");
    printf("Nhap ten hoac SDT de tim kiem: ");
    fgets(query, 50, stdin); removeNewline(query);

    if (strlen(query) == 0) {
        printf("Query khong duoc rong.\n");
        return;
    }

    char lowerQuery[50];
    for (int i = 0; i < strlen(query); i++) {
        lowerQuery[i] = tolower((unsigned char)query[i]);
    }
    lowerQuery[strlen(query)] = '\0';

    int foundCount = 0;
    printf("\n================================= DANH SACH BENH NHAN  =====================================\n");
    printf("| %-8s | %-20s | %-15s | %-12s | %-9s |\n",
              "CardID", "Ho Ten", "Dien Thoai", "Cong No", "Luot Kham");
    printf("+----------+----------------------+-----------------+--------------+-----------+\n");

    for (int i = 0; i < pCount; i++) {
        char lowerName[50];
        for (int j = 0; j < strlen(patients[i].name); j++) {
            lowerName[j] = tolower((unsigned char)patients[i].name[j]);
        }
        lowerName[strlen(patients[i].name)] = '\0';

        if (strstr(lowerName, lowerQuery) != NULL || strstr(patients[i].phone, query) != NULL) {
            printf("| %-8s | %-20s | %-15s | %-9.3lfVND | %-9d |\n",
                patients[i].cardId,
                patients[i].name,
                patients[i].phone,
                patients[i].debt,
                patients[i].visitDays);
            foundCount++;
        }
    }
    printf("+----------+----------------------+-----------------+--------------+-----------+\n");

    if (!foundCount) printf("Khong tim thay ket qua phu hop.\n");
    else printf("Tim thay %d ket qua.\n", foundCount);
}

void f06_sortPatients() {
    if (pCount == 0) {
        printf("Danh sach trong.\n");
        return;
    }

    int choice;
    printf("\n--- F06: SAP XEP DANH SACH ---\n");
    printf("1. Tang dan theo Cong no (Debt)\n");
    printf("2. Giam dan theo Cong no (Debt)\n");
    printf("Chon: ");

    if (scanf("%d", &choice) != 1) {
        printf("Loi: Lua chon khong hop le.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    for (int i = 0; i < pCount - 1; i++) {
        for (int j = i + 1; j < pCount; j++) {
            int condition = 0;
            if (choice == 1) condition = (patients[i].debt > patients[j].debt);
            else if (choice == 2) condition = (patients[i].debt < patients[j].debt);
            else continue;

            if (condition) {
                Patient temp = patients[i];
                patients[i] = patients[j];
                patients[j] = temp;
            }
        }
    }
    if (choice == 1 || choice == 2) {
        printf("Da sap xep. Vui long chon chuc nang Hien thi (4) de xem.\n");
    } else {
        printf("Lua chon sap xep khong hop le.\n");
    }
}

void f07_addRecord() {
    if (rCount >= MAX_RECORDS) {
        printf("Bo nho Record day!\n");
        return;
    }

    char id[10], date[20], status[20];
    printf("\n--- F07: GHI NHAN KHAM BENH ---\n");

    printf("Nhap cardId benh nhan: ");
    fgets(id, 10, stdin); removeNewline(id);

    int idx = findPatientIndex(id);
    if (idx == -1) {
        printf("Loi: cardId khong ton tai.\n");
        return;
    }

    int day, month, year;
    bool dateValid = false;

    do {
        printf("Nhap Ngay kham (dd/mm/yyyy): ");
        fgets(date, sizeof(date), stdin);
        removeNewline(date);

        parse_date(date, &day, &month, &year);

        if (is_valid_date(day, month, year)) {
            if (checkVisitDateExists(id, date)) {
                printf("=> LOI: Benh nhan %s da co luot kham vao ngay %s. Vui long kiem tra lai.\n", patients[idx].name, date);
            } else {
                dateValid = true;
            }
        } else {
            printf("=> LOI: Ngay thang nam khong hop le (%s). Vui long nhap lai.\n", date);
        }
    } while (!dateValid);

    Record r;
    sprintf(r.recId, "REC%03d", rCount + 1);
    strcpy(r.cardId, id);
    strcpy(r.date, date);

    printf("Nhap trang thai (Tai kham/Theo doi...): ");
    fgets(status, 20, stdin); removeNewline(status);
    strcpy(r.status, status);

    records[rCount++] = r;
    patients[idx].visitDays++;
    patients[idx].debt += 100000.0;

    printf("Ghi nhan kham benh thanh cong! (recId: %s, Cong no moi: %.3lfVND)\n", r.recId, patients[idx].debt);

}

void f08_listPatientHistory() {
    char id[10];
    printf("\n--- F08: XEM LICH SU KHAM BENH ---\n");
    printf("Nhap cardId benh nhan can xem: ");
    fgets(id, 10, stdin); removeNewline(id);

    int idx = findPatientIndex(id);
    if (idx == -1) {
        printf("Loi: Khong tim thay benh nhan co cardId: %s\n", id);
        return;
    }

    listPatientRecords(id, patients[idx].name);
}

void printMenu() {
    printf("\n==================== QUAN LY PHONG KHAM ====================\n");
    printf("1. Tiep nhan benh nhan moi (F01)\n");
    printf("2. Cap nhat thong tin benh nhan (F02)\n");
    printf("3. Xuat vien/Xoa ho so (F03)\n");
    printf("4. Hien thi danh sach benh nhan (Phan Trang) (F04)\n");
    printf("5. Tim kiem benh nhan theo ten (F05)\n");
    printf("6. Sap xep danh sach benh nhan (F06)\n");
    printf("7. Ghi nhan kham benh (F07)\n");
    printf("8. Xem lich su kham benh (F08)\n");
    printf("0. Thoat chuong trinh \n");
    printf("===========================================================\n");
    printf("Nhap lua chon cua ban: ");
}

int main() {
    int choice;

    initializeDefaultData();
    printf(">> Khoi dong lan dau. Da nap %d benh nhan mau va %d luot kham mau.\n", pCount, rCount);

    while (1){
        printMenu();

        if (scanf("%d", &choice) != 1) {
            printf("Dau vao khong hop le. Vui long nhap so.\n");
            clearInputBuffer();
            continue;
        }

        clearInputBuffer();

        switch (choice) {
            case 1: f01_addPatient();
                    break;
            case 2: f02_updatePatient();
                    break;
            case 3: f03_dischargePatient();
                    break;
            case 4: f04_listPatients();
                    break;
            case 5: f05_searchPatient();
                    break;
            case 6: f06_sortPatients();
                    break;
            case 7: f07_addRecord();
                    break;
            case 8: f08_listPatientHistory();
                    break;
            case 0:
                printf("Thoat chuong trinh. Tam biet!\n");
                exit(0);
            default:
                printf("Chuc nang khong ton tai. Vui long chon lai.\n");
        }
    }
    return 0;

}
