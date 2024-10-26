#include <stdio.h>
#include <string.h>

void print_padded_message(const char *input) {
  char buffer[128]; // Buffer to hold the final formatted string
  int input_length = strlen(input);
  int total_length = input_length + 2; // 2 for "[" and "]"

  // Format the string with padding if it's shorter than 20 characters
  if (total_length < 20) {
    snprintf(buffer, sizeof(buffer), "[%s]%*s", input, 20 - total_length, "");
  } else {
    snprintf(buffer, sizeof(buffer), "[%s]", input); // No padding needed
  }

  printf("%s", buffer);
}

int main() {
  // Test cases
  print_padded_message("Hello"); // Output: "[Hello]          "
  printf("test");
  print_padded_message(
      "LongStringThatExceeds"); // Output: "[LongStringThatExceeds]"
  return 0;
}
