#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "structures.h"
#include "declarations.h"
#include <math.h>

//globals
struct TagsList jumpTags = {0,NULL};
struct TagsList memoryTags = {0,NULL};
struct Memory memory={NULL,0};
struct Register registers[]={{0,0},{1,0},{2,0},{3,0},{4,0},{5,0},{6,0},{7,0},{8,0},{9,0},{10,0},{11,0},{12,0},{13,0},{14,0},{15,0}};
short stateRegisterValue=0;
struct TagsList inputsList={0,NULL};

//functions
void addJumpTag(char* tag,size_t target) 
{
    struct Tag tagS;
    extern struct TagsList jumpTags;

    tagS.target=target;
    tagS.hash=hash(tag);

    if(jumpTags.length==0) 
    {
        jumpTags.tags[0]=tagS;
        jumpTags.length=1;
    }
    else
    {
        if(jumpTags.length % TAGS_BUFF_SIZE==0) jumpTags.tags=realloc(jumpTags.tags,(jumpTags.length + TAGS_BUFF_SIZE) * sizeof(struct Tag));

        jumpTags.tags[jumpTags.length]=tagS;
        jumpTags.length++;
    }
}

void addMemoryTag(char* tag,size_t target,size_t rows) 
{
    struct Tag tagS;
    extern struct TagsList memoryTags;

    tagS.target=target;
    tagS.hash=hash(tag);
    tagS.array_len=rows;

    if(memoryTags.length==0) 
    {
        memoryTags.tags[0]=tagS;
        memoryTags.length=1;
    }
    else
    {
        if(memoryTags.length % TAGS_BUFF_SIZE==0) memoryTags.tags=realloc(memoryTags.tags,(memoryTags.length + TAGS_BUFF_SIZE) * sizeof(struct Tag));

        memoryTags.tags[memoryTags.length]=tagS;
        memoryTags.length++;
    }

}

size_t allocateMemory(char* tag,long value,short size,bool makeTag,size_t rows) 
{
    size_t address;
    struct MemoryCell cell;
    extern struct TagsList memoryTags;
    extern struct Memory memory;

    if(memoryTags.length==0) address=0;
    else address=memory.cells[memory.length-1].address+memory.cells[memory.length-1].size;

    if(pow(2,(8*size)-1)<abs(value)) logError("Zbyt duza liczba!",rows);

    cell.size=size;
    cell.value=value;
    cell.address=address;

    if(memory.length==0) 
    {
        memory.cells=malloc(sizeof(struct Memory));
        memory.cells[0]=cell;
        memory.length=1;
    }
    else
    {
        memory.cells=realloc(memory.cells,(memory.length+1) * sizeof(struct Memory));
        memory.cells[memory.length]=cell;
        memory.length++;
    }

    if(makeTag) addMemoryTag(tag,address,rows);

    return cell.address;
}

long getMemoryAddress(char * tag,int line) 
{
    size_t address;
    long hashV;
    size_t i;
    extern struct TagsList memoryTags;

    hashV=hash(tag);
    
    for(i=0;i<memoryTags.length;i++) 
        if(hashV==memoryTags.tags[i].hash) return memoryTags.tags[i].target;

    logError("Nie odnaleziono kom\242rki pami\251ci o podanej etykiecie!",line);
}

size_t getMemoryRows(size_t addressA) 
{   
    extern struct TagsList memoryTags;
    size_t i;

    for(i=0;i<memory.length;i++) 
        if(memoryTags.tags[i].target==addressA) return memoryTags.tags[i].array_len;
    
    logError("Nieodnaleziono podanego adresu!",0);
}

size_t getMemoryIndex(size_t addressA) 
{   
    extern struct Memory memory;
    size_t i;

    for(i=0;i<memoryTags.length;i++) 
        if(memory.cells[i].address==addressA) return i;

    logError("Nieodnaleziono podanego adresu!",0);
}

long getMemoryValue(size_t addressA,int line) 
{   
    extern struct Memory memory;
    size_t i;

    for(i=0;i<memory.length;i++) 
        if(memory.cells[i].address==addressA) return memory.cells[i].value;

    return 0;
}

long getMemoryValueByIndex(size_t index) 
{  
    extern struct Memory memory;

    return (memory.cells[index]).value;
}

