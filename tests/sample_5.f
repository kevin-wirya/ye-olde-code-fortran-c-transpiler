*     GOTO and Arithmetic
      PROGRAM TEST5
      IMPLICIT NONE
      REAL X, Y, Z
      X = 2.5E1
      Y = 3.0D-1
      Z = X ** 2 + Y * 5.0 / 2.0 - 1.0
      PRINT *, Z
      GOTO 99
      PRINT *, 'This is skipped'
   99 CONTINUE
      END
