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