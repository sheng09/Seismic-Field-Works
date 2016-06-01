C     Subroutine TPTT -- Return travel time and phase info from a tau-p
C        based tabulation a la Buland and Kennett.
C
C     Called via:
C        x = tptt(id,delta,depth,dtdd,dtdh)
C        n = mtptt(id,delta,depth,max,idphs,tt,dtdd,dtdh)
C        n = ntptt(id,delta,depth,max,idphs,tt,dtdd,dtdh,d2tdd2)
C
C     Assumes:
C        id - character phase identifier (e.g., S, P, ScS, SKS) or 'all'
C        delta - distance (in degrees)
C        depth - depth (km)
C        max - maximum number of travel time values to be returned
C
C     Returns:
C        Function value (tptt):
C           travel time (in seconds)
C           -1 - Unknown phase id
C           -2 - More than one travel time associated with that branch -- be
C                more specific with your phase name or use mtptt
C
C        Function value (mtptt):
C           number of travel times returned for this phase.  This may be
C           larger than max, meaning that more are returned than space was
C           provided for.
C
C        idphs - character array of matching phase ids (e.g., PKPdf matches
C           PKP as the input ID).  Number of values returned is <= max.
C        tt - array of returned travel time values.  The number of values
C           is always <= max.
C        dtdd - dt/d delta of the phase in question.  If mtptt is used,
C           this is an array of values.
C        dtdh - dt/d depth of the phase in question.  If mtptt is used,
C           this is an array of values.
C        d2tdd2 - d2t/delta2 of the phase in question.  If mtptt is used,
C           this is an array of values.
C
C     Set travel time tables to be use by calling the tpmod routine; see
C     below.
C
C     To strip phase names returned by any of these routines of their branch
C     identifiers (e.g. PKPdf -> PKP), see the phgen routine, below.
C
C     Based on "ttimes" code distributed by Buland and Kennett.  By
C        G. Helffrich Carnegie/DTM Aug 28, 1991.
C
C     New routine: tpttsv(z,vp,vs) to return P and S source velocities.
C        G. Helffrich UB/14 Oct. 2007

      subroutine tpttsv(depth,vp,vs)
      real u(2)
      character idph*8

      call tpttsub('P',0.0,depth,n,1,u,idph,tr,dtddr,dtdhr,d2tddr)
      vp = 1/u(1)
      if (u(2).gt.0) then
	 vs = 1/u(2)
      else
         vs = 0.0
      endif
      end

      function tptt(id,delta,depth,dtdd,dtdh)
      character id*(*)
      real delta, depth, dtdd, dtdh, v(2)
      parameter (max=100)
      character idph(max)*8
      real tr(max),dtddr(max),dtdhr(max),d2tddr(max)

      call tpttsub(id,delta,depth,n,max,v,idph,tr,dtddr,dtdhr,d2tddr)

C     Check results for absent or multiple arrivals.
      if (n .eq. 0) then
	 tptt = -1
      else if (n .gt. 1) then
	 tptt = -2
      else
	 tptt = tr(1)
	 dtdd = dtddr(1)
	 dtdh = dtdhr(1)
      endif
      end

      function mtptt(id,delta,depth,max,idph,tt,dtdd,dtdh)
      character id*(*), idph(max)*(*)
      real delta, depth, tt(max), dtdd(max), dtdh(max), v(2)
      parameter (maxph=100)
      character idphr(maxph)*16
      real tr(maxph),dtddr(maxph),dtdhr(maxph),d2tddr(maxph)

C     Call subroutine to do the real work
      call tpttsub(id,delta,depth,n,maxph,
     &             v,idphr,tr,dtddr,dtdhr,d2tddr)

C     Copy n or max results for return.
      do 10 i=1,min(n,max)
	 idph(i) = idphr(i)
	 tt(i) = tr(i)
	 dtdd(i) = dtddr(i)
	 dtdh(i) = dtdhr(i)
10    continue
      mtptt = n
      end

      function ntptt(id,delta,depth,max,idph,tt,dtdd,dtdh,d2tdd2)
      character id*(*), idph(max)*(*)
      real delta, depth, tt(max), dtdd(max), dtdh(max), d2tdd2(max)
      parameter (maxph=100)
      character idphr(maxph)*16
      real tr(maxph),dtddr(maxph),dtdhr(maxph),d2tddr(maxph),v(2)

C     Call subroutine to do the real work
      call tpttsub(id,delta,depth,n,maxph,
     &             v,idphr,tr,dtddr,dtdhr,d2tddr)

