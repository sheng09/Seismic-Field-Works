      subroutine emdlv(r,vp,vs)
c        Velocity model subroutine for (geophysical) Mars model from Sohl and 
c        Spohn (1997).
c
c     Reference:
c        Sohl, F., & Spohn, T. (1997). The interior structure of Mars: 
c        Implications from SNC meteorites. Journal Of Geophysical Research - 
c        Solid Earth, 102(E1), 1613–1635.
c
      parameter (re=3390.00)
      call vss97a(re-r,vp,vs,rho)
      end

      subroutine emdld(n,cpr,name)
      parameter (re=3390.00,nd=5)
      dimension cpr(nd),rd(nd)
      character*(*) name
      character*20 modnam
      logical first
      data rd/1922., 1357.0, 1030., 110., 0./
      data modnam/'ss97a'/, first/.true./
      if (first) then
	 first = .false.
	 do i=1,nd
	    rd(i) = re-rd(i)
         enddo
      endif
      n=nd
      do i=1,nd
         cpr(i)=rd(i)
      enddo
      name=modnam
      return
      end

      subroutine vss97a(pz,pp,ps,prho)
      parameter (nz=77)
      real z(nz),rho(nz),vp(nz),vs(nz)
      real interp

      data (z(i),rho(i),vp(i),vs(i),i=1,nz) /
     +      0.0,  2.818,  7.678,  4.017,
     +     40.0,  2.824,  7.739,  4.053,
     +     90.0,  2.830,  7.816,  4.099,
     +    110.0,  2.833,  7.846,  4.117,
     +    110.0,  3.527,  8.185,  4.558,
     +    140.0,  3.530,  8.225,  4.578,
     +    190.0,  3.535,  8.291,  4.610,
     +    240.0,  3.539,  8.356,  4.643,
     +    290.0,  3.544,  8.422,  4.676,
     +    340.0,  3.549,  8.488,  4.708,
     +    390.0,  3.553,  8.554,  4.741,
     +    440.0,  3.558,  8.620,  4.774,
     +    490.0,  3.562,  8.686,  4.806,
     +    540.0,  3.572,  8.751,  4.839,
     +    590.0,  3.585,  8.817,  4.872,
     +    640.0,  3.597,  8.883,  4.904,
     +    690.0,  3.609,  8.949,  4.937,
     +    740.0,  3.622,  9.015,  4.970,
     +    790.0,  3.634,  9.080,  5.002,
     +    840.0,  3.646,  9.146,  5.035,
     +    890.0,  3.658,  9.212,  5.068,
     +    940.0,  3.671,  9.278,  5.100,
     +    990.0,  3.683,  9.344,  5.133,
     +   1030.0,  3.693,  9.396,  5.159,
     +   1030.0,  3.989,  9.634,  5.329,
     +   1040.0,  3.991,  9.642,  5.334,
     +   1090.0,  4.001,  9.686,  5.362,
     +   1140.0,  4.011,  9.730,  5.390,
     +   1190.0,  4.021,  9.773,  5.418,
     +   1240.0,  4.031,  9.817,  5.446,
     +   1290.0,  4.041,  9.860,  5.474,
     +   1340.0,  4.051,  9.904,  5.502,
     +   1357.0,  4.054,  9.919,  5.512,
     +   1357.0,  4.214,  10.139,  5.698,
     +   1390.0,  4.220,  10.166,  5.715,
     +   1440.0,  4.230,  10.208,  5.740,
     +   1490.0,  4.239,  10.249,  5.765,
     +   1540.0,  4.248,  10.291,  5.790,
     +   1590.0,  4.258,  10.332,  5.815,
     +   1640.0,  4.267,  10.374,  5.840,
     +   1690.0,  4.276,  10.415,  5.865,
     +   1740.0,  4.285,  10.457,  5.890,
     +   1790.0,  4.295,  10.498,  5.915,
     +   1840.0,  4.304,  10.540,  5.940,
     +   1890.0,  4.313,  10.581,  5.965,
     +   1922.0,  4.319,  10.608,  5.981,
     +   1922.0,  6.936,  6.015,  0.000,
     +   1940.0,  6.944,  6.026,  0.000,
     +   1990.0,  6.967,  6.060,  0.000,
     +   2040.0,  6.989,  6.093,  0.000,
     +   2090.0,  7.012,  6.126,  0.000,
     +   2140.0,  7.035,  6.159,  0.000,
     +   2190.0,  7.058,  6.188,  0.000,
     +   2240.0,  7.076,  6.210,  0.000,
     +   2290.0,  7.095,  6.231,  0.000,
     +   2340.0,  7.114,  6.252,  0.000,
     +   2390.0,  7.132,  6.274,  0.000,
     +   2440.0,  7.151,  6.295,  0.000,
     +   2490.0,  7.169,  6.316,  0.000,
     +   2540.0,  7.181,  6.338,  0.000,
     +   2590.0,  7.194,  6.356,  0.000,
     +   2640.0,  7.207,  6.373,  0.000,
     +   2690.0,  7.220,  6.389,  0.000,
     +   2740.0,  7.232,  6.406,  0.000,
     +   2790.0,  7.244,  6.422,  0.000,
     +   2840.0,  7.251,  6.439,  0.000,
     +   2890.0,  7.258,  6.455,  0.000,
     +   2940.0,  7.265,  6.472,  0.000,
     +   2990.0,  7.272,  6.482,  0.000,
     +   3040.0,  7.279,  6.487,  0.000,
     +   3090.0,  7.284,  6.493,  0.000,
     +   3140.0,  7.287,  6.498,  0.000,
     +   3190.0,  7.291,  6.504,  0.000,
     +   3240.0,  7.294,  6.509,  0.000,
     +   3290.0,  7.297,  6.515,  0.000,
     +   3340.0,  7.300,  6.520,  0.000,
     +   3390.0,  7.303,  6.525,  0.000/

      pp = interp(z,vp,nz,1,pz,err)
      ps = interp(z,vs,nz,1,pz,err)
      prho = interp(z,rho,nz,1,pz,err)
      end

      real function interp(xa,ya,n,npts,x,err)
