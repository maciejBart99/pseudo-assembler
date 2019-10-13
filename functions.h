#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#ifndef functions
#define functions
#endif

struct OrderList parseScript(char *fileName);

int logError(char*message);

struct CharArray str_split(char* a_str, const char a_delim);

size_t getline(char **lineptr, size_t *n, FILE *stream);

bool checkIfKeyWord(char* word);

void executeScript(struct OrderList orders);

long getMicrotime();

char *trim(char *str);

bool isNumeric(const char *str);

void clearMemory(struct OrderList listToClear);

long getMemoryAddress(char * tag,int line);

long getMemoryValue(size_t addressA,int line);

size_t getMemoryRows(size_t addressA);

size_t getMemoryIndex(size_t addressA);

long getMemoryValueByIndex(size_t index);