#include <stdio.h>
#include <string.h>

struct Book {
    int id;
    int popularity;
};

int main() {
    int capacity, Q;
    scanf("%d %d", &capacity, &Q);

    struct Book shelf[capacity];
    int accessOrder[capacity]; // keeps track of access order indices
    int size = 0; // current number of books on shelf

    // Initialize
    for (int i = 0; i < capacity; i++) {
        shelf[i].id = -1; // -1 indicates empty slot
        accessOrder[i] = -1;
    }

    for (int i = 0; i < Q; i++) {
        char operation[10];
        scanf("%s", operation);

        if (strcmp(operation, "ADD") == 0) {
            int x, y;
            scanf("%d %d", &x, &y);

            int found = -1;
            // Check if book already exists
            for (int j = 0; j < size; j++) {
                if (shelf[j].id == x) {
                    found = j;
                    break;
                }
            }

            if (found != -1) {
                // Update popularity and move to most recent
                shelf[found].popularity = y;

                // Update accessOrder: move 'found' index to end (most recently used)
                int k;
                for (k = 0; k < size; k++) {
                    if (accessOrder[k] == found) break;
                }
                // Shift left
                for (int j = k; j < size - 1; j++) {
                    accessOrder[j] = accessOrder[j + 1];
                }
                accessOrder[size - 1] = found;
            } else {
                if (size < capacity) {
                    // Add new book at next free slot
                    shelf[size].id = x;
                    shelf[size].popularity = y;
                    accessOrder[size] = size;
                    size++;
                } else {
                    // Shelf full: remove least recently used (first in accessOrder)
                    int removeIndex = accessOrder[0];
                    shelf[removeIndex].id = x;
                    shelf[removeIndex].popularity = y;

                    // Shift accessOrder left and add new index at end
                    for (int j = 0; j < capacity - 1; j++) {
                        accessOrder[j] = accessOrder[j + 1];
                    }
                    accessOrder[capacity - 1] = removeIndex;
                }
            }

        } else if (strcmp(operation, "ACCESS") == 0) {
            int x;
            scanf("%d", &x);
            int found = -1;
            for (int j = 0; j < size; j++) {
                if (shelf[j].id == x) {
                    found = j;
                    break;
                }
            }

            if (found != -1) {
                printf("%d\n", shelf[found].popularity);

                // Move accessed book to most recent in accessOrder
                int k;
                for (k = 0; k < size; k++) {
                    if (accessOrder[k] == found) break;
                }
                for (int j = k; j < size - 1; j++) {
                    accessOrder[j] = accessOrder[j + 1];
                }
                accessOrder[size - 1] = found;
            } else {
                printf("-1\n");
            }
        }
    }

    return 0;
}
