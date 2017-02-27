// TODO: implement Barnes interpolation

void BARNES (void)
{
    /*
        This program performs an objective analysis of a scalar
        field using Barnes' scheme. It can also be applied to
        a vector field provided each component is treated
        separately. The program doesn't include any bad data
        verification since this depends on the field to be analysed.
        A trivial setup has, however, been arranged in subroutine
        INTERP.
    */

    const ns = 85, l = 27, m = 15;
    common /dist/x(ns),y(ns),ib(ns),jb(ns),xg(l),yg(m)
    real   value(ns),ylat(ns),xlong(ns),hg(l,m)
    data   wlong,slat,dx,dy/97.,39.,0.5,0.5/

    /*
        variable definitions.

        ns    : number of station data
        l     : east-west   dimension of the analysis domain
        m     : north-south dimension of the analysis domain
        slat  : southernmost latitude
        wlong : westernmost longitude
        dx,dy : grid size in the x and y directions
    */

    open (10,file='station.da2',status='old    ')
    open (12, file='out6.dat   ',status='unknown')
    open (13, file='out6.grads',form='unformatted',status='unknown',
    *          access='direct',recl=l*m)

    /*
        Read station data
        ii     : station number
        ylat   : station latitude
        xlong  : station longitude
        value  : station value of the field to analyse
    */

    81   format(2x,i2,2x,3f10.2)
    do 5200 i = 1, ns
    read (10,81)ii,ylat(i),xlong(i),value(i)
    5200 continue

    /*
        Compute the coordinates of the stations(meters) from the origin
        (wlong,slat). Also find the coordinates of the closest grid point
        to the station ,based on the following relationship :
            iposx = ifact*difx + 1 , and
            iposy = jfact*dify + 1 , where
        ifact and jfact are the number of grid points within 1 degrees of
        latitude ,and can be expressed as , ifact = integer(1./dx) and
        jfact = integer(1./dy). Difx,dify are the absolute values of the
        differences in degrees between the station and the origin,
        in the x,y directions,respectively.
    */

    do 5202 i = 1, ns
    x(i)   = (wlong-xlong(i))*cos(ylat(i))*1.111e5
    y(i)   = (ylat(i)-slat)*1.111e5
    ib(i)  = 2*(wlong-xlong(i)) + 1
    jb(i)  = 2*(ylat(i) - slat) + 1
    5202 continue
    /*
        Compute the distance of all grid points from origin.
    */
    do 5204 i = 1, l
    xg(i)  = (i-1)*1.111e5*dx
    5204 continue
    do 5206 j = 1, m
    yg(j)  = (j-1)*1.111e5*dy
    5206 continue
    /*
        Perform the objective analysis .
    */
    call OBJAN2 (value,hg,L,M,1)
    /*
        Write outputs, 12 for ASCII ,13 for GRADS
    */
    write (12,1000) ((hg(i,j),i=1,l),j=1,m)
    1000 format (10f8.2)
    write (13,rec=1) ((hg(i,j),i=1,l),j=1,m)
}

void OBJAN2 (value, hg, ix, iy, number)
{
    /*
        This subroutine performs an objective analysis
        following barnes method .
    */
    parameter (ns = 85, l = 27, m = 15)
    common /dist/ x(ns),y(ns),ib(ns),jb(ns),xg(l),yg(m)
    real value(ns),h(ns),g(L,M),hg(ix,iy)
    /*
        Set the number of passes
    */
    nop       = 4
    do 5210 n = 1, nop
    call INTERP (value,hg,ix,iy,number)
    /*
        For the first iteration,the observed values are stored in h.
    */
    if (n.eq.1) then
    do 5211 k = 1, ns
    h(k)   = value (k)
    5211 continue
    do 5212 i = 1, ix
    do 5212 j = 1, iy
    g(i,j) = hg(i,j)
    5212 continue
    else
    /*
        Second and third iterations.
    */
    do 5213 i = 1, ix
    do 5213 j = 1, iy
    g(i,j) = g(i,j) + hg(i,j)
    5213 continue
    endif
    /*
        Fourth iteration .
    */
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
}

void INTERP (value, hg, ix, iy, number)
{
    /*
        This subroutine Compute the weights for Barnes' scheme
        and the values of the interpolated functions.
    */
    parameter(ns=85,l=27,m=15)
    common /dist/x(ns),y(ns),ib(ns),jb(ns),xg(l),yg(m)
    real value(ns),hg(ix,iy)
    21  format(10f8.2)
    /*
        Define the radius of influence .
    */
    r         = 2.75e5
    rsqr      = r*r
    do 5220 i = 1, ix
    do 5220 j = 1, iy
    suma   = 0.
    sumb   = 0.
    do 5221 k = 1,ns
    /*
        Compute distance between grid point and station .
    */
    dist2  = (x(k)-xg(i)*cos(39.+dy*(j-1)))**2
    &                           +(y(k)-yg(j))**2
    /*
        Set an arbitary test.Critical value needs to be
        changed  corresponding to input data set.
    */
    if (value(k).gt.5000.) go to 5221
    /*
        Check if the station is within the
        radius of influence .
    */
    if (dist2.gt.rsqr) go to 5221
    /*
        compute the weigths (eq. 5.28)
    */
    wgt       = exp(-4.*dist2/rsqr)
    /*
        compute the interpolated value (eq. 5.29)
    */
    prod      = value(k)*wgt
    suma      = suma+wgt
    sumb      = sumb+prod
    5221 continue
    hg(i,j)   = sumb/suma
    5220 continue
    return
}
