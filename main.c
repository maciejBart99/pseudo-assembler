#include "declarations.h"
#include <windows.h>

int main(int argc, char **argv) {
    char fileName[STRING_BUFF_SIZE], outputs[STRING_BUFF_SIZE];
    char lastTag = ' ';
    int i;
    bool hasSetFile = false;
    long time, start;
    bool hasSetOutput = false;
    struct OrderList orders;
    struct CharArray oArray;
    extern struct Core core;
    HANDLE hConsole;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    //parse given arguments
    if (argc == 2) {
        strcpy(fileName, argv[1]);
        hasSetFile = true;
    } else
        for (i = 1; i < argc; i++) {
            if (*argv[i] == '-') {
                if (*(argv[i] + 1) == 's') core.mode = STEPS;
                else if (*(argv[i] + 1) == 'g') core.mode = GUI;
                else if (*(argv[i] + 1) == 'n') core.mode = NONE;
                else lastTag = *(argv[i] + 1);
            } else {
                switch (lastTag) {
                    case 'f':
                        strcpy(fileName, argv[i]);
                        hasSetFile = true;
                        break;
                    case 'o':
                        strcpy(outputs, argv[i]);
                        hasSetOutput = true;
                        break;
                    case 'i':
                        parseInput(argv[i]);
                        break;
                    case ' ':
                        strcpy(fileName, argv[i]);
                        hasSetFile = true;
                        break;
                }
                lastTag = ' ';
            }
        }

    //Hello message
    printf("\nPseudo-Assembler by Maciej \210ukasik 2019...\n\n");
    printf("Przed u\276yciem zapoznaj si\251 z za\210\245czon\245 instrukcj\245!\n");

    //check if user has selected file and mode
    if (!hasSetFile) askForFile(fileName);

    if (core.mode == UNDEF) askForMode();

    if (core.mode != NONE) system("cls");

    //start execution
    if (core.mode != GUI) conventionalInterface(fileName, outputs, hasSetOutput);
    else if (core.mode == GUI) mainGUI(fileName, outputs);

    //reset console settings
    SetConsoleTextAttribute(hConsole, WHITE);
    showcursor();
    return 0;
}

void conventionalInterface(char *fileName, char *outputs, bool hasSetOutput) {
    HANDLE hConsole;
    struct OrderList orders;
    long time, start;
    extern struct Core core;

    //set up console settings
    hidecursor();
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, GREEN);

    printf("\nWybrano plik %s\n\n", fileName);
    SetConsoleTextAttribute(hConsole, WHITE);

    //compile program
    printf("Trwa przetwarzanie pliku \253r\242d\210owego...\n");
    orders = parseScript(fileName);

    start = getMicrotime();

    //start program
    printf("\nStart programu...\n");
    SetConsoleTextAttribute(hConsole, GREEN);
    if (core.mode == STEPS) {
        printf("\nWszystkie rejestry i pami\251\206 puste...\n\n");
        SetConsoleTextAttribute(hConsole, WHITE);
        printf("(Spacja) - Nast\251pna linijka (Enter) - Uruchom");
    }
    SetConsoleTextAttribute(hConsole, WHITE);
    executeScript(orders, false);

    //measure time
    time = (long) ((getMicrotime() - start) / 1000) + 1;

    //dump registers in step mode
    if(core.mode==STEPS) dumpRegisters();

    //print output
    SetConsoleTextAttribute(hConsole, YELLOW);
    if (hasSetOutput) printOutput(outputs);

    //print success msg
    SetConsoleTextAttribute(hConsole, GREEN);
    if (core.mode == NONE)
        printf("\nWykonywanie programu trwa\210o ok. %d ms i zako\344czy\210o si\251 sukcesem!\n", time);
    else printf("\nWykonywanie programu zako\344czy\210o si\251 sukcesem!\n");
}

//parse string containing input varibles passed by user
void parseInput(char *args) {
    struct CharArray arForVaribles, arForEquals, arForTable;
    struct Label *tmpLabel;
    extern struct Core core;
    char *buffer;
    int i;

    arForVaribles = str_split(args, ';', SPLIT_LIMIT);

    for (i = 0; i < arForVaribles.length; i++) {
        arForEquals = str_split(arForVaribles.array[i], '=', 2);

        tmpLabel = malloc(sizeof(struct Label));
        tmpLabel->key.hash = hash(arForEquals.array[0]);
        buffer = arForEquals.array[1];

        if (*buffer == '[') {
            buffer++;
            buffer[strlen(buffer) - 1] = '\0';
            arForTable = str_split(strdup(buffer), ',', SPLIT_LIMIT);
            tmpLabel->value.text = buffer;
            tmpLabel->length = arForTable.length;
        } else {
            tmpLabel->length = 1;
            tmpLabel->value.target = atoi(buffer);
        }

        if (core.userInput.length > 0) tmpLabel->previous = core.userInput.first;
        core.userInput.first = tmpLabel;
        core.userInput.length++;
    }
}

void askForMode() {
    HANDLE hConsole;
    extern struct Core core;
    int key;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    while (true) {
        SetConsoleTextAttribute(hConsole, YELLOW);
        printf("\n\nW jakim trybie chcesz uruchomi\206 interpreter?\n");
        SetConsoleTextAttribute(hConsole, WHITE);
        printf(" 1) - Tylko wynik\n 2) - Wizulizacja dzia\210ania\n 3) - Lista krok\242w\n\n Wybieram:");
        key = getchar();

        switch (key) {
            case 49:
                core.mode = NONE;
                return;
            case 50:
                core.mode = GUI;
                return;
            case 51:
                core.mode = STEPS;
                return;
        }
    }
}

void askForFile(char *input) {
    HANDLE hConsole;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, YELLOW);
    printf("\nWybierz plik, kt\242ry chcesz uruchomi\206:");
    SetConsoleTextAttribute(hConsole, WHITE);
    gets(input);
}