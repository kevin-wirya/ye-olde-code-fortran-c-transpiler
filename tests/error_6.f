C     error_6.f: COMMON Block Variable Count Mismatch
      PROGRAM ERR6
      IMPLICIT NONE
      INTEGER A, B
      COMMON /BLK/ A, B
      CALL SUB()
      END

      SUBROUTINE SUB()
      IMPLICIT NONE
      INTEGER X
      COMMON /BLK/ X
      PRINT *, X
      RETURN
      END