size_t getJumpTarget(char * tag,int line) 
{   
    long hashV;
    size_t i;
    extern struct TagsList jumpTags;

    hashV=hash(tag);

    for(i=0;i<jumpTags.length;i++) 
        if(hashV==jumpTags.tags[i].hash) return jumpTags.tags[i].target;

    logError("Nie odnaleziono wskazanej etykiety!",line);
}

void setMemoryValue(size_t addressA,long value,int line) 
{   
    extern struct Memory memory;
    size_t i;

    for(i=0;i<memory.length;i++) 
    {
        if(memory.cells[i].address==addressA) 
        {
            memory.cells[i].value=value;
            return;
        }
    }

    logError("Nie odnaleziono kom\242rki pami\251ci o podanym adresie!",line);
}

long getRegisterAdress(char*name,int line) 
{
    short tmp;

    if(isNumeric(name)) 
    {
        tmp=atoi(name);

        if(tmp<FIRST_REGISTER||tmp>LAST_REGISTER) logError("Odwo\210anie do rejestru musi si\251 odbywa\206 poprzez liczb\251 od 0 do 15!",line);
        else return tmp;
    } 
    else logError("Odwo\210anie do rejestru musi si\251 odbywa\206 poprzez liczb\251 od 0 do 15!",line);
}

void mathOperation(char* to,char* from, enum DataSource secondSource,enum OpeartionType type,int line) 
{
    int valueA,valueB,addressA,addressB;
    extern struct Register registers[];

    to=trim(to);
    from=trim(from);

    addressA=getRegisterAdress(to,line);
    valueA=registers[addressA].value;

    if(secondSource==MEMORY) valueB=getMemoryValue(getMemoryAddress(from,line),line);
    else 
    {
        addressB=getRegisterAdress(from,line);
        valueB=registers[addressB].value;
    }

    switch (type)
    {
        case ADD:   registers[addressA].value=valueB+valueA;
            break;
        case SUBSTRACT:   registers[addressA].value=valueA-valueB;
            break;
        case MULTIPLY:   registers[addressA].value=valueB*valueA;
            break;
        case DIVIDE:   registers[addressA].value=valueA/valueB;
            break;
        case COMPARE:   registers[addressA].value=valueA-valueB;
            break;
    }

    if(registers[addressA].value>0) stateRegisterValue=STATE_POSITIVE;
    else if(registers[addressA].value<0) stateRegisterValue=STATE_NEGATIVE;
    else stateRegisterValue=STATE_ZERO;
    

    if(type==COMPARE) registers[addressA].value=valueA;
    else  printRegisterChange(addressA);
    
}

void transferData(char* to,char* from, enum TransferType type,int line) 
{
    int valueA,valueB,addressA,addressB;
    struct CharArray ar;
    char * buffer;
    extern struct Register registers[];

    to=trim(to);
    from=trim(from);

    addressA=getRegisterAdress(to,line);

    if(type==MtoR||type==RtoM) 
    {
        ar=str_split(strdup(trim(from)),'(',2);

        if(ar.length==2) 
        {
            buffer=ar.array[1];
            buffer[strlen(buffer)-1]='\0';

            addressB=atoi(ar.array[0])+registers[getRegisterAdress(buffer,line)].value;
        } 
        else addressB=getMemoryAddress(from,line);
    }

    switch (type)
    {
        case MtoR: registers[addressA].value=getMemoryValue(addressB,line);
                    printRegisterChange(addressA);
            break;
        case RtoM: setMemoryValue(addressB,registers[addressA].value,line);
            break;
        case RtoR: 
                    addressB=getRegisterAdress(from,line);
                    registers[addressA]=registers[addressB];
                    printRegisterChange(addressA);
            break;
        case AtoR: registers[addressA].value=getMemoryAddress(from,line);
            break;
    }
}

struct Order parseOrder(char *line,int index,int orginal) 
{
    struct CharArray args;
    struct Order order;
    struct Tag tag;
    int elementsCount;
    struct CharArray arForDS;
    size_t tmpSize;

    args=str_split(strdup(line),' ',3);

    if(checkIfKeyWord(args.array[0])) args=str_split(line,' ',2);

    if(args.length>3) logError("B\210\245d sk\210adni!",orginal);

    if(args.length==1) 
    {
        order.tag=trim(args.array[0]);
        order.command=(long)CMD_NONE;
        order.orginal_line=orginal;
        addJumpTag(trim(args.array[0]),index);

        return order;
    }

