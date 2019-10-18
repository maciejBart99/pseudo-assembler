#ifndef structures
#define structures
#endif

struct CharArray {
    char** array;
    size_t length;
};

struct OrderList {
    struct Order* orders;
    size_t length;
};

struct Memory {
    struct MemoryCell* cells;
    size_t length;
};

struct Order
{
    long command;
    char *tag;
    char *args;
    size_t orginal_line;
};

struct TagsList {
    size_t length;
    struct Tag *tags;
};

struct Tag {
    long hash;
    char * add;
    int target;
    size_t array_len;
};

struct Register
{
    short number;
    int value;
};

struct MemoryCell
{
    short size;
    long value;
    size_t address;
};

