C     error_9.f: Syntax Error - Malformed IF condition
      PROGRAM ERR9
      IMPLICIT NONE
      INTEGER X
      X = 5
      IF X .GT. 0 THEN
          PRINT *, X
      ENDIF
      END
