#include "crud.h"


int handle_cmd_set(int socket, Request_Packet *req, Hash_Table *table) {
    char key[req->var_one_name_len+1];
    char value[req->var_one_data_len+1];
    char* message;

    memcpy(key, req->data, req->var_one_name_len);
    memcpy(value, req->data + req->var_one_name_len, req->var_one_data_len);

    key[req->var_one_name_len] = '\0';
    value[req->var_one_data_len] = '\0';
    
    if (!key || !value) {
        message = "Empty key and values are received";
        send_header_response(socket, 0, message, FAILURE);
        return -1;
    }
    
    if (!is_valid_var_name(key)) {
        message = "Invalid variable name received";
        send_header_response(socket, 0, message, FAILURE);
        return -1;
    }

    if (req->operation == SET_STR) {
        if (insert(table, key, value)) {
            size_t size = snprintf(NULL, 0, "successfully set %s to %s", key, value);
            if (size < 0) return -1;
            
            message = malloc(size+1);
            if (message == NULL) return -1;
            
            snprintf(message, size + 1, "successfully set %s to %s", key, value);
            send_header_response(socket, 0, message, SUCCESS);

            free(message);
            return 0;
        }else {
            message = "Failed to save data";
            send_header_response(socket, 0, message, FAILURE);
            return -1;
        }
    } else if (req->operation == SET_NUM) {
        if (get_token_type(value) != TOKEN_DATA) {
            message = "Invalid data type received";
            send_header_response(socket, 0, message, FAILURE);
            return -1;
        }

        if (insert(table, key, value)) {
            size_t size = snprintf(NULL, 0, "successfully set %s to %s", key, value);
            if (size < 0) return -1;
            
            message = malloc(size+1);
            if (message == NULL) return -1;
            
            snprintf(message, size + 1, "successfully set %s to %s", key, value);
            send_header_response(socket, 0, message, SUCCESS);

            free(message);
            return 0;
        } else {
            message = "Failed to save data";
            send_header_response(socket, 0, message, FAILURE);
            return -1;
        }
    } else if (req->operation == UPDATE) {
        char *old_val = search(table, key);
        if (!old_val) {
            message = "key not found";
            send_header_response(socket, 0, message, FAILURE);
            return -1;
        }
        
        if (get_token_type(old_val) == TOKEN_DATA && get_token_type(value) != TOKEN_DATA) {
            message = "Invalid type for the given key";
            send_header_response(socket, 0, message, FAILURE);
            return -1;
        }

        if (update(table, key, value)) {
            size_t size = snprintf(NULL, 0, "successfully set %s to %s", key, value);
            if (size < 0) return -1;
            
            message = malloc(size+1);
            if (message == NULL) return -1;
            
            snprintf(message, size + 1, "successfully set %s to %s", key, value);
            send_header_response(socket, 0, message, SUCCESS);

            free(message);
            return 0;
        } else {
            message = "Failed to update the given key";
            send_header_response(socket, 0, message, FAILURE);
            return -1;
        }
    }

    message = "Invalid operation selected";
    send_header_response(socket, 0, message, FAILURE);
    return -1;
}

int handle_cmd_get(int socket, Request_Packet *req, Hash_Table *table) {
    // <OPR> <VAR_NAME>

    char key[req->var_one_name_len+1];
    memcpy(key, req->data, req->var_one_name_len);
    key[req->var_one_name_len] = '\0';
    char* message;

    if (!key) {
        message = "Empty key received";
        send_header_response(socket, 0, message, FAILURE);
        return -1;
    }
    
    if (!is_valid_var_name(key)) {
        message = "Invalid key received";
        send_header_response(socket, 0, message, FAILURE);
        return -1;
    }

    if (req->operation == GET) {
        char *value = search(table, key);
        if (value == NULL) {
            message = "key not found";
            send_header_response(socket, 0, message, FAILURE);
            return -1;
        }

        size_t size = snprintf(NULL, 0, "%s = %s", key, value);
        if (size < 0) return -1;
            
        message = malloc(size+1);
        if (message == NULL) return -1;
        
        snprintf(message, size + 1, "%s = %s", key, value);
        send_header_response(socket, 0, message, SUCCESS);
        free(message);
        return 0;
    } else if (req->operation == DELETE) {
        char *value = search(table, key);
        if (value == NULL) {
            message = "key not found";
            send_header_response(socket, 0, message, FAILURE);
            return -1;
        }

        if (delete_node(table, key)) {
            size_t size = snprintf(NULL, 0, "%s is deleted", key);
            if (size < 0) return -1;
                
            message = malloc(size+1);
            if (message == NULL) return -1;
            
            snprintf(message, size + 1, "%s = %s", key, value);
            send_header_response(socket, 0, message, SUCCESS);
            free(message);
            return 0;
        }

        message = "failed to delete";
        send_header_response(socket, 0, message, FAILURE);
        return -1;
    }

    message = "Invalid operation selected";
    send_header_response(socket, 0, message, FAILURE);
    return -1;
}

int handle_cmd_list(int socket, Request_Packet *req, Hash_Table *table) {
    // <OPR> 
    char* message;
    if (req->operation == LIST) {
        /* NOTE: list_all function handles the sending of response and the headers in it no need to deal with it specially */
        list_all(socket, table);
    } else if (req->operation == DEL_ALL) {
        clear_table(table);
        message = "All elements deleted successfully";
        send_header_response(socket, 0, message, SUCCESS);
        return 0;
    }

    message = "Invalid operation received";
    send_header_response(socket, 0, message, FAILURE);
    return -1;
}