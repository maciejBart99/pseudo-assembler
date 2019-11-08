WEKTOR_1 DS 5*INT
WEKTOR_2 DS 5*INT
WEKTOR   DS 10*INT
MAX      DC INT(10)
MAX_W    DC INT(5)
JEDEN    DC INT(1)
CZTERY   DC INT(4)

SR 0,0
SR 1,1
SR 2,2

LA 3,WEKTOR_1
LA 4,WEKTOR_2
LA 5,WEKTOR

PETLA C 0,MAX
      JZ KONIEC
      A 0,JEDEN
      L 6,0(3)
      L 7,0(4)

      C 2,MAX_W
      JZ WEZ_Z_1
      C 1,MAX_W
      JZ WEZ_Z_2

      CR 6,7
      JP WEZ_Z_2

       WEZ_Z_1 ST 6,0(5)
                  A 3,CZTERY
                  A 1,JEDEN
                  A 5,CZTERY
                  J PETLA

       WEZ_Z_2 ST 7,0(5)
               A 4,CZTERY
               A 2,JEDEN
               A 5,CZTERY
               J PETLA

KONIEC