C    Function to interpolate between tabulated values.  The function
C       returns the interpolated result, plus an error.  A number of
C       points to either side of the desired value is given as an argument.
C       This value should be as small as feasible, since large values cause
C       more variation in the interpolated result.
      real xa(n), ya(n), x, value, err

C     Run through tables and bracket value requested.  Interpolate
C        with a more limited number of points.
      if (xa(1) .lt. xa(n)) then
	 ilo = 1
	 ihi = n
      else
	 ilo = n
	 ihi = 1
      endif
10    continue
	 i = (ihi + ilo)/2
	 if (x .eq. xa(i)) then
	    interp = ya(i)
	    err = 0.0
	    return
	 endif
	 if (x .lt. xa(i)) then
	    ihi = i
	 else
	    ilo = i
	 endif
      if (abs(ihi - ilo) .gt. 1) go to 10
      if (ihi .lt. ilo) then
	 i = ihi
	 ihi = ilo
	 ilo = i
      endif

C     Have desired value bracketed.  Adjust bounds and interpolate.
      j = max(1,ilo-npts+1)
      k = min(n,ihi+npts-1)
      if (k-j+1 .ne. 2*npts) then
	 if (j .eq. 1) then
	    k=min(j+2*npts-1,n)
	 else
	    j=max(1,k-2*npts+1)
	 endif
      endif
      call polint(xa(j),ya(j),k-j+1,x,value,err)
      interp = value
      return
      end

      SUBROUTINE POLINT(XA,YA,N,X,Y,DY)
      PARAMETER (NMAX=25) 
      DIMENSION XA(N),YA(N),C(NMAX),D(NMAX)
      IF (N .GT. NMAX) PAUSE '**POLINT:  TOO MUCH DATA.'
      NS=1
      DIF=ABS(X-XA(1))
      DO 11 I=1,N 
        DIFT=ABS(X-XA(I))
        IF (DIFT.LT.DIF) THEN
          NS=I
          DIF=DIFT
        ENDIF
        C(I)=YA(I)
        D(I)=YA(I)
11    CONTINUE
      Y=YA(NS)
      NS=NS-1
      DO 13 M=1,N-1
        DO 12 I=1,N-M
          HO=XA(I)-X
          HP=XA(I+M)-X
          W=C(I+1)-D(I)
          DEN=HO-HP
          IF(DEN.EQ.0.)PAUSE
          DEN=W/DEN
          D(I)=HP*DEN
          C(I)=HO*DEN
12      CONTINUE
        IF (2*NS.LT.N-M)THEN
          DY=C(NS+1)
        ELSE
          DY=D(NS)
          NS=NS-1
        ENDIF
        Y=Y+DY
13    CONTINUE
      RETURN
      END
