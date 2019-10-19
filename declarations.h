#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#ifndef declarations
#define declarations
#endif

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

//enums def
enum OpeartionType{ADD,SUBSTRACT,MULTIPLY,DIVIDE,COMPARE};

enum TransferType{MtoR,RtoM,RtoR,AtoR};

enum DataSource {MEMORY,REGISTER};

enum Command {CMD_A=98,CMD_AR=3316,CMD_LA=3662,CMD_S=116,CMD_SR=3910,CMD_M=110,CMD_MR=3712,CMD_D=101,CMD_DR=3415,CMD_L=109,CMD_ST=3912,CMD_LR=3679,CMD_DS=3416,CMD_DC=3400,CMD_J=107,CMD_JZ=3621,CMD_JP=3611,CMD_JN=3609,CMD_C=100,CMD_CR=3382,CMD_NONE=0};

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

void clearMemory(struct OrderList listToClear);

long getMemoryAddress(char * tag,int line);

long getMemoryValue(size_t addressA,int line);

size_t getMemoryRows(size_t addressA);

size_t getMemoryIndex(size_t addressA);

long getMemoryValueByIndex(size_t index);

void initMemory();

void printOutput(char* outputs);

long hash(char *str);

void printRegisterChange(short reg_num);

void parseInput(char * args);

char * decToHexU2(long num);