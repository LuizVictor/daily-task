#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILE_NAME "tasks.dat"
#define COL_ID 1
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
  print_border_segment(COL_ID);
  print_border_segment(COL_NAME);
  print_border_segment(COL_HOUR);
  print_border_segment(COL_DAYS);
  print_border_segment(COL_STATUS);
  print_border_segment(COL_DATE);
  printf("+\n");
}

static void print_table_header(void) {
  print_separator();
  printf("| %-*s | %-*s | %-*s | %-*s | %-*s | %-*s |\n", COL_ID, "#" ,COL_NAME, "Task", COL_HOUR,
         "Hour", COL_DAYS, "Days", COL_STATUS, "Status", COL_DATE,
         "Created at");
  print_separator();
}

static void print_task_row(const int count, const Task *task) {
  char created[COL_DATE + 1];
  struct tm *tm_info = localtime(&task->createdAt);
  if (tm_info != NULL)
    strftime(created, sizeof(created), "%Y-%m-%d %H:%M:%S", tm_info);
  else
    strcpy(created, "-");

  // %-*.*s pads to the column width and truncates longer text
  printf("| %-*.*d | %-*.*s | %-*.*s | %-*.*s | %-*.*s |%-*s |\n", COL_ID, COL_ID, count,COL_NAME, COL_NAME,
         task->name, COL_HOUR, COL_HOUR, task->hour, COL_DAYS, COL_DAYS,
         task->days, COL_STATUS, COL_STATUS, status_str[task->status], COL_DATE,
         created);
}

int load_task(Task **tasks) {
  FILE *fp;

  fp = fopen(FILE_NAME, "rb");
  if (fp == NULL) {
    printf("ERROR LOADING FILE");
    return 1;
  }

  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);
  rewind(fp);

  size_t count = size / sizeof(Task);
  if (count < 1) {
    fclose(fp);
    return 1;
  }

  *tasks = malloc(count * sizeof(Task));
  if (*tasks == NULL) {
    fclose(fp);
    return 1;
  }

  if (fread(*tasks, sizeof(Task), count, fp) != count) {
      printf("ERROR READING FILE\n");
      free(*tasks);
      *tasks = NULL;
      fclose(fp);
      return 1;
  }

  fclose(fp);
  return (int)count;
}

int list_tasks(void) {
  Task *tasks;

  print_table_header();
  int count = load_task(&tasks);
  for (int i = 0; i < count; i++) {
    print_task_row(i, &tasks[i]);
  }
  print_separator();

  return 0;
}
int main(int argc, char *argv[]) {
  if (strcmp("add", argv[1]) == 0)
    create_task(argv);

  if (strcmp("list", argv[1]) == 0)
    list_tasks();

  return 0;
}
