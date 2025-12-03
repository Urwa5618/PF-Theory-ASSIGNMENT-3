#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define INITIAL_CAPACITY 5
#define NAME_SIZE 100
#define DATE_SIZE 11   // YYYY-MM-DD + null terminator
#define BATCH_SIZE 20
#define MEMBERSHIP_SIZE 10
#define INTEREST_SIZE 10

typedef struct {
    int studentID;
    char fullName[NAME_SIZE];
    char batch[BATCH_SIZE];        // CS, SE, Cyber Security, AI
    char membershipType[MEMBERSHIP_SIZE]; // IEEE / ACM
    char registrationDate[DATE_SIZE];     // YYYY-MM-DD
    char dateOfBirth[DATE_SIZE];         // YYYY-MM-DD
    char interest[INTEREST_SIZE];       // IEEE / ACM / Both
} Student;

// Global in-memory database
Student *students = NULL;
int numStudents = 0;
int capacity = INITIAL_CAPACITY;

// Error handler for memory allocation
void handleMemoryError() {
    fprintf(stderr, "Memory allocation failed: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
}

// Load database from binary file
void loadDatabase(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("No existing database found. Starting fresh.\n");
        return;
    }

    // Get file size
    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    rewind(fp);

    int recordCount = fileSize / sizeof(Student);
    if (recordCount > 0) {
        students = malloc(recordCount * sizeof(Student));
        if (!students) handleMemoryError();
        capacity = recordCount;
        fread(students, sizeof(Student), recordCount, fp);
        numStudents = recordCount;
    }

    fclose(fp);
}

// Save database to binary file
void saveDatabase(const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("Error opening file for writing");
        return;
    }

    fwrite(students, sizeof(Student), numStudents, fp);
    fclose(fp);
}

// Check if studentID exists
int isDuplicateID(int studentID) {
    for (int i = 0; i < numStudents; i++) {
        if (students[i].studentID == studentID) return 1;
    }
    return 0;
}

// Add a new student
void addStudent(Student s) {
    if (isDuplicateID(s.studentID)) {
        printf("Student ID %d already exists. Cannot add duplicate.\n", s.studentID);
        return;
    }

    // Grow array if needed
    if (numStudents >= capacity) {
        capacity *= 2;
        Student *temp = realloc(students, capacity * sizeof(Student));
        if (!temp) handleMemoryError();
        students = temp;
    }

    students[numStudents++] = s;
    printf("Student added successfully!\n");
}

// Update student's batch or membership
void updateStudent(int studentID) {
    int found = 0;
    for (int i = 0; i < numStudents; i++) {
        if (students[i].studentID == studentID) {
            found = 1;
            printf("Current Batch: %s\n", students[i].batch);
            printf("Enter new Batch: ");
            scanf("%s", students[i].batch);

            printf("Current Membership: %s\n", students[i].membershipType);
            printf("Enter new Membership: ");
            scanf("%s", students[i].membershipType);
            printf("Student record updated.\n");
            break;
        }
    }
    if (!found) printf("Student ID %d not found.\n", studentID);
}

// Delete a student
void deleteStudent(int studentID) {
    int found = 0;
    for (int i = 0; i < numStudents; i++) {
        if (students[i].studentID == studentID) {
            found = 1;
            // Shift remaining students
            memmove(&students[i], &students[i + 1], (numStudents - i - 1) * sizeof(Student));
            numStudents--;
            printf("Student deleted successfully.\n");
            return;
        }
    }
    if (!found) printf("Student ID %d not found.\n", studentID);
}

// Display all students
void displayAllStudents() {
    if (numStudents == 0) {
        printf("No students registered.\n");
        return;
    }
    printf("\nAll Registered Students:\n");
    for (int i = 0; i < numStudents; i++) {
        Student s = students[i];
        printf("ID: %d, Name: %s, Batch: %s, Membership: %s, RegDate: %s, DOB: %s, Interest: %s\n",
               s.studentID, s.fullName, s.batch, s.membershipType,
               s.registrationDate, s.dateOfBirth, s.interest);
    }
}

// Generate batch-wise report
void generateBatchReport() {
    char batch[BATCH_SIZE];
    char membership[MEMBERSHIP_SIZE];
    printf("Enter Batch (CS/SE/Cyber/AI): ");
    scanf("%s", batch);
    printf("Enter Membership (IEEE/ACM/Both): ");
    scanf("%s", membership);

    printf("\nBatch-wise Report:\n");
    int found = 0;
    for (int i = 0; i < numStudents; i++) {
        if (strcmp(students[i].batch, batch) == 0 &&
            (strcmp(students[i].interest, membership) == 0 ||
             strcmp(membership, "Both") == 0)) {
            Student s = students[i];
            printf("ID: %d, Name: %s, Membership: %s, Interest: %s\n",
                   s.studentID, s.fullName, s.membershipType, s.interest);
            found = 1;
        }
    }
    if (!found) printf("No matching records found.\n");
}

// Menu-driven interface
void menu() {
    int choice;
    Student temp;
    int studentID;

    while (1) {
        printf("\n--- IEEE/ACM Membership Manager ---\n");
        printf("1. Register New Student\n2. Update Student\n3. Delete Student\n4. View All Students\n5. Generate Batch Report\n6. Exit\nChoice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline

        switch (choice) {
            case 1:
                printf("Enter Student ID: "); scanf("%d", &temp.studentID); getchar();
                printf("Enter Full Name: "); fgets(temp.fullName, NAME_SIZE, stdin); temp.fullName[strcspn(temp.fullName, "\n")] = 0;
                printf("Enter Batch (CS/SE/Cyber/AI): "); scanf("%s", temp.batch);
                printf("Enter Membership Type (IEEE/ACM): "); scanf("%s", temp.membershipType);
                printf("Enter Registration Date (YYYY-MM-DD): "); scanf("%s", temp.registrationDate);
                printf("Enter Date of Birth (YYYY-MM-DD): "); scanf("%s", temp.dateOfBirth);
                printf("Enter Interest (IEEE/ACM/Both): "); scanf("%s", temp.interest);

                addStudent(temp);
                saveDatabase("members.dat");
                break;
            case 2:
                printf("Enter Student ID to update: "); scanf("%d", &studentID);
                updateStudent(studentID);
                saveDatabase("members.dat");
                break;
            case 3:
                printf("Enter Student ID to delete: "); scanf("%d", &studentID);
                deleteStudent(studentID);
                saveDatabase("members.dat");
                break;
            case 4:
                displayAllStudents();
                break;
            case 5:
                generateBatchReport();
                break;
            case 6:
                free(students);
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }
}

int main() {
    loadDatabase("members.dat");
    menu();
    return 0;
}
