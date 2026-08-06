C     COMMON block
      PROGRAM TEST9
      IMPLICIT NONE
      INTEGER G1, G2
      COMMON /GLOBALS/ G1, G2
      G1 = 10
      G2 = 20
      CALL PRTGLB()
      END
      
      SUBROUTINE PRTGLB()
      IMPLICIT NONE
      INTEGER G1, G2
      COMMON /GLOBALS/ G1, G2
      PRINT *, G1, G2
      RETURN
      END
