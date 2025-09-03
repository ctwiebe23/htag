#include <stdio.h>
#include <stdlib.h>

int strLen(char* str) {
    int n = 0;
    for (int i = 0; str[i] != 0; i++) {
        n++;
    }
    return n;
}

void readStdin() {
    int buf, prev = EOF;

    // `prev` stores previous character, is used as a buffer so that the
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

    int textPtr = 0;
    int textLen = 255;
    char* text = malloc(sizeof(char) * textLen);
    for (int i = 0; i < textLen; i++) {
        text[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        // is CLA long flag?
        if (clas[i][0] == '-' && clas[i][1] == '-' && clas[i][2] != 0) {
            if (i == n - 1) {
                fprintf(stderr, "error: attribute lacks value\n");
                exit(2);
            }

            printf(" %s=\"%s\"", clas[i] + 2, clas[i + 1]);
            i++;
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
        return 1;
    }

    char* tagName = argv[1];

    printf("<%s", tagName);

    char* text = readClas(argv + 2, argc - 2);
    printf(">");

    if (text == NULL) {
        readStdin();
    } else {
        printf("%s", text);
    }

    printf("</%s>\n", tagName);

    return 0;
}
