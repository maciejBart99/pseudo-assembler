TABLICA DS 10*INT
MAX DS INT
LIMIT DC INT(9)
CZTERY DC INT(4)
JEDEN DC INT(1)

SR 0,0
SR 5,5
LA 6,TABLICA

PETLA C 0,LIMIT
    JZ KONIEC
    L 2,0(6)
    CR 2,5
    JN ZWIEKSZ
    LR 5,2

    ZWIEKSZ A 6,CZTERY
            A 0,JEDEN
                                                
            J PETLA

KONIEC ST 5,MAX