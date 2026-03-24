/*
Project Title: Expense Tracker
Student Name: Shivansh Agarwal
Register No: 25BCE5239
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "expenses.dat"
#define BUDGET_FILE "budget.dat"

// Structures
struct Expense {
    int day, month, year;
    char category[20];
    float amount;
    char note[50];
};

struct Budget {
    int month, year;
    float amount;
};

// Helper input function
void getInput(char *buffer, int size) {
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0; // remove newline
}

// Get valid month
int getValidMonth(int *year) {
    char input[50];
    int m;

    printf("Enter month and year (mm yyyy): ");
    getInput(input, sizeof(input));

    if (sscanf(input, "%d %d", &m, year) != 2) {
        printf("Invalid input!\n");
        return -1;
    }

    if (m < 1 || m > 12) {
        printf("Invalid month!\n");
        return -1;
    }

    return m;
}

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

// MAIN
int main() {
    char input[20];
    int choice;

    while (1) {
        printf("\n===== Expense Tracker =====\n");
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

        getInput(input, sizeof(input));
        sscanf(input, "%d", &choice);

        switch (choice) {
            case 1: setBudget(); break;
            case 2: addExpense(); break;
            case 3: viewExpenses(); break;
            case 4: monthlyTotal(); break;
            case 5: categoryTotal(); break;
            case 6: searchExpense(); break;
            case 7: compareBudgets(); break;
            case 8: checkBudget(); break;
            case 9:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }
}

// Add expense
void addExpense() {
    struct Expense e;
    char input[100];

    FILE *fp = fopen(FILE_NAME, "ab");
    if (!fp) {
        printf("File error!\n");
        return;
    }

    printf("Enter Date (dd mm yyyy): ");
    getInput(input, sizeof(input));
    if (sscanf(input, "%d %d %d", &e.day, &e.month, &e.year) != 3) {
        printf("Invalid date!\n");
        fclose(fp);
        return;
    }

    printf("Enter Category: ");
    getInput(e.category, sizeof(e.category));

    printf("Enter Amount: ");
    getInput(input, sizeof(input));
    if (sscanf(input, "%f", &e.amount) != 1 || e.amount <= 0) {
        printf("Invalid amount!\n");
        fclose(fp);
        return;
    }

    printf("Enter Note: ");
    getInput(e.note, sizeof(e.note));

    fwrite(&e, sizeof(e), 1, fp);
    fclose(fp);

    printf("Expense added successfully!\n");
}

// View expenses
void viewExpenses() {
    struct Expense e;
    FILE *fp = fopen(FILE_NAME, "rb");

    if (!fp) {
        printf("No data found!\n");
        return;
    }

    printf("\n--- All Expenses ---\n");
    while (fread(&e, sizeof(e), 1, fp)) {
        printf("%02d-%02d-%d | %-10s | %.2f | %s\n",
               e.day, e.month, e.year,
               e.category, e.amount, e.note);
    }

    fclose(fp);
}

// Monthly total
void monthlyTotal() {
    struct Expense e;
    int m, y;
    float total = 0;

    m = getValidMonth(&y);
    if (m == -1) return;

    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No data!\n");
        return;
    }

    while (fread(&e, sizeof(e), 1, fp)) {
        if (e.month == m && e.year == y)
            total += e.amount;
    }

    fclose(fp);
    printf("Total expense: %.2f\n", total);
}

// Category total
void categoryTotal() {
    struct Expense e;
    char cat[20];
    float total = 0;

    printf("Enter category: ");
    getInput(cat, sizeof(cat));

    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No data!\n");
        return;
    }

    while (fread(&e, sizeof(e), 1, fp)) {
        if (strcmp(e.category, cat) == 0)
            total += e.amount;
    }

    fclose(fp);
    printf("Total for %s: %.2f\n", cat, total);
}

// Search expense
void searchExpense() {
    struct Expense e;
    char input[50];
    int choice;

    printf("1. Search by Date\n2. Search by Category\nEnter choice: ");
    getInput(input, sizeof(input));
    sscanf(input, "%d", &choice);

    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No data!\n");
        return;
    }

    if (choice == 1) {
        int d, m, y;
        printf("Enter date (dd mm yyyy): ");
        getInput(input, sizeof(input));
        sscanf(input, "%d %d %d", &d, &m, &y);

        while (fread(&e, sizeof(e), 1, fp)) {
            if (e.day == d && e.month == m && e.year == y)
                printf("%s | %.2f | %s\n", e.category, e.amount, e.note);
        }
    } 
    else if (choice == 2) {
        char cat[20];
        printf("Enter category: ");
        getInput(cat, sizeof(cat));

        while (fread(&e, sizeof(e), 1, fp)) {
            if (strcmp(e.category, cat) == 0)
                printf("%02d-%02d-%d | %.2f | %s\n",
                       e.day, e.month, e.year, e.amount, e.note);
        }
    } 
    else {
        printf("Invalid choice!\n");
    }

    fclose(fp);
}

// Set budget
void setBudget() {
    struct Budget b;
    char input[50];

    b.month = getValidMonth(&b.year);
    if (b.month == -1) return;

    FILE *fp = fopen(BUDGET_FILE, "ab");
    if (!fp) {
        printf("File error!\n");
        return;
    }

    printf("Enter budget amount: ");
    getInput(input, sizeof(input));
    if (sscanf(input, "%f", &b.amount) != 1 || b.amount <= 0) {
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
        if (b.month == m && b.year == y) {
            fclose(fp);
            return b.amount;
        }
    }

    fclose(fp);
    return -1;
}

// Compare budgets
void compareBudgets() {
    struct Budget b;
    struct Expense e;

    FILE *fb = fopen(BUDGET_FILE, "rb");
    if (!fb) {
        printf("No budget data!\n");
        return;
    }

    printf("\n--- Budget Comparison ---\n");

    while (fread(&b, sizeof(b), 1, fb)) {
        float total = 0;

        FILE *fe = fopen(FILE_NAME, "rb");
        if (!fe) {
            printf("No expense data!\n");
            fclose(fb);
            return;
        }

        while (fread(&e, sizeof(e), 1, fe)) {
            if (e.month == b.month && e.year == b.year)
                total += e.amount;
        }

        fclose(fe);

        printf("%02d-%d | Budget: %.2f | Spent: %.2f | ",
               b.month, b.year, b.amount, total);

        if (total > b.amount)
            printf("Exceeded\n");
        else
            printf("Within\n");
    }

    fclose(fb);
}

// Check budget
void checkBudget() {
    struct Expense e;
    int m, y;
    float total = 0, budget;

    m = getValidMonth(&y);
    if (m == -1) return;

    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No expense data!\n");
        return;
    }

    while (fread(&e, sizeof(e), 1, fp)) {
        if (e.month == m && e.year == y)
            total += e.amount;
    }

    fclose(fp);

    budget = getBudget(m, y);
    if (budget == -1) {
        printf("No budget set!\n");
        return;
    }

    printf("Total: %.2f | Budget: %.2f\n", total, budget);
    printf("Remaining: %.2f\n", budget - total);

    if (total > budget)
        printf("⚠ Budget exceeded!\n");
    else
        printf("Within budget.\n");
}