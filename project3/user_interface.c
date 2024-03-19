/*
Mahati Gorthy
119821602
mgorthy */
#include "document.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

static int validate_line(const char *line);
static void validate_command(Document *doc, char *line);

/* This program offers a text-based interface to the document manager system */
static void validate_command(Document *doc, char *line) {
  char cmd[MAX_STR_SIZE + 1], arg1[MAX_STR_SIZE + 1], arg2[MAX_STR_SIZE + 1],
      arg3[MAX_STR_SIZE + 1];
  int num_args = sscanf(line, "%s%s%s%s", cmd, arg1, arg2, arg3);
  int paragraph_number = 0;
  int line_number = 0;
  char result[MAX_STR_SIZE] = "";
  char target[MAX_STR_SIZE] = "";
  char replacement[MAX_STR_SIZE] = "";
  int curr_char = 0;
  int count = 0;
  char *p = line;
  char *curr;

  if (strcmp(cmd, "add_paragraph_after") == 0) {
    /* This command will add a paragraph to the document. The ”Invalid Command”
     * message will be generated when necessary */
    if (num_args == 2) {
      paragraph_number = atoi(arg1);
      if (paragraph_number == 0 && arg1[0] != '0') {
        printf("Invalid Command\n");
        return;
      }
      if (paragraph_number < 0) {
        printf("Invalid Command\n");
        return;
      }
      if (add_paragraph_after(doc, paragraph_number) == FAILURE) {
        printf("add_paragraph_after failed\n");
      }
    } else {
      printf("Invalid Command\n");
    }
  } else if (strcmp(cmd, "add_line_after") == 0) {
    /* This command will add a line with the specified line number. The ”Invalid
     * Command” message will be generated when necessary */
    if (num_args == 4 && strstr(arg3, "*") != NULL) {
      paragraph_number = atoi(arg1);
      line_number = atoi(arg2);

      if (paragraph_number == 0 && arg1[0] != '0') {
        printf("Invalid Command\n");
        return;
      }

      if (line_number == 0 && arg2[0] != '0') {
        printf("Invalid Command\n");
        return;
      }

      /* Traverses to retrieve the full line that comes after * */
      for (curr_char = 0; curr_char < strlen(line); curr_char++) {
        if (count > 0) {
          result[count - 1] = *p;
          count++;
        }
        if (*p == '*') {
          count++;
        }
        p++;
      }
      if (paragraph_number <= 0 || line_number < 0) {
        printf("Invalid Command\n");
        return;
      }
      if (add_line_after(doc, paragraph_number, line_number, result) ==
          FAILURE) {
        printf("add_line_after failed\n");
      }
    } else {
      printf("Invalid Command\n");
    }
  } else if (strcmp(cmd, "print_document") == 0) {
    /* This command will print the document information (printdocument function
     * output). The ”Invalid Command” message will be generated if any data
     * appears after print document */
    if (num_args == 1) {
      print_document(doc);
    } else {
      printf("Invalid Command\n");
    }
  } else if (strcmp(cmd, "quit") == 0 || strcmp(cmd, "exit") == 0) {
    /* This command will exit the user interface.  The ”Invalid Command” message
     * will be generated whenany data appears after quit. */
    if (num_args == 1) {
      exit(EXIT_SUCCESS);
    } else {
      printf("Invalid Command\n");
    }
  } else if (strcmp(cmd, "append_line") == 0) {
    /* This command will append a line to the specified parapgrah. The "Invalid
     * Command" message will be generated when necessary */
    if ((num_args == 4 || num_args == 3) && strstr(arg2, "*") != NULL) {
      paragraph_number = atoi(arg1);

      if (paragraph_number == 0 && arg1[0] != '0') {
        printf("Invalid Command\n");
        return;
      }

      for (curr_char = 0; curr_char < strlen(line); curr_char++) {
        if (count > 0) {
          result[count - 1] = *p;
          count++;
        }
        if (*p == '*') {
          count++;
        }
        p++;
      }
      if (paragraph_number < 0) {
        printf("Invalid Command\n");
        return;
      }
      if (append_line(doc, paragraph_number, result) == FAILURE) {
        printf("append_line failed\n");
      }
    } else {
      printf("Invalid Command\n");
    }
  } else if (strcmp(cmd, "load_file") == 0) {
    /* This command will load the specified file into the current document. The
     * ”Invalid Command” message will be generated when necessary */
    if (num_args == 2) {
      if (load_file(doc, arg1) == FAILURE) {
        printf("load_file failed\n");
      }
    } else {
      printf("Invalid Command\n");
      return;
    }
  } else if (strcmp(cmd, "save_document") == 0) {
    /* This command will save the curent document to the specified file.  The
     * ”Invalid Command” message will be generated when necessary */
    if (num_args == 2) {
      if (save_document(doc, arg1) == FAILURE)
        printf("save_document failed\n");
    } else {
      printf("Invalid Command\n");
    }
  } else if (strcmp(cmd, "reset_document") == 0) {
    /* This command will reset the curent document. The ”Invalid Command”
     * message will be generated when any data appears after reset_document. */
    if (num_args == 1) {
      reset_document(doc);
    } else {
      printf("Invalid Command\n");
    }
  } else if (strcmp(cmd, "remove_line") == 0) {
    /* This command will remove the specified line from the paragraph.  The
     * ”Invalid Command” message will be generated when necessary */
    if (num_args == 3) {
      paragraph_number = atoi(arg1);
      line_number = atoi(arg2);

      if (paragraph_number == 0 && arg1[0] != '0') {
        printf("Invalid Command\n");
        return;
      }

      if (line_number == 0 && arg2[0] != '0') {
        printf("Invalid Command\n");
        return;
      }
      if (paragraph_number < 0 || line_number < 0) {
        printf("Invalid Command\n");
        return;
      }
      if (remove_line(doc, paragraph_number, line_number) == FAILURE) {
        printf("remove_line failed\n");
      }
    } else {
      printf("Invalid Command\n");
    }
  } else if (strcmp(cmd, "replace_text") == 0) {
    /* This command will replace the string ”TARGET” with ”REPLACEMENT”.  The
     * ”Invalid Command”message will be generated when necessary */
    if (num_args == 4) {
      char *start_quote1 = strstr(line, "\"");
      char *end_quote1 = NULL;
      char *start_quote2 = NULL;
      char *end_quote2 = NULL;

      if (start_quote1) {
        end_quote1 = strstr(start_quote1 + 1, "\"");
      }

      if (end_quote1) {
        start_quote2 = strstr(end_quote1 + 1, "\"");
      }

      if (start_quote2) {
        end_quote2 = strstr(start_quote2 + 1, "\"");
      }

      if (!start_quote1 || !end_quote1 || !start_quote2 || !end_quote2) {
        printf("Invalid Command\n");
        return;
      }

      strncpy(target, start_quote1 + 1, end_quote1 - start_quote1 - 1);
      target[end_quote1 - start_quote1 - 1] = '\0';

      strncpy(replacement, start_quote2 + 1, end_quote2 - start_quote2 - 1);
      replacement[end_quote2 - start_quote2 - 1] = '\0';

      if (replace_text(doc, target, replacement) == FAILURE) {
        printf("replace_text failed\n");
      }
    } else {
      printf("Invalid Command\n");
    }
  } else if (strcmp(cmd, "remove_text") == 0) {
    /* This command will remove the string ”TARGET”.  The ”Invalid Command”
     * message will be generated when ”TARGET” is missing */
    if (num_args == 2) {

      /* Gets the string between the quotes */
      curr = strstr(line, "\"");
      count = 0;
      curr++;
      while (*curr != '"') {
        target[count] = *curr;
        count++;
        curr++;
      }
      remove_text(doc, target);
    } else {
      printf("Invalid Command\n");
    }
  } else if (strcmp(cmd, "highlight_text") == 0) {
    /* This command will highlight the string ”TARGET”. The ”Invalid Command”
     * message will be generated when ”TARGET” is missing */
    if (num_args == 2) {

      /* Gets the string between the quotes */
      curr = strstr(line, "\"");
      count = 0;
      curr++;
      while (*curr != '"') {
        target[count] = *curr;
        count++;
        curr++;
      }
      highlight_text(doc, target);
    } else {
      printf("Invalid Command\n");
    }
  } else {
    printf("Invalid Command\n");
  }
}

