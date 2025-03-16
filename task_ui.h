#ifndef TASK_UI_H
#define TASK_UI_H

#include <stdio.h>
#include "task_manager.h"

// UI function declarations
void displayMenu(FILE *output);
void handleAddTask(TaskList *list);
void handleMarkTaskComplete(TaskList *list);
void handleDeleteTask(TaskList *list);
void handleListTasks(TaskList *list);
void handleSaveTasksToFile(TaskList *list, const char *filename);
void handleDeleteDataFile(const char *filename);

#endif // TASK_UI_H