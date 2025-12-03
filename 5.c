#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define INITIAL_CAPACITY 2   // Start with small capacity

// Function to handle memory allocation errors
void handleMemoryError() {
    fprintf(stderr, "Memory allocation failed: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
}

// Insert a line at a specific index
void insertLine(char ***lines, int *numLines, int *capacity, int index, const char *text) {
    if (index < 0 || index > *numLines) {
        printf("Invalid index.\n");
        return;
    }

    // Grow array if needed
    if (*numLines >= *capacity) {
        *capacity *= 2;
        char **temp = realloc(*lines, (*capacity) * sizeof(char*));
        if (!temp) handleMemoryError();
        *lines = temp;
    }

    // Shift pointers to make space (not actual strings)
    memmove(&(*lines)[index + 1], &(*lines)[index], (*numLines - index) * sizeof(char*));

    // Allocate exact memory for new line
    (*lines)[index] = malloc(strlen(text) + 1);
    if (!(*lines)[index]) handleMemoryError();
    strcpy((*lines)[index], text);

    (*numLines)++;
}

// Delete a line at a specific index
void deleteLine(char ***lines, int *numLines, int index) {
    if (index < 0 || index >= *numLines) {
        printf("Invalid index.\n");
        return;
    }

    // Free memory for the line
    free((*lines)[index]);

    // Shift pointers to compact array
    memmove(&(*lines)[index], &(*lines)[index + 1], (*numLines - index - 1) * sizeof(char*));
    (*numLines)--;
}

// Print all lines
void printAllLines(char **lines, int numLines) {
    for (int i = 0; i < numLines; i++) {
        printf("%d: %s\n", i, lines[i]);
    }
}

// Free all memory
void freeAll(char **lines, int numLines) {
    for (int i = 0; i < numLines; i++) {
        free(lines[i]);
    }
    free(lines);
}

// Shrink array to fit number of lines
void shrinkToFit(char ***lines, int numLines, int *capacity) {
    if (numLines == 0) {
        free(*lines);
        *lines = NULL;
        *capacity = 0;
        return;
    }

    char **temp = realloc(*lines, numLines * sizeof(char*));
    if (!temp) handleMemoryError();
    *lines = temp;
    *capacity = numLines;
}

// Save buffer to a file
void saveToFile(char **lines, int numLines, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("Error opening file for writing");
        return;
    }

    for (int i = 0; i < numLines; i++) {
        fprintf(fp, "%s\n", lines[i]);
    }

    fclose(fp);
    printf("File saved successfully.\n");
}

// Load buffer from a file
void loadFromFile(char ***lines, int *numLines, int *capacity, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening file for reading");
        return;
    }

    char buffer[1024];
    *numLines = 0;

    // Allocate initial array
    *lines = malloc(INITIAL_CAPACITY * sizeof(char*));
    if (!*lines) handleMemoryError();
    *capacity = INITIAL_CAPACITY;

    while (fgets(buffer, sizeof(buffer), fp)) {
        // Remove newline
        buffer[strcspn(buffer, "\n")] = '\0';
        insertLine(lines, numLines, capacity, *numLines, buffer);
    }

    fclose(fp);
    printf("File loaded successfully.\n");
}

int main() {
    char **lines = NULL;
    int numLines = 0;
    int capacity = INITIAL_CAPACITY;

    // Allocate initial array
    lines = malloc(capacity * sizeof(char*));
    if (!lines) handleMemoryError();

    int choice;
    char text[1024];
    int index;
    char filename[256];

    while (1) {
        printf("\n--- Text Editor ---\n");
        printf("1. Insert Line\n2. Delete Line\n3. Print All Lines\n4. Shrink To Fit\n5. Save to File\n6. Load from File\n7. Exit\nChoice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline

        switch (choice) {
            case 1:
                printf("Enter index to insert: ");
                scanf("%d", &index);
                getchar();
                printf("Enter text: ");
                fgets(text, sizeof(text), stdin);
                text[strcspn(text, "\n")] = '\0'; // Remove newline
                insertLine(&lines, &numLines, &capacity, index, text);
                break;
            case 2:
                printf("Enter index to delete: ");
                scanf("%d", &index);
                getchar();
                deleteLine(&lines, &numLines, index);
                break;
            case 3:
                printAllLines(lines, numLines);
                break;
            case 4:
                shrinkToFit(&lines, numLines, &capacity);
                printf("Shrunk to fit %d lines.\n", numLines);
                break;
            case 5:
                printf("Enter filename to save: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = '\0';
                saveToFile(lines, numLines, filename);
                break;
            case 6:
                printf("Enter filename to load: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = '\0';
                loadFromFile(&lines, &numLines, &capacity, filename);
                break;
            case 7:
                freeAll(lines, numLines);
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}
