C     How to test the speed: (1) compile with f77 with highest optimization.
C     (2) time the program. (eg. time a.out), (3) report the user sec/1000, 
C     which is microsecond per Monte Carlo sweep per spin on that machine.
C     This program works only if INTEGER is 32 bits.
C     Last update: 23 Oc 2002
C
C     SWENDSEN-WANG ALGORITHM OF 2D ISING MODEL 
C     MICROSECOND PER MC PER SPIN/MACHINE:  
C     
C     0.017/Exalit workstation (Intel Xeon E5-2697 v4 2.30 GHz 2016), 
C     0.051/Itanium2(1GHz), 0.064/DS20E833MHz, 0.075/DS20E667MHz(imre),
C     0.065/Pentium III 1GHz(pgf77), 0.12/nano1, 0.105/Aplha 600au,
C     0.14/Alpha 500au, 0.21/R10000, 0.35/Pentium Pro200, 0.36/Dec7000, 
C     0.36/AlphaStation 250, 0.5/HPJ210, 0.57/IBM 3CT, 0.62/HPJ200, 0.7/HP735, 
C     0.7/Pentium160, 0.84/R8000, 0.8/Dec4000, 0.88/SGI Challenge M, 
C     1.3/HP712|60, 1.5/HP720, 2.2/IBM 3090, 2.4/VP100, 3.3/Iris indigo, 
C     3.5/IBM 320, 8/Cray Y-MP EL,
C     8.8/Sparc Station, 17/Vax 3200, 21/PC486|33, 32/Vax750, 1300/PC AT,
C
      PARAMETER (L=1000, L2=L*L)                                         
      DIMENSION LIST(L2),IS(L2)                                         
      DATA BETA,IBM,MCSTEP/ 1.0,1,1000/ 
      IPR = (1.0 - 2.0*EXP(-2.0*0.440678/BETA)) * 2147483647.0          
      MCHALF=MCSTEP/2
      MAV=0
      DO 1 I = 1, L2                                                    
 1    IS(I) = 1                                                         
      DO 2 MC = 1, MCSTEP                                               
      LIST(1) = 1                                                       
      DO 3 I = 2, L2                                                    
      IF(IS(I-1).NE.IS(I))GOTO 4                                        
      IBM = IBM * 16807                                                 
      IF ( IBM .GT. IPR ) GOTO 4                                        
      LIST(I) = LIST(I-1)                                               
      GOTO 5                                                            
 4    LIST(I) = I                                                       
 5    IF (I .LE. L)       GOTO 3
      MB = I - L                                                        
      IF(IS(MB).NE.IS(I)) GOTO 3                                        
      IBM = IBM * 65539                                                 
      IF ( IBM .GE. IPR ) GOTO 3                                        
 6      MB = LIST(MB)                                                   
        IF(MB.NE.LIST(MB))GOTO 6                                        
        MA  = LIST(I)                                                   
        ISMALL = MIN0(MA,MB)                                            
        LIST(MAX0(MA,MB)) = ISMALL                                      
        LIST(I) = ISMALL                                                
 3    CONTINUE                                                          
      INC = 0                                                           
      DO 7 I =1, L2                                                     
      IF ( I .EQ. LIST(I) ) THEN                                        
        INC = INC + 1                                                   
        IS(I) = INC                                                     
      ELSE                                                              
        MA = LIST(I)                                                    
 8      MA = LIST(MA)                                                   
        IF(MA.NE.LIST(MA))GOTO 8                                        
        IS(I) = IS(MA)                                                  
      END IF                                                            
 7    CONTINUE                                                          
      DO 9 I = 1, INC                                                   
      IBM = IBM * 16807                                                 
 9    LIST(I) = ISIGN(1,IBM)                                            
      MAG=0
      DO 10 I = 1, L2                                                   
      IS(I) = LIST(IS(I))                                               
 10   MAG = MAG + IS(I)
 2    IF(MC.GT.MCHALF) MAV=MAV+IABS(MAG)
      IF ( MAV .EQ. 143010564 ) THEN
        PRINT *, 'ANSWER CORRECT'
      ELSE 
        PRINT *, 'ANSWER IN ERROR', MAV
      END IF
      END                                                               
