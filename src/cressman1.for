      program CRESSMAN                                                           
c
c  This program  performs an objective analysis of a vector 
c  field using  Cressman's successive corrections technique.
c  The method consists of modifying an initial guess field 
c  using obervations.
c
      parameter (l = 21,m = 13,np = 7)                                                
      real sumx(l,m),sumy(l,m),icount(l,m),store(l,m)                      
      real a(3,l,m),udev(1500),vdev(1500),rinfo(7,1500)
      real zstd(3),datau(l,m,np),datav(l,m,np)                                                                  
      common ug(l,m),vg(l,m)                                                    
      equivalence(icount,store)                                                 
      real id                                                                   
 1000 format(' too many vectors read ')                                         
c
c  define constants
c
      data zstd/1500.,3171.,12353./                                             
      data pres,imax,g,nmaps,lev,time/700.0,1500,9.81,1,1,12./
      data rltmn,rlnmn,rltinc,rlninc /-15.,-25.,2.5,2.5/
c
c   Definitions  : 
c   unit 1       : station data 
c   unit 7       : first guess field
c   unit 8       : output field in ASCII
c   unit 9       : output field in GrADS format
c
      open(1,file = 'station.dat',status='old')                                   
      open(7,file = 'uv21.dat',status='old')                                   
      open(8,file = 'out.dat',status='unknown')
      open(9,file='out.grads',status='unknown',form='unformatted',
     *       access='direct',recl=l*m)
c                                       
      do 5100 n = 1,nmaps                                                           
          nvect = 0                                                                   
          id    = 6.0                                                                    
c
c     read cloud motion vectors                                          
c
  20  read(1,1004,end=30)rlat,rlon,spd,dir                                      
 1004 format(2f12.2,4x,2f12.2)                                                  
c
c    Check validity of input data
c
  12  if (dir.gt.360.) go to 20
      if (dir.eq.999.) go to 20
      if (dir.lt.0.  ) go to 20
      if (spd.le.0.  ) go to 20
      if (spd.eq.999.) go to 20
c
c  Arrange data in array rinfo.This array is described 
c  in subroutine OBJAN . Note that for this example the
c  wind speed is in (m/s) and is converted to knots before
c  calling the objective analysis scheme(convertion factor 
c  is equal to 1.94).
c
      nvect          = nvect+1                                                             
      rinfo(7,nvect) = spd*1.94                                                   
      rinfo(6,nvect) = dir                                                        
      rinfo(5,nvect) = time                                                       
      rinfo(3,nvect) = rlon                                                       
      rinfo(4,nvect) = zstd(lev)                                                  
      rinfo(2,nvect) = rlat                                                       
      rinfo(1,nvect) = id                                                         
c
c  Display input data as they are read.
c 
      write(6,1004)rlat,rlon,spd*1.94,dir                                       
      if (nvect .le. 1500) goto 20                                               
      print 1000                                                                 
      stop ' too many vectors read '                                              
   30 continue                                                                  
      print *,'number of vector read' ,nvect
c                                                                               
c  Read the guess field                                                         
c
 878  format(10f8.2)                                                           
      do 5102 ip = 1,np                                                          
         read (7,878) ((datau(i,j,ip),i=1,l),j=1,m)                               
         read (7,878) ((datav(i,j,ip),i=1,l),j=1,m)                               
 5102 continue                                                                
c
c Extract the wind field at 500 mb .
c
      do 5104 i  = 1, l                                                            
      do 5104 j  = 1, m                                                            
         ug(i,j) = datau (i,j,4)                                               
         vg(i,j) = datav (i,j,4)                                               
 5104 continue                                                                
c
c  Write the observed u,v fields .                                                
c
      call BUFF (ug,l*m,8,2)                                                   
      call BUFF (vg,l*m,8,2)                                                   
      call BUFFGRADS (ug,l*m,9,2,1)                                                   
      call BUFFGRADS (vg,l*m,9,2,2)                                                   
