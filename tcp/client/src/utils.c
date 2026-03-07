#include "utils.h"


bool is_valid_var_name(const char *token) {
    const char *keywords[] = {
        "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum",
        "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed",
        "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
    };

    if (token == NULL || *token == '\0') return false;
    if (!(isalpha((unsigned char)token[0]) || token[0] == '_')) return false;
    for (int i = 1; token[i] != '\0' ; i++) {
        if (token[i] == '-' && token[i+1] == '\0') return false;
        if (!(isalnum((unsigned char)token[i]) || token[i] == '_' || token[i] == '-')) return false;
    }

    int keyword_count = sizeof(keywords)/sizeof(keywords[0]);
    for (int i = 0; i < keyword_count; i++) {
        if (strcmp(keywords[i], token) == 0) return false;
    }

    return true;
}

enum token_type get_token_type(const char *token) {
    errno = 0;
    char *end;
    double value = strtod(token, &end);
    if (token == end) return TOKEN_VARIABLE;
    if (errno == ERANGE) return TOKEN_ERR;
    if (*end != '\0') return TOKEN_VARIABLE;
    if (!isfinite(value)) return TOKEN_INF;
    return TOKEN_DATA;
}

