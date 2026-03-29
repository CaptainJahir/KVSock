#include "main.h"



int main (int argc, char *argv[]) {
    while (1) {
        char *inp = NULL;
        size_t inp_size = 0;
        ssize_t nread;
        nread = getline(&inp, &inp_size, stdin);
        if (nread < 0) {
            if (feof(stdin)) {
                free(inp);
                break;
            } else {
                free(inp);
                perror("Failed to take input");
                break;
            }
        }
        inp[strcspn(inp, "\n")] = '\0'; /* removing the '\n' character from the input because while comparing it's interfering */

        char *trimmed = trim(inp);
        if (*trimmed == '\0') continue;
        if (strcasecmp(trimmed, "exit") == 0) return 0;
        char copy_data[strlen(trimmed)+1];
        strcpy(copy_data, trimmed);
        int max_tokens = 4;
        char *tokens[max_tokens];
        size_t token_sizes[max_tokens];
        int token_count = tokens_extractor(copy_data, tokens, token_sizes, max_tokens);
        
        size_t dynamic_arr_size = 0;
        for (int i = 1; i < token_count; i++) { // exclude 0 because it's operation and we won't be adding it to the array
            dynamic_arr_size += token_sizes[i] + 1;
        }
        
        Request_Packet *req = calloc(1, sizeof(Request_Packet) + dynamic_arr_size);

        bool success;
        switch (check_operation_type(tokens[0])) {
            case CMD_SET:
                if (token_count != 3) {
                    fprintf(stderr, "Usage: <SET_STR|SET_NUM|UPDATE> <var_name> <value>\n\n");
                    continue;
                } else {
                    success = handle_cmd_set(req, tokens, token_sizes);
                    if (!success) continue;
                }
                break;
            case CMD_GET:
                if (token_count != 2) {
                    fprintf(stderr, "Usage: <GET|DELETE> <var_name>\n\n");
                    continue;
                } else {
                    success = handle_cmd_get(req, tokens, token_sizes);
                    if (!success) continue;
                }
                break;
            case CMD_LIST:
                if (token_count != 1) {
                    fprintf(stderr, "Usage: <LIST|DEL_ALL>\n\n");
                    continue;
                } else {
                    success = handle_cmd_list(req, tokens, token_sizes);
                    if (!success) continue;
                }
                break;
            case CMD_ARITH:
                if (token_count != 4) {
                    fprintf(stderr, "Usage: <OPR> <STOR_VAR> <VAR_ONE|DATA_ONE> <VAR_TWO|DATA_TWO>\n\n");
                    continue;
                } else {
                    success = handle_cmd_arith(req, tokens, token_sizes);
                    if (!success) continue;
                }
                break;
            default:
                fprintf(stderr, "Unknow command\n\n");
                continue;
                break;
        }
        // send_data(&req);
        // printf("\n");
        printf("\n");
        free(inp);
    }
    return 0;
}
