WEKTOR DS 100000*INT
MAX DC INT(100000)
JEDEN DC INT(1)
CZTERY DC INT(4)
WYNIK DS INT

SR 0,0
SR 2,2
LA 1,WEKTOR

PETLA C 0,MAX
    JZ KONIEC_P
    ST 0,0(1)
    A 0,JEDEN
    A 1,CZTERY
    J PETLA

KONIEC_P SR 0,0
LA 1,WEKTOR

PETLA_2 C 0,MAX
    JZ KONIEC
    A 2,0(1)
    A 0,JEDEN
    A 1,CZTERY
    J PETLA_2

KONIEC ST 2,WYNIK