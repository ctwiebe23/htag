#include <stdio.h>
#include <stdlib.h>

#define RETCODE_OK 0
#define RETCODE_NO_TAG_NAME 1
#define RETCODE_NO_ATTR_VALUE 2

#define HELP_MESSAGE \
    "htag\n" \
    "  Create html in the shell\n" \
    "\n" \
    "usage\n" \
    "  htag tag-name [--attr-name attr-val]... [text-content]...\n" \
    "\n" \
    "options\n" \
    "  -h       Print this message\n" \
    "  --       Parse flags like text after this is read\n" \
    "\n" \
    "return codes\n" \
    "  0        Returned on success\n" \
    "  1        Returned when no tag name is provided\n" \
    "  2        Returned when an attribute is not given a value\n" \
    "\n" \
    "README can be found at https://github.com/ctwiebe23/htag\n"

int strLen(char* str) {
    int n = 0;
    for (int i = 0; str[i] != 0; i++) {
        n++;
    }
    return n;
}

void readStdin() {
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

char* readClas(char** clas, int n) {
    char hasNotPrinted = 1;
    char canReadAttributes = 1;

    int textPtr = 0;
    int textLen = 255;
    char* text = malloc(sizeof(char) * textLen);
    for (int i = 0; i < textLen; i++) {
        text[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        // is CLA long flag?
        if (canReadAttributes && clas[i][0] == '-' && clas[i][1] == '-') {
            if (clas[i][2] == 0) {
                canReadAttributes = 0;
            } else {
                if (i == n - 1) {
                    fprintf(stderr, "error: attribute lacks value\n");
                    exit(RETCODE_NO_ATTR_VALUE);
                }

                printf(" %s=\"%s\"", clas[i] + 2, clas[i + 1]);

                i++;  // next CLA (value) is already consumed here
            }

            continue;
        }

        int n = strLen(clas[i]);

        if (textPtr + n >= textLen) {
            text = realloc(text, textLen *= 2);
        }

        if (hasNotPrinted) {
            hasNotPrinted = 0;
        } else if (n != 0) {
            text[textPtr++] = ' ';
        }

        for (int k = 0; k < n; k++) {
            text[textPtr++] = clas[i][k];
        }

        text[textPtr] = 0;
    }

    return (hasNotPrinted) ? NULL : text;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "error: no tag name given\n");
        return RETCODE_NO_TAG_NAME;
    }

    char* tagName = argv[1];

    if (tagName[0] == '-' && tagName[1] == 'h' && tagName[2] == 0) {
        printf(HELP_MESSAGE);
        return RETCODE_OK;
    }

    printf("<%s", tagName);

    char* text = readClas(argv + 2, argc - 2);
    // attributes are printed by readClas
    printf(">");

    if (text == NULL) {
        readStdin();
    } else {
        printf("%s", text);
    }

    printf("</%s>\n", tagName);

    return RETCODE_OK;
}
