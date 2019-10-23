#ifndef structures
#define types

//enums
enum OpeartionType{ADD,SUBSTRACT,MULTIPLY,DIVIDE,COMPARE};

enum TransferType{MtoR,RtoM,RtoR,AtoR};

enum DataSource {MEMORY,REGISTER};

enum Command {CMD_A=98,CMD_AR=3316,CMD_LA=3662,CMD_S=116,CMD_SR=3910,CMD_M=110,CMD_MR=3712,CMD_D=101,CMD_DR=3415,CMD_L=109,CMD_ST=3912,CMD_LR=3679,CMD_DS=3416,CMD_DC=3400,CMD_J=107,CMD_JZ=3621,CMD_JP=3611,CMD_JN=3609,CMD_C=100,CMD_CR=3382,CMD_NONE=0};

//structure used in str_split
struct CharArray 
{
    char** array;
    size_t length;
};

//numerical cmd representation
struct Order
{
    int commandHash;
    unsigned long tagHash;
    int args[3];
    int orginal_line;
};
struct OrderList 
{
    struct Order* orders;
    int length;
};

//a label representing key - target realtion implemetned as a stack element
struct Label
{
    struct Label* previous;
    //key can either be address or hash
    union
    {
        int address;
        unsigned long hash;
    } key;
    //value can either be text or numercial target(ex. address)
    union
    {
        char*text;
        int target;
    } value;
    int length;
};
//stack head and counter
struct Labels
{
    struct Label *first;
    int length;
};

//OOP-like structure to gather all crucial elements
struct Core
{
    struct Labels labels;
    struct Labels jumpLabels;
    struct Labels userInput;
    int * memory;
    int registers[16];
    short stateRegisterValue; 
};

#endif

