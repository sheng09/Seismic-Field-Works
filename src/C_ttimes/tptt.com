C     Common block declarations for module name information for tau-p
C     subroutines

      character modnam*64
      logical first

      common /tpttmd/ first,modnam
