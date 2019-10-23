#include "declarations.h"

//global
struct Core core={
    {0,NULL}, //init memory labels
    {0,NULL}, //init jump labels
    {0,NULL}, //init user input
    NULL, //memory prt
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //registers
    0 //state register
};

//functions
int addLabel(unsigned long hash,size_t length,short size)
{
    int address,i;
    extern struct Core core;
    struct Label newLabel,*previous;

    newLabel.key.hash=hash;
    newLabel.length=length;

    if(core.labels.length>0)
    {
        newLabel.previous=core.labels.first;
        address=core.labels.first->value.target + (4 * core.labels.first->length);
    }
    else address=0;

    newLabel.value.target=address;

    core.labels.first=(struct Label*)malloc(sizeof(struct Label));
    *core.labels.first=newLabel;
    core.labels.length++;

    //check if the value of this cell has been typed by the user
    if(core.userInput.length>0)
    {
        previous=core.userInput.first;
        for(i=0;i<core.userInput.length;i++)
        {
            if(previous->key.hash==hash)
            {
                previous->key.address=address;
                break;
            }
        }
    }
    return address;
}

struct Label* getLabel(unsigned long hash) 
{
    size_t i;
    extern struct Core core;
    struct Label* previous=core.labels.first;

    for(i=0;i<core.labels.length;i++)
    {
        if(previous->key.hash==hash) return previous;
      
        previous=previous->previous;
    }

    return NULL;
}

void addJumpLabel(long hash,int address,short size)
{
    extern struct Core core;

    struct Label newLabel;

    newLabel.key.hash=hash;
    newLabel.length=1;

    if(core.jumpLabels.length>0) newLabel.previous=core.jumpLabels.first;

    newLabel.value.target=address;

    core.jumpLabels.first=(struct Label*)malloc(sizeof(struct Label));
    *core.jumpLabels.first=newLabel;
    core.jumpLabels.length++;
}

struct Label* getJumpLabel(long hash) 
{
    size_t i;
    extern struct Core core;
    struct Label* previous=core.jumpLabels.first;

    for(i=0;i<core.jumpLabels.length;i++)
    {
        if(previous->key.hash==hash) return previous;
        previous=previous->previous;
    }
    return NULL;
}

int getMemoryValue(int address)
{
    extern struct Core core;
    return core.memory[address/sizeof(int)];
}

void setMemoryValue(int address,int value)
{
    extern struct Core core;
    core.memory[address/sizeof(int)]=value;
}

int getRegisterValue(short address,int line)
{
    extern struct Core core;
    return core.registers[getRegisterAdress(address,line)];
}

void setRegisterValue(short address,int value,int line)
{
    extern struct Core core;
    core.registers[getRegisterAdress(address,line)]=value;
}

short getState()
{
    extern struct Core core;
    return core.stateRegisterValue;
}

void updateStateRegister(short value)
{
    extern struct Core core;
    core.stateRegisterValue=value;
}

short getRegisterAdress(short name,int line) 
{
    if(name<FIRST_REGISTER||name>LAST_REGISTER) logError("Odwo\210anie do rejestru musi si\251 odbywa\206 poprzez liczb\251 od 0 do 15!",line);
    else return name;
}

void mathOperation(struct Order* order, enum DataSource secondSource,enum OpeartionType type) 
{
    int valueA,valueB,addressB;

    valueA=getRegisterValue(order->args[0],order->orginal_line);

    if(secondSource==MEMORY)
    {
        if(order->args[2]>0) addressB=order->args[1]+getRegisterValue(order->args[2]-1,order->orginal_line);
        else addressB=order->args[1];  

        valueB=getMemoryValue(addressB); 
    }
    else valueB=getRegisterValue(order->args[1],order->orginal_line);

    switch (type)
    {
        case ADD:   setRegisterValue(order->args[0],valueB+valueA,order->orginal_line);
            break;
        case SUBSTRACT:  setRegisterValue(order->args[0],valueB-valueA,order->orginal_line);
            break;
        case MULTIPLY:  setRegisterValue(order->args[0],valueB*valueA,order->orginal_line);
            break;
        case DIVIDE:  setRegisterValue(order->args[0],valueA/valueB,order->orginal_line);
            break;
        case COMPARE:   setRegisterValue(order->args[0],valueA-valueB,order->orginal_line);
            break;
    }

    if(getRegisterValue(order->args[0],order->orginal_line)>0) updateStateRegister(STATE_POSITIVE);
    else if(getRegisterValue(order->args[0],order->orginal_line)<0) updateStateRegister(STATE_NEGATIVE);
    else updateStateRegister(STATE_ZERO);

    if(type==COMPARE) setRegisterValue(order->args[0],valueA,order->orginal_line);
    else printRegisterChange(order->args[0]);
}

