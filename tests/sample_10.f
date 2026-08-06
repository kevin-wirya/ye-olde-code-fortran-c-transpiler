C     Comprehensive
      PROGRAM TEST10
      IMPLICIT NONE
      INTEGER N, I, SUM
      REAL X(10)
      LOGICAL DONE
      COMMON /STATE/ DONE
      
      READ *, N
      SUM = 0
      DONE = .FALSE.
      
      DO 50 I = 1, N
          X(I) = 1.0E0 * I
          SUM = SUM + I
          IF (SUM .GE. 100) THEN
              DONE = .TRUE.
              GOTO 999
          ENDIF
   50 CONTINUE
      
  999 CONTINUE
      PRINT *, 'SUM:', SUM, 'DONE:', DONE
      END
