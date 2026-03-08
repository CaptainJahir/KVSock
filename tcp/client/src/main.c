#include "main.h"



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

        /* this data will be passed into command handling functions */
        char packet_input[MAX_INP_SIZE];
        memset(&packet_input, 0, sizeof(packet_input));
        strcpy(packet_input, trimmed);

        bool success;
        switch (check_operation_type(token)) {
            case CMD_SET:
                if (word_count != 3) {
                    fprintf(stderr, "Usage: <SET_STR|SET_NUM|UPDATE> <var_name> <value>\n");
                    continue;
                } else {
                    success = handle_cmd_set(packet_input, &req);
                    if (!success) continue;
                }
                break;
            case CMD_GET:
                if (word_count != 2) {
                    fprintf(stderr, "Usage: <GET|DELETE> <var_name>\n");
                    continue;
                } else {
                    success = handle_cmd_get(packet_input, &req);
                    if (!success) continue;
                }
                break;
            case CMD_LIST:
                if (word_count != 1) {
                    fprintf(stderr, "Usage: <LIST>\n");
                    continue;
                } else {
                    success = handle_cmd_list(packet_input, &req);
                    if (!success) continue;
                }
                break;
            case CMD_ARITH:
                if (word_count != 4) {
                    fprintf(stderr, "Usage: <OPR> <STOR_VAR> <VAR_ONE|DATA_ONE> <VAR_TWO|DATA_TWO>\n");
                    continue;
                } else {
                    success = handle_cmd_arith(packet_input, &req);
                    if (!success) continue;
                }
                break;
            default:
                fprintf(stderr, "Unknow command\n");
                continue;
                break;
        }
        send_data(&req);
        printf("\n");
    }
    return 0;
}


/*

printf("operation      : %s\n", req.operation);
printf("var_one_name   : %s\n", req.var_one_name);
printf("var_one_data   : %s\n", req.var_one_data);
printf("var_two_name   : %s\n", req.var_two_name);

*/