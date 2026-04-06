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

struct command command_types[] = {
    {"SET_STR", CMD_SET},
    {"SET_NUM", CMD_SET},
    {"UPDATE", CMD_SET},
    {"GET", CMD_GET},
    {"DELETE", CMD_GET},
    {"DEL_ALL", CMD_LIST},
    {"LIST", CMD_LIST},
    {"ADD", CMD_ARITH},
    {"SUB", CMD_ARITH},
    {"MUL", CMD_ARITH},
    {"DIV", CMD_ARITH},
};

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

enum command_type check_operation_type(const char *cmd) {
    for (int i = 0; i < sizeof(command_types)/sizeof(command_types[0]); i++) {
        if (strcasecmp(cmd, command_types[i].name) == 0) {
            return command_types[i].type;
        }
    }
    return CMD_UNKNOWN;
}

int count_token (const char data[]) {
    int count = 0;
    int in_word = 0;
    for (int i = 0; data[i] != '\0'; i++) {
        if (data[i] != ' ' && in_word == 0) {
            count++;
            in_word = 1;
        }else if (data[i] == ' ') {
            in_word = 0;
        }   
    }   
    return count;
}

char* trim(char *str) {
    if (str == NULL || str[0] == '\0') return str;
    
    while (*str == ' ') str++;
    int len = strlen(str);
    if (len == 0) return str;
    
    while (len > 0 && str[len-1] == ' ') len--;
    str[len] = '\0';
    return str;
}

int tokens_extractor(char *inp, char *tokens[], size_t token_sizes[], int max_tokens) {
    if (!inp || !*inp)
        return 0;
    /* 
        NOTE: trimmed data will be passed into the function so i am not handling trim separately
        To handle cases such as `SET_STR|UPDATE varName lorem ipusm dollar sit emit ...`
        we'll extract util SET_STR | UPDATE varName and then trim the remaining part directly into message
        the remaing operations doesn't need this because in their data part there is no need for spaces
    */

    char* ptr = inp;
    char* first_word_start = ptr;
    while (*ptr != '\0' && !isspace(*ptr)) ptr++;
    char* first_word_end = ptr;
    int count = 0;
    size_t first_word_len = first_word_end - first_word_start;
    tokens[0] = malloc(first_word_len + 1);
    memcpy(tokens[0], first_word_start, first_word_len);
    tokens[0][first_word_len] = '\0';

    if (strcasecmp(tokens[0], "SET_STR") == 0 || strcasecmp(tokens[0], "UPDATE") == 0) {
        while (*ptr != '\0' && isspace(*ptr)) ptr++;

        char* second_word_start = ptr;
        while (*ptr != '\0' && !isspace(*ptr)) ptr++;
        char* second_word_end = ptr;

        while (*ptr != '\0' && isspace(*ptr)) ptr++;

        char* remaing_data_start = ptr;
        char* remaing_data_end = inp + strlen(inp);

        size_t second_word_len = second_word_end - second_word_start;
        size_t remaing_len = remaing_data_end - remaing_data_start;
        
        tokens[1] = malloc(second_word_len + 1);
        tokens[2] = malloc(remaing_len + 1);

        memcpy(tokens[1], second_word_start, second_word_len);
        tokens[1][second_word_len] = '\0';
        memcpy(tokens[2], remaing_data_start, remaing_len);
        tokens[2][remaing_len] = '\0';

        /*  uint8_t -> opr && uint8_t -> var_name && uint16_t -> data */
        if (first_word_len <= 255 && second_word_len <= 255 && remaing_len <= 65535) {
            token_sizes[0] = (int) first_word_len;
            token_sizes[1] = (int) second_word_len;
            token_sizes[2] = (int) remaing_len;
        } else {
            fprintf(stderr, "Error: value too large to fit in int\n");
            return -1;
        }
        
        count = 3;
    } else {
        char *token = strtok(inp, " ");
        while (token != NULL && count < max_tokens) {
            tokens[count] = token;
            token_sizes[count] = strlen(token);
            token = strtok(NULL, " ");
            count++;
        }
    }
    return count;
}

char* to_upper_case(char *data) {
    char *start = data;
    while (*data != '\0') {
        *data = toupper((unsigned char) *data);
        data++;
    }
    return start;
}
