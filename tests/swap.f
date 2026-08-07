      PROGRAM ORDERPAIR
      IMPLICIT NONE
      INTEGER X, Y
      LOGICAL SWAPPED

      X = 10
      Y = 3
      SWAPPED = .FALSE.

      CALL ORDER(X, Y, SWAPPED)

      IF (SWAPPED) THEN
          PRINT *, 'SWAPPED'
      ELSE
          PRINT *, 'NOT SWAPPED'
      ENDIF

      PRINT *, X, Y
      END


      SUBROUTINE ORDER(A, B, FLAG)
      IMPLICIT NONE
      INTEGER A, B
      LOGICAL FLAG
      INTEGER TEMP

      IF (A .GT. B) THEN
          TEMP = A
          A = B
          B = TEMP
          FLAG = .TRUE.
      ELSE
          FLAG = .FALSE.
      ENDIF

      RETURN
      END