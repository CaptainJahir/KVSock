#include <stdio.h>
#include <string.h>

#define MAX_INP_SIZE 512

/*

basic commands segrigation:
<TYPE>     <OPR_STRUCTURE>              <OPR_NAMES>                      <OPR_SEGRIGATION_NAME>

type A ->  <OPR> <VAR_NAME> <DATA>      => SET_STR, SET_INT, UPDATE      => SET
type B ->  <OPR> <VAR_NAME>             => GET, DELETE                   => GET
type C ->  <OPR>                        => LIST                          => LIST
type D ->  <RAND_VALUE_OR_TXT>          => INVALID OPERATION             => UNK_THROW_ERR

*/

enum command_type {
    CMD_SET,
    CMD_GET,
    CMD_LIST,
    CMD_UNKNOWN
};

struct command {
    const char *name;
    enum command_type type;
};

struct command command_types[] = {
    {"SET_STR", CMD_SET},
    {"SET_INT", CMD_SET},
    {"UPDATE", CMD_SET},
    {"GET", CMD_GET},
    {"DELETE", CMD_GET},
    {"LIST", CMD_LIST},
};

struct Request_Packet {
    char operation[16];
    char var_one_name[64];
    char var_one_data[256];
    char var_two_name[64];
    char var_two_data[256];
};