C     Copy n or max results for return.
      do 10 i=1,min(n,max)
	 idph(i) = idphr(i)
	 tt(i) = tr(i)
	 dtdd(i) = dtddr(i)
	 dtdh(i) = dtdhr(i)
	 d2tdd2(i) = d2tddr(i)
10    continue
      ntptt = n
      end

      subroutine tpttsub(id,delta,depth,n,max,
     +   usrc,idr,tr,dtddr,dtdhr,d2tddr)
      character id*(*), idr(max)*(*)
      real delta,depth,tr(max),dtddr(max),dtdhr(max),d2tddr(max)

      parameter (maxph=100)
      character phcdr(maxph)*8, phnull*8
      character phnmg*8
      real usrc(2), dddpr(maxph)
      logical flags(3), ok
      include 'tptt.com'
      data flags /3*.false./

      if (first) then
	 do 4 i=1,len(phnull)
	    phnull(i:i) = char(0)
4        continue
C        Search for unused fortran i/o unit
	 do 5 i=99,9,-1
	    inquire(unit=i,opened=ok)
	    if (.not. ok) go to 7
5        continue
	 stop '**TPTT:  Can''t find an unused I/O unit!'
7        continue
	 call tabin(i,modnam,flags)
	 first = .false.
      endif

      if (max .gt. maxph) pause '**TPTT:  MAX value too big.'

C     Clear prior phase names
      do 10 i=1,maxph
	 phcdr(i) = phnull
10    continue
C     phcdr(1) = id
      phcdr(1) = 'all'

C     Call routines to do the real work
      call brnset(1,phcdr,flags)
      call depset(depth,usrc)
      call trtm(delta,max,nret,tr,dtddr,dtdhr,dddpr,phcdr)

C     Return only phases that were requested.
      n = 0
      do 20 i=1,min(nret,max)
	 if (id .eq. 'all' .or. id .eq. phnmg(phcdr(i))) then
	    n = n + 1
	    idr(n) = phcdr(i)
	    tr(n) = tr(i)
	    dtdhr(n) = dtdhr(i)
	    dtddr(n) = dtddr(i)
	    d2tddr(n) = dddpr(i)
	 endif
20    continue
      end

      character*(*) function phnmg(id)
C     phnmg  --  Return a "generic" phase name given one with branch names,
C                etc. in it.  The branch names are removed.
C
C     Assumes:
C        id - character identifier of phase name.
C
C     Returns:
C        function result -
C           name stripped of any "junk" that will inhibit a phase name match.
C
C     Notes
C        The items stripped are the suffixes 'ab' 'ac' 'bc' and 'df', and
C        crustal suffixes 'g', 'b', and 'n'.

      character id*(*)
      parameter (nsfx = 9, lsfx = 5, lindx=10)
      integer indx(lindx), sfxl(nsfx)
      character sfx(nsfx)*(lsfx)
      data sfx /'ab ', 'ac ', 'bc ', 'df ','n  ','g  ','b  ',
     +          'diff ','dif '/
      data sfxl/ 1,     1,     1,     1,    0,    0,    0   ,
     +           3,     2 /

C     Search through all characters in string.  Eliminate 'suffix' characters.
      i = 1
      n = 0
10    continue
         if (i .gt. len(id)) go to 50
	 do 30 j=1,nsfx
	    nlen = sfxl(j)
	    if (len(id)-i .ge. nlen) then
	       if (sfx(j)(1:1+nlen) .eq. id(i:i+nlen)) then
		  i = i + nlen + 1
		  go to 10
	       endif
	    endif
30       continue
	 n = n + 1
	 if (n .gt. lindx) pause '**TPTT:  Phase name too complex.'
	 indx(n) = i
	 i = i + 1
      go to 10

C     Collect remaining characters and glue together for result
50    continue
      phnmg = ' '
      do 60 i=1,n
	 phnmg(i:i) = id(indx(i):indx(i))
60    continue
      end

C     tpmod  --  Set module name for use with tau-p tables.
C
C     called via:  call tpmod(module)
C
C     Assumes:
C        modnam - suffix of module name.  Default directory is prefixed to
C                 this value.

      subroutine tpmod(module)
      character module*(*)
      include 'tptt.com'

      i = index(modnam,'/')
      if (i .eq. 0) then
	 modnam = module
      else
	 j = i+1
	 do 10 k=i+1,len(modnam)
	    i = index(modnam(j:),'/') + j
	    if (i .eq. j) go to 11
	    j = i
10       continue
11       continue
	 modnam = modnam(1:j-1) // module
      endif
      first = .true.
      end

      block data
C     Initialize common values for module name.
      include 'tptt.com'

      data first /.true./
      include 'modnam.inc'
      end
