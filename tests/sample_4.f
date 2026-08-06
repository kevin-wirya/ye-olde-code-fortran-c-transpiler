C     IF-THEN-ELSE and Logical
      PROGRAM TEST4
      IMPLICIT NONE
      LOGICAL FLAG, RESULT
      INTEGER A, B
      A = 5
      B = 10
      FLAG = .TRUE.
      IF (A .LT. B .AND. FLAG) THEN
          RESULT = .NOT. .FALSE.
          PRINT *, 'A is less than B'
      ELSE
          RESULT = .FALSE.
          PRINT *, 'A is >= B'
      ENDIF
      END
