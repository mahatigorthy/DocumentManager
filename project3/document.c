/*
Mahati Gorthy
119821602
mgorthy */
#include "document.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

static int replace_in_line(Document *doc, const char *target,
                           const char *replacement, int curr_para,
                           int curr_line);

/* This program is a document manager. This program allows you to add
 * parpargaphs, lines to paragraphs, replace text, and edit a document. */

/* Initializes the document to be empty. The function will also initialize the
 * document's name based on the provided parameter value. */
int init_document(Document *doc, const char *name) {
  /* Return FAILURE if the doc is NULL or the name is NULL */
  if (doc == NULL || name == NULL) {
    return FAILURE;
  }

  /* Return FAILURE if the length of the name exceeds the max string size */
  if (strlen(name) > MAX_STR_SIZE) {
    return FAILURE;
  }

  doc->number_of_paragraphs = 0;

  strcpy(doc->name, name);

  return SUCCESS;
}

/* Sets the number of paragraphs to be 0 */
int reset_document(Document *doc) {
  if (doc == NULL) {
    return FAILURE;
  }

  doc->number_of_paragraphs = 0;

  return SUCCESS;
}

/* Prints the document’s name, number of paragraphs, followed by the paragraphs.
 * Each paragraph is separated by a blank line */
int print_document(Document *doc) {
  int curr_para, curr_line = 0;

  if (doc == NULL) {
    return FAILURE;
  }

  printf("Document name: \"%s\"\n", doc->name);
  printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);

  /* Traverses through all paragraphs and lines */
  for (curr_para = 0; curr_para < doc->number_of_paragraphs; curr_para++) {
    for (curr_line = 0; curr_line < doc->paragraphs[curr_para].number_of_lines;
         curr_line++) {
      printf("%s\n", doc->paragraphs[curr_para].lines[curr_line]);
    }

    if (curr_para != doc->number_of_paragraphs - 1) {
      printf("\n");
    }
  }

  return SUCCESS;
}

/* Adds a paragraph after the specified paragraph number */
int add_paragraph_after(Document *doc, int paragraph_number) {
  int curr_para;

  if (doc == NULL || doc->number_of_paragraphs >= MAX_PARAGRAPHS ||
      paragraph_number > doc->number_of_paragraphs) {
    return FAILURE;
  }

  /* Traverses through all the paragraphs in the document */
  for (curr_para = doc->number_of_paragraphs; curr_para > paragraph_number;
       curr_para--) {
    doc->paragraphs[curr_para] = doc->paragraphs[curr_para - 1];
  }

  doc->paragraphs[paragraph_number].number_of_lines = 0;
  doc->number_of_paragraphs++;

  return SUCCESS;
}

/* Adds a new line after the line with the specified line number */
int add_line_after(Document *doc, int paragraph_number, int line_number,
                   const char *new_line) {
  int curr_line;

  if (doc == NULL || paragraph_number > doc->number_of_paragraphs ||
      doc->paragraphs[paragraph_number - 1].number_of_lines >=
          MAX_PARAGRAPH_LINES ||
      line_number > doc->paragraphs[paragraph_number - 1].number_of_lines ||
      new_line == NULL) {
    return FAILURE;
  }

  /* Traverses through all the lines of a given parapgaph (I started at 0 so
   * parapgrah_number - 1) */
  for (curr_line = doc->paragraphs[paragraph_number - 1].number_of_lines;
       curr_line > line_number; curr_line--) {
    strcpy(doc->paragraphs[paragraph_number - 1].lines[curr_line],
           doc->paragraphs[paragraph_number - 1].lines[curr_line - 1]);
  }
  strcpy(doc->paragraphs[paragraph_number - 1].lines[line_number], new_line);
  doc->paragraphs[paragraph_number - 1].number_of_lines++;

  return SUCCESS;
}

/* Returns the number of lines in a paragraph using the number_of_lines
 * parameter */
int get_number_lines_paragraph(Document *doc, int paragraph_number,
                               int *number_of_lines) {
  if (doc == NULL || number_of_lines == NULL ||
      paragraph_number > doc->number_of_paragraphs) {
    return FAILURE;
  }

  *number_of_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;

  return SUCCESS;
}

/* Appends a line to the specified parapgraph */
int append_line(Document *doc, int paragraph_number, const char *new_line) {
  if (doc == NULL || paragraph_number > doc->number_of_paragraphs ||
      doc->paragraphs[paragraph_number - 1].number_of_lines >=
          MAX_PARAGRAPH_LINES ||
      new_line == NULL) {
    return FAILURE;
  }

  /* Just calls the add_line_after method, by calling to the last line */
  add_line_after(doc, paragraph_number,
                 doc->paragraphs[paragraph_number - 1].number_of_lines,
                 new_line);

  return SUCCESS;
}

/* Removes the specified line from the paragraph */
int remove_line(Document *doc, int paragraph_number, int line_number) {
  int curr_line = 0;

  if (doc == NULL || paragraph_number <= 0 ||
      paragraph_number > doc->number_of_paragraphs || line_number < 0 ||
      line_number >= doc->paragraphs[paragraph_number - 1].number_of_lines) {
    return FAILURE;
  }

  /* Starts traversing at the end and moves backwards to remove the line (shifts
   * the other lines too) */
  for (curr_line = line_number - 1;
       curr_line < doc->paragraphs[paragraph_number - 1].number_of_lines;
       curr_line++) {
    strcpy(doc->paragraphs[paragraph_number - 1].lines[curr_line],
           doc->paragraphs[paragraph_number - 1].lines[curr_line + 1]);
  }

  doc->paragraphs[paragraph_number - 1].number_of_lines--;

  return SUCCESS;
}

/* The function will add the first data_lines number of lines from the data
 * array to the document */
int load_document(Document *doc, char data[][MAX_STR_SIZE + 1],
                  int data_lines) {
  int curr_paragraph = 0, curr = 0;

  if (doc == NULL || data == NULL || data_lines == 0) {
    return FAILURE;
  }

  /* Function initializes first paragraph by default */
  add_paragraph_after(doc, 0);

  for (curr = 0; curr < data_lines; curr++) {
    /* If the current line is not empty, add it to the document */
    if (strlen(data[curr]) > 0) {
      add_line_after(doc, curr_paragraph + 1,
                     doc->paragraphs[curr_paragraph].number_of_lines,
                     data[curr]);
    } else { /* If the current line is empty, start a new paragraph */
      curr_paragraph++;
      add_paragraph_after(doc, curr_paragraph);
    }
  }

  return SUCCESS;
}

/* This is a helper function for the replace_text method which replaces a word
 * in a certain line */
static int replace_in_line(Document *doc, const char *target,
                           const char *replacement, int curr_para,
                           int curr_line) {
  char result[MAX_STR_SIZE + 1] = {0};
  char *pos = strstr(
      doc->paragraphs[curr_para].lines[curr_line],
      target); /* Finds the first occurence of target in the current line */

  while (pos != NULL) { /* Checks to see if target is in the string */
    strcpy(result, pos + strlen(target));
    strcpy(pos, replacement);
    strcat(pos, result);
    pos = strstr(pos + strlen(replacement), target);
  }

  return 0;
}

/* THe function will replace the text target with the text replacement
 * everywhere it appears in the document */
int replace_text(Document *doc, const char *target, const char *replacement) {
  int curr_line, curr_para = 0;

  if (doc == NULL || target == NULL || replacement == NULL) {
    return FAILURE;
  }

  /* Traverses through the lines of the document by traversing through
   * paragraphs and runs replace_in_line on each line */
  for (curr_para = 0; curr_para < doc->number_of_paragraphs; curr_para++) {
    for (curr_line = 0; curr_line < doc->paragraphs[curr_para].number_of_lines;
         curr_line++) {
      replace_in_line(doc, target, replacement, curr_para, curr_line);
    }
  }

  return SUCCESS;
}

/* Function higlights the text associated with target everywhere it appears in
 * the document by surrounding the text with the strings HIGHLIGHT_START_STR and
 * HIGLIGHT_END_STR */
int highlight_text(Document *doc, const char *target) {
  char temp[MAX_STR_SIZE + 1] = {0};

  if (doc == NULL || target == NULL) {
    return FAILURE;
  }

  /* Combines the strings such that HIGLIGHT_START_STR is outside, target is in
   * the middle, and HIGLIGHT_END_STR is at the end */
  strcat(temp, HIGHLIGHT_START_STR);
  strcat(temp, target);
  strcat(temp, HIGHLIGHT_END_STR);
  replace_text(doc, target, temp);

  return SUCCESS;
}

/* The function will remove the text target everywhere it appears in the
 * document */
int remove_text(Document *doc, const char *target) {
  const char *temp = "";

  if (doc == NULL || target == NULL) {
    return FAILURE;
  }

  /* Calls the replace_text method and replaces target with "" */
  replace_text(doc, target, temp);

  return SUCCESS;
}

/* This function will load data from a file. By default a paragraph will beadded
 * and any blank lines (line with only spaces as defined by isspace()) will mark
 * the beginning ofa new paragraph. */
int load_file(Document *doc, const char *filename) {
  FILE *input;
  char line[MAX_STR_SIZE + 1];
  int curr_paragraph = 0;

  if (doc == NULL || filename == NULL) {
    return FAILURE;
  }

  if ((input = fopen(filename, "r")) == NULL) {
    return FAILURE;
  } else {
    /* Function initializes first paragraph by default */
    add_paragraph_after(doc, 0);

    while (fgets(line, MAX_STR_SIZE + 1, input) != NULL) {
      if (isspace(line[0])) {
        curr_paragraph++;
        add_paragraph_after(doc, curr_paragraph);
      } else {
        if (strlen(line) > 0 && line[strlen(line) - 1] == '\n') {
          line[strlen(line) - 1] = '\0';
        }
        add_line_after(doc, curr_paragraph + 1,
                       doc->paragraphs[curr_paragraph].number_of_lines, line);
      }
    }
    fclose(input);
    return SUCCESS;
  }
}

/*  This function will print the paragraphsassociated with a document to the
 * specified file (overwriting the file). Each paragraph will be separatedby a
 * newline. */
int save_document(Document *doc, const char *filename) {
  FILE *output_stream;
  int curr_para = 0, curr_line = 0;

  if (doc == NULL || filename == NULL) {
    return FAILURE;
  }

  /* Output file */
  if ((output_stream = fopen(filename, "w")) == NULL) {
    return FAILURE;
  }

  for (curr_para = 0; curr_para < doc->number_of_paragraphs; curr_para++) {
    for (curr_line = 0; curr_line < doc->paragraphs[curr_para].number_of_lines;
         curr_line++) {
      fprintf(output_stream, "%s\n",
              doc->paragraphs[curr_para].lines[curr_line]);
    }

    fprintf(output_stream, "\n");
  }

  /* Closing stream */
  fclose(output_stream);

  return SUCCESS;
}