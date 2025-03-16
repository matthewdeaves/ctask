#include <stdio.h>
#include <stdlib.h>
#include "task_manager.h"
#include "task_ui.h"

int main() {
    int choice;
    TaskList taskList;
    
    // Initialize the task list
    initTaskList(&taskList);
    
    // Try to load existing tasks
    if (loadTasksFromFile(&taskList, DEFAULT_FILENAME)) {
        printf("Tasks loaded from file.\n");
    }
    
    do {
        displayMenu(stdout);
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline
        
        switch(choice) {
            case 1:
                handleAddTask(&taskList);
                break;
            case 2:
                handleListTasks(&taskList);
                break;
            case 3:
                handleMarkTaskComplete(&taskList);
                break;
            case 4:
                handleDeleteTask(&taskList);
                break;
            case 5:
                handleSaveTasksToFile(&taskList, DEFAULT_FILENAME);
                break;
            case 6:
                handleDeleteDataFile(DEFAULT_FILENAME);
                initTaskList(&taskList); // Reset the task list after deletion
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