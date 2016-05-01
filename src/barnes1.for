      program BARNES
c
c  This program performs an objective analysis of a scalar
c  field using Barnes' scheme. It can also be apllied to
c  a vector field provided each component is treated 
c  separately. The program doesn't include any bad data
c  verification since this depends on the field to be analysed.
c  A trivial setup has ,however,been arranged in subroutine 
c  INTERP.
c
      parameter( ns = 85,l = 27,m = 15 )
      common /dist/x(ns),y(ns),ib(ns),jb(ns),xg(l),yg(m)
      real   value(ns),ylat(ns),xlong(ns),hg(l,m)
      data   wlong,slat,dx,dy/97.,39.,0.5,0.5/
c
c  variable definitions.
c
c  ns    : number of station data
c  l     : east-west   dimension of the analysis domain
c  m     : north-south dimension of the analysis domain
c  slat  : southernmost latitude
c  wlong : westernmost longitude
c  dx,dy : grid size in the x and y directions
c
      open (10,file='station.da2',status='old    ')
      open (12, file='out6.dat   ',status='unknown')
      open (13, file='out6.grads',form='unformatted',status='unknown',
     *          access='direct',recl=l*m)
c
c  Read station data
c  ii     : station number
c  ylat   : station latitude
c  xlong  : station longitude
c  value  : station value of the field to analyse
c
 81   format(2x,i2,2x,3f10.2)
      do 5200 i = 1, ns
	 read (10,81)ii,ylat(i),xlong(i),value(i)
 5200 continue
c
c  Compute the coordinates of the stations(meters) from the origin 
c  (wlong,slat). Also find the coordinates of the closest grid point
c  to the station ,based on the following relationship :
c     iposx = ifact*difx + 1 , and 
c     iposy = jfact*dify + 1 , where 
c  ifact and jfact are the number of grid points within 1 degrees of
c  latitude ,and can be expressed as , ifact = integer(1./dx) and 
c  jfact = integer(1./dy). Difx,dify are the absolute values of the 
c  differences in degrees between the station and the origin,
c  in the x,y directions,respectively.
c
      do 5202 i = 1, ns
	 x(i)   = (wlong-xlong(i))*cos(ylat(i))*1.111e5
	 y(i)   = (ylat(i)-slat)*1.111e5
	 ib(i)  = 2*(wlong-xlong(i)) + 1
	 jb(i)  = 2*(ylat(i) - slat) + 1
 5202 continue
c
c  Compute the distance of all grid points from origin.
c
      do 5204 i = 1, l
	 xg(i)  = (i-1)*1.111e5*dx
 5204 continue
      do 5206 j = 1, m
	 yg(j)  = (j-1)*1.111e5*dy
 5206 continue
c
c  Perform the objective analysis .
c
	call OBJAN2 (value,hg,L,M,1)
c
c  Write outputs, 12 for ASCII ,13 for GRADS
c
      write (12,1000) ((hg(i,j),i=1,l),j=1,m)
 1000 format (10f8.2)
      write (13,rec=1) ((hg(i,j),i=1,l),j=1,m)

      stop
      end

      subroutine OBJAN2 (value,hg,ix,iy,number)
c
c  This subroutine performs an objective analysis
c  following barnes method .
c      
      parameter (ns = 85, l = 27, m = 15)
      common /dist/ x(ns),y(ns),ib(ns),jb(ns),xg(l),yg(m)       
      real value(ns),h(ns),g(L,M),hg(ix,iy)
c
c  Set the number of passes
c
      nop       = 4
      do 5210 n = 1, nop
	call INTERP (value,hg,ix,iy,number)
c
c  For the first iteration,the observed values are stored in h.
c
	 if (n.eq.1) then 
      do 5211 k = 1, ns        
	 h(k)   = value (k)
 5211 continue
      do 5212 i = 1, ix
      do 5212 j = 1, iy
	 g(i,j) = hg(i,j)
 5212 continue
	 else
c
c  Second and third iterations.
c
      do 5213 i = 1, ix
      do 5213 j = 1, iy
	 g(i,j) = g(i,j) + hg(i,j)
 5213 continue
	 endif
c
c  Fourth iteration .
c
	 if (n.eq.4) then
      do 5214 i = 1, ns
      value(k)  = h(k)
 5214 continue
      do 5215 i = 1, ix
      do 5215 j = 1, iy
	 hg(i,j)= g(i,j)
 5215 continue
	else
      do 5216 k = 1, ns
      value(k)  = h(k)-0.25*(g(ib(k),jb(k))+g(ib(k)+1,jb(k))
     &                 +g(ib(k),jb(k)+1)+g(ib(k)+1,jb(k)+1))
 5216 continue
	endif
 5210 continue
      return
      end
      subroutine INTERP (value,hg,ix,iy,number)
c
c  This subroutine Compute the weights for Barnes' scheme 
c  and the values of the interpolated functions.
c 
      parameter(ns=85,l=27,m=15)
      common /dist/x(ns),y(ns),ib(ns),jb(ns),xg(l),yg(m)
      real value(ns),hg(ix,iy)
  21  format(10f8.2)
c
c  Define the radius of influence .
c
      r         = 2.75e5
      rsqr      = r*r
      do 5220 i = 1, ix
      do 5220 j = 1, iy
	 suma   = 0.
	 sumb   = 0.
      do 5221 k = 1,ns
c
c  Compute distance between grid point and station .
c
      dist2  = (x(k)-xg(i)*cos(39.+dy*(j-1)))**2
     &                           +(y(k)-yg(j))**2
c
c  Set an arbitary test.Critical value needs to be 
c  changed  corresponding to input data set.
c
      if (value(k).gt.5000.) go to 5221
c
c  Check if the station is within the 
c  radius of influence .
c
      if (dist2.gt.rsqr) go to 5221
c
c  compute the weigths (eq. 5.28)
c
      wgt       = exp(-4.*dist2/rsqr)
c
c  compute the interpolated value (eq. 5.29)
c
      prod      = value(k)*wgt
      suma      = suma+wgt
      sumb      = sumb+prod
 5221 continue
      hg(i,j)   = sumb/suma
 5220 continue
      return
      end
