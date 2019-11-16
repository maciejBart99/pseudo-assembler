#include "declarations.h"
#include <sys/time.h>

//generate a hash for a string
unsigned long hash(char *str)
{
    unsigned long hash = HASH_BASE;
    int c;

    while (c = *str++)  hash = ((hash << 5) + hash) + c;

    return hash;
}

//trim a string
char *trim(char *str)
{
    size_t len = 0;
    char *frontp = str;
    char *endp = NULL;

    if( str[0] == '\0' ) return str;

    len = strlen(str);
    endp = str + len;

    while( isspace((unsigned char) *frontp) ) { ++frontp; }
    if( endp != frontp ) while( isspace((unsigned char) *(--endp)) && endp != frontp ) {}

    if( frontp != str && endp == frontp ) *str = '\0';
    else if( str + len - 1 != endp ) *(endp + 1) = '\0';

    endp = str;
    if( frontp != str )
    {
        while( *frontp ) { *endp++ = *frontp++; }
        *endp = '\0';
    }

    return str;
}

//check if string is a number
bool isNumeric(char *str) 
{
    while(*str != '\0')
    {
        if(*str < '0' || *str > '9') return false;

        str++;
    }
    return true;
}

//get time in ms
long getMicrotime()
{
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

//POSIX-like getline function for windows
size_t getline(char **lineptr, size_t *n, FILE *stream) 
{
    char *bufptr = NULL;
    char *p = bufptr;
    size_t size;
    int c;

    bufptr = *lineptr;
    size = *n;

    c = fgetc(stream);
    if (c == EOF) return -1;

    if (bufptr == NULL) 
    {
        bufptr = malloc(128);

        if (bufptr == NULL) return -1;

        size = 128;
    }
    p = bufptr;
    while(c != EOF) 
    {
        if ((p - bufptr) > (size - 1)) 
        {
            size = size + 128;
            bufptr = realloc(bufptr, size);

            if (bufptr == NULL) return -1;
        }
        *p++ = c;
        if (c == '\n') break;

        c = fgetc(stream);
    }

    *p++ = '\0';
    *lineptr = bufptr;
    *n = size;

    return p - bufptr - 1;
}

//split function with limit
struct CharArray str_split(char* a_str, char a_delim,int limit)
{
    struct CharArray result;
    bool hasSetPointer=false;
    bool lastSpaceOrDel=false;
    bool lastWasDel=false;
    size_t idx=0;
    size_t tmp=0;
    size_t size=1;
    size_t element=0;

    a_str=trim(a_str);

    if(*(a_str)=='\0') 
    {
        result.length=0;
        return result;
    }

    //calculate array size
    while (*(a_str+idx)!='\0')
    {
        if(*(a_str+idx)==a_delim) size++;
        idx++;
    }

    if(size>limit) size=limit;

    result.array=malloc(size*sizeof(char*));
    idx=0;

    //split string
    while (*(a_str+idx) != '\0')
    {
        if(element>=limit-1)
        {
            *(result.array+element)=strdup(a_str+tmp);
            hasSetPointer=true;
            break;
        }

        if(*(a_str+idx)==a_delim)
        {
            if(lastWasDel) tmp++;
            else
            {
                *(a_str+idx)='\0';
                *(result.array+element)=strdup(a_str+tmp);
                hasSetPointer=false;
                element++;
                lastWasDel=true;
                tmp=idx+1;
            }
        }
        else lastWasDel=false;
        
        idx++;
    }
    if(!hasSetPointer) *(result.array+element)=strdup(a_str+tmp);

    result.length=size;
    return result;
}

//check if a word is keyword
bool checkIfKeyWord(char* word) 
{
    char keyWords[][CMD_LENGTH]={"A","AR","LA","S","SR","M","MR","D","DR","L","ST","LR","DS","DC","J","JZ","JP","JN","C","CR"};
    size_t length,i;

    length=sizeof(keyWords)/(CMD_LENGTH*sizeof(char));

    for (i = 0; i < length; i++) if(strcmp(keyWords[i],word)==0) return true;
        
    return false;
}

//return max of two values
long max(long a,long b)
{
    if(a>b) return a;
    else return b;
}


