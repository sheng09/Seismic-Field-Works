      program ttimes
      save
      parameter (max=60, Re=6371.0)
      logical log,prnt(3),okm,overb,otoa
      character*8 phcd(max),phlst(10)
      character*64 modnam, arg
      dimension tt(max),dtdd(max),dtdh(max),dddp(max),mn(max),ts(max)
      dimension toa(max),usrc(2)
c     data in/1/,modnam/'iasp91'/,phlst(1)/'query'/,prnt(3)/.true./
      data in/1/,phlst(1)/'query'/,prnt(3)/.true./
      data okm/.false./, overb/.true./, otoa/.false./
      include 'modnam.inc'
      include 'version.inc'

c     Environment variable LIBTAUMOD is module name for test table.  If
c     unset, defaults to value of MODLIB.
      ix = indexr(modnam,'/')+1
      call evget('LIBTAUMOD',arg)
      if (arg .ne. ' ') then
	 if (arg(1:1) .eq. '/') then
	    modnam = arg
	 else
	    modnam(ix:) = arg
	 endif
      endif
c
      pi = 4*atan(1.0)
      degkm = Re*pi/180
      prnt(1) = .false.
      prnt(2) = .false.
      iskip = 0

      do 5 i=1,iargc()
	 if (i .le. iskip) go to 5
	 call getarg(i,arg)
	 if (arg .eq. '-debug') then
c           If either prnt(1) or prnt(2) is made .true., call assign for
c           output on unit 10.
	    prnt(1) = .true.
	    prnt(2) = .true.
	    open (10,file='ttim1.lis')
	 else if (arg .eq. '-model') then
	    iskip = i+1
	    call getarg(i+1,arg)
	    if (arg .ne. ' ') then
	       if (arg(1:1) .eq. '/') then
		  modnam = arg
	       else
		  modnam(ix:) = arg
	       endif

	    endif
	 else if (arg .eq. '-units') then
	    iskip = i+1
	    call getarg(i+1,arg)
	    if (arg .eq. 'km') then
	       okm = .true.
	    else if (arg(1:3) .eq. 'deg') then
	       okm = .false.
	    else
	       write(0,*) '**Unrecognized -units name'
	       okm = .false.
	    endif
	 else if (arg(1:5) .eq. '-verb') then
	    overb = .true.
	 else if (arg(1:3) .eq. '-ta') then
	    otoa = .true.
	 else if (arg(1:2) .eq. '-t') then
	    overb = .false.
	 else
	    write(0,*) '**Unrecognized parameter "',
     +         arg(1:index(arg,' ')-1),'", skipping.'
	 endif
5     continue

      i = indexr(modnam,'/')+1
      if (overb) then
         write(6,*) ver
         write(6,*) 'This routine for calculating travel times for'
         write(6,*) 'specific distances uses a set of precalculated'
         write(6,*) 'tau-p tables for the ',
     +      modnam(i:index(modnam,' ')),'model.'
         write(6,*)
      else
         write(6,'(3(1x,a))') ver,modnam(i:index(modnam,' ')-1),'model'
      endif

      call tabin(in,modnam,prnt)
      if (overb) then
      write(6,*) 'The source depth has to be specified and also'
      write(6,*) 'the phase codes or keywords for the required branches'
      write(6,*) 'ALL will give all available branches'
      write(6,*) 'P  gives P-up,P,Pdiff,PKP, and PKiKP'
      write(6,*) 'P+ gives P-up,P,Pdiff,PKP,PKiKP,PcP,pP,pPdiff,pPKP,' 
      write(6,*) '         pPKiKP,sP,sPdiff,sPKP, and sPKiKP'
      write(6,*) 'S  gives S-up,S,Sdiff, and SKS'
      write(6,*) 'S+ gives S-up,S,Sdiff,SKS,sS,sSdiff,sSKS,pS,pSdiff,'
      write(6,*) '         and pSKS '
      write(6,*) 'basic gives P+ and S+ as well as '
      write(6,*) '         ScP, SKP, PKKP, SKKP, PP, and PKPPKP '
      write(6,*)
      write(6,*) 'or give a generic phase name'
      write(6,*)
      write(6,*) 'You will have to enter a distance,'
      write(6,*) 'if this is negative a new depth is calculated'
      write(6,*) 'TO EXIT: give negative depth'
      write(6,*)
      endif
      call brnset(1,phlst,prnt)

