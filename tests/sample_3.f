C     Arrays and DO loops
      PROGRAM TEST3
      IMPLICIT NONE
      INTEGER MATRIX(3,3)
      INTEGER I, J
      DO 10 I = 1, 3
          DO 20 J = 1, 3
              MATRIX(I,J) = I * J
   20     CONTINUE
   10 CONTINUE
      PRINT *, 'Matrix init done'
      END
