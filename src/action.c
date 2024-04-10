#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_QUERY_LENGTH 2048

// Struct to hold params information
struct Params {
  char name[50];
  int age;
  char color[20];
};

// Function to fill the struct parameters with empty values
void initEmptyParams(struct Params *params) {
    strcpy(params->name, "");
    params->age = 0;
    strcpy(params->color, "");
}

// Function to parse query string and populate Params struct
void parseQueryString(char *query, struct Params *params) {
  char *token;
  const char delim[] = "&=";
  token = strtok(query, delim);
  
  while(token != NULL) {
    if (strcmp(token, "name") == 0) {
      token = strtok(NULL, delim);
      strcpy(params->name, token);
    } else if (strcmp(token, "age") == 0) {
      token = strtok(NULL, delim);
      params->age = atoi(token);
    } else if (strcmp(token, "color") == 0) {
      token = strtok(NULL, delim);
      strcpy(params->color, token);
    }
    token = strtok(NULL, delim);
  }
}

int main() {
  char query[MAX_QUERY_LENGTH];
  struct Params params;

  // Initialize params with empty values
  initEmptyParams(&params);

  // Parse query string and populate Params struct
  parseQueryString(getenv("QUERY_STRING"), &params);

  // Headers
  printf("Content-type: text/html\n");
  printf("\n");

  // HTML
  printf("<!DOCTYPE html>\n");
  printf("<html lang=\"en\">\n");
  printf("<head>\n");
  printf("<meta charset=\"UTF-8\">\n");
  printf("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
  printf("<title>Wellcome</title>\n");
  printf("</head>\n");
  printf("<body>\n");
  printf("<h1>Hello %s</h1>\n", params.name);
  printf("<p>Your age is %d years and your favourite color is %s.</p>\n", params.age, params.color);
  printf("</body>\n");
  printf("</html>\n");

  return 0;
}
