#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task_ui.h"
#include "task_manager.h"
#include "date_utils.h"

void displayMenu(FILE *output) {
    fprintf(output, "\n===== Task Manager =====\n");
    fprintf(output, "1. Add a new task\n");
    fprintf(output, "2. List all tasks\n");
    fprintf(output, "3. Mark a task as complete\n");
    fprintf(output, "4. Delete a task\n");
    fprintf(output, "5. Save tasks to file\n");
    fprintf(output, "6. Delete data file\n");
    fprintf(output, "7. Exit\n");
}

void handleListTasks(TaskList *list) {
    listTasks(list, stdout);
}

void handleSaveTasksToFile(TaskList *list, const char *filename) {
    if (saveTasksToFile(list, filename)) {
        printf("Tasks saved to file successfully!\n");
    } else {
        printf("Error saving tasks to file!\n");
    }
}

void handleDeleteDataFile(const char *filename) {
    if (deleteDataFile(filename)) {
        printf("Data file deleted successfully!\n");
    } else {
        printf("Error deleting data file!\n");
        perror("Error");  // This will print the specific error message
    }
}

void handleAddTask(TaskList *list) {
    char description[MAX_TASK_DESC];
    int hasDueDate = 0;
    int year = 0, month = 0, day = 0;
    
    printf("Enter task description: ");
    fgets(description, MAX_TASK_DESC, stdin);
    
    // Remove newline character
    description[strcspn(description, "\n")] = 0;
    
    // Ask for due date
    char dueDateChoice;
    printf("Add a due date? (y/n): ");
    scanf("%c", &dueDateChoice);
    getchar(); // Consume newline
    
    if (dueDateChoice == 'y' || dueDateChoice == 'Y') {
        int validDate = 0;
        
        while (!validDate) {
            printf("Enter due date (YYYY MM DD) or type 'skip' to skip: ");
            
            // Check if user wants to skip
            char input[10];
            if (scanf("%9s", input) == 1) {
                if (strcmp(input, "skip") == 0) {
                    getchar(); // Consume the rest of the line
                    printf("Skipping due date.\n");
                    hasDueDate = 0;
                    break;
                }
                
                // Try to parse the input as a year
                if (sscanf(input, "%d", &year) == 1) {
                    // Now read the month and day
                    if (scanf("%d %d", &month, &day) == 2) {
                        getchar(); // Consume newline
                        
                        if (isValidDate(year, month, day)) {
                            validDate = 1;
                            hasDueDate = 1;
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
    }
    
    if (addTask(list, description, hasDueDate, year, month, day)) {
        printf("Task added successfully!\n");
    } else {
        printf("Failed to add task!\n");
    }
}

void handleMarkTaskComplete(TaskList *list) {
    int id;
    
    if (list->taskCount == 0) {
        printf("No tasks available.\n");
        return;
    }
    
    listTasks(list, stdout);
    
    printf("Enter the ID of the task to mark as complete: ");
    scanf("%d", &id);
    getchar(); // Consume newline
    
    if (markTaskComplete(list, id)) {
        printf("Task marked as complete!\n");
    } else {
        printf("Task with ID %d not found.\n", id);
    }
}

void handleDeleteTask(TaskList *list) {
    int id;
    
    if (list->taskCount == 0) {
        printf("No tasks available.\n");
        return;
    }
    
    listTasks(list, stdout);
    
    printf("Enter the ID of the task to delete: ");
    scanf("%d", &id);
    getchar(); // Consume newline
    
    if (deleteTask(list, id)) {
        printf("Task deleted successfully!\n");
    } else {
        printf("Task with ID %d not found.\n", id);
    }
}