c                                    choose source depth
 3    call query('Source depth (km):',log)
      read(*,*,end=13,err=13)zs

      if(zs.lt.0.) go to 13
      call depset(zs,usrc)
      if (usrc(1) .gt. 0) then
         vsrc = ((Re-zs)/Re)/usrc(1)
	 etap = (Re-zs)/(vsrc*180/pi)
      endif
      if (usrc(2) .gt. 0) then
         vsrc = ((Re-zs)/Re)/usrc(2)
	 etas = (Re-zs)/(vsrc*180/pi)
      endif
c                                    loop on delta
 1    write(*,*)
      if (okm) then
         call query('Enter range:',log)
         read(*,*,end=13,err=13)delta
	 delta=delta/degkm
      else
	 call query('Enter delta:',log)
         read(*,*,end=13,err=13)delta
      endif
      if(delta.lt.0.) go to 3
      if(okm)then
         if (otoa) then
            write(6,203)
	 else
	    write(6,103)
	 endif
      else
         if (otoa) then
            write(6,202)
	 else
            write(6,102)
	 endif
      endif

      call trtm(delta,max,n,tt,dtdd,dtdh,dddp,phcd)

      if(okm)delta=delta*degkm
      if(n.le.0)then
         if(okm)then
            write(*,104) 'range',delta
         else
            write(*,104) 'delta',delta
         endif
      else

	 do 4 i=1,n
            mn(i)=int(tt(i)/60.)
            ts(i)=amod(tt(i),60.)
	    if (0 .ne. index('pP',phcd(i)(1:1))) then
	       eta = etap
	    else
	       eta = etas
	    endif
	    toa(i) = 180/pi*asin(abs(dtdd(i))/eta)
	    if (dtdh(i) .gt. 0) toa(i) = 180 - toa(i)
	    if(okm)then
	       dtdd(i)=dtdd(i)/degkm
	       dddp(i)=dddp(i)/degkm**2
	    endif
 4       continue
c
	 if (.not.okm) then
	    if (otoa) then
               write(*,200) delta,
     1            (i,phcd(i),tt(i),toa(i),dtdd(i),dtdh(i),dddp(i),
     2             i=1,n)
            else
               write(*,100) delta,
     1            (i,phcd(i),tt(i),mn(i),ts(i),dtdd(i),dtdh(i),dddp(i),
     2             i=1,n)
            endif
         else
	    if (otoa) then
               write(*,201) delta,
     1            (i,phcd(i),tt(i),toa(i),1e3*dtdd(i),dtdh(i),dddp(i),
     2             i=1,n)
            else
               write(*,101) delta,
     1            (i,phcd(i),tt(i),mn(i),ts(i),1e3*dtdd(i),dtdh(i),
     2             dddp(i),i=1,n)
            endif
         endif
      endif
      go to 1
c                                    end delta loop
 100  format(/1x,f6.2,i5,2x,a,f9.2,i4,f7.2,f11.4,1p2e11.2/
     1 (7x,i5,2x,a,0pf9.2,i4,f7.2,f11.4,1p2e11.2))
 101  format(/1x,f8.1,i3,2x,a,f9.2,i4,f7.2,f11.4,1p2e11.2/
     1 (7x,i5,2x,a,0pf9.2,i4,f7.2,f11.4,1p2e11.2))
 102  format(2x,'delta',
     1    '    # code       time(s)   (min s)     dT/dD',
     2    '     dT/dh      d2T/dD2')
 103  format(2x,'range',
     1    '    # code       time(s)   (min s)  dT/dD(ms/km)',
     2        '  dT/dh     d2T/dD2')
 104  format(/1x,'No arrivals for ',a,' =',f7.2)
 200  format(/1x,f6.2,i5,2x,a,f9.2,3x,f7.2,1x,f11.4,1p2e11.2/
     1 (7x,i5,2x,a,0pf9.2,3x,f7.2,1x,f11.4,1p2e11.2))
 201  format(/1x,f8.1,i3,2x,a,f9.2,3x,f7.2,1x,f11.4,1p2e11.2/
     1 (7x,i5,2x,a,0pf9.2,3x,f7.2,1x,f11.4,1p2e11.2))
 202  format(2x,'delta',
     1    '    # code       time(s)  take-off     dT/dD',
     2    '     dT/dh      d2T/dD2')
 203  format(2x,'range',
     1    '    # code       time(s)  take-off  dT/dD(ms/km)',
     2        '  dT/dh     d2T/dD2')
 13   continue
      close(in)
      if (prnt(1) .or. prnt(2)) close(10)
      call exit(0)
      end

      integer function indexr(string,ch)
      character string*(*), ch*(*)

      lch = len(ch)
      do 1 i=len(string)-lch,1,-1
	 if (string(i:i+lch-1) .eq. ch) go to 10
1     continue
      i = 0
10    continue
      indexr = i
      end