void transferData(struct Order *order, enum TransferType type) 
{
    int valueA,valueB,addressB;

    valueA=getMemoryValue(order->args[0]);

    if((type==RtoM||type==MtoR)&&order->args[2]>0) addressB=order->args[1]+getRegisterValue(order->args[2]-1,order->orginal_line);
    else addressB=order->args[1];

    switch (type)
    {
        case MtoR: setRegisterValue(order->args[0],getMemoryValue(addressB),order->orginal_line);
                    printRegisterChange(order->args[0]);
            break;
        case RtoM: setMemoryValue(addressB,getRegisterValue(order->args[0],order->orginal_line));
            break;
        case RtoR: setRegisterValue(order->args[0],getRegisterValue(order->args[1],order->orginal_line),order->orginal_line);
                    printRegisterChange(order->args[0]);
            break;
        case AtoR: setRegisterValue(order->args[0],order->args[1],order->orginal_line);
                    printRegisterChange(order->args[0]);
            break;
    }
}

struct Order parseOrder(char *line,int index,int orginal) 
{
    struct CharArray argsBySpace,argsByComa,argsByParent,argsByStar;
    struct Order order;
    size_t labelSize;
    long tmp,tmpHash;

    argsBySpace=str_split(strdup(line),' ',3);

    if(checkIfKeyWord(argsBySpace.array[0])) argsBySpace=str_split(line,' ',2);

    if(argsBySpace.length>3) logError("B\210\245d sk\210adni!",orginal);

    order.commandHash=(argsBySpace.length>1)?hash(trim(argsBySpace.array[argsBySpace.length-2])):(unsigned long)CMD_NONE;
    order.orginal_line=orginal;

    if(argsBySpace.length>1)
    {
        argsByComa=str_split(trim(argsBySpace.array[argsBySpace.length-1]),',',2);

        argsByStar=str_split(trim(argsByComa.array[argsByComa.length-1]),'*',2);

        argsByParent=str_split(trim(argsByStar.array[argsByStar.length-1]),'(',2);

        if(argsByStar.length>2||argsByParent.length>2||argsByComa.length>2) logError("B\210\245d sk\210adni!",orginal);

        if(argsByComa.length==2) order.args[0]=atoi(trim(argsByComa.array[0]));
        else if(argsByStar.length==2) order.args[0]=atoi(trim(argsByStar.array[0]));
        else order.args[2]=0;

        tmp=max(argsByComa.length,argsByStar.length)-1;

        if(argsByParent.length==1) 
        {
            if(isNumeric(argsByParent.array[0]))
            {
                order.args[tmp]=atoi(trim(argsByParent.array[0]));
                order.args[2]=0;
            }
            else
            {
                order.args[tmp]=(argsByComa.length==2)?getLabel(hash(trim(argsByParent.array[0])))->value.target:hash(trim(argsByParent.array[0]));
                if(argsByComa.length==2) order.args[2]=MEMORY_REGISTER+1;
            }
            order.args[tmp+1]=0;
        }
        else
        {
            argsByParent.array[1][strlen(argsByParent.array[1])-1]='\0';
            order.args[tmp]=(isNumeric(argsByParent.array[0]))?atoi(trim(argsByParent.array[0])):((argsByComa.length==2)?getLabel(hash(trim(argsByParent.array[0])))->value.target:hash(trim(argsByParent.array[0])));
            order.args[tmp+1]=atoi(argsByParent.array[1])+1;
        }

        if(argsBySpace.length==3)
        {
            order.tagHash=hash(argsBySpace.array[0]);
            labelSize=(argsByStar.length>1)?atoi(trim(argsByStar.array[0])):1;

            if((enum Command)order.commandHash==CMD_DC||(enum Command)order.commandHash==CMD_DS) order.tagHash=addLabel(order.tagHash,labelSize,4);
            else addJumpLabel(order.tagHash,index*sizeof(struct Order),sizeof(struct Order));
        }
        else order.tagHash=0;
    }
    else
    {
        order.tagHash=hash(trim(argsBySpace.array[0]));
        addJumpLabel(order.tagHash,index*sizeof(struct Order),sizeof(struct Order));
    }
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

