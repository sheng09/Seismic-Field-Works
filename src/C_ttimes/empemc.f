      subroutine emdlv(r,vp,vs)
c         set up information on earth model (specified by
c         subroutine call emiasp)
c         set dimension of cpr,rd  equal to number of radial
c         discontinuities in model
      save
      parameter (np=9)
      character*(*) name
      character*20 modnam
      dimension cpr(np)
      common /emdlc/rd(np)
      data rd /
     1  1217.1,
     2  3485.7,
     3  5701.0,
     4  5951.0,
     5  6151.0,
     6  6251.0,
     7  6336.0,
     8  6351.0,
     9  6371./
      data modnam/'pemc'/
c
      call vpemc(r,vp,vs,rho)
      return
c
      entry emdld(n,cpr,name)
      n=np
      do 1 i=1,np
 1    cpr(i)=rd(i)
      name=modnam
      return
      end

      SUBROUTINE VPEMC(R,VP,VS,rho)
C     Gives Vp and Vs at a level of R km in PEM-C Earth model 
C     (dziewonski et al., 1975).
C     Input: R - radius in km 
C     Output: VP - P velocity 
C             VS - S velocity 
      X=R/6371. 
C     IF(L.GT.0) GO TO (80,70,60,50,40,30,20,10,5),L
      IF(R.GT.1217.101) GO TO 10
C     Inner core, layer 9 
    5 VP=11.24094-4.09689*X*X 
      VS=3.56454-3.45241*X*X
	rho=13.01219-8.45292*x*x
      L=9 
      RETURN
C     Outer core, layer 8 
   10 IF(R.GT.3485.701) GO TO 20
      VP=(-13.67046*X+3.75665)*X+10.03904 
      VS=0. 
	rho=12.58416-((7.11215*x+1.94128)*x+1.69929)*x
      L=8 
      RETURN
C     Lower mantle (7)
   20 IF(R.GT.5701.001) GO TO 30
      VP=((-5.30512*X+4.68676)*X-6.38826)*X+16.69287
      VS=((-6.25575*X+9.39892)*X-6.85512)*X+9.20501 
	rho=6.81430-(1.18531*x+1.66273)*x
      L=7 
      RETURN
C     Transition zone (6) 
   30 IF(R.GT.5951.001) GO TO 40
      VP=21.05692-12.31433*X
      VS=15.04371-10.69726*X
	rho=11.11978-7.870548*x
      L=6 
      RETURN
C     Below LVZ (5) 
   40 IF(R.GT.6151.001) GO TO 50
      VP=25.60797-17.63609*X
      VS=13.52229-9.32106*X 
	rho=7.15855-3.85999*x
      L=5 
      RETURN
C     LVZ(4)
   50 IF(R.GT.6251.001) GO TO 60
      VP=7.8475 
      VS=4.4586 
	rho=7.15855-3.85999*x
      L=4 
      RETURN
C     Above LVZ (3) 
   60 IF(R.GT.6336.001) GO TO 70
      VP=8.02 
      VS=4.69 
	rho=7.15855-3.85999*x
      L=3 
      RETURN
C     Lower crust (2) 
   70 IF(R.GT.6351.001) GO TO 80
      VP=6.5
      VS=3.75 
	rho=2.92
      L=2 
      RETURN
C     Upper crust (Layer 1) 
   80 VP=5.80 
      VS=3.45 
	rho=2.72
      L=1 
      RETURN
      END 
