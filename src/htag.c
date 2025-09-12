#include <stdio.h>
#include <stdlib.h>

// boolean "type"
#define BOOL  char
#define TRUE  1
#define FALSE 0

// return codes
#define RETCODE_OK            0
#define RETCODE_NO_ATTR_VALUE 1

// constants
#define INITIAL_TEXT_LENGTH 255  // initial size of the allocated string

#define VERSION "htag 1.2.0\n"

#define HELP_MESSAGE                                                          \
    VERSION                                                                   \
    "  Create html in the shell\n"                                            \
    "\n"                                                                      \
    "usage\n"                                                                 \
    "  htag tag-name [--attr-name attr-val]... [text-content]...\n"           \
    "\n"                                                                      \
    "options\n"                                                               \
    "  -h, --help\n"                                                          \
    "       Print this message\n"                                             \
    "  -V, --version\n"                                                       \
    "       Print the program's version\n"                                    \
    "  --   Parse flags like text after this is read\n"                       \
    "\n"                                                                      \
    "return codes\n"                                                          \
    "  0    Returned on success\n"                                            \
    "  1    Returned when an attribute is not given a value\n"                \
    "\n"                                                                      \
    "README can be found at https://ctwiebe23.github.io/htag\n"

/**
 * Gets the length of a null-terminated string.
 * @param str Pointer to the string.
 * @return The length of the string.  0 is the pointer is null.
 */
int strLen(char* str)
{
    if (str == NULL) {
        return 0;
    }

    int n = 0;

    for (int i = 0; str[i] != 0; i++) {
        n++;
    }

    return n;
}

/**
 * Tests whether or not the given null-terminated strings are equal.
 * @param a The first string.
 * @param b The second string.
 * @return Whether or not the strings are equal.  If either or both of the
 * pointers are null, returns false.
 */
BOOL strEq(char* a, char* b)
{
    if (a == NULL || b == NULL) {
        return FALSE;
    }

    for (int i = 0; TRUE; i++) {
        if (a[i] != b[i]) {
            return FALSE;
        }

        // since we know that both characters are equal, then if a[i] is 0 so
        // is b[i]
        if (a[i] == 0) {
            return TRUE;
        }
    }
}

/**
 * Reads input from the STDIN and prints it to the STDOUT.
 */
void readAndPrintStdin()
{
    int buf, prev = EOF;

    // prev stores previous character, is used as a buffer so that the
    // last character isn't printed
    while (EOF != (buf = fgetc(stdin))) {
        if (prev != EOF) {
            printf("%c", prev);
        }

        prev = buf;
    }

    // print trailing character ONLY IF it isn't a newline
    if (prev != '\n' && prev != EOF) {
        printf("%c", prev);
    }
}

/**
 * Reads the command line arguments excluding the program name and tag name.
 * Prints all attributes to the STDOUT.
 * @param bodyArgs The command line arguments sans program and tag name.
 * @param n The number of command line arguments.
 * @return The body text if any is found, else null.
 */
char* readBodyArgsAndPrintAttributes(char** bodyArgs, int n)
{
    BOOL noBodyText = TRUE;
    BOOL canReadAttrs = TRUE;

    int textPtr = 0;
    int textLen = INITIAL_TEXT_LENGTH;
    char* text = malloc(sizeof(char) * textLen);
    text[0] = 0;  // null-terminating character

    for (int i = 0; i < n; i++) {
        // is CLA long flag?
        if (canReadAttrs && bodyArgs[i][0] == '-' && bodyArgs[i][1] == '-') {
            if (bodyArgs[i][2] == 0) {
                // the flag is "--"
                canReadAttrs = FALSE;
            } else {
                if (i == n - 1) {
                    fprintf(stderr, "error: attribute lacks value\n");
                    exit(RETCODE_NO_ATTR_VALUE);
                }

                printf(" %s=\"%s\"", bodyArgs[i] + 2, bodyArgs[i + 1]);

                i++;  // next CLA (value) is already consumed here
            }

            continue;
        }

        int m = strLen(bodyArgs[i]);

        if (textPtr + m >= textLen) {
            text = realloc(text, textLen *= 2);
        }

        if (noBodyText) {
            noBodyText = FALSE;
        } else if (m != 0) {
            text[textPtr++] = ' ';
        }

        for (int k = 0; k < m; k++) {
            text[textPtr++] = bodyArgs[i][k];
        }

        // end with null character
        text[textPtr] = 0;
    }

    return noBodyText ? NULL : text;
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf(HELP_MESSAGE);
        return RETCODE_OK;
    }

    char* tagName = argv[1];

    if (strEq(tagName, "-h") || strEq(tagName, "--help")) {
        printf(HELP_MESSAGE);
        return RETCODE_OK;
    }

    if (strEq(tagName, "-V") || strEq(tagName, "--version")) {
        printf(VERSION);
        return RETCODE_OK;
    }

    printf("<%s", tagName);
    char* text = readBodyArgsAndPrintAttributes(argv + 2, argc - 2);
    printf(">");

    if (text == NULL) {
        readAndPrintStdin();
    } else {
        printf("%s", text);
    }

    printf("</%s>\n", tagName);

    return RETCODE_OK;
}
