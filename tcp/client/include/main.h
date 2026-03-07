#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "sockets.h"
#include "commands.h"

#define MAX_INP_SIZE 512

/*

basic commands segrigation:
<TYPE>     <OPR_STRUCTURE>                                              <OPR_NAMES>                      <OPR_SEGREGATION_NAME>

type A ->  <OPR> <VAR_NAME> <DATA>                                      => SET_STR, SET_NUM, UPDATE      => SET
type B ->  <OPR> <VAR_NAME>                                             => GET, DELETE                   => GET
type C ->  <OPR>                                                        => LIST                          => LIST
type D ->  <OPR> <STOR_VAR> <VAR_ONE|DATA_ONE> <VAR_TWO|DATA_TWO>       => ADD, SUB, MUL, DIV            => ARITH
type E ->  <RAND_VALUE_OR_TXT>                                          => INVALID OPERATION             => UNK_THROW_ERR

*/

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

struct command command_types[] = {
    {"SET_STR", CMD_SET},
    {"SET_NUM", CMD_SET},
    {"UPDATE", CMD_SET},
    {"GET", CMD_GET},
    {"DELETE", CMD_GET},
    {"LIST", CMD_LIST},
    {"ADD", CMD_ARITH},
    {"SUB", CMD_ARITH},
    {"MUL", CMD_ARITH},
    {"DIV", CMD_ARITH},
};



#endif