#include <stdio.h>
#include <string.h>

struct Employee {
    int id;
    char name[30];
    char designation[30];
    float salary;
};

// Function to display all employees
void displayEmployees(struct Employee emp[], int n) {
    printf("\n--- Employee Records ---\n");
    for(int i = 0; i < n; i++) {
        printf("%d\t%s\t%s\t%.2f\n", emp[i].id, emp[i].name, emp[i].designation, emp[i].salary);
    }
}

// Function to find employee with highest salary
void findHighestSalary(struct Employee emp[], int n) {
    int maxIndex = 0;
    for(int i = 1; i < n; i++) {
        if(emp[i].salary > emp[maxIndex].salary)
            maxIndex = i;
    }
    printf("\nHighest Salary Employee:\n");
    printf("%d\t%s\t%s\t%.2f\n",
           emp[maxIndex].id, emp[maxIndex].name,
           emp[maxIndex].designation, emp[maxIndex].salary);
}

// Function to search employee by ID or Name
void searchEmployee(struct Employee emp[], int n) {
    int choice;
    printf("\nSearch by:\n1. ID\n2. Name\nEnter choice: ");
    scanf("%d", &choice);

    if(choice == 1) {
        int id;
        printf("Enter ID: ");
        scanf("%d", &id);
        for(int i = 0; i < n; i++) {
            if(emp[i].id == id) {
                printf("Found: %d\t%s\t%s\t%.2f\n",
                       emp[i].id, emp[i].name, emp[i].designation, emp[i].salary);
                return;
            }
        }
    } else {
        char name[30];
        printf("Enter Name: ");
        scanf("%s", name);
        for(int i = 0; i < n; i++) {
            if(strcmp(emp[i].name, name) == 0) {
                printf("Found: %d\t%s\t%s\t%.2f\n",
                       emp[i].id, emp[i].name, emp[i].designation, emp[i].salary);
                return;
            }
        }
    }
    printf("Employee not found!\n");
}

void giveBonus(struct Employee emp[], int n, float threshold) {
    for(int i = 0; i < n; i++) {
        if(emp[i].salary < threshold)
            emp[i].salary *= 1.10;  // 10% bonus
    }
}

int main() {
    int n;
    printf("Enter number of employees: ");
    scanf("%d", &n);

    struct Employee emp[n];

  
    for(int i = 0; i < n; i++) {
        printf("Enter ID, Name, Designation, Salary: ");
        scanf("%d %s %s %f", &emp[i].id, emp[i].name, emp[i].designation, &emp[i].salary);
    }

    displayEmployees(emp, n);
    findHighestSalary(emp, n);
    searchEmployee(emp, n);

    giveBonus(emp, n, 50000);
    printf("\nAfter Bonus:\n");
    displayEmployees(emp, n);
    return 0;
}
