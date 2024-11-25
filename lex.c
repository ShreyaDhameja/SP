#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100

bool isDelimiter(char chr) {
    return strchr(" +-*/%,;><=()[]{}", chr) != NULL;
}

bool isOperator(char chr) {
    return strchr("+-*/><=", chr) != NULL;
}

bool isValidIdentifier(char* str) {
    return !isdigit(str[0]) && !isDelimiter(str[0]);
}

bool isKeyword(char* str) {
    const char* keywords[] = {
        "auto", "break", "case", "char", "const", "continue", "default", "do",
        "double", "else", "enum", "extern", "float", "for", "goto", "if", "int",
        "long", "register", "return", "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
    };
    int i;
    for (i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(str, keywords[i]) == 0) return true;
    }
    return false;
}

bool isInteger(char* str) {
	int i;
    for (i = 0; str[i]; i++) {
        if (!isdigit(str[i])) return false;
    }
    return *str != '\0';
}

void analyzeToken(char* str) {
    if (isKeyword(str))
        printf("Token: Keyword, Value: %s\n", str);
    else if (isInteger(str))
        printf("Token: Integer, Value: %s\n", str);
    else if (isValidIdentifier(str))
        printf("Token: Identifier, Value: %s\n", str);
    else
        printf("Token: Unidentified, Value: %s\n", str);
}

void lexicalAnalyzer(char* input) {
    int left = 0, right = 0, len = strlen(input);

    while (right <= len) {
        if (!isDelimiter(input[right]) && input[right] != '\0') {
            right++;
        } else {
            if (left != right) {
                char temp = input[right];
                input[right] = '\0';
                analyzeToken(&input[left]);
                input[right] = temp;
            }
            if (input[right] != '\0' && isOperator(input[right]))
                printf("Token: Operator, Value: %c\n", input[right]);
            right++;
            left = right;
        }
    }
}

int main() {
    char lex_input[MAX_LENGTH];

    printf("Enter the expression to analyze: ");
    fgets(lex_input, MAX_LENGTH, stdin);

    lex_input[strcspn(lex_input, "\n")] = '\0';

    printf("For Expression \"%s\":\n", lex_input);
    lexicalAnalyzer(lex_input);

    return 0;
}
