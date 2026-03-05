#include "main.h"

enum command_type check_operation_type(const char *cmd) {
    for (int i = 0; i < sizeof(command_types)/sizeof(command_types[0]); i++) {
        if (strcasecmp(cmd, command_types[i].name) == 0) {
            return command_types[i].type;
        }
    }
    return CMD_UNKNOWN;
}

int count_token (char data[]) {
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

int main (int argc, char *argv[]) {
    while (1) {
        struct Request_Packet req;
        memset(&req, 0, sizeof(req));
        char inp[MAX_INP_SIZE];
        memset(inp, 0, sizeof(inp));

        fgets(inp, sizeof(inp), stdin);
        inp[strcspn(inp, "\n")] = '\0'; /* removing the '\n' character from the input because while comparing it's interfering */
        
        char *trimmed = inp;
        while (*trimmed == ' ') trimmed++;
        int inp_end = strlen(trimmed)-1;
        while (inp_end > 0 && trimmed[inp_end] == ' ') inp_end--;
        trimmed[inp_end+1] = '\0';

        if (*trimmed == '\0') continue;
        if (strcasecmp(trimmed, "exit") == 0) return 0;
        char copy_data[MAX_INP_SIZE];
        memset(&copy_data, 0, sizeof(copy_data));
        strcpy(copy_data, trimmed);
        int word_count = count_token(copy_data);

        char *token = strtok(copy_data, " ");

        switch (check_operation_type(token)) {
            case CMD_SET:
                if (word_count != 3) {
                    fprintf(stderr, "Usage: <SET_STR|SET_INT|UPDATE> <var_name> <value>\n");
                    continue;
                } else {
                    strncpy(req.operation, token, sizeof(req.operation)-1);
                    strncpy(req.var_one_name, strtok(NULL, " "), sizeof(req.var_one_name)-1);
                    strncpy(req.var_one_data, strtok(NULL, " "), sizeof(req.var_one_data)-1);
                }
                break;
            case CMD_GET:
                if (word_count != 2) {
                    fprintf(stderr, "Usage: <GET|DELETE> <var_name>\n");
                    continue;
                } else {
                    strncpy(req.operation, token, sizeof(req.operation)-1);
                    strncpy(req.var_one_name, strtok(NULL, " "), sizeof(req.var_one_name)-1);
                }
                break;
            case CMD_LIST:
                if (word_count != 1) {
                    fprintf(stderr, "Usage: <LIST>\n");
                    continue;
                } else {
                    strncpy(req.operation, token, sizeof(req.operation)-1);
                }
                break;
            default:
                fprintf(stderr, "Unknow command\n");
                continue;
                break;
        }
        printf("\n");
    }
    return 0;
}

/*

// TODO: this print statements are for testing 

printf("operation      : %s\n", req.operation);
printf("var_one_name   : %s\n", req.var_one_name);
printf("var_one_data   : %s\n", req.var_one_data);
printf("var_two_name   : %s\n", req.var_two_name);
printf("var_two_data   : %s\n", req.var_two_data);

*/