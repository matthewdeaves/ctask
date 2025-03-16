#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "date_utils.h"

#define MAX_TASK_DESC 100
#define MAX_TASKS 100
#define FILENAME "tasks.dat"

// Define a structure for our tasks
typedef struct {
    char description[MAX_TASK_DESC];
    int completed;
    int id;
    int year;
    int month;
    int day;
    int hasDueDate;
} Task;

// Global variables
Task tasks[MAX_TASKS];
int taskCount = 0;

// Function prototypes
void addTask();
void listTasks();
void markTaskComplete();
void deleteTask();
void saveTasksToFile();
void loadTasksFromFile();
void deleteDataFile();
void displayMenu();

int main() {
    int choice;
    
    // Try to load existing tasks
    loadTasksFromFile();
    
    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline
        
        switch(choice) {
            case 1:
                addTask();
                break;
            case 2:
                listTasks();
                break;
            case 3:
                markTaskComplete();
                break;
            case 4:
                deleteTask();
                break;
            case 5:
                saveTasksToFile();
                break;
            case 6:
                deleteDataFile();
                break;
            case 7:
                printf("Exiting program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
        
        printf("\n");
    } while(choice != 7);
    
    return 0;
}

void displayMenu() {
    printf("\n===== Task Manager =====\n");
    printf("1. Add a new task\n");
    printf("2. List all tasks\n");
    printf("3. Mark a task as complete\n");
    printf("4. Delete a task\n");
    printf("5. Save tasks to file\n");
    printf("6. Delete data file\n");
    printf("7. Exit\n");
}

void addTask() {
    if (taskCount >= MAX_TASKS) {
        printf("Task list is full!\n");
        return;
    }
    
    printf("Enter task description: ");
    fgets(tasks[taskCount].description, MAX_TASK_DESC, stdin);
    
    // Remove newline character
    tasks[taskCount].description[strcspn(tasks[taskCount].description, "\n")] = 0;
    
    tasks[taskCount].completed = 0;
    tasks[taskCount].id = taskCount + 1;
    
    // Ask for due date
    char hasDueDate;
    printf("Add a due date? (y/n): ");
    scanf("%c", &hasDueDate);
    getchar(); // Consume newline
    
    if (hasDueDate == 'y' || hasDueDate == 'Y') {
        int year, month, day;
        int validDate = 0;
        
        while (!validDate) {
            printf("Enter due date (YYYY MM DD) or type 'skip' to skip: ");
            
            // Check if user wants to skip
            char input[10];
            if (scanf("%9s", input) == 1) { // limit input to 9 char given max size of input variable
                if (strcmp(input, "skip") == 0) {
                    getchar(); // Consume the rest of the line
                    printf("Skipping due date.\n");
                    tasks[taskCount].hasDueDate = 0;
                    break;
                }
                
                // Try to parse the input as a year
                if (sscanf(input, "%d", &year) == 1) {
                    // Now read the month and day
                    if (scanf("%d %d", &month, &day) == 2) {
                        getchar(); // Consume newline
                        
                        if (isValidDate(year, month, day)) {
                            validDate = 1;
                            tasks[taskCount].year = year;
                            tasks[taskCount].month = month;
                            tasks[taskCount].day = day;
                            tasks[taskCount].hasDueDate = 1;
                            printf("Due date set to: %d-%02d-%02d\n", year, month, day);
                        } else {
                            printf("Invalid date. Please enter a valid date or type 'skip'.\n");
                        }
                    } else {
                        printf("Invalid input format. Please use YYYY MM DD format or type 'skip'.\n");
                        while (getchar() != '\n'); // Clear input buffer
                    }
                } else {
                    printf("Invalid input format. Please use YYYY MM DD format or type 'skip'.\n");
                    while (getchar() != '\n'); // Clear input buffer
                }
            } else {
                printf("Error reading input. Please try again.\n");
                while (getchar() != '\n'); // Clear input buffer
            }
        }
    } else {
        tasks[taskCount].hasDueDate = 0;
    }
    
    printf("Task added successfully!\n");
    taskCount++;
}

void listTasks() {
    if (taskCount == 0) {
        printf("No tasks available.\n");
        return;
    }
    
    printf("\n===== Your Tasks =====\n");
    for (int i = 0; i < taskCount; i++) {
        printf("%d. [%s] %s", 
               tasks[i].id, 
               tasks[i].completed ? "X" : " ", 
               tasks[i].description);
        
        // Display due date if it exists
        if (tasks[i].hasDueDate) {
            printf(" (Due: %d-%02d-%02d)", 
                   tasks[i].year, tasks[i].month, tasks[i].day);
        }
        printf("\n");
    }
}

void markTaskComplete() {
    int id;
    int found = 0;
    
    if (taskCount == 0) {
        printf("No tasks available.\n");
        return;
    }
    
    listTasks();
    
    printf("Enter the ID of the task to mark as complete: ");
    scanf("%d", &id);
    getchar(); // Consume newline
    
    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].id == id) {
            tasks[i].completed = 1;
            printf("Task marked as complete!\n");
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf("Task with ID %d not found.\n", id);
    }
}

void deleteTask() {
    int id;
    int found = 0;
    
    if (taskCount == 0) {
        printf("No tasks available.\n");
        return;
    }
    
    listTasks();
    
    printf("Enter the ID of the task to delete: ");
    scanf("%d", &id);
    getchar(); // Consume newline
    
    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].id == id) {
            // Move all tasks after this one up by one position
            for (int j = i; j < taskCount - 1; j++) {
                tasks[j] = tasks[j + 1];
            }
            taskCount--;
            printf("Task deleted successfully!\n");
            
            // Update IDs for remaining tasks
            for (int j = 0; j < taskCount; j++) {
                tasks[j].id = j + 1;
            }
            
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf("Task with ID %d not found.\n", id);
    }
}

void saveTasksToFile() {
    FILE *file = fopen(FILENAME, "wb");
    
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }
    
    // Write the number of tasks first
    fwrite(&taskCount, sizeof(int), 1, file);
    
    // Write each task
    for (int i = 0; i < taskCount; i++) {
        fwrite(&tasks[i], sizeof(Task), 1, file);
    }
    
    fclose(file);
    printf("Tasks saved to file successfully!\n");
}

void loadTasksFromFile() {
    FILE *file = fopen(FILENAME, "rb");
    
    if (file == NULL) {
        // File doesn't exist yet, that's okay
        return;
    }
    
    // Read the number of tasks
    fread(&taskCount, sizeof(int), 1, file);
    
    // Read each task
    for (int i = 0; i < taskCount; i++) {
        fread(&tasks[i], sizeof(Task), 1, file);
    }
    
    fclose(file);
    printf("Tasks loaded from file.\n");
}

void deleteDataFile() {
    // Attempt to remove the file
    int result = remove(FILENAME);
    
    if (result == 0) {
        printf("Data file '%s' deleted successfully.\n", FILENAME);
        // Reset the task count since we've deleted all saved tasks
        taskCount = 0;
    } else {
        printf("Error deleting data file '%s'.\n", FILENAME);
        perror("Error");  // This will print the specific error message
    }
}