    if(args.length==3)
    {
        order.tag=args.array[0];
        order.command=(enum Command)hash(args.array[1]);
        order.args=args.array[2];
        order.orginal_line=orginal;

        if(order.command!=CMD_DS&&order.command!=CMD_DC) addJumpTag(order.tag,index);
    } 
    else if(checkIfKeyWord(args.array[0])) 
    {
        if(args.length==2) 
        {
            order.command=(enum Command)hash(trim(args.array[0]));
            order.args=trim(args.array[1]);
            order.orginal_line=orginal;
        } 
        else logError("B\210\245d sk\210adni!",orginal);

    }
    else logError("B\210\245d sk\210adni!",orginal);

    return order;
}

struct OrderList parseScript(char *fileName) 
{
    struct Order * resultList;
    struct OrderList list;
    struct Order buffer;
    FILE *fptr;
    char * line = NULL;
    size_t line_num=0;
    size_t len = 0;
    ssize_t read;
    size_t resultLength=0;

    resultList=malloc(sizeof(struct Order)*ORDERS_CHUNK);

    //open script
    fptr = fopen(fileName, "r");
    if (fptr == NULL) logError("Nie można otworzyć wskazanego pliku",0);

    //read script line by line
    while ((read = getline(&line, &len, fptr)) != -1) 
    {
        line_num++;

        if(line[strlen(line)-1]=='\n') line[strlen(line)-1] = 0;

        if(trim(line)[0]=='/'&&trim(line)[1]=='/') continue;

        if(resultLength % ORDERS_CHUNK==0&&resultList!=0) resultList=realloc(resultList,(resultLength + ORDERS_CHUNK)*sizeof(struct Order *));

        if(strlen(trim(line))>2) 
        {
            resultLength++;
            resultList[resultLength-1]=parseOrder(line,resultLength-1,line_num);
        }
    }
    fclose(fptr);    

    list.length=resultLength;
    list.orders=resultList;

    return list;
}

void initMemory()
{
    extern struct TagsList jumpTags;
    extern struct TagsList memoryTags;
    extern struct TagsList inputsList;

    //alocate memory for address linkers
    jumpTags.tags=malloc(sizeof(struct Tag)*TAGS_BUFF_SIZE);
    memoryTags.tags=malloc(sizeof(struct Tag)*TAGS_BUFF_SIZE);
    inputsList.tags =malloc(sizeof(struct Tag)*TAGS_BUFF_SIZE);
}

//function to handle DS DC instructions
void manageDataSection(struct Order* order,struct CharArray args,int line) 
{
    size_t up,i;
    bool hasTag=false;
    struct Tag tag;
    struct CharArray arForDC,arForTab,arForInput;
    char * dType,*value;  
    short size;
    long valueInt,hashV;

    arForTab=str_split(args.array[0],'*',3);

    if(checkIfKeyWord(order->tag)) logError("Z/245a nazwa zmiennej!",line);

    //check if array
    if(arForTab.length==1)
    {
        arForDC=str_split(trim(arForTab.array[0]),'(',3);
        up=1;
    }
    else
    {
        arForDC=str_split(trim(arForTab.array[1]),'(',3);
        up=_atoi64(trim(arForTab.array[0]));
    }

    if(up<1) logError("Nie mo\276na utworzy\206 tablicy o podanym rozmiarze!",line);

    //check data type
    if(strcmp(arForDC.array[0],"INTEGER")||strcmp(arForDC.array[0],"INT")||strcmp(arForDC.array[0],"INT32")) size=SIZE_INT32;
    else if(strcmp(arForDC.array[0],"SHORT")||strcmp(arForDC.array[0],"INT16")) size=SIZE_INT16;
    else if(strcmp(arForDC.array[0],"BYTE")||strcmp(arForDC.array[0],"CHAR")||strcmp(arForDC.array[0],"INT8")) size=SIZE_INT8;
    else logError("Nieobs\210ugiwany typ danych!",line);

    //retrive value
    if(arForDC.length==2)
    {
        value=arForDC.array[1];
        
        value[strlen(value)-1] = 0;
        valueInt=_atoi64(value);
        if(valueInt>pow(2,(size*8)-1)||valueInt<-pow(2,(size*8)-1)) logError("Liczba z poza zakresu!",line);

    }
    else if(arForDC.length==1) valueInt=0;   
    else logError("B\210\245d sk\210adni!",line);

    if(order->command==CMD_DS)
    {
        hashV=hash(order->tag);

        for(i=0;i<inputsList.length;i++) 
        {
            tag=inputsList.tags[i];
            if(hashV==tag.hash) 
            {
                if(tag.array_len==1) valueInt=tag.target;
                else if(tag.target!=-1) hasTag=true;
                break;
            }
        }
    }

    if(hasTag) 
    {
        arForInput=str_split(trim(tag.add),',',SPLIT_LIMIT);
        //if this field has been completed by user
        if(arForInput.length!=tag.array_len) logError("Dana tablica ma z\210y wymiar!",0);

        for(size_t i=0;i<up;i++) 
            setMemoryValue(allocateMemory(order->tag,valueInt,size,i==0,up),atoi(trim(arForInput.array[i])),line);
        
    } 
    else 
    {
        //alocate single/array cell
        for(size_t i=0;i<up;i++) allocateMemory(order->tag,valueInt,size,i==0,up);
    }
}

