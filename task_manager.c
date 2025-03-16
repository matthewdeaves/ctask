#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task_manager.h"
#include "date_utils.h"

#define MAX_TASK_DESC 100
#define MAX_TASKS 100
#define DEFAULT_FILENAME "tasks.dat"

// Initialize a new task list
void initTaskList(TaskList *list) {
    list->taskCount = 0;
}

// Core business logic functions
int addTask(TaskList *list, const char *description, int hasDueDate, int year, int month, int day) {
    if (list->taskCount >= MAX_TASKS) {
        return 0; // Failure - list is full
    }
    
    strncpy(list->tasks[list->taskCount].description, description, MAX_TASK_DESC - 1);
    list->tasks[list->taskCount].description[MAX_TASK_DESC - 1] = '\0'; // Ensure null termination
    
    list->tasks[list->taskCount].completed = 0;
    list->tasks[list->taskCount].id = list->taskCount + 1;
    list->tasks[list->taskCount].hasDueDate = hasDueDate;
    
    if (hasDueDate) {
        if (!isValidDate(year, month, day)) {
            return 0; // Failure - invalid date
        }
        list->tasks[list->taskCount].year = year;
        list->tasks[list->taskCount].month = month;
        list->tasks[list->taskCount].day = day;
    }
    
    list->taskCount++;
    return 1; // Success
}

void listTasks(const TaskList *list, FILE *output) {
    if (list->taskCount == 0) {
        fprintf(output, "No tasks available.\n");
        return;
    }
    
    fprintf(output, "\n===== Your Tasks =====\n");
    for (int i = 0; i < list->taskCount; i++) {
        fprintf(output, "%d. [%s] %s", 
               list->tasks[i].id, 
               list->tasks[i].completed ? "X" : " ", 
               list->tasks[i].description);
        
        // Display due date if it exists
        if (list->tasks[i].hasDueDate) {
            fprintf(output, " (Due: %d-%02d-%02d)", 
                   list->tasks[i].year, list->tasks[i].month, list->tasks[i].day);
        }
        fprintf(output, "\n");
    }
}

int markTaskComplete(TaskList *list, int id) {
    for (int i = 0; i < list->taskCount; i++) {
        if (list->tasks[i].id == id) {
            list->tasks[i].completed = 1;
            return 1; // Success
        }
    }
    return 0; // Failure - task not found
}

int deleteTask(TaskList *list, int id) {
    for (int i = 0; i < list->taskCount; i++) {
        if (list->tasks[i].id == id) {
            // Move all tasks after this one up by one position
            for (int j = i; j < list->taskCount - 1; j++) {
                list->tasks[j] = list->tasks[j + 1];
            }
            list->taskCount--;
            
            // Update IDs for remaining tasks
            for (int j = 0; j < list->taskCount; j++) {
                list->tasks[j].id = j + 1;
            }
            
            return 1; // Success
        }
    }
    return 0; // Failure - task not found
}

int saveTasksToFile(const TaskList *list, const char *filename) {
    FILE *file = fopen(filename, "wb");
    
    if (file == NULL) {
        return 0; // Failure
    }
    
    // Write the number of tasks first
    if (fwrite(&list->taskCount, sizeof(int), 1, file) != 1) {
        fclose(file);
        return 0; // Failure
    }
    
    // Write each task
    for (int i = 0; i < list->taskCount; i++) {
        if (fwrite(&list->tasks[i], sizeof(Task), 1, file) != 1) {
            fclose(file);
            return 0; // Failure
        }
    }
    
    fclose(file);
    return 1; // Success
}

int loadTasksFromFile(TaskList *list, const char *filename) {
    FILE *file = fopen(filename, "rb");
    
    if (file == NULL) {
        // File doesn't exist yet, that's okay
        return 0; // Not a failure, just no file yet
    }
    
    // Read the number of tasks
    if (fread(&list->taskCount, sizeof(int), 1, file) != 1) {
        fclose(file);
        return 0; // Failure
    }
    
    // Read each task
    for (int i = 0; i < list->taskCount; i++) {
        if (fread(&list->tasks[i], sizeof(Task), 1, file) != 1) {
            fclose(file);
            return 0; // Failure
        }
    }
    
    fclose(file);
    return 1; // Success
}

int deleteDataFile(const char *filename) {
    return (remove(filename) == 0) ? 1 : 0; // 1 for success, 0 for failure
}