        if(resultLength % ORDERS_CHUNK==0&&resultList!=0) resultList=realloc(resultList,(resultLength + ORDERS_CHUNK)*sizeof(struct Order));

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

//function to handle DS DC instructions
void manageDataSection(struct Order* order) 
{
    int value,up,i,j;
    struct Label *label;
    struct CharArray arByComa;
    bool hasFound=false;
    extern struct Core core;

    if(order->args[2]==0) up=1;
    else up=order->args[0];

    if(up<1) logError("Nie mo\276na utworzy\206 tablicy o podanym rozmiarze!",order->orginal_line);

    if(order->args[2]==0) value=order->args[1]-1;
    else value=order->args[2]-1;

    //check if this cell has been typed by user
    if(core.userInput.length>0)
    {
        label=core.userInput.first;

        for(j=0;j<core.userInput.length;j++)
        {
            if(label->key.address==order->tagHash)
            {
                hasFound=true;

                if(label->length==1) setMemoryValue(order->tagHash,label->value.target);
                else
                {
                    arByComa=str_split(label->value.text,',',SPLIT_LIMIT);

                    for(j=0;j<arByComa.length;j++) setMemoryValue(order->tagHash+(sizeof(int)*j),atoi(arByComa.array[j]));
                }
            }
            label=label->previous;
        }
    }
    if(!hasFound) for(i=0;i<up;i++) setMemoryValue(order->tagHash+(sizeof(int)*i),value);
}

int executeOrder(struct Order* order,int cmdAddress) 
{
    switch((enum Command)order->commandHash)
    {
        case CMD_NONE: return cmdAddress; break;
        //memory allocation
        case CMD_DS: manageDataSection(order); break;
        case CMD_DC: manageDataSection(order); break;
        //math operations
        case CMD_A: mathOperation(order,MEMORY,ADD); break;
        case CMD_AR: mathOperation(order,REGISTER,ADD); break;

        case CMD_S: mathOperation(order,MEMORY,SUBSTRACT); break;
        case CMD_SR: mathOperation(order,REGISTER,SUBSTRACT); break;

        case CMD_M: mathOperation(order,MEMORY,MULTIPLY); break;
        case CMD_MR: mathOperation(order,REGISTER,MULTIPLY); break;

        case CMD_D: mathOperation(order,MEMORY,DIVIDE); break;
        case CMD_DR: mathOperation(order,REGISTER,MULTIPLY); break;
        
        case CMD_C: mathOperation(order,MEMORY,COMPARE); break;
        case CMD_CR: mathOperation(order,REGISTER,COMPARE); break;
        //data transfer
        case CMD_L: transferData(order,MtoR); break;
        case CMD_LR: transferData(order,RtoR); break;
        case CMD_ST: transferData(order,RtoM); break;
        case CMD_LA: transferData(order,AtoR); break;
        //jumps
        case CMD_J: return(getJumpLabel(order->args[0])->value.target)-sizeof(struct Order); break;
        case CMD_JN: if(getState()==STATE_NEGATIVE) return (getJumpLabel(order->args[0])->value.target)-sizeof(struct Order); break;
        case CMD_JP: if(getState()==STATE_POSITIVE) return (getJumpLabel(order->args[0])->value.target)-sizeof(struct Order); break;
        case CMD_JZ: if(getState()==STATE_ZERO) return (getJumpLabel(order->args[0])->value.target)-sizeof(struct Order); break;

        default: logError("Nieznane polecenie w %d lini!",order->orginal_line);
    }

    return cmdAddress;
}

void executeScript(struct OrderList orders)
{
    extern struct Core core;
    struct Order* order;
    int ordersSize,*ptr;
    
    core.memory=malloc(core.labels.first->value.target+(sizeof(int)*core.labels.first->length));

    ptr=&core.registers[CMD_REGISTER];
    ordersSize=sizeof(struct Order)*orders.length;
    setRegisterValue(MEMORY_REGISTER,ordersSize,0);

    for(*ptr=0;*ptr<ordersSize;*ptr+=sizeof(struct Order)) {
        order=&orders.orders[*ptr/sizeof(struct Order)];

        *ptr=executeOrder(order,*ptr);
    }
}