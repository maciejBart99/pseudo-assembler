#ifndef declarations
#define declarations
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <conio.h>
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
#define REG_DISPLAY_SIZE 26
#define GUI_CMD_MARGIN 3


#define HIGHLIGHT 23
#define BLUE 1
#define RED 4
#define GREEN 10
#define YELLOW 14
#define WHITE 15
#define CYAN 3
#define GREY 8


//functions
struct OrderList parseScript(char *fileName);

void logError(char*message,int line);

struct CharArray str_split(char* a_str, const char a_delim,int limit);

size_t getline(char **lineptr, size_t *n, FILE *stream);

bool checkIfKeyWord(char* word);

void executeScript(struct OrderList orders,bool useGUI);

long getMicrotime();

char *trim(char *str);

bool isNumeric(char *str);

int addLabel(unsigned long hash,char * orginal,size_t length,short size);

struct Label* getLabel(unsigned long hash,bool allowNULL,int line);

void addJumpLabel(unsigned long hash,int address,short size);

struct Label* getJumpLabel(unsigned long hash,bool allowNULL,int line);

int getMemoryValue(int address);

void setMemoryValue(int address,int value);

int getRegisterValue(short address,int line);

void setRegisterValue(short address,int value,int line);

short getRegisterAdress(short name,int line) ;

short getState();

void updateStateRegister(short value);

void printOutput(char* outputs);

unsigned long hash(char *str);

void printMemoryChange(int address,char * cmd);

void printRegisterChange(short reg_num,char * cmd);

void printMemoryInit(char * cmd,int size,int address,bool def,int value);

void conventionalInterface(char * fileName,char * outputs,bool hasSetOutput);

void printJump(char * cmd,bool success);

void printCompare(short state,char * cmd);

void parseInput(char * args);

char * decToHexU2(long num);

void setIsDeclared(int address);

void updateGUI(int line,int address);

void renderRegisters(int columns);

void renderMemoryAndCode(int columns,int rows,int address);

void mainGUI(char * fileName,char * outputs,bool hasSetOutputs);

void moveTo(int a, int b);

void hidecursor();

void askForMode();

void askForFile(char * input);

void showcursor();

void dumpRegisters();

long max(long a,long b);

void sigintHandler(int sig_num);

#endif