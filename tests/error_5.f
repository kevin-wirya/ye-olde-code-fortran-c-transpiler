C     error_5.f: Logical Operator Type Mismatch
      PROGRAM ERR5
      IMPLICIT NONE
      INTEGER X, Y
      LOGICAL RES
      X = 5
      Y = 10
      IF (X .AND. Y) THEN
          PRINT *, 'BAD'
      ENDIF
      END