size_t executeOrder(struct Order* order,int line,int orignalLine) 
{
    struct CharArray arguments;
    extern short stateRegisterValue;

    arguments=str_split(strdup(order->args),',',SPLIT_LIMIT);

    switch((enum Command)order->command)
    {
        case CMD_NONE: return line;
        //memory allocation
        case CMD_DS: manageDataSection(order,arguments,orignalLine); break;
        case CMD_DC: manageDataSection(order,arguments,orignalLine); break;
        //math operations
        case CMD_A: mathOperation(arguments.array[0],arguments.array[1],MEMORY,ADD,orignalLine); break;
        case CMD_AR: mathOperation(arguments.array[0],arguments.array[1],REGISTER,ADD,orignalLine); break;

        case CMD_S: mathOperation(arguments.array[0],arguments.array[1],MEMORY,SUBSTRACT,orignalLine); break;
        case CMD_SR: mathOperation(arguments.array[0],arguments.array[1],REGISTER,SUBSTRACT,orignalLine); break;

        case CMD_M: mathOperation(arguments.array[0],arguments.array[1],MEMORY,MULTIPLY,orignalLine); break;
        case CMD_MR: mathOperation(arguments.array[0],arguments.array[1],REGISTER,MULTIPLY,orignalLine); break;

        case CMD_D: mathOperation(arguments.array[0],arguments.array[1],MEMORY,DIVIDE,orignalLine); break;
        case CMD_DR: mathOperation(arguments.array[0],arguments.array[1],REGISTER,MULTIPLY,orignalLine); break;
        
        case CMD_C: mathOperation(arguments.array[0],arguments.array[1],MEMORY,COMPARE,orignalLine); break;
        case CMD_CR: mathOperation(arguments.array[0],arguments.array[1],REGISTER,COMPARE,orignalLine); break;
        //data tranfer
        case CMD_L: transferData(arguments.array[0],arguments.array[1],MtoR,orignalLine); break;
        case CMD_LR: transferData(arguments.array[0],arguments.array[1],RtoR,orignalLine); break;
        case CMD_ST: transferData(arguments.array[0],arguments.array[1],RtoM,orignalLine); break;
        case CMD_LA: transferData(arguments.array[0],arguments.array[1],AtoR,orignalLine); break;
        //jumps
        case CMD_J: return(getJumpTarget(arguments.array[0],line)-1); break;
        case CMD_JN: if(stateRegisterValue==STATE_NEGATIVE) return getJumpTarget(arguments.array[0],line)-1; break;
        case CMD_JP: if(stateRegisterValue==STATE_POSITIVE) return getJumpTarget(arguments.array[0],line)-1; break;
        case CMD_JZ: if(stateRegisterValue==STATE_ZERO) return getJumpTarget(arguments.array[0],line)-1; break;

        default: logError("Nieznane polecenie w %d lini!",orignalLine);
    }

    return line;
}

void executeScript(struct OrderList orders)
{
    struct Order* order;
    size_t i;
    extern struct Register registers[];

    for(i=0;i<orders.length;i++) {
        order=&orders.orders[i];

        //write to cmd reg current line number
        registers[CMD_REGISTER].value=i;
        i=executeOrder(order,i,order->orginal_line);
    }
}