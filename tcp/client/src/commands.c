#include "commands.h"


 bool handle_cmd_set(Request_Packet *packet, char *tokens[], size_t token_sizes[]) {
    bool is_int = false;
    if (strcasecmp(tokens[0], "SET_NUM") == 0) {
        is_int = true;
        packet->operation = SET_NUM;
    } else if (strcasecmp(tokens[0], "SET_STR") == 0) {
        packet->operation = SET_STR;
    } else if (strcasecmp(tokens[0], "UPDATE") == 0) {
        packet->operation = UPDATE;
    } else {
        return false;
    }

    char *var_name = tokens[1];
    if (!is_valid_var_name(var_name)) {
        fprintf(stderr, "Invalid variable name: %s \n\n", var_name);
        return false;
    }
    packet->var_one_name_len = token_sizes[1];
    
    char *value = tokens[2];
    if (is_int && get_token_type(value) != TOKEN_DATA) {
        fprintf(stderr, "Invalid data for the type Number name: %s \n\n", value);
        return false;
    }

    packet->var_one_data_len = token_sizes[2];
    char *data_arr_ptr = packet->data;

    memcpy(data_arr_ptr, tokens[1], packet->var_one_name_len);
    data_arr_ptr += packet->var_one_name_len;

    memcpy(data_arr_ptr, tokens[2], packet->var_one_data_len);
    return true;
}

bool handle_cmd_get(Request_Packet *packet, char *tokens[], size_t token_sizes[]) {
    if (strcasecmp(tokens[0], "GET") == 0) {
        packet->operation = GET;
    } else if (strcasecmp(tokens[0], "DELETE") == 0) {
        packet->operation = DELETE;
    } else {
        return false;
    }
    
    char *var_name = tokens[1];
    if (!is_valid_var_name(var_name)) {
        fprintf(stderr, "Invalid variable name: %s \n\n", var_name);
        return false;
    }
    packet->var_one_name_len = token_sizes[1];
    memcpy(packet->data, tokens[1], packet->var_one_name_len);
    return true;
}


bool handle_cmd_list(Request_Packet *packet, char *tokens[], size_t token_sizes[]) {
    if (strcasecmp(tokens[0], "LIST") == 0) {
        packet->operation = LIST;
    } else if (strcasecmp(tokens[0], "DEL_ALL") == 0) {
        packet->operation = DEL_ALL;
    } else {
        return false;
    }
    return true;
}

/* 

<OPR> <STOR_VAR> <VAR_ONE|DATA_ONE> <VAR_TWO|DATA_TWO> 
NOTE:
      For simplicity in this handle_cmd_arith function i am storing both the data or varible names in place of variable 
      names in the network packet.

*/

bool handle_cmd_arith(Request_Packet *packet, char *tokens[], size_t token_sizes[]) {
    if (strcasecmp(tokens[0], "ADD") == 0) {
        packet->operation = ADD;
    } else if (strcasecmp(tokens[0], "SUB") == 0) {
        packet->operation = SUB;
    } else if (strcasecmp(tokens[0], "MUL") == 0) {
        packet->operation = MUL;
    } else if (strcasecmp(tokens[0], "DIV") == 0) {
        packet->operation = DIV;
    } else {
        return false;
    }

    if (!is_valid_var_name(tokens[1])) return false;

    char *data_arr_ptr = packet->data;

    packet->arith_store_var_len = token_sizes[1];
    memcpy(data_arr_ptr, tokens[1], packet->arith_store_var_len);
    data_arr_ptr += packet->arith_store_var_len;

    packet->var_one_name_len = token_sizes[2];
    memcpy(data_arr_ptr, tokens[2], packet->var_one_name_len);
    data_arr_ptr += packet->var_one_name_len;

    packet->var_two_name_len = token_sizes[3];
    memcpy(data_arr_ptr, tokens[3], packet->var_two_name_len);

    return true;
}