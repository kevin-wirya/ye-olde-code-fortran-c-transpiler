C     error_4.f: Subroutine Parameter Arity Mismatch
      PROGRAM ERR4
      IMPLICIT NONE
      INTEGER X
      CALL FOO(X)
      END

      SUBROUTINE FOO(A, B)
      IMPLICIT NONE
      INTEGER A, B
      PRINT *, A, B
      RETURN
      END
