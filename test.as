P    DS INT
Q DS   INT
WYNIK DS INT

L 0  ,Q
L 1,   P

     NWD CR 0,1

JZ   KONIEC

JN              ODEJMIJ
SR    0,1
J NWD

ODEJMIJ SR 1,0
J    NWD



KONIEC ST 0,WYNIK


