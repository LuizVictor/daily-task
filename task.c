#include <stdio.h>
#include <string.h>
#include <time.h>

#define FILE_NAME "tasks.dat"

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

int save_task_file(Task *task) {
  FILE *fptr = fopen(FILE_NAME, "ab");
  if (fptr == NULL)
    return 1;

  size_t written = fwrite(task, sizeof(Task), 1, fptr);

  fclose(fptr);
  return written;
}

int create_task(char *argv[]) {
  Task task;
  strcpy(task.name, argv[2]);
  strcpy(task.hour, argv[3]);
  strcpy(task.days, argv[4]);
  task.status = TODO;
  time_t currentTime;
  task.createdAt = time(&currentTime);

  save_task_file(&task);
  return 0;
}

int main(int argc, char *argv[]) {
  if (strcmp("add", argv[1]) == 0) {
    create_task(argv);
  }

  return 0;
}