c
c  Alter the wind field by setting its v-component                              
c  to zero. this is to be used as initial guess field .
c                         
      call ZERO1 (vg,l,m)                                                        
c                                                                               
c  Write the initial guess field .                                               
c                                                                               
      call BUFF (ug,l*m,8,2)                                                   
      call BUFF (vg,l*m,8,2)                                                   
      call BUFFGRADS (ug,l*m,9,2,3)                                                   
      call BUFFGRADS (vg,l*m,9,2,4)                                                   
c                                                                              
      print 1001                                                                 
 1001 format(///,10x,'begin objective analysis  ',i2)                           
c                                                                               
c  Perform the objective analysis                                               
c                                                                               
      call OBJAN (rinfo,ug,vg,l,m,nvect,time,lev,sumx
     &            ,sumy,a,icount,udev,vdev,store,rltmn
     &            ,rlnmn,rltinc,rlninc,zstd)                                              
c                                                                               
c   Write the analyzed fields .                                                   
c                                                                               
      call BUFF (ug,l*m,8,2)                                                     
      call BUFF (vg,l*m,8,2)
      call BUFFGRADS (ug,l*m,9,2,5)                                                   
      call BUFFGRADS (vg,l*m,9,2,6)                                                   
c                                                     
 5100 continue                                                                  
      stop                                                                      
      end                                                                       

      subroutine BUFF (a,lm,id,ip)
c
c  This subroutine is used to either 
c  read or write an array 'a' of dimension
c  lm = l*m ,to unit id.
c
c  Input     : 
c
c  a         : 1D array to be read/written
c  lm        : dimension of 'a'
c  id        : unit number 
c  ip        : flag for read/write 
c              ip=1 : read ,ip=2 : write
c
c                                               
      real a(lm)                                                           
      if (ip.eq.2) go to 30                                                       
      read (id,878) (a(ii),ii=1,lm)                                              
      write (*,*) (a(ii),ii=1,lm,10)                                            
      return                                                                    
   30 write(id,878) (a(ii),ii=1,lm)                                             
  878 format(5e13.6)                                                            
      return                                                                    
      end

      subroutine BUFFGRADS (a,lm,id,ip,num)
c
c  This subroutine is used to either 
c  read or write an array 'a' of dimension
c  lm = l*m ,to unit id.
c
c  Input     : 
c
c  a         : 1-D array to be read/written
c  lm       : dimension of 'a'
c  id        : unit number 
c  ip        : flag for read/write 
c              ip=1 : read ,ip=2 : write
c num     : record number for direct access
c
c                                               
      real a(l*m)                                                           
      if (ip.eq.2) go to 30                                                       
      read (id,878) (a(ii),ii=1,lm)                                              
      write (*,*) (a(ii),ii=1,lm,10)                                            
      return                                                                    
   30 write(id,rec=num) (a(ii),ii=1,lm)                                             
  878 format(5e13.6)                                                            
      return                                                                    
      end

      subroutine ZERO1 (a,l,m)                                                    
c
c  This subroutine initialize
c  2D fields to zero
c
      dimension a(l,m)                                                          
      do 5130 i = 1, l                                                               
      do 5130 j = 1, m                                                                
 5130    a(i,j) = 0.                                                            
      return                                                                    
      end                                                                       

      subroutine OBJAN (rinfo,ug,vg,l,m,nvect,time,lev,sumx,sumy,a,              
     &       icount,udev,vdev,store,rltmn,rlnmn,rltinc,rlninc,zstd)
c                    
c  This subroutine performs an objective analysis of the u and v          
c  components of the wind over a limited domain.  it uses the successive
c  corrections with 4 scans through the data set.         
c  the data need not be located exclusively within the analysis domain.        
c  to be provided by the user in the calling program.
c
c  Definition of variables .                          
c                                                                              
c  rinfo        : floating point variable dimensioned 7*(number of datum)          
c  rinfo(1,i)   : through rinfo(7,i) contains 7 pieces of information
c              
c  For each datum number i.
c                                                    
c  rinfo(1,i)   : whole number from 1.0 to 9.0 indicating source of datum.       
c  rinfo(2,i)   : degrees latitude of observation(+=north,-=south).              
c  rinfo(3,i)   : degrees rlongitude of observation(+=east,-=west).              
c  rinfo(4,i)   : pressure level of observation in mb.                           
c  rinfo(5,i)   : time of observation in hours.                                  
c  rinfo(6,i)   : wind direction in degrees(0.=north,90.=east,etc.).             
c  rinfo(7,i)   : wind speed in knots.                                           
c  l,m          : integer number of north-south and east-west grid points,       
c                  respectively.                                                               
c  nvect        : integer number of wind observations submitted.                 
c  time         : time of analysis in hours(floating point).                     
c  lev          : for the time being, set lev=1(integer).                        
c  ug,vg        : dimensioned (l*m) are the u and v components of the 
c                 firstguess field.u and v components of the analyzed 
c                 field are returned in these variables after calling 
c                 the subroutine.                   
c  rltmn,rlnmn  : southernmost latitude and westernmost longitude of the            
c                 analysis domain(+=north or east,-=south or west).                       
c  rltinc,rlninc: s latitude and longitude increments, respectively,            
c                 of the grid in degrees.                                                     
c                                                                              
c  the remaining variables are used as internal storage by OBJAN and           
c  must have the following dimensions in the calling program.                  
c                                                                              
c  sumx,sumy,icount,store dimensioned l*m. note that icount should           
c  be equivalenced to store in the calling program.                            
c  a is dimensioned (3*l*m) and udev,vdev dimensioned (nvect).
c                                 
      real sumx(l,m),sumy(l,m),denom(3),rainf(4)                                    
      real rinfo(7,nvect),ug(l,m),vg(l,m),ermx(4),gamma(9)                 
      character *7 ierror(4),source                                             
      dimension a(3,l,m),icount(l,m),udev(nvect),vdev(nvect)                    
      real store(l,m),zstd(3)                                              
      data denom/.25,-.50,1.0/                                                  
      data ierror/'value','time','height','location'/                           
      data ermx/50.,40.,30.,30./                                                
      data gamma/5*1.,.2,.2,.5,1./                                              
      print 115,l,m,nvect,time                                                  
  115 format(//,5x,'l ='i4,5x,'m ='i4,5x,'number vectors submitted ='i5,        
     1  5x,'time of analysis'f4.0,' z',//)                                      
c                                                                               
c  define some constants                                                          
c                                                                               
      trainf    = 6.0                                                                
      hrainf    = 500.                                                               
      l1        = l-1                                                                    
      m1        = m-1                                                                    
      rlnmx     = rlnmn+rlninc*l1                                                     
      rltmx     = rltmn+rltinc*m1                                                    
      avect     = float(nvect)                                                        
      xlm       = float(l*m)                                                            
      xl1m1     = float(l1*m1)                                                        
      d         = sqrt(xl1m1/avect)                                                       
c                                                                               
c   rainf(1),rainf(2) have been modified as well as nop                         
c                                                                               
      rainf(1)  = 5.0*d                                                            
      rainf(2)  = 4.0*d                                                            
      rainf(3)  = 3.0*d                                                            
      rainf(4)  = 2.0*d
c                                                            
c   number of operation                                                       
c
      nop=4                                                                     
      print 1010,d,nop                                                          
 1010 format(1h ,5x,'average station separation is'f4.1,'
     & grid lengths',/,5x,'objective analysis will be done 
     & for'i4,'  passes',//)              
      alpha     = -alog(0.4)/trainf**2                                                
      zalpha    = -alog(.5)/hrainf**2                                                
      pi180     = 4.*atan(1.0)/180.                                                   
      speedmx   = 200.                                                              
      dout      = rainf(1)*rlninc                                                      
      dout2     = -dout                                                               
      k         = 0                                                                       
c                                                                               
c  check quality of initial data .compute time and height weighting
c  factors.change direction and speed to u and v in meters per second.               
c                                                                               
      do 5110 i = 1, nvect                                                           
         if (    rinfo(7,i).gt.speedmx.or.rinfo(6,i).gt.360.
     &       .or.rinfo(6,i).lt.0.) ierr=1                                                                  
         if (   rinfo(7,i).gt.speedmx.or.rinfo(6,i).gt.360.
     &       .or.rinfo(6,i).lt.0.) go to 5                                                                 
c                                                                               
c  calculation of u,v from dir,spd 'check units of incoming spd'.           
c  u and v should be calculated to give units of m/sec.                     
c                                                                               
         r      = rinfo(6,i)*pi180                                                        
      rinfo(6,i)= .5155*rinfo(7,i)*sin(r)                                        
      rinfo(7,i)= .5155*rinfo(7,i)*cos(r)                                        
         t      = abs(rinfo(5,i)-time)                                                    
         if (t.gt.6.) t = abs(t-24.)                                                  
         if (t.gt.trainf) ierr = 2                                                     
         if (t.gt.trainf) go to 5                                                   
      rinfo(5,i)= exp(-alpha*t*t)                                                
         zz     = abs(rinfo(4,i)-zstd(lev))                                              
         if (zz.gt.hrainf) ierr = 3                                                    
         if (zz.gt.hrainf) go to 5                                                  
      rinfo(4,i)= exp(-zalpha*zz*zz)                                             
c                                                                               
c  find position of vector on grid and put coordinates in place of           
c  latitude and longitude                                                    
c                                                                               
         d1     = rinfo(2,i)-rltmx                                                       
         d2     = rinfo(2,i)-rltmn                                                       
         d3     = rinfo(3,i)-rlnmx                                                       
         d4     = rinfo(3,i)-rlnmn                                                       
         d2     = -d2                                                                    
         d4     = -d4                                                                    
      if(d1.gt.dout.or.d2.gt.dout.or.d3.gt.dout.or.d4.gt.dout) ierr=4            
      if(d1.gt.dout.or.d2.gt.dout.or.d3.gt.dout.or.d4.gt.dout) go to 5          
      if(d1.gt.0.or.d2.gt.0.or.d3.gt.0.or.d4.gt.0) rinfo(1,i)=                  
     &-abs(rinfo(1,i))                                                          
      rinfo(2,i)= -d2/rltinc+1.                                                  
      rinfo(3,i)= -d4/rlninc+1.                                                  
      go to 8                                                                   
    5 print 1006,i,ierror(ierr),(rinfo(kk,i),kk=1,7)                            
 1006 format(1x,'vector number'i5'  rejected (bad ',a8,2x,7f5.1)                
      go to 5110                                                                  
8        k      = k+1                                                                     
      do 5111 ii= 1, 7                                                               
 5111    rinfo(ii,k)=rinfo(ii,i)                                                   
 5110 continue                                                                  
      nn        = nvect-k                                                                
      if (k.ne.nvect) print 100,nn,k                                             
  100 format(1h ,5x,'there were',i4,3x,'vectors rejected, leaving 
     &a total of',i5)                                                                 
      nvect     = k                                                                   
      avect     = float(nvect)                                                        
c
c  find variance of initial data                                             
c
      usum      = 0.                                                                   
      vsum      = 0.                                                                 
      do 5112 i = 1, nvect                                                          
         usum   = usum+rinfo(6,i)                                                      
         vsum   = vsum+rinfo(7,i)                                                      
 5112 continue                                                                  
      ubar      = usum/avect                                                           
      vbar      = vsum/avect                                                           
      uvar      = 0.                                                                   
      vvar      = 0.                                                                   
      do 5113 i = 1,nvect                                                          
         uvar   = uvar+(ubar-rinfo(6,i))*(ubar-rinfo(6,i))                             
         vvar   = vvar+(vbar-rinfo(7,i))*(vbar-rinfo(7,i))                             
 5113 continue                                                                  
      uvar      = uvar/avect                                                           
      vvar      = vvar/avect                                                           
c                                                                               
c  interpolation of data                                                     
c                                                                               
      do 5114 ipass=1,nop
c                                                       
      print 1990,ipass                                                          
 1990 format(///,1h ,18x,'pass number'i3)
c                                       
         r      = rainf(ipass)                                                            
         r2     = r*r                                                                    
         ermax  = ermx(ipass)                                                         
c                                                                               
c  first find deviations at stations from guess field                        
c                                                                               
      do 5115 ist = 1, nvect                                                         
         x      = rinfo(3,ist)                                                            
         y      = rinfo(2,ist)                                                            
c                                                                               
c  calculate deviation at station inside grid                                
c                                                                               
         if (rinfo(1,ist).gt.0.) go to 15                                           
      go to 5115                                                                  
15    continue                                                                  
         ix     = x+.5                                                                   
         iy     = y+.5                                                                   
      if (ix.eq.1) ix=2                                                          
      if (ix.eq.l) ix=l1                                                         
      if (iy.eq.1) iy=2                                                          
      if (iy.eq.m) iy=m1                                                         
         xp     = x-float(ix)                                                            
         yp     = y-float(iy)                                                            
         dx     = 1.                                                                     
         dy     = 1.                                                                     
         yk1    = yp*(yp-dy)                                                            
         yk2    = (yp+dy)*(yp-dy)                                                       
         yk3    = (yp+dy)*yp                                                            
         xk1    = xp*(xp-dx)                                                            
         xk2    = (xp+dx)*(xp-dx)                                                       
         xk3    = (xp+dx)*xp                                                            
         g1     = xk1*yk1*denom(1)                                                       
         g2     = xk2*yk1*denom(2)                                                       
         g3     = xk3*yk1*denom(1)                                                       
         g4     = xk1*yk2*denom(2)                                                       
         g5     = xk2*yk2*denom(3)                                                       
         g6     = xk3*yk2*denom(2)                                                       
         g7     = xk1*yk3*denom(1)                                                       
         g8     = xk2*yk3*denom(2)                                                       
         g9     = xk3*yk3*denom(1)                                                       
         ixm    = ix-1                                                                  
         ixp    = ix+1                                                                  
         iym    = iy-1                                                                  
         iyp    = iy+1
c                                                                  
         uval   = g1*ug(ixm,iym)+g2*ug(ix,iym)+g3*ug(ixp,iym)
     &              +g4*ug(ixm,iy)+g5*ug(ix,iy)+g6*ug(ixp,iy)
     &           +g7*ug(ixm,iyp)+g8*ug(ix,iyp)+g9*ug(ixp,iyp)
c                            
         vval   = g1*vg(ixm,iym)+g2*vg(ix,iym)+g3*vg(ixp,iym)
     &              +g4*vg(ixm,iy)+g5*vg(ix,iy)+g6*vg(ixp,iy)
     &           +g7*vg(ixm,iyp)+g8*vg(ix,iyp)+g9*vg(ixp,iyp)
c                            
      udev(ist) = rinfo(6,ist)-uval                                               
      vdev(ist) = rinfo(7,ist)-vval                                               
      if (abs(udev(ist)).gt.ermax) go to 39                                      
      if (abs(vdev(ist)).gt.ermax) go to 39                                      
      go to 5115                                                                  
39    continue                                                                  
      source    = 'inside'                                                           
      xlat      = (rinfo(2,ist)-1.)*rltinc+rltmn                                       
      xlon      = (rinfo(3,ist)-1.)*rlninc+rlnmn                                       
      print 166,source,ist,xlat,xlon,rinfo(6,ist),rinfo(7,ist),                 
     &  uval,vval,udev(ist),vdev(ist)                                           
  166 format(1x,'bad piece of data 'a7,' grid    vector number'i5,              
     &  '  lat='f6.1,'  long='f6.1,                                             
     &  '  u='g10.3,'  v='g10.3,/,10x,'  uguess='g10.3,'  vguess='g10.3,        
     &  '  udev='g10.3,'  vdev='g10.3)                                          
      udev(ist) = 999.                                                            
      vdev(ist) = 999.                                                            
 5115 continue                                                                  
c
c     calculate deviation at station outside grid                               
c                                                                               
      do 5116 ist=1,nvect                                                         
         id     = ifix(abs(rinfo(1,ist))+0.01)                                           
      if (rinfo(1,ist).gt.0.) go to 5116                                           
         x      = rinfo(3,ist)                                                            
         y      = rinfo(2,ist)                                                            
         a1     = 0.                                                                     
         a2     = 0.                                                                     
         a3     = 0.                                                                     
         jj     = y                                                                      
         j1     = jj-r-1                                                                 
      if (j1.gt.m) go to 44                                                      
         j2     = jj+r+1                                                                 
      if (j2.lt.1) go to 44                                                      
      if (j1.lt.1) j1 = 1                                                          
      if (j2.gt.m) j2 = m                                                          
         ii     = x                                                                      
         i1     = ii-r-1                                                                 
         i2     = ii+r+1                                                                 
      if (i1.gt.l) go to 44                                                      
      if (i2.lt.1) go to 44                                                      
      if (i1.lt.1) i1=1                                                          
      if (i2.gt.l) i2=l                                                          
      do 5117 i = i1, i2                                                             
         xdist  = abs(x-float(i))                                                     
      do 5118 j = j1,j2                                                             
         ydist  = abs(y-float(j))                                                     
         dist2  = xdist*xdist+ydist*ydist                                             
         p1     = r2-dist2                                                               
      if (p1) 5118,5118,5518                                                           
 5518    p2     = r2+dist2                                                               
         weight = p1/p2                                                              
         a1     = a1+weight*ug(i,j)                                                      
         a2     = a2+weight*vg(i,j)                                                      
         a3     = a3+weight                                                              
 5118 continue                                                                  
 5117 continue                                                                  
      if (a3.lt.0.0000000001) go to 44                                           
      udev(ist) = rinfo(6,ist)-a1/a3                                              
      vdev(ist) = rinfo(7,ist)-a2/a3                                              
      uval      = a1/a3                                                                
      vval      = a2/a3                                                                
      if (abs(udev(ist)).gt.ermax) go to 46                                      
      if (abs(vdev(ist)).gt.ermax) go to 46                                      
      go to 5116                                                                  
   46 continue                                                                  
      source    = 'outside'                                                          
      xlat      = (rinfo(2,ist)-1.)*rltinc+rltmn                                       
      xlon      = (rinfo(3,ist)-1.)*rlninc+rlnmn                                       
      print 166,source,ist,xlat,xlon,rinfo(6,ist),rinfo(7,ist),                 
     &  uval,vval,udev(ist),vdev(ist)                                           
44    continue                                                                  
      udev(ist) = 999.                                                            
      vdev(ist) = 999.                                                            
 5116 continue                                                                  
c                                                                               
c    calculation of correction to grid point value.                            
c                                                                               
      cgmax     = 0.5*r                                                               
      do 5119 i = 1,l                                                               
      do 5119 j = 1,m                                                               
        a(1,i,j)= 0.0                                                              
        a(2,i,j)= 0.0                                                              
        a(3,i,j)= 0.0                                                              
      sumx(i,j) = 0.                                                              
      sumy(i,j) = 0.                                                              
      icount(i,j)= 0                                                             
      do 5119 k = 1, 3                                                               
 5119  a(k,i,j) = 0.
c                                                                
      do 5120 ist=1,nvect                                                         
         id     = ifix(abs(rinfo(1,ist))+0.01)                                           
      if (udev(ist).eq.999) go to 5120                                             
         x      = rinfo(3,ist)                                                            
         y      = rinfo(2,ist)                                                            
         jj     = y                                                                      
         j1     = jj-r-1                                                                 
      if (j1.gt.m) go to 5120                                                      
         j2     = jj+r+1                                                                 
      if (j2.lt.1) go to 5120                                                      
      if (j1.lt.1) j1 = 1                                                          
      if (j2.gt.m) j2 = m                                                          
         ii     = x                                                                      
         i1     = ii-r-1                                                                 
         i2     = ii+r+1                                                                 
      if (i1.gt.l) go to 5120                                                      
      if (i2.lt.1) go to 5120                                                      
      if (i2.gt.l) i2=l                                                          
      if (i1.lt.1) i1=1                                                          
      do 5121 i = i1, i2                                                             
         xx     = i                                                                      
         xdist  = abs(x-xx)                                                           
      do 5122 j = j1,j2                                                             
         yy     = j                                                                      
         ydist  = abs(y-yy)                                                           
         dist2  = xdist*xdist+ydist*ydist                                             
         p1     = r2-dist2                                                               
         if(p1) 5122,5122,5522                                                           
 5522    p2     = r2+dist2                                                               
         weight = p1/p2*rinfo(5,ist)*gamma(id)*rinfo(4,ist)                          
      a(1,i,j)  = a(1,i,j)+udev(ist)*weight                                        
      a(2,i,j)  = a(2,i,j)+vdev(ist)*weight                                        
      a(3,i,j)  = a(3,i,j)+weight                                                  
      icount(i,j)= icount(i,j)+1                                                 
      sumx(i,j) = sumx(i,j)+x-xx                                                  
      sumy(i,j) = sumy(i,j)+y-yy                                                  
 5122 continue                                                                  
 5121 continue                                                                  
 5120 continue                                                                  
      do 5123 i = 1, l                                                               
      do 5123 j = 1, m                                                               
      if (icount(i,j).eq.0) go to 5123                                             
         xc     = sumx(i,j) / float(icount(i,j))                                           
         yc     = sumy(i,j)/float(icount(i,j))                                           
      sumx(i,j) = sqrt(xc*xc+yc*yc)                                               
c                                                                               
c  calculate the corrected fields                                            
c
         p1dp2  = 1.                                                                  
      sumx(i,j) = abs(sumx(i,j))                                                  
      if (sumx(i,j).lt.cgmax) go to 5162                                           
         p1dp2  = (r-sumx(i,j))/(0.5*r)                                               
 5162 if (a(3,i,j)) 5123,5123,5168                                                     
 5168 continue                                                                  
         ug(i,j)= ug(i,j)+a(1,i,j)/a(3,i,j)*p1dp2                                   
         vg(i,j)= vg(i,j)+a(2,i,j)/a(3,i,j)*p1dp2                                   
 5123 continue                                                                  
c                                                                               
c  find root mean square error in u and v                                    
c                                                                               
      k         = 0                                                                       
      ve        = 0.                                                                     
      ue        = 0.                                                                     
      do 5124 ist= 1, nvect                                                         
      if (rinfo(1,ist).le.0.) go to 5124                                           
         x      = rinfo(3,ist)                                                            
         y      = rinfo(2,ist)                                                            
         ix     = x+.000001                                                              
         iy     = y+.000001                                                              
         if(ix.eq.l) ix=l1                                                         
         if(iy.eq.m) iy=m1                                                         
         ixp1   = ix+1                                                                 
         iyp1   = iy+1                                                                 
         u1     = ug(ix,iy)
         v1     = vg(ix,iy)
         u2     = ug(ix,iyp1)
         v2     = vg(ix,iyp1)
         u3     = ug(ixp1,iyp1)
         v3     = vg(ixp1,iyp1)
         u4     = ug(ixp1,iy)
         v4     = vg(ixp1,iy)
         d1     = x-float(ix)                                                            
         d2     = y-float(iy)                                                            
         uu     = u1*(1.-d1)*(1.-d2)+u4*d1*(1.-d2)+
     &            u3*d1*d2+u2*d2*(1.-d1)              
         vv     = v1*(1.-d1)*(1.-d2)+v4*d1*(1.-d2)+
     &            v3*d1*d2+v2*d2*(1.-d1)              
         u1     = (rinfo(6,ist)-uu)                                                      
         ue     = ue+u1*u1                                                               
         v1     = rinfo(7,ist)-vv                                                        
         ve     = ve+v1*v1                                                               
         k      = k+1                                                                     
 5124    continue                                                                  
      ue        = ue/float(k)                                                            
      ve        = ve/float(k)                                                            
      urms      = sqrt(ue)                                                             
      vrms      = sqrt(ve)                                                             
      vec       = sqrt(ue+ve)                                                           
      print 200,urms,vrms,vec                                                   
  200 format(//,18x,'root mean square u error',f10.3,2x,'m/sec',/,              
     1  18x,'root mean square v error',f10.3,2x,'m/sec',                        
     2  /,18x,'rms vector error'f10.3,2x,'m/sec')                               
  305 format('(1h0,'i2'i4)')                                                    
 5114 continue                                                                  
      call smooth(vg,store,l,m)                                                 
      call smooth(ug,store,l,m)
c                                                 
c  find variance of analyzed field
c                                           
      usum      = 0.                                                                   
      vsum      = 0.                                                                   
      do 5125 i = 1, l                                                             
      do 5125 j = 1, m                                                             
         usum   = usum + ug(i,j)                                                         
         vsum   = vsum + vg(i,j)                                                         
 5125 continue                                                                  
      ubar2     = usum/xlm                                                            
      vbar2     = vsum/xlm                                                            
      uvar2     = 0.                                                                  
      vvar2     = 0.                                                                  
      do 5126 i = 1, l                                                             
      do 5126 j = 1, m                                                             
         uvar2  = uvar2+(ug(i,j)-ubar2)*(ug(i,j)-ubar2)                               
         vvar2  = vvar2+(vg(i,j)-vbar2)*(vg(i,j)-vbar2)                               
 5126 continue                                                                  
      uvar2     = uvar2/xlm                                                           
      vvar2     = vvar2/xlm                                                           
      upctv     = (uvar2/uvar)*100.                                                   
      vpctv     = (vvar2/vvar)*100.                                                   
      print 2060,ubar,vbar,uvar,vvar                                            
 2060 format(/,1h ,10x,'initial data   ubar='f6.2,'  vbar='f6.2,'  u var        
     1iance='f6.2,'  v variance='f6.2)                                          
      print 2064,ubar2,vbar2,uvar2,vvar2                                        
 2064 format(/,1h ,10x,'analyzed field   ubar='f6.2,'  vbar='f6.2,'  u v        
     1ariance='f6.2,'  v variance='f6.2)                                        
      print 2066,upctv,vpctv                                                    
 2066 format(/,1h ,10x,'u per cent variance'f6.1,/,1h ,10x,'v per cent v        
     1ariance'f6.1)                                                             
      return                                                                    
      end

      subroutine SMOOTH (a,b,l,m)
c
c  This subroutine performs smoothing
c  on a 2D array . Input array 'a' is
c  modified on output.
c                                                
      real a(l,m),b(l,m)                                                   
      l1        = l-1                                                                    
      m1        = m-1                                                                    
      rnu       = 0.1                                                                   
      rnu1      = 0.5*rnu*(1.-rnu)                                                     
      rnu2      = 0.25*rnu*rnu                                                         
      do 5120 i = 2,l1                                                              
      ip=i+1                                                                    
      im=i-1                                                                    
      do 5120 j = 2,m1                                                              
         b(i,j) = a(i,j)+rnu1*(a(im,j)+a(ip,j)+a(i,j+1)
     &             +a(i,j-1)-4.*a(i,j))+rnu2*(a(im,j+1)
     &        +a(im,j-1)+a(ip,j+1)+a(ip,j-1)-4.*a(i,j))                 
 5120 continue                                                                  
      do 5121 i = 2, l1                                                              
      do 5121 j = 2, m1                                                              
         a(i,j) = b(i,j)                                                             
 5121 continue                                                                  
      return                                                                    
      end                                                                       
