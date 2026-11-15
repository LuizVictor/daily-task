#include <stdio.h>
#include <string.h>
#include <time.h>

#define FILE_NAME "tasks.dat"
#define COL_NAME 20
#define COL_HOUR 6
#define COL_DAYS 15
#define COL_STATUS 10
#define COL_DATE 19

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

static void print_border_segment(int width) {
  putchar('+');
  for (int i = 0; i < width + 2; i++)
    putchar('-');
}

static void print_separator(void) {
  print_border_segment(COL_NAME);
  print_border_segment(COL_HOUR);
  print_border_segment(COL_DAYS);
  print_border_segment(COL_STATUS);
  print_border_segment(COL_DATE);
  printf("+\n");
}

static void print_table_header(void) {
  print_separator();
  printf("| %-*s | %-*s | %-*s | %-*s | %-*s |\n", COL_NAME, "Task", COL_HOUR,
         "Hour", COL_DAYS, "Days", COL_STATUS, "Status", COL_DATE,
         "Created at");
  print_separator();
}

static void print_task_row(const Task *task) {
  char created[COL_DATE + 1];
  struct tm *tm_info = localtime(&task->createdAt);
  if (tm_info != NULL)
    strftime(created, sizeof(created), "%Y-%m-%d %H:%M:%S", tm_info);
  else
    strcpy(created, "-");

  // %-*.*s pads to the column width and truncates longer text
  printf("| %-*.*s | %-*.*s | %-*.*s | %-*.*s | %-*s |\n", COL_NAME, COL_NAME,
         task->name, COL_HOUR, COL_HOUR, task->hour, COL_DAYS, COL_DAYS,
         task->days, COL_STATUS, COL_STATUS, status_str[task->status], COL_DATE,
         created);
}

int list_tasks(void) {
  FILE *fptr = fopen(FILE_NAME, "rb");
  if (fptr == NULL) {
    printf("ERROR OPEN FILE\n");
    return 1;
  }

  Task task;
  print_table_header();
  while (fread(&task, sizeof(Task), 1, fptr) == 1) {
    print_task_row(&task);
  }
  print_separator();

  fclose(fptr);
  return 0;
}
int main(int argc, char *argv[]) {
  if (strcmp("add", argv[1]) == 0)
    create_task(argv);

  if (strcmp("list", argv[1]) == 0)
    list_tasks();

  return 0;
}
