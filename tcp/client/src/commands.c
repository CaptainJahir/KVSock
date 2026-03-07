#include "commands.h"

/*

struct Request_Packet {
    char operation[16];
    char var_one_name[64];
    char var_one_data[256];
    char var_two_name[64];
    // char var_two_data[256];
};

*/

/* TODO: error handling in case of string copy failed then i need to handle errors i think */


bool handle_cmd_set(char *data, struct Request_Packet *packet) {
    memset(packet, 0, sizeof(*packet));
    char *operation = strtok(data, " ");
    strncpy(packet->operation, operation, sizeof(packet->operation)-1);

    bool is_int = false;
    if (strcasecmp(operation, "SET_NUM") == 0) {
        is_int = true;
    };
    
    char *var_name = strtok(NULL, " ");
    if (!is_valid_var_name(var_name)) {
        fprintf(stderr, "Invalid variable name: %s \n", var_name);
        return false;
    }
    strncpy(packet->var_one_name, var_name, sizeof(packet->var_one_name)-1);

    char *value = strtok(NULL, " ");
    if (is_int && get_token_type(value) != TOKEN_DATA) {
        fprintf(stderr, "Invalid data for the type Number name: %s \n", value);
        return false;
    }
    strncpy(packet->var_one_data, value, sizeof(packet->var_one_data)-1);
    return true;
}


bool handle_cmd_get(char *data, struct Request_Packet *packet) {
    memset(packet, 0, sizeof(*packet));
    char *operation = strtok(data, " ");
    strncpy(packet->operation, operation, sizeof(packet->operation)-1);
    
    char *var_name = strtok(NULL, " ");
    if (!is_valid_var_name(var_name)) {
        fprintf(stderr, "Invalid variable name: %s \n", var_name);
        return false;
    }
    strncpy(packet->var_one_name, var_name, sizeof(packet->var_one_name)-1);
    return true;
}


bool handle_cmd_list(char *data, struct Request_Packet *packet) {
    memset(packet, 0, sizeof(*packet));
    char *operation = strtok(data, " ");
    strncpy(packet->operation, operation, sizeof(packet->operation)-1);
    return true;
}

/* 

<OPR> <STOR_VAR> <VAR_ONE|DATA_ONE> <VAR_TWO|DATA_TWO> 
NOTE:
      For simplicity in this handle_cmd_arith function i am storing both the data or varible names in place of variable 
      names in the network packet.

*/

bool handle_cmd_arith(char *data, struct Request_Packet *packet) {
    memset(packet, 0, sizeof(*packet));
    char *operation = strtok(data, " ");
    strncpy(packet->operation, operation, sizeof(packet->operation)-1);

    char *store_var = strtok(NULL, " ");
    if (!is_valid_var_name(store_var)) return false;
    
    char *var_one_name = strtok(NULL, " ");
    strncpy(packet->var_one_name, var_one_name, sizeof(packet->var_one_name)-1);

    char *var_two_name = strtok(NULL, " ");
    strncpy(packet->var_two_name, var_two_name, sizeof(packet->var_two_name)-1);
    return true;
}