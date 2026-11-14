#include <stdio.h>
#include <string.h>
#include <time.h>

typedef enum { TODO, DONE, ERROR } Status;
const char *const status_str[] = {
    [TODO] = "todo",
    [DONE] = "done",
    [ERROR] = "error",
};

typedef struct {
  char name[100];
  char hour[5];
  char days[27];
  Status status;
  time_t createdAt;
} Task;

Task create_task(char *argv[]) {
  Task task;
  strcpy(task.name, argv[2]);
  strcpy(task.hour, argv[3]);
  strcpy(task.days, argv[4]);
  task.status = TODO;
  time_t currentTime;
  task.createdAt = time(&currentTime);

  return task;
}

int main(int argc, char *argv[]) {
  if (strcmp("add", argv[1]) == 0) {
    Task task = create_task(argv);

    printf("Task: %s \n", task.name);
    printf("Hour: %s \n", task.hour);
    printf("Days: %s \n", task.days);
    printf("Status: %s \n", status_str[task.status]);
    printf("Created_at: %s", ctime(&task.createdAt));
  }

  return 1;
}
