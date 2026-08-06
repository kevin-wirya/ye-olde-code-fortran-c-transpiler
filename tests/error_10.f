C     error_10.f: Syntax Error - Malformed DO loop
      PROGRAM ERR10
      IMPLICIT NONE
      INTEGER I
      DO 10 I 1, 5
          PRINT *, I
   10 CONTINUE
      END
