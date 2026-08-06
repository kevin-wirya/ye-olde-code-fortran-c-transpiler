C     REAL FUNCTION (prefix)
      PROGRAM TEST7
      IMPLICIT NONE
      REAL X, Y, CALC
      X = 5.0
      Y = CALC(X)
      PRINT *, Y
      END
      
      REAL FUNCTION CALC(VAL)
      IMPLICIT NONE
      REAL VAL
      CALC = VAL ** 2.0
      RETURN
      END