/* Helper function which helps to check if a line is a blank like or a comment
 */
static int validate_line(const char *line) {
  char character;
  int still_a_space = 1;
  int i = 0;

  for (i = 0; i < strlen(line); i++) {
    if (!isspace(line[i]) && line[i] != '\0') {
      still_a_space = 0;
    }
  }

  sscanf(line, " %c", &character);

  if (still_a_space == 1) {
    return 0;
  }

  return (character != '#') && (strlen(line) != 0) && (strcmp(line, "\n"));
}

/* Text-based user interface to the document manager system */
int main(int argc, char *argv[]) {
  Document doc;
  FILE *input;
  char line[MAX_STR_SIZE + 1];

  /* Initialize document which the operations and commands will be performed on
   */
  init_document(&doc, "main_document");

  /* Deciding where input will come from */
  if (argc == 1) {
    input = stdin; /* Standard input */
    printf("> ");

    while (fgets(line, MAX_STR_SIZE + 1, input) != NULL &&
           strcmp(line, "quit\n") && strcmp(line, "exit\n")) {

      if (validate_line(line) == 0) {
        printf("> "); /* Generate new prompt if the line is a blank line or
                         comment */
      } else {
        /* Remove newline character */
        if (strlen(line) > 0 && line[strlen(line) - 1] == '\n') {
          line[strlen(line) - 1] = '\0';
        }
        validate_command(&doc, line);
        printf("> ");
      }
    }
  } else if (argc == 2) { /* If a file is named, the program will read its data
                             from the file */
    /* Using file provided by the user */
    input = fopen(argv[1], "r");

    /* If there is an error opening the file, the program will print the message
     * FILENAME cannot be opened */
    if (input == NULL) {
      fprintf(stderr, "%s cannot be opened.", argv[1]);
      exit(EX_OSERR);
    }

    while (fgets(line, MAX_STR_SIZE + 1, input) != NULL &&
           strcmp(line, "quit\n") && strcmp(line, "exit\n")) {

      if (validate_line(line) == 0) {

      } else {
        /* Remove newline character */
        if (strlen(line) > 0 && line[strlen(line) - 1] == '\n') {
          line[strlen(line) - 1] = '\0';
        }
        validate_command(&doc, line);
      }
    }

  } else {
    fprintf(stderr, "Usage: a.out\n");
    fprintf(stderr, "Usage: a.out <filename>\n");
    exit(EX_USAGE);
  }

  /* Closing the stream */
  fclose(input);
  exit(EXIT_SUCCESS);
}