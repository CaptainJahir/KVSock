#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>

enum token_type {
    TOKEN_VARIABLE,
    TOKEN_DATA,
    TOKEN_ERR,
    TOKEN_INF
};

bool is_valid_var_name(const char *token);
enum token_type get_token_type(const char *token);

/*

basic commands segrigation:
<TYPE>     <OPR_STRUCTURE>                                              <OPR_NAMES>                      <OPR_SEGREGATION_NAME>

type A ->  <OPR> <VAR_NAME> <DATA>                                      => SET_STR, SET_NUM, UPDATE      => SET
type B ->  <OPR> <VAR_NAME>                                             => GET, DELETE                   => GET
type C ->  <OPR>                                                        => LIST, DEL_ALL                 => LIST
type D ->  <OPR> <STOR_VAR> <VAR_ONE|DATA_ONE> <VAR_TWO|DATA_TWO>       => ADD, SUB, MUL, DIV            => ARITH
type E ->  <RAND_VALUE_OR_TXT>                                          => INVALID OPERATION             => UNK_THROW_ERR

*/

enum {
    SET_STR,
    SET_NUM,
    UPDATE,
    GET,
    DELETE,
    DEL_ALL,
    LIST,
    ADD,
    SUB,
    MUL,
    DIV
};

typedef uint8_t Operations;

typedef struct Request_Packet {
    Operations operation;
    uint8_t var_one_name_len;
    uint16_t var_one_data_len;
    uint8_t var_two_name_len;
    uint8_t arith_store_var_len;
    char data[]; // NOTE: when you run size of function this array size won't be returned and you need to handle it manually and same for copy function i think
} Request_Packet;

typedef struct Response_Header {
    uint16_t item_count;
    int8_t success; // TODO: i have changed this from unsigned to signed so change everywhere else
    uint8_t msg_len; // NOTE: if the variable data length exceeds 255 chars when setting. SOL: turnicate or just leave it as it is
    char msg[];
} Response_Header;

typedef struct Response_Packet {
    uint16_t val_len;
    uint8_t key_len;
    char data[];
} Response_Packet;

enum command_type {
    CMD_SET,
    CMD_GET,
    CMD_LIST,
    CMD_ARITH,
    CMD_UNKNOWN
};

struct command {
    const char *name;
    enum command_type type;
};

enum command_type check_operation_type(const char *cmd);
int count_token (const char data[]);
int tokens_extractor(char *inp, char *tokens[], size_t token_sizes[], int max_tokens);

char* trim(char *str);
char* to_upper_case(char *data);

#endif