WEKTOR DS 10*INT
MAX DC INT(10)
JEDEN DC INT(1)
CZTERY DC INT(4)
ZERO DC INT(0)

SR 0,0
LA 1,WEKTOR

PETLA C 0,MAX
      JZ KONIEC
      A 0,JEDEN
      L 2,0(1)
      LR 7,1
      A 1,CZTERY
      C 2,ZERO
      JN PETLA

      LR 3,0
      LR 4,1
      PETLA_U C 3,MAX
              JZ KONIEC_U
              L 5,0(4)
              LR 6,4
              A 3,JEDEN
              A 4,CZTERY
              C 5,ZERO
              JP PETLA_U
              JZ PETLA_U
              ST 2,0(6)
              ST 5,0(7)
              J PETLA
      KONIEC_U
      C 2,ZERO
      JZ PETLA

      LR 3,0
      LR 4,1
      PETLA_0 C 3,MAX
              JZ KONIEC_0
              L 5,0(4)
              LR 6,4
              A 3,JEDEN
              A 4,CZTERY
              C 5,ZERO
              JP PETLA_0
              ST 2,0(6)
              ST 5,0(7)
              J PETLA
       KONIEC_0
KONIEC