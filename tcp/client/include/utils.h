#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>

enum token_type {
    TOKEN_VARIABLE,
    TOKEN_DATA,
    TOKEN_ERR,
    TOKEN_INF
};

bool is_valid_var_name(const char *token);
enum token_type get_token_type(const char *token);

#endif