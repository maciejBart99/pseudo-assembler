#include "declarations.h"
#include <windows.h>

static struct OrderList orders;

//init graphical simulation
void mainGUI(char * fileName,char * outputs,bool hasSetOutputs) {
    HANDLE hConsole;
    extern struct OrderList orders;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    orders = parseScript(fileName);
    executeScript(orders, true);

    system("pause");
    system("cls");
    showcursor();
    if (hasSetOutputs)
    {
        printf("Program wygenerowa\210 nast\251puj\245ce wyj\230cie:");
        SetConsoleTextAttribute(hConsole, YELLOW);
        printOutput(outputs);
    }
    SetConsoleTextAttribute(hConsole, GREEN);
    printf("\nWykonywanie programu zako\344czy\210o si\251 sukcesem!\n");
    SetConsoleTextAttribute(hConsole, WHITE);
}

//render frame
void updateGUI(int line,int address)
{
    extern struct Core core;
    HANDLE hConsole;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;
    static int lastWidth=0;

    address=address / sizeof(struct Order);

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    if(lastWidth==0||lastWidth!=columns)
    {
        hidecursor();
        system("cls");
    }
    else moveTo(0,0);
    printf("(Spacja) - Nast\251pna linijka (Enter) - Uruchom\n");
    renderRegisters(columns);

    renderMemoryAndCode(columns,rows,address);

    core.lastChange.target=OTHER;
    lastWidth=columns;
}

//render registers bar
void renderRegisters(int columns)
{
    int i,buffer=0;
    extern struct Core core;
    HANDLE hConsole;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, YELLOW);
    printf("<Rejestry>\n");
    SetConsoleTextAttribute(hConsole, WHITE);
    for(i=0;i<=LAST_REGISTER;i++)
    {
        if(columns<buffer+REG_DISPLAY_SIZE)
        {
            printf("\n");
            buffer=0;
        }

        if(core.lastChange.target==REG&&core.lastChange.address==i) SetConsoleTextAttribute(hConsole, GREEN);

        printf("%3d = %8d(0x%s)",i,getRegisterValue(i,0),decToHexU2(getRegisterValue(i,0)));
        buffer+=REG_DISPLAY_SIZE;
        SetConsoleTextAttribute(hConsole, WHITE);
    }
    switch (getState())
    {
        case 0: SetConsoleTextAttribute(hConsole, WHITE); break;
        case 1: SetConsoleTextAttribute(hConsole, GREEN); break;
        case 2: SetConsoleTextAttribute(hConsole, RED); break;
    }
    printf("\n\n Flaga w s\210owie stanu programu = %d\n\n",getState());
    SetConsoleTextAttribute(hConsole, WHITE);
}

//render code and memory labels
void renderMemoryAndCode(int columns,int rows,int address)
{
    HANDLE hConsole;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int i,vertical,entryPoint;
    struct Label * previous=NULL;
    extern struct Core core;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, YELLOW);
    printf("%-40s%s","<Program>","<Pami\251\206>\n");
    SetConsoleTextAttribute(hConsole, WHITE);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    vertical=max(5,rows-csbi.dwCursorPosition.Y)-1;

    entryPoint=(orders.length<vertical)?0:((address+GUI_CMD_MARGIN<=vertical)?0:address-vertical+GUI_CMD_MARGIN);

    if(core.labels.length>0) previous=core.labels.first;

    for(i=0;i<vertical;i++)
    {
        if(i+entryPoint==address) SetConsoleTextAttribute(hConsole, HIGHLIGHT);

        if(i+entryPoint<orders.length) printf("%-40s",orders.orders[i+entryPoint].orginal_text);
        else if(i+entryPoint==orders.length) printf("%-40s","<Koniec>");
        else printf("%-40s","");
        SetConsoleTextAttribute(hConsole, WHITE);
        if(previous!=NULL)
        {
            if(core.lastChange.target==MEM&&core.lastChange.address==previous->value.target)  SetConsoleTextAttribute(hConsole, GREEN);
            else if(!previous->declared) SetConsoleTextAttribute(hConsole, GREY);

            if(previous->length==1) printf("%s = %d (0x%s)",previous->orginal,getMemoryValue(previous->value.target),decToHexU2(getMemoryValue(previous->value.target)));
            else printf("%s TAB(%d)",previous->orginal,previous->length);

            if(i<core.labels.length-1) previous=previous->previous;
            else previous=NULL;
        }
        printf("\n");
        SetConsoleTextAttribute(hConsole, WHITE);
    }
}

//move cl cursor to cords
void moveTo(int a, int b)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {b, a};
    SetConsoleCursorPosition(hConsole, pos);
}

//hide cursor in cl
void hidecursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

//show cursor in cl
void showcursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 30;
    info.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &info);
}