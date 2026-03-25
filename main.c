/*
Project Title:
Student Name: Shivansh Agarwal
Register No: 25BCE5239
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILE_NAME "expenses.dat"
#define BUDGET_FILE "budget.dat"

// Expense structure
struct Expense {
    int day, month, year;
    char category[20];
    float amount;
    char note[50];
};

// Budget structure
struct Budget {
    int month, year;
    float amount;
};

// Function declarations
void addExpense();
void viewExpenses();
void monthlyTotal();
void categoryTotal();
void searchExpense();
void setBudget();
void compareBudgets();
void checkBudget();
float getBudget(int, int);

int main() {
    int choice;
    while (1) {
        printf("\n     Expense Tracker     \n");
        printf("1. Set Budget\n");
        printf("2. Add Expense\n");
        printf("3. View All Expenses\n");
        printf("4. Monthly Total\n");
        printf("5. Category-wise Total\n");
        printf("6. Search Expense\n");
        printf("7. Compare All Budgets\n");
        printf("8. Check Budget\n");
        printf("9. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        switch (choice) {
            case 1: setBudget(); break;
            case 2: addExpense(); break;
            case 3: viewExpenses(); break;
            case 4: monthlyTotal(); break;
            case 5: categoryTotal(); break;
            case 6: searchExpense(); break;
            case 7: compareBudgets(); break;
            case 8: checkBudget(); break;
            case 9: exit(0);
            default: printf("Invalid choice!\n");
        }
    }
}

// Add expense with budget warning after note entry
void addExpense() {
    struct Expense e;
    FILE *fp = fopen(FILE_NAME, "ab");
    if (!fp) { printf("File error!\n"); return; }

    // Date input (dd mm yyyy)
    printf("Enter Date (dd mm yyyy): ");
    char dayStr[3], monthStr[3];
    int year;

    if (scanf("%2s %2s %d", dayStr, monthStr, &year) != 3 ||
        strlen(dayStr) != 2 || strlen(monthStr) != 2 || year < 1900) {
        printf("Invalid date format! Use dd mm yyyy with two-digit day and month.\n");
        fclose(fp);
        return;
    }

    int day = atoi(dayStr);
    int month = atoi(monthStr);

    if(day < 1 || day > 31 || month < 1 || month > 12) {
        printf("Invalid date!\n");
        fclose(fp);
        return;
    }

    e.day = day;
    e.month = month;
    e.year = year;

    // Category
    printf("Enter Category: ");
    scanf("%s", e.category);

    // Amount
    printf("Enter Amount: ");
    if (scanf("%f", &e.amount) != 1 || e.amount <= 0) {
        printf("Invalid amount!\n");
        fclose(fp);
        return;
    }

    // Note
    printf("Enter Note: ");
    getchar(); // clear newline
    scanf(" %[^\n]", e.note);

    // Save expense
    fwrite(&e, sizeof(e), 1, fp);
    fclose(fp);
    printf("Expense added successfully!\n");

    // Check budget for this month and show warning after adding
    float budget = getBudget(e.month, e.year);
    if (budget != -1) {
        struct Expense temp;
        FILE *fexp = fopen(FILE_NAME, "rb");
        float total = 0;
        if (fexp) {
            while(fread(&temp, sizeof(temp), 1, fexp)) {
                if (temp.month == e.month && temp.year == e.year)
                    total += temp.amount;
            }
            fclose(fexp);
        }

        if (total > budget) {
            printf("Warning: Your budget for %02d-%d has been exceeded!\n", e.month, e.year);
            printf("Budget: %.2f | Total Spent: %.2f\n", budget, total);
        }
    }
}

// View all expenses
void viewExpenses() {
    struct Expense e;
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) { printf("No data found!\n"); return; }

    printf("\n--- All Expenses ---\n");
    while (fread(&e, sizeof(e), 1, fp)) {
        printf("%02d-%02d-%d | %-10s | %.2f | %s\n",
               e.day, e.month, e.year, e.category, e.amount, e.note);
    }
    fclose(fp);
}

// Monthly total with all expenses printed
void monthlyTotal() {
    struct Expense e;
    int m, y;
    float total = 0;
    char mStr[3];

    printf("Enter month and year (mm yyyy): ");
    if (scanf("%2s %d", mStr, &y) != 2 || strlen(mStr) != 2 || y < 1900) {
        printf("Invalid input! Use mm yyyy with two-digit month.\n");
        return;
    }

    if (mStr[0] < '0' || mStr[0] > '9' || mStr[1] < '0' || mStr[1] > '9') {
        printf("Invalid month format!\n");
        return;
    }

    m = atoi(mStr);
    if (m < 1 || m > 12) { 
        printf("Invalid month! Must be 01-12\n"); 
        return; 
    }

    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) { printf("No data!\n"); return; }

    printf("\n--- Expenses for %02d-%d ---\n", m, y);
    printf("Date       | Category   | Amount   | Note\n");
    printf("-------------------------------------------\n");

    while (fread(&e, sizeof(e), 1, fp)) {
        if (e.month == m && e.year == y) {
            printf("%02d-%02d-%d | %-10s | %.2f | %s\n",
                   e.day, e.month, e.year, e.category, e.amount, e.note);
            total += e.amount;
        }
    }
    fclose(fp);
    printf("-------------------------------------------\n");
    printf("Total expense for %02d-%d: %.2f\n", m, y, total);
}

// Category total
void categoryTotal() {
    struct Expense e;
    char cat[20];
    float total = 0;
    printf("Enter category: ");
    scanf("%s", cat);

    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) { printf("No data!\n"); return; }
    while (fread(&e, sizeof(e), 1, fp)) {
        if (strcmp(e.category, cat) == 0) total += e.amount;
    }
    fclose(fp);
    printf("Total for %s: %.2f\n", cat, total);
}

// Search expense
void searchExpense() {
    struct Expense e;
    int choice;
    printf("1. Search by Date\n2. Search by Category\nEnter choice: ");
    scanf("%d", &choice);

    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) { printf("No data!\n"); return; }

    if (choice == 1) {
        char dayStr[3], monthStr[3];
        int d, m, y;
        printf("Enter date (dd mm yyyy): ");
        if(scanf("%2s %2s %d", dayStr, monthStr, &y) != 3 ||
           strlen(dayStr) != 2 || strlen(monthStr) != 2 || y < 1900) {
            printf("Invalid date format! Use dd mm yyyy with two-digit day/month.\n");
            fclose(fp);
            return;
        }
        d = atoi(dayStr);
        m = atoi(monthStr);
        if(d < 1 || d > 31 || m < 1 || m > 12) {
            printf("Invalid date!\n");
            fclose(fp);
            return;
        }

        while (fread(&e, sizeof(e), 1, fp)) {
            if (e.day == d && e.month == m && e.year == y)
                printf("%s | %.2f | %s\n", e.category, e.amount, e.note);
        }
    } else if (choice == 2) {
        char cat[20];
        printf("Enter category: ");
        scanf("%s", cat);
        while (fread(&e, sizeof(e), 1, fp)) {
            if (strcmp(e.category, cat) == 0)
                printf("%02d-%02d-%d | %.2f | %s\n",
                       e.day, e.month, e.year, e.amount, e.note);
        }
    } else printf("Invalid choice!\n");

    fclose(fp);
}

// Set budget
void setBudget() {
    struct Budget b;
    char mStr[3];

    printf("Enter month and year (mm yyyy): ");
    if (scanf("%2s %d", mStr, &b.year) != 2 || strlen(mStr) != 2 || b.year < 1900) {
        printf("Invalid input! Use mm yyyy with two-digit month.\n");
        return;
    }

    if (mStr[0] < '0' || mStr[0] > '9' || mStr[1] < '0' || mStr[1] > '9') {
        printf("Invalid month format!\n");
        return;
    }

    b.month = atoi(mStr);
    if (b.month < 1 || b.month > 12) { printf("Invalid month! Must be 01-12\n"); return; }

    FILE *fp = fopen(BUDGET_FILE, "ab");
    if (!fp) { printf("File error!\n"); return; }
    printf("Enter budget amount: ");
    if (scanf("%f", &b.amount) != 1 || b.amount <= 0) {
        printf("Invalid amount!\n");
        fclose(fp);
        return;
    }
    fwrite(&b, sizeof(b), 1, fp);
    fclose(fp);
    printf("Budget saved successfully!\n");
}

// Get budget
float getBudget(int m, int y) {
    struct Budget b;
    FILE *fp = fopen(BUDGET_FILE, "rb");
    if (!fp) return -1;
    while (fread(&b, sizeof(b), 1, fp)) {
        if (b.month == m && b.year == y) { fclose(fp); return b.amount; }
    }
    fclose(fp);
    return -1;
}

// Compare budget
void compareBudgets() {
    struct Budget b;
    struct Expense e;
    FILE *fb = fopen(BUDGET_FILE, "rb");
    if (!fb) { printf("No budget data!\n"); return; }

    printf("\n--- Budget Comparison ---\n");
    while (fread(&b, sizeof(b), 1, fb)) {
        float total = 0;
        FILE *fe = fopen(FILE_NAME, "rb");
        if (!fe) { printf("No expense data!\n"); fclose(fb); return; }
        while (fread(&e, sizeof(e), 1, fe)) {
            if (e.month == b.month && e.year == b.year) total += e.amount;
        }
        fclose(fe);
        printf("%02d-%d | Budget: %.2f | Spent: %.2f | ", b.month, b.year, b.amount, total);
        printf("%s\n", total > b.amount ? "Exceeded" : "Within");
    }
    fclose(fb);
}

// Check budget
void checkBudget() {
    struct Expense e;
    int m, y;
    float total = 0, budget;
    char mStr[3];

    printf("Enter month and year (mm yyyy): ");
    if (scanf("%2s %d", mStr, &y) != 2 || strlen(mStr) != 2 || y < 1900) {
        printf("Invalid input! Use mm yyyy with two-digit month.\n");
        return;
    }

    if (mStr[0] < '0' || mStr[0] > '9' || mStr[1] < '0' || mStr[1] > '9') {
        printf("Invalid month format!\n");
        return;
    }

    m = atoi(mStr);
    if (m < 1 || m > 12) { printf("Invalid month! Must be 01-12\n"); return; }

    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) { printf("No expense data!\n"); return; }
    while (fread(&e, sizeof(e), 1, fp)) {
        if (e.month == m && e.year == y) total += e.amount;
    }
    fclose(fp);

    budget = getBudget(m, y);
    if (budget == -1) { printf("No budget set for this month!\n"); return; }

    printf("Total Expense: %.2f | Budget: %.2f\n", total, budget);
    printf("Remaining: %.2f\n", budget - total);
    printf("%s\n", total > budget ? "Warning: Budget exceeded!" : "Within budget.");
}