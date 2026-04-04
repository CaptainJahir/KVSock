#include "arithmetic.h"

int store_arithmetic_result_and_respond(int socket, Hash_Table* table, long double val, char* store_var) {
    char* message;
    char* arith_store_val;

    int val_size = snprintf(NULL, 0, "%Lf", val);
    if (val_size < 0) {
        message = "Failed to save data";
        send_header_response(socket, 0, message, FAILURE);
        return -1;
    }
        
    arith_store_val = malloc(val_size + 1);
    if (arith_store_val == NULL){
        message = "Failed to save data";
        send_header_response(socket, 0, message, FAILURE);
        return -1;
    }
        
    int copy = snprintf(arith_store_val, val_size+1, "%Lf", val);
    if (copy < 0) {
        free(arith_store_val);
        message = "Failed to save data";
        send_header_response(socket, 0, message, FAILURE);
        return -1;
    }

    if (search(table, store_var)) {
        update(table, store_var, arith_store_val);
    } else {
        insert(table, store_var, arith_store_val);
    }
        
    int size = snprintf(NULL, 0, "successfully set %s to %s", store_var, arith_store_val);
    if (size < 0){
        free(arith_store_val);
        return -1;
    };
            
    message = malloc(size+1);
    if (message == NULL) {
        free(arith_store_val);
        return -1;
    };
            
    snprintf(message, size + 1, "successfully set %s to %s", store_var, arith_store_val);
    send_header_response(socket, 0, message, SUCCESS);

    free(message);
    free(arith_store_val);
    return 0;
}

int handle_cmd_arith(int socket, Request_Packet *req, Hash_Table *table) {
    char store_var[req->arith_store_var_len+1];
    char arg_one[req->var_one_name_len+1];
    char arg_two[req->var_two_name_len+1];
    char* message;

    memcpy(store_var, req->data, req->arith_store_var_len);
    memcpy(arg_one, req->data + req->arith_store_var_len, req->var_one_name_len);
    memcpy(arg_two, req->data + req->arith_store_var_len + req->var_one_name_len, req->var_two_name_len);

    store_var[req->arith_store_var_len] = '\0';
    arg_one[req->var_one_name_len] = '\0';
    arg_two[req->var_two_name_len] = '\0';
    
    if (store_var[0] == '\0' || arg_one[0] == '\0' || arg_two[0] == '\0') {
        message = "Empty key or values are received";
        send_header_response(socket, 0, message, FAILURE);
        return -1;
    }
    
    if (!is_valid_var_name(store_var)) {
        message = "Invalid variable name received";
        send_header_response(socket, 0, message, FAILURE);
        return -1;
    }

    char* arg_one_val;
    char* arg_two_val;
    
    arg_one_val = is_valid_var_name(arg_one) ? search(table, arg_one) : arg_one;
    
    /* TOKEN_DATA => string of TYPE double */
    
    if (arg_one_val == NULL || get_token_type(arg_one_val) != TOKEN_DATA) {
        message = "Invalid variable type received";
        send_header_response(socket, 0, message, FAILURE);
        return -1;    
    }
    
    arg_two_val = is_valid_var_name(arg_two) ? search(table, arg_two) : arg_two;
    
    if (arg_two_val == NULL || get_token_type(arg_two_val) != TOKEN_DATA) {
        message = "Invalid variable type received";
        send_header_response(socket, 0, message, FAILURE);
        return -1;
    }
    

    if (req->operation == ADD) {
        long double val = strtold(arg_one_val, NULL) + strtold(arg_two_val, NULL);
        int success = store_arithmetic_result_and_respond(socket, table, val, store_var);
        if (success < 0) {
            fprintf(stderr, "Failed to save and generate response\n");
            return -1;
        }

        return 0;
    } else if (req->operation == SUB) {
        long double val = strtold(arg_one_val, NULL) - strtold(arg_two_val, NULL);
        int success = store_arithmetic_result_and_respond(socket, table, val, store_var);
        if (success < 0) {
            fprintf(stderr, "Failed to save and generate response\n");
            return -1;
        }

        return 0;
    } else if (req->operation == MUL) {
        long double val = strtold(arg_one_val, NULL) * strtold(arg_two_val, NULL);
        int success = store_arithmetic_result_and_respond(socket, table, val, store_var);
        if (success < 0) {
            fprintf(stderr, "Failed to save and generate response\n");
            return -1;
        }

        return 0;
    } else if (req->operation == DIV) {
        if (arg_two_val == 0) {
            message = "Cannot divide with zero";
            send_header_response(socket, 0, message, FAILURE);
            return -1;    
        }
         
        long double val = strtold(arg_one_val, NULL) / strtold(arg_two_val, NULL);
        int success = store_arithmetic_result_and_respond(socket, table, val, store_var);
        if (success < 0) {
            fprintf(stderr, "Failed to save and generate response\n");
            return -1;
        }
        
        return 0;
    }

    message = "Invalid operation selected";
    send_header_response(socket, 0, message, FAILURE);
    return -1;
}
