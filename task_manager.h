#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <stdio.h>

#define MAX_TASK_DESC 100
#define MAX_TASKS 100
#define DEFAULT_FILENAME "tasks.dat"

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

// Define a structure to hold the task list
typedef struct {
    Task tasks[MAX_TASKS];
    int taskCount;
} TaskList;

// Core business logic function declarations
void initTaskList(TaskList *list);
int addTask(TaskList *list, const char *description, int hasDueDate, int year, int month, int day);
void listTasks(const TaskList *list, FILE *output);
int markTaskComplete(TaskList *list, int id);
int deleteTask(TaskList *list, int id);
int saveTasksToFile(const TaskList *list, const char *filename);
int loadTasksFromFile(TaskList *list, const char *filename);
int deleteDataFile(const char *filename);

#endif // TASK_MANAGER_H