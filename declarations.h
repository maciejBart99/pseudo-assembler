#ifndef declarations
#define declarations
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include "types.h"


//def
#define TAGS_BUFF_SIZE 10
#define ORDERS_CHUNK 100
#define LAST_REGISTER 15
#define CMD_REGISTER 14
#define MEMORY_REGISTER 15
#define FIRST_REGISTER 0
#define SPLIT_LIMIT 1000
#define STATE_NEGATIVE 2
#define STATE_POSITIVE 1
#define STATE_ZERO 0
#define SIZE_INT8 1
#define SIZE_INT16 2
#define SIZE_INT32 4
#define STRING_BUFF_SIZE 200
#define CMD_LENGTH 3
#define HASH_BASE 1

//functions
struct OrderList parseScript(char *fileName);

void logError(char*message,int line);

struct CharArray str_split(char* a_str, const char a_delim,int limit);

size_t getline(char **lineptr, size_t *n, FILE *stream);

bool checkIfKeyWord(char* word);

void executeScript(struct OrderList orders);

long getMicrotime();

char *trim(char *str);

bool isNumeric(char *str);

int addLabel(unsigned long hash,size_t length,short size);

struct Label* getLabel(unsigned long hash) ;

void addJumpLabel(long hash,short size);

struct Label* getJumpLabel(long hash);

int getMemoryValue(int address);

void setMemoryValue(int address,int value);

int getRegisterValue(short address,int line);

void setRegisterValue(short address,int value,int line);

short getRegisterAdress(short name,int line) ;

short getState();

void updateStateRegister(short value);

void printOutput(char* outputs);

unsigned long hash(char *str);

void printRegisterChange(short reg_num);

void parseInput(char * args);

char * decToHexU2(long num);

long max(long a,long b);

#endif