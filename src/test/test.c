#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <linux/limits.h>

int main() {
	char *filepath = "./test/img/asd/putty";

  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("cwd: %s\n", cwd);
  } else {
    printf("Failed to get cwd");
    return 1;
  }

  filepath + 2;

  strcat(strcat(cwd, "/"), filepath);
  printf("%s\n", cwd);

  return 0;
}