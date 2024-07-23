# Document Manager

This code implements a document manager in C, allowing you to perform various operations such as adding paragraphs and lines, replacing text, and editing documents. Here's a detailed description:

### Header Inclusions and Declarations

- `#include "document.h"`: Includes the header file containing the `Document` structure and related constants.
- `#include <ctype.h>`: Includes functions to test and map characters, such as `isspace`.
- `#include <stdio.h>`: Includes input and output functions.
- `#include <string.h>`: Includes functions to handle strings.

### Helper Function

- `static int replace_in_line(Document *doc, const char *target, const char *replacement, int curr_para, int curr_line)`: This helper function replaces occurrences of `target` with `replacement` in a specific line of the document.

### Initialization and Reset Functions

- `int init_document(Document *doc, const char *name)`: Initializes a document, setting its name and ensuring it's empty.
  - Checks if the `doc` or `name` is `NULL` and if the name length exceeds `MAX_STR_SIZE`.
  - Sets `number_of_paragraphs` to 0 and copies the name into the document's name field.

- `int reset_document(Document *doc)`: Resets the document by setting the number of paragraphs to 0.
  - Checks if the `doc` is `NULL`.

### Printing Function

- `int print_document(Document *doc)`: Prints the document's name, number of paragraphs, and the content of each paragraph.
  - Iterates through paragraphs and lines to print the document's content.

### Adding Functions

- `int add_paragraph_after(Document *doc, int paragraph_number)`: Adds a paragraph after the specified paragraph number.
  - Checks for valid parameters and shifts existing paragraphs as needed.

- `int add_line_after(Document *doc, int paragraph_number, int line_number, const char *new_line)`: Adds a line after the specified line number in the specified paragraph.
  - Checks for valid parameters and shifts existing lines within the paragraph as needed.

- `int append_line(Document *doc, int paragraph_number, const char *new_line)`: Appends a line to the specified paragraph.
  - Calls `add_line_after` to add the new line at the end of the paragraph.

### Removing Function

- `int remove_line(Document *doc, int paragraph_number, int line_number)`: Removes the specified line from the paragraph.
  - Checks for valid parameters and shifts remaining lines up to fill the gap.

### Utility Functions

- `int get_number_lines_paragraph(Document *doc, int paragraph_number, int *number_of_lines)`: Returns the number of lines in a specified paragraph.
  - Checks for valid parameters and assigns the number of lines to `number_of_lines`.

- `int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], int data_lines)`: Loads data into the document from an array of strings.
  - Adds a new paragraph and lines based on the provided data, handling empty lines as paragraph separators.

### Text Manipulation Functions

- `int replace_text(Document *doc, const char *target, const char *replacement)`: Replaces all occurrences of `target` with `replacement` in the document.
  - Iterates through paragraphs and lines, using `replace_in_line` to perform replacements.

- `int highlight_text(Document *doc, const char *target)`: Highlights the specified `target` text by surrounding it with highlight markers.
  - Constructs a highlighted version of the target and calls `replace_text`.

- `int remove_text(Document *doc, const char *target)`: Removes all occurrences of `target` from the document.
  - Calls `replace_text` with an empty string as the replacement.

### File Handling Functions

- `int load_file(Document *doc, const char *filename)`: Loads document content from a file.
  - Reads lines from the file and adds them to the document, handling blank lines as paragraph separators.

- `int save_document(Document *doc, const char *filename)`: Saves the document's content to a file.
  - Writes paragraphs and lines to the file, separating paragraphs with blank lines.

### Summary

This code provides a comprehensive document management system with capabilities to initialize, reset, print, add, remove, and manipulate text within a document. It also includes functions to load and save document content to and from files, making it versatile for various text editing tasks.
