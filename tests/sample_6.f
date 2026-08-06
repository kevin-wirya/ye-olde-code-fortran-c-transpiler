C     SUBROUTINE with array
      PROGRAM TEST6
      IMPLICIT NONE
      INTEGER ARR(5)
      INTEGER N
      N = 5
      CALL FILL(ARR, N)
      PRINT *, ARR(1)
      END
      
      SUBROUTINE FILL(A, SZ)
      IMPLICIT NONE
      INTEGER SZ, I
      INTEGER A(SZ)
      DO 100 I = 1, SZ
          A(I) = I * 10
  100 CONTINUE
      RETURN
      END
