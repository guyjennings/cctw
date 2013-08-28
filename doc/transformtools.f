c***********************************
c Computes the inverse of a matrix
c***********************************
	subroutine myeigensystem(matrix,eigenvectors,eigenvalues)
	implicit none

	double precision matrix(3,3),eigenvectors(3,3), eigenvalues(3)
	double precision amat(3,3), tolerance, xyz(3), newxyz(3), diffvec(3), diff
	double precision vec0(3), vec1(3), vec2(3), vec3(3), val1, val2, val3, lambda
	integer i, j, n, maxiterations

	maxiterations = 100000
	tolerance = 1.0E-10
	
	do i=1,3
	do j=1,3
		amat(i,j) = matrix(i,j)
	end do
	end do
	
	call srand(0)
	do i=1,3
		vec0(i) = rand(0)
	end do
c	print *, "random vector = ", vec0

	call copyvec(vec0,xyz)
	do n = 1,maxiterations
		call matdotvec(amat,xyz,newxyz)
		call normvec(newxyz,val1,newxyz)
		call addvec(newxyz,dble(1),xyz,dble(-1),diffvec)
		call normvec(diffvec,diff,diffvec)
		call copyvec(newxyz,xyz)
		call copyvec(xyz,vec1)
c		print *, "n = ", n, ", xyz = ", vec1, ", diff = ", diff
		if (diff < tolerance) goto 100
	end do
	print *, "Iteration limit of", maxiterations, " exceeded"

100	call copyvec(vec0,xyz)
	do n = 1,maxiterations
		call matdotvec(amat,xyz,newxyz)
		call vecdotvec(newxyz,vec1,lambda)
		call addvec(newxyz,dble(1),vec1,-lambda,newxyz)
		call normvec(newxyz,val2,newxyz)
		call addvec(newxyz,dble(1),xyz,dble(-1),diffvec)
		call normvec(diffvec,diff,diffvec)
		call copyvec(newxyz,xyz)
		call copyvec(xyz,vec2)
c		print *, "n = ", n, ", xyz = ", xyz, ", diff = ", diff
		if (diff < tolerance) goto 200
	end do
	print *, "Iteration limit of", maxiterations, " exceeded"
	
200	call crossvec(vec1,vec2,xyz)
	call normvec(xyz,val3,xyz)
	call matdotvec(amat,xyz,xyz)
	call normvec(xyz,val3,vec3)
	
	do j=1,3
		eigenvectors(1,j) = vec1(j)
		eigenvectors(2,j) = vec2(j)
		eigenvectors(3,j) = vec3(j)
	end do
	eigenvalues(1) = val1
	eigenvalues(2) = val2
	eigenvalues(3) = val3
	
	
	print *, "Envelope Covariance Eigenvalues: ", eigenvalues
	print *, "Envelope Covariance Eigenvectors: "
	do i=1,3
		print *, (eigenvectors(i,j), j=1,3)
	end do

	return
	end

c ******************************
c Determine the global grid parameters given an envelope of qlab points from the dataset.
c ******************************
	subroutine envelope(samplename, envnum, envelopepoints, resworstq, ubmat, opnum, operators, symmetrize)
	implicit none
	integer maximages, maxpoints, maxops
	parameter (maximages = 1000, maxpoints = 1000000, maxops = 48)
	
	character samplename*20, gridfilename*20, symmfilename*20
	double precision envelopepoints(9*maximages,3), resworstq, ubmat(3,3), operators(maxops,3,3)
	integer envnum, opnum
	logical symmetrize
	
	integer m, n, i, j, nchars, griddim(3), pointnum
	double precision gridbasis(3,3), gridorigin(3), determ, pointlist(maxpoints,3), center(3)
	double precision covariance(3,3), eigenvectors(3,3), eigenvalues(3)
	double precision envmin(3), envmax(3), eiginv(3,3), ubimat(3,3), xyz(3), tempop(3,3), tempscalar


	call invmat(ubmat,ubimat)
	
	if (symmetrize) then
		symmfilename = samplename(1:nchars(samplename)) // '.sym'
		open (unit=8,file=symmfilename,access="sequential",status='old')
			read (8,*) opnum
			if (opnum.gt.maxops) then
				print *, "Number of operators exceeds maxops = ", maxops
				stop
			end if
			do n=1,opnum
				read(8,*) ((operators(n,i,j), j=1,3), i=1,3)
			end do
		close(8)
		print *, "Symmetrization operators (hkl basis):"
		do n=1,opnum
			print *, ((operators(n,i,j), j=1,3), i=1,3)
		end do
	else
		opnum = 1
		do i=1,3
		do j=1,3
			operators(1,i,j) = 0
			if (i.eq.j) operators(1,i,j) = 1
		end do
		end do
	end if


c	use any available symmetry to extend the envelope
	pointnum = envnum*opnum
	do m=1,envnum
	do n=1,opnum
		do i=1,3
			do j=1,3
				tempop(i,j) = operators(n,i,j)
			end do
		end do
		do i=1,3
			xyz(i) = envelopepoints(m,i)
		end do
		call qlab2hkl(ubimat,xyz,xyz)
		call matdotvec(tempop, xyz, xyz)
		call hkl2qlab(ubmat,xyz,xyz)
		do i=1,3
			pointlist(opnum*(m-1) + n,i) = xyz(i)
		end do
c		print *, "n = ", n, " point = ", (xyz(i), i=1,3)
	end do
	end do
	
	
c	find dateset center of mass
	do j=1,3
		center(j) = 0
		do n=1,pointnum
			center(j) = center(j) + pointlist(n,j)/pointnum
		end do
	end do
c	print *, "center: ", center
	do n=1,pointnum
		do j=1,3
			pointlist(n,j) = pointlist(n,j) - center(j)
		end do
	end do
	
c	compute covariance
	do i=1,3
	do j=1,3
		covariance(i,j) = 0
		do n=1,pointnum
			covariance(i,j) = covariance(i,j) + pointlist(n,i)*pointlist(n,j)/(pointnum-1)
		end do
	end do
	end do
	print *, ""
	print *, ""
	print *, "Envelope Covariance Matrix:"
	print *, ((covariance(i,j),j=1,3),i=1,3)
	
c	find eigenvectors
	call myeigensystem(covariance,eigenvectors,eigenvalues)
	call invmat(eigenvectors, eiginv)

c	rotate envelope points and determine min/max coordinate limits in rotated system
	do j=1,3
		envmin(j) =  1000000
		envmax(j) = -1000000
	end do
	do n=1,pointnum
		do j=1,3
			xyz(j) = pointlist(n,j)
		end do
		call matdotvec(eigenvectors, xyz, xyz)
		do j=1,3
			if (xyz(j).lt.envmin(j)) envmin(j) = xyz(j)
			if (xyz(j).gt.envmax(j)) envmax(j) = xyz(j)
c			print *, "j  = ", j, ", current = ", xyz(j), ", min = ", envmin(j), ", max = ", envmax(j)
		end do
	end do
	print *, "Rotated coordinate minima: ", envmin
	print *, "Rotated coordinate maxima: ", envmax	
	
	
c	compute qlab origin and basis (unrotated)
	call matdotvec(eiginv, envmin, gridorigin)
	call addvec(gridorigin,dble(1),center,dble(1),gridorigin)
	
	call identitymat(gridbasis)
	do j=1,3
		gridbasis(j,j) = envmax(j) - envmin(j)	
	end do
	call matdotmat(eiginv,gridbasis,gridbasis)

	print *, "Global origin (Q): ", gridorigin
	print *, "Global basis (Q):"
	do i=1,3
		print *, (gridbasis(i,j),j=1,3)
	end do
	call determinant(gridbasis,determ)
	print *, "Global volume (Q): ", determ

c	determine optimistic grid dimensions
	do j=1,3
		do i=1,3
			xyz(i) = gridbasis(i,j)
		end do
		call vecdotvec(xyz,xyz,tempscalar)
		griddim(j) = nint(sqrt(tempscalar)/resworstq)
	end do

c	transform origin and basis from qlab to hkl system
	call matdotvec(ubimat,gridorigin,gridorigin)
	call matdotmat(ubimat,gridbasis,gridbasis)

	print *, "Optimal global grid dimensions: ", griddim
	print *, "Global origin (HKL): ", gridorigin
	print *, "Global basis (HKL): "
	do i=1,3
		print *, (gridbasis(i,j),j=1,3)
	end do
	print *, ""
	print *, ""
	
	gridfilename = samplename(1:nchars(samplename)) // '.glb'
	open (unit=8,file=gridfilename,access="sequential",status='unknown')
		write (8,'(1X, 3F12.6)') gridorigin
		do i=1,3
			write (8,'(1X, 3F12.6)') (gridbasis(i,j), j=1,3)
		end do
		write (8,'(1X, 3I12)') griddim
	close(8)
	
	return
	end


c *****************************************************************************
c ******* For a processed grid voxel, takes accumulated component totals and calculates the intensity (and esd)
c *****************************************************************************

	subroutine interpolate(careful,varconst1,varconst2,upperlimit,totals,intensity,esd)
	implicit none
	integer interpnum
	parameter (interpnum=22)
	
	logical careful
	double precision varconst1, varconst2, upperlimit, intensity, esd
	real totals(interpnum)

	integer i,j
	double precision tmat(4,4), v(4), err1(4), err2(4), W, tempintensity, tempesd
	logical singular, simple

	simple = .FALSE.
	W = totals(1)
	if (abs(w).lt.0.000001) then
		intensity = 0
		esd = 0
		return
	else
		do i=1,14
			totals(i) = totals(i)/W
		end do
		do i=15,interpnum
			totals(i) = totals(i)/W**2
		end do
	end if
	
	if (careful) then
		tmat(1,1) = 1.0
		tmat(2,2) = totals(6)
		tmat(3,3) = totals(7)
		tmat(4,4) = totals(8)
		tmat(1,2) = totals(3)
		tmat(2,1) = totals(3)
		tmat(1,3) = totals(4)
		tmat(3,1) = totals(4)
		tmat(1,4) = totals(5)
		tmat(4,1) = totals(5)
		tmat(2,3) = totals(9)
		tmat(3,2) = totals(9)
		tmat(2,4) = totals(11)
		tmat(4,2) = totals(11)
		tmat(3,4) = totals(10)
		tmat(4,3) = totals(10)
		
		call invmatfour(tmat,tmat,singular)

		intensity = totals(2)
		esd = sqrt(varconst1*totals(19) + varconst2*totals(15))
		if (.not.(singular)) then
			v(1) = totals(2)
			v(2) = totals(12)
			v(3) = totals(13)
			v(4) = totals(14)
			err1(1) = totals(19)
			err1(2) = totals(20)
			err1(3) = totals(21)
			err1(4) = totals(22)
			err2(1) = totals(15)
			err2(2) = totals(16)
			err2(3) = totals(17)
			err2(4) = totals(18)
			tempintensity = 0
			tempesd = 0
			do i=1,4
				tempintensity = tempintensity + tmat(1,i)*v(i)
				tempesd = tempesd + (tmat(1,i)**2)*(varconst1*err1(i) + varconst2*err2(i))
			end do
			if ((tempintensity.ge.0).and.(tempintensity.le.upperlimit).and.(tempesd.ge.0).and.(tempesd.le.upperlimit)) then
				intensity = tempintensity
				esd = sqrt(tempesd)
			end if
		end if
	else
		intensity = totals(2)
		esd = 1
	end if

	intensity = nint(min(max(intensity,0),upperlimit));
	esd = nint(min(max(esd,1),upperlimit));

c	print *, "intensity = ", intensity, ", esd = ", esd
c	if ((intensity.lt.0).or.(esd.lt.1)) print *, "SMALLERROR", "int = ", intensity, ", esd = ", esd 
c	if ((intensity.gt.upperlimit).or.(esd.gt.upperlimit)) print *, "BIGERROR", "int = ", intensity, ", esd = ", esd 

	return
	end
	
c *****************************************************************************
c ******* For an injected box, identifies and calculates incremental contributions to nearby grid voxels
c *****************************************************************************

	subroutine calcincrement(careful,griddim,box,rawintensity,mc,listnum,binlist,incrementlist)
	implicit none
	integer maxlist, interpnum
	parameter (maxlist = 10000, interpnum = 22)
	
	logical careful
	integer griddim(3), listnum, binlist(3,maxlist)
	double precision box(2,2,2,3), mc
	real rawintensity, incrementlist(interpnum,maxlist)

	integer i, j, k, m, imin, imax, jmin, jmax, kmin, kmax, myfloor, maxtest
	double precision boxcenter(3), sigma, radiusmult, tempr, tempvec(3), x, y, z, w
	
c  ********* Find the box center
	do m=1,3
		boxcenter(m) = 0
	end do
	do i=1,2
	do j=1,2
	do k=1,2
		do m=1,3
			boxcenter(m) = boxcenter(m) + box(i,j,k,m)/8.0
		end do
	end do
	end do
	end do

c  ********* Find the longest center to corner distance in grid space
	sigma = 0
	do i=1,2
	do j=1,2
	do k=1,2
		do m=1,3
			tempvec(m) = box(i,j,k,m) - boxcenter(m)
		end do
		call normvec(tempvec,tempr,tempvec)
		if (tempr.gt.sigma) sigma = tempr
	end do
	end do
	end do

c  ********* Establish the range of neighboring grid voxels
	radiusmult = 1.5

c	These bin indices are prepared for a zero-referenced array in main.c
	imin = max(0,nint(boxcenter(1) - radiusmult*sigma))
	imax = min(griddim(1)-1,nint(boxcenter(1) +radiusmult*sigma))
	jmin = max(0,nint(boxcenter(2) - radiusmult*sigma))
	jmax = min(griddim(2)-1,nint(boxcenter(2) +radiusmult*sigma))
	kmin = max(0,nint(boxcenter(3) - radiusmult*sigma))
	kmax = min(griddim(3)-1,nint(boxcenter(3) +radiusmult*sigma))
	maxtest = (kmax-kmin+1)*(jmax-jmin+1)*(imax-imin+1)
	if (maxtest.gt.maxlist) then
		print *, maxtest, " >", maxlist, " WARNING: Use coarser grid or increase the [maxlist] parameter."
		stop
	end if

c	print *, "boxcenter = ", (boxcenter(i), i=1,3)
c	print *, "sigma = ", sigma, ", listnum = ", maxtest

c  ********** Fill the incrementlist with weighted contributions
	listnum = 0
	do i=imin,imax
	do j=jmin,jmax
	do k=kmin,kmax
		listnum = listnum + 1
		binlist(1,listnum) = i
		binlist(2,listnum) = j
		binlist(3,listnum) = k
		x = i - boxcenter(1)
		y = j - boxcenter(2)
		z = k - boxcenter(3)
c		w = 1
		w = exp(-(x**2 + y**2 + z**2)/(2*sigma**2))/sqrt(2*3.1415927)/sigma
		if (careful) then
			incrementlist(1,listnum) = w
			incrementlist(2,listnum) = w*rawintensity*mc
			incrementlist(3,listnum) = w*x
			incrementlist(4,listnum) = w*y
			incrementlist(5,listnum) = w*z
			incrementlist(6,listnum) = w*(x**2)
			incrementlist(7,listnum) = w*(y**2)
			incrementlist(8,listnum) = w*(z**2)
			incrementlist(9,listnum) = w*(x*y)
			incrementlist(10,listnum) = w*(y*z)
			incrementlist(11,listnum) = w*(z*x)
			incrementlist(12,listnum) = w*x*rawintensity*mc
			incrementlist(13,listnum) = w*y*rawintensity*mc
			incrementlist(14,listnum) = w*z*rawintensity*mc
			incrementlist(15,listnum) = (w**2)*(mc**2)
			incrementlist(16,listnum) = (w**2)*(x**2)*(mc**2)
			incrementlist(17,listnum) = (w**2)*(y**2)*(mc**2)
			incrementlist(18,listnum) = (w**2)*(z**2)*(mc**2)
			incrementlist(19,listnum) = (w**2)*rawintensity*(mc**2)
			incrementlist(20,listnum) = (w**2)*(x**2)*rawintensity*(mc**2)
			incrementlist(21,listnum) = (w**2)*(y**2)*rawintensity*(mc**2)
			incrementlist(22,listnum) = (w**2)*(z**2)*rawintensity*(mc**2)
		else
			incrementlist(1,listnum) = w
			incrementlist(2,listnum) = w*rawintensity*mc
		end if

c		if((i.eq.0).and.(j.eq.0).and.(k.eq.5)) then
c			print *, "listnum, = ", listnum, ", binlist = ", (binlist(i,k), i=1,3)
c			print *, (incrementlist(i,listnum), i=1,interpnum)
c			print *, x, y, z, rawintensity, w
c		end if

	end do
	end do
	end do
	
c	print *, "imin = ", imin, ", imax = ", imax, ", jmin = ", jmin, ", jmax = ", jmax, ", kmin = ", kmin, ", kmax = ", kmax, ", listnum = ", listnum
c	do k=1,listnum
c		print *, "k = ", k, ", binlist = ", (binlist(i,k), i=1,3), ", incrementlist = ", (incrementlist(i,k), i=1,interpnum)
c	end do
	
c	print *, "rawintensity = ", rawintensity, ", mc = ", mc
	return
	end


c ******************************
c Read in a parallelepiped-shaped HKL grid definition and output a transform that converts a grid voxel index into an HKL coordinate.
c griddim contains the number of points (not increments) in each direction of the grid.
c HKLvector = gridbasis.[gridvoxel/(griddim-1)] + gridorigin        gridvoxel = (griddim-1)*[inverse(gridbasis).(HKLvector - gridorigin)]
c ******************************
	subroutine creategrid(samplename, numchunks, chunknum, ubmat, gridbasis, gridbasisinv, gridorigin, griddim, gridlabels, globalgrid)
	implicit none
	
	logical globalgrid
	double precision resolute, ubmat(3,3), gridbasis(3,3), gridbasisinv(3,3), gridorigin(3), globalrange(3,2)
	integer numchunks, chunknum, griddim(3)
	character*80 gridlabels(3), vecstring, rformit, t0, t1, t2, t3, n1, n2, n3
	character samplename*20, gridfilename*20
	double precision vec0(3),vec1(3),vec2(3),vec3(3),slowvec(3)
	integer i, j, nchars, slowdim, chunksize, chunkrem, myceiling

		if (globalgrid) then
			gridfilename = samplename(1:nchars(samplename)) // '.glb'
		else
			gridfilename = samplename(1:nchars(samplename)) // '.grd'
		end if

		open (unit=8,file=gridfilename,access="sequential",status='old')
		read (8,*) gridorigin
		do i=1,3
			read (8,*) (gridbasis(i,j), j=1,3)
		end do
		read (8,*) griddim
		close(8)

		print *, "Temporary test"
		print *, "Grid basis (HKL): Each column spans a grid edge"
		do i=1,3
			print *, (gridbasis(i,j),j=1,3)
		end do
		print *, ""

		slowdim = griddim(1)
		do i=1,3
			slowvec(i) = gridbasis(i,1)
		end do
		chunksize = myceiling(dble(slowdim)/dble(numchunks))
		chunkrem = min(chunknum*chunksize,slowdim)-(chunknum-1)*chunksize

		griddim(1) = chunkrem
		do i=1,3
			gridorigin(i) = gridorigin(i) + slowvec(i)*dble((chunknum-1)*chunksize)/dble(max(slowdim-1,1))
			gridbasis(i,1) = slowvec(i)*dble(max(chunkrem-1,1))/dble(max(slowdim-1,1))
		end do


		call invmat(gridbasis,gridbasisinv)

		do i=1,3
			vec0(i) = gridorigin(i)
			vec1(i) = gridbasis(i,1)+gridorigin(i)
			vec2(i) = gridbasis(i,2)+gridorigin(i)
			vec3(i) = gridbasis(i,3)+gridorigin(i)
		end do
		t0 = vecstring(vec0)
		t1 = vecstring(vec1)
		t2 = vecstring(vec2)
		t3 = vecstring(vec3)
		n1 = rformit(dble(griddim(1)),0)
		n2 = rformit(dble(griddim(2)),0)
		n3 = rformit(dble(griddim(3)),0)
		gridlabels(1) = 'HKL = '//t0(1:nchars(t0))//' to '//t1(1:nchars(t1))//" in nx = "//n1(1:nchars(n1))//" points (slow)"
		gridlabels(2) = 'HKL = '//t0(1:nchars(t0))//' to '//t2(1:nchars(t2))//" in ny = "//n2(1:nchars(n2))//" points (med)"
		gridlabels(3) = 'HKL = '//t0(1:nchars(t0))//' to '//t3(1:nchars(t3))//" in nz = "//n3(1:nchars(n3))//" points (fast)"
		
		print *, "Grid dimensions: ", griddim
		print *, "Grid origin (HKL): ", gridorigin
		print *, "Grid basis (HKL): Each column spans a grid edge"
		do i=1,3
			print *, (gridbasis(i,j),j=1,3)
		end do
		do i=1,3
			print *, gridlabels(i)
		end do
	return
	end

c*****************************
c Sets up a number of important transformation matrices and vectors
c*****************************
	subroutine creatematrices(gon,orienterrors,unitcell,dist,xtrans,umat,uimat,bmat,bimat,ubmat,ubimat,gmat,gimat,dmat,dimat,omat,oimat,cD,nD)
	double precision gon(4),orienterrors(4),unitcell(6),dist,xtrans(3)
	double precision umat(3,3),uimat(3,3),bmat(3,3),bimat(3,3),ubmat(3,3),ubimat(3,3),gmat(3,3),gimat(3,3),dmat(3,3),dimat(3,3),omat(3,3),oimat(3,3),cD(3),nD(3)
	double precision twotheta,omega,chi,phi,yaw,pitch,roll,gonpitch
	double precision zzz(3,3),tvec(3)
	integer i,j
	
	twotheta  = gon(1)
	omega     = gon(2)
	phi       = gon(3)
	chi       = gon(4)

	pitch     = orienterrors(1)
	roll      = orienterrors(2)
	yaw       = orienterrors(3)
	gonpitch  = orienterrors(4)

c Define the transform that rotates detector axes into lab axes (omat).
c The inverse transforms detector coords into lab coords (oimat).
c When all angles are zero, +X(det) = -y(lab), +Y(det) = +z(lab), and +Z(det) = -x(lab)
	omat(1,1) =  0
	omat(2,1) =  0
	omat(3,1) = -1
	omat(1,2) = -1
	omat(2,2) =  0
	omat(3,2) =  0
	omat(1,3) =  0
	omat(2,3) =  1
	omat(3,3) =  0
	call invmat(omat,oimat)
	
c Define the matrix (dimat) that physically orients the detector.
c It also transforms detector coords into lab coords.
c Operation order:    yaw -> pitch -> roll -> twotheta -> gonpitch
	call identitymat(dimat)
	call createrotation(3,yaw,zzz)
	call matdotmat(zzz,dimat,dimat)
	call createrotation(2,pitch,zzz)
	call matdotmat(zzz,dimat,dimat)
	call createrotation(1,roll,zzz)
	call matdotmat(zzz,dimat,dimat)
	call createrotation(3,twotheta,zzz)
	call matdotmat(zzz,dimat,dimat)
	call createrotation(2,gonpitch,zzz)
	call matdotmat(zzz,dimat,dimat)
	call invmat(dimat,dmat)
	
c Define the matrix that physically orients the goniometer head into place (gmat)
c Its inverse transforms lab coords into head coords (gimat)
	call identitymat(gmat)
	call createrotation(3,phi,zzz)
	call matdotmat(zzz,gmat,gmat)
	call createrotation(1,chi,zzz)
	call matdotmat(zzz,gmat,gmat)
	call createrotation(3,omega,zzz)
	call matdotmat(zzz,gmat,gmat)
	call createrotation(2,gonpitch,zzz)
	call matdotmat(zzz,gmat,gmat)
	call invmat(gmat,gimat)

c Compute useful matrices associate with the ub matrix (u, b, u_inverse, b_inverse, ub_inverse, qmetric).
c ubimat transforms goniometer head coords into hkl coords.
c A few other potentially useful matrices are computed as well.
	call invmat(ubmat,ubimat)
	call createb(unitcell,bmat)
	call invmat(bmat,bimat)
	call matdotmat(ubmat,bimat,umat)
	call invmat(umat,uimat)
	
c 	Determine crystal position as seen from detector center in lab frame
	cD(1) =  dist
	cD(2) =  0
	cD(3) =  0
	call createrotation(3,twotheta,zzz)
	call matdotvec(zzz,cD,cD)
	call createrotation(2,gonpitch,zzz)
	call matdotvec(zzz,cD,cD)
	call matdotvec(gmat,xtrans,tvec)
	call addvec(cD,dble(1),tvec,dble(-1),cD)
	call scalevec(cD,dble(-1),cD)
	
c 	Determine detector plane normal direction -- parallel to beam when all angles are zero.
	do i=1,3
		nD(i) = dimat(i,1)
	end do
	
c	print *, "cD: ", cD
c	print *, "nD: ", nD
c	print *, ((ubmat(i,j),i=1,3),j=1,3) 
	
	return
	end
	

c ******************************
c Prepare for efficient reachback  -- returns frameportion
c ******************************
	subroutine reachback(framesize, wavelength, pixelsize, resworstg, det0, gridbasis, gridbasisinv, gridorigin, griddim, ubmat, ubimat, gmat, gimat, dmat, dimat, omat, oimat, cD, nD, reachbackmask, skipframe)
	implicit none
	integer maxframesize
	parameter (maxframesize = 2048)
	
	integer framesize, griddim(3)
	double precision wavelength, pixelsize, resworstg, det0(2), gridbasis(3,3), gridbasisinv(3,3), gridorigin(3)
	double precision ubmat(3,3), ubimat(3,3), gmat(3,3), gimat(3,3), dmat(3,3), dimat(3,3), omat(3,3), oimat(3,3), cD(3), nD(3)
	logical skipframe, reachbackmask(maxframesize+1,maxframesize+1)

	integer i, j, jj, row, col, f, tempi
	integer numsolutions, xsolutions(14), rmin, rmax, buffer
	double precision faceshkl(6,3), pointshkl(6,3), h0(3),h1(3),h2(3),h3(3), fudge, nudge0(3),nudge1(3),nudge2(3),nudge3(3)
	double precision  q0(3), qhat(3), xhat(3), yhat(3), zhat(3), nhat(6,3), p0(3), p1(6,3), p2(6,3), ggamma(6), minapproach, alpha, beta(2)
	double precision xyz(3), nvec(3), dummy, tempn, tempd, tempg(3), hkl(3), detpix(2), detpos(3), y, a, b, c
	logical inside

	skipframe = .TRUE.
	do row=1,maxframesize+1
	do col=1,maxframesize+1
		reachbackmask(row,col) = .FALSE.
	end do
	end do

c	The nudge expands the reachback box radius by #=fudge bins to avoid misfires near the faces of the true box.
	fudge = max(2,nint(resworstg))

	do i=1,3
		nudge1(i) = 2*gridbasis(i,1)/MAX(griddim(1)-1,1)
		nudge2(i) = 2*gridbasis(i,2)/MAX(griddim(2)-1,1)
		nudge3(i) = 2*gridbasis(i,3)/MAX(griddim(3)-1,1)
		nudge0(i) = -(nudge1(i) + nudge2(i) + nudge3(i))/2
	end do
	do i=1,3
		h0(i) = gridorigin(i) + nudge0(i)*fudge
		h1(i) = gridbasis(i,1)+ nudge1(i)*fudge
		h2(i) = gridbasis(i,2)+ nudge2(i)*fudge
		h3(i) = gridbasis(i,3)+ nudge3(i)*fudge
	end do

	xhat(1) = 1
	xhat(2) = 0
	xhat(3) = 0
	yhat(1) = 0
	yhat(2) = 1
	yhat(3) = 0
	zhat(1) = 0
	zhat(2) = 0
	zhat(3) = 1

	call crossvec(h2,h1,xyz)
	do i=1,3
		faceshkl(1,i) = xyz(i)
		pointshkl(1,i) = h0(i)
	end do

	call crossvec(h1,h2,xyz)
	do i=1,3
		faceshkl(2,i) = xyz(i)
		pointshkl(2,i) = h0(i)+h3(i)
	end do

	call crossvec(h3,h2,xyz)
	do i=1,3
		faceshkl(3,i) = xyz(i)
		pointshkl(3,i) = h0(i)
	end do

	call crossvec(h2,h3,xyz)
	do i=1,3
		faceshkl(4,i) = xyz(i)
		pointshkl(4,i) = h0(i) + h1(i)
	end do

	call crossvec(h1,h3,xyz)
	do i=1,3
		faceshkl(5,i) = xyz(i)
		pointshkl(5,i) = h0(i)
	end do

	call crossvec(h3,h1,xyz)
	do i=1,3
		faceshkl(6,i) = xyz(i)
		pointshkl(6,i) = h0(i) + h2(i)
	end do

	call matdotvec(dmat,cD,xyz)
	call matdotvec(omat,xyz,p0)
	
	do f=1,6
		do i=1,3
			xyz(i) = faceshkl(f,i)	
		end do
		call matdotvec(ubmat,xyz,xyz)
		call matdotvec(gmat,xyz,xyz)
		call normvec(xyz,dummy,qhat)
		call matdotvec(dmat,qhat,xyz)
		call matdotvec(omat,xyz,nvec)
		do i=1,3
			nhat(f,i) = nvec(i)
		end do
		if (abs(abs(nhat(f,3))-1).lt.0.001) then
			do i=1,3
				p1(f,i) = xhat(i)
				p2(f,i) = yhat(i)
			end do
		else
			call crossvec(nvec,zhat,xyz)
			call crossvec(xyz,nvec,xyz)
			call normvec(xyz,dummy,xyz)
			do i=1,3
				p1(f,i) = xyz(i)
			end do
			call crossvec(nvec,xyz,xyz)
			call normvec(xyz,dummy,xyz)
			do i=1,3
				p2(f,i) = xyz(i)
			end do
		end if
	
		do i=1,3
			xyz(i) = pointshkl(f,i)	
		end do
		call matdotvec(ubmat,xyz,xyz)
		call matdotvec(gmat,xyz,q0)
		q0(1) = q0(1) + 1/wavelength

		call vecdotvec(q0,qhat,minapproach)
		if (abs(minapproach*wavelength).le.1) then
			ggamma(f) = acos(minapproach*wavelength)
		else
			ggamma(f) = -10
		end if

c		print *, "face = ", f
c		print *, "	p0:   ", (p0(i), i=1,3)
c		print *, "	minapproach*wavelength = ", minapproach*wavelength
c		print *, "	ggamma: ", ggamma(f)
c		print *, "	nhat: ", (nhat(f,i), i=1,3)
c		print *, "	p1:   ", (p1(f,i), i=1,3)
c		print *, "	p2:   ", (p2(f,i), i=1,3)

	end do
	

	do row=1,framesize+1
c		recall that the actual pixel positions assume the first pixel to be (0,0).	
		detpix(1) = 0                                         
		detpix(2) = row-1
		call getdetpos(pixelsize,detpix,det0,detpos)
		y = detpos(2)
			
		xsolutions(1) = 0+1
		xsolutions(2) = framesize+1
		do j=3,14
			xsolutions(j) = 0
		end do
		numsolutions = 2
		do f=1,6
			if (ggamma(f).ge.0) then                                            
				c = ((p0(2)-y)*nhat(f,3) - nhat(f,2)*p0(3))*cos(ggamma(f))
				a = ((p0(2)-y)*p1(f,3) - p1(f,2)*p0(3))*sin(ggamma(f))
				b = ((p0(2)-y)*p2(f,3) - p2(f,2)*p0(3))*sin(ggamma(f))
				if ((a**2+b**2-c**2.gt.0).and.(a**2+b**2.gt.0)) then
					if (abs(a).lt.0.000001) then
						beta(1) = -asin(c/b)
						beta(2) = -(beta(1) + 3.1415927)
					else
						beta(1) = atan2(b,a) + acos(-c/sqrt(a**2+b**2))
						beta(2) = atan2(b,a) - acos(-c/sqrt(a**2+b**2))
					end if
					do j=1,2
						tempn = nhat(f,1)*cos(ggamma(f)) + p1(f,1)*sin(ggamma(f))*cos(beta(j)) + p2(f,1)*sin(ggamma(f))*sin(beta(j))
						tempd = nhat(f,3)*cos(ggamma(f)) + p1(f,3)*sin(ggamma(f))*cos(beta(j)) + p2(f,3)*sin(ggamma(f))*sin(beta(j))
						alpha = -p0(3)/tempd
						detpos(1) = p0(1) - p0(3)*tempn/tempd
						call getdetpix(pixelsize,detpix,det0,detpos)
						tempi = nint(detpix(1))+1
c	print *, "row: ", row, ", face: ", f, ", cut#: ", j, ", abc: (", a,b,c, "), alpha: ", alpha, ", beta: ", beta(j), ", xsol: ", tempi
						if ((tempi.ge.1).and.(tempi.le.framesize+1).and.(alpha.gt.0)) then
c	print *, f, tempi, row
							numsolutions = numsolutions+1
							xsolutions(numsolutions) = tempi
						end if
					end do
				end if
			end if
		end do

		do jj=1,numsolutions
		do j=2,numsolutions
			if (xsolutions(j).lt.xsolutions(j-1)) then
				tempi = xsolutions(j-1)
				xsolutions(j-1) = xsolutions(j)
				xsolutions(j) = tempi
			end if
		end do
		end do
		
c	do j=1,numsolutions
c		print *, row,xsolutions(j)
c	end do
		
		rmin = -1
		rmax = -1
		do j=2,numsolutions
			detpix(1) = (xsolutions(j-1) + xsolutions(j))/2
			detpix(2) = row
			call getdetpos(pixelsize,detpix,det0,xyz)
			call pixel2qlab(wavelength,gimat,dimat,oimat,cD,nD,xyz,xyz)
			call qlab2hkl(ubimat,xyz,hkl)
			call hkl2grid(gridorigin,gridbasisinv,griddim,hkl,tempg)
			inside = .TRUE.
			do i=1,3
				inside = (inside).and.(tempg(i).ge.(-fudge)).and.(tempg(i).le.(DBLE(griddim(i)-1)+fudge))
			end do
			if (inside) then
				skipframe = .FALSE.
				if (rmin.lt.0) then
					rmin = xsolutions(j-1)
					rmax = xsolutions(j)
				else
					rmin = min(rmin,xsolutions(j-1))
					rmax = max(rmax,xsolutions(j))
				end if
			end if
c	print *, "row: ", row, ", col: ", detpix(1), ", sol: ", xsolutions(j-1), inside, xsolutions(j), ", bin: ", bin, ", limits: ", rmin, rmax, ", hkl: ", hkl 
		end do

		if (rmin.gt.0) then
c	print *, "row: ", row, ", rmin: ", rmin, ", rmax: ", rmax
			do col=rmin,rmax
c				Notice that the rows and columns are transposed here for the benefit of the C routines that use this mask.
				reachbackmask(col,row) = .TRUE.
			end do
		end if

	end do

	return
	end


c ******************************
c Calculate the best and worst Q resolutions of the dataset -- merely informative.
c ******************************
	subroutine findqresolution(datasetinfo, numimages, framesize, pixelsize, wavelength, resbestq, resworstq, distmin)
	implicit none
	integer maximages
	parameter (maximages = 1000)

	integer numimages, framesize, griddim(3)
	double precision datasetinfo(11,maximages), pixelsize, wavelength, resbestq, resworstq, distmin
	integer imagenum, i, j
	double precision gon(4), dist, ttnom, domega, ttmin, ttmax, ttprime, distmod, ttfar, tempbest, tempworst
	
	resbestq = 10000
	resworstq = 0
	distmin = 10000
	do imagenum = 1,numimages
		dist = datasetinfo(3,imagenum)
		do i=1,4
			gon(i) = (datasetinfo(3+i,imagenum) + datasetinfo(7+i,imagenum))/2
		end do

		ttnom = gon(1)
		domega = (datasetinfo(9,imagenum) - datasetinfo(5,imagenum))

		ttmin = ttnom - atan(pixelsize*framesize/2/dist)
		ttmax = ttnom + atan(pixelsize*framesize/2/dist)
		if (ttmin * ttmax .lt. 0) then
			ttprime = 0
		else
			ttprime = min(abs(ttmin), abs(ttmax))
		end if
		
c		The smallest perpendicular detector distance.
		if (dist.lt.distmin) distmin = dist

c		The best lateral pixel resolution (presumably worse than the best perpendicular resolution).
		distmod = dist/cos(abs(ttprime - ttnom))
		tempbest = pixelsize/distmod/wavelength
		if (tempbest.lt.resbestq) resbestq = tempbest
		
c		The worst resolution in the sweep direction through the Ewald sphere (presumably worse than the worst lateral pixel res).
		ttfar = max(abs(ttmin),abs(ttmax))
		tempworst = 2*sin(ttfar/2)*cos(ttfar/2)*domega/wavelength
		if (tempworst.gt.resworstq) resworstq = tempworst
	end do

	if (resbestq.gt.resworstq) resworstq = resbestq
		
	return
	end


c ******************************
c Calculate the best and worst grid resolutions of the dataset.
c ******************************
	subroutine convertresolution(ubmat, gridbasis, griddim, resconvert)
	implicit none

	integer griddim(3)
	double precision ubmat(3,3), gridbasis(3,3), resconvert
	integer imagenum, i, j
	double precision tempreal, tempmat(3,3)
	
	call matdotmat(ubmat,gridbasis,tempmat)
	do i=1,3
		do j=1,3
			tempmat(i,j) = tempmat(i,j)/dble(max(griddim(j)-1,1))
		end do
	end do
	
	resconvert = 1000000
	do i=1,3
		tempreal = 0;
		do j=1,3
			tempreal = tempreal + tempmat(i,j)**2
		end do
		resconvert = min(resconvert,sqrt(tempreal))
	end do		
	return
	end


c ******************************
c Convert nominal pixel to detector XYZ position
c Assumes that detpix = (0,0) is the first pixel
c ******************************
	subroutine getdetpos(pixelsize,detpix,det0,detpos)
	implicit none
	double precision pixelsize, detpix(2), det0(2), detpos(3)
	integer i
	
	do i=1,2
		detpos(i) = (detpix(i)+0.5 - det0(i))*pixelsize
	end do
	detpos(3) = 0

	return
	end

c ******************************
c Convert detector XYZ position to pixel coordinate
c Assumes that detpix = (0,0) is the first pixel
c ******************************
	subroutine getdetpix(pixelsize,detpix,det0,detpos)
	implicit none
	double precision pixelsize, detpix(2), det0(2), detpos(3)
	integer i
	
	do i=1,2
		detpix(i) = anint(detpos(i)/pixelsize + det0(i) - 0.5)
	end do

	if (abs(detpos(3)).gt.0.01) print *, "Warning: pixel transform problem"

	return
	end

c ******************************
c Calculate the lorentz factor of a pixel -- multiply it by observed intensity.
c ******************************
	subroutine lorentzfactor(distmin,dimat,oimat,cD,nD,invec,multcorrect)
	implicit none
	double precision distmin, dimat(3,3),oimat(3,3),cd(3),nD(3),invec(3),multcorrect
	double precision pdist, cosang,xyz(3)
	
	call copyvec(invec,xyz)

c	print *, xyz
	call matdotvec(oimat,xyz,xyz)
c	print *, xyz
	call matdotvec(dimat,xyz,xyz)
c	print *, xyz

	call addvec(xyz,dble(1),cD,dble(-1),xyz)
	call normvec(xyz,pdist,xyz)
	call vecdotvec(xyz,nD,cosang)
	
	multcorrect = 1/abs(cosang*(distmin/pdist)**2)
	
	return
	end
	
c ******************************
c Do the detector pixel to q-space transform (bad name, actually goes to gonihead coords)
c ******************************
	subroutine pixel2qlab(wavelength,gimat,dimat,oimat,cD,nD,invec,outvec)
	implicit none
	double precision wavelength,gimat(3,3),dimat(3,3),oimat(3,3),cd(3),nD(3),invec(3),outvec(3)
	double precision dummy,xyz(3)
	
	call copyvec(invec,xyz)

c	print *, xyz
	call matdotvec(oimat,xyz,xyz)
c	print *, xyz
	call matdotvec(dimat,xyz,xyz)
c	print *, xyz

	call addvec(xyz,dble(1),cD,dble(-1),xyz)
	call normvec(xyz,dummy,xyz)
	call scalevec(xyz,1/wavelength,xyz)
	xyz(1) = xyz(1) - 1/wavelength

c	print *, xyz
	call matdotvec(gimat,xyz,xyz)
c	print *,xyz
	
	call copyvec(xyz,outvec)

	return
	end
	
c ******************************
c Do the q-space to detector pixel transform (bad name, actually from gonihead coords)
c ******************************
	subroutine qlab2pixel(wavelength,gmat,dmat,omat,cD,nD,invec,outvec)
	implicit none
	double precision wavelength,gmat(3,3),dmat(3,3),omat(3,3),cD(3),nD(3),invec(3),outvec(3)
	double precision dmin,cosang,pdist,dummy,xyz(3)
	
	call copyvec(invec,xyz)

c	print *, xyz
	call matdotvec(gmat,xyz,xyz)
c	print *, xyz

	xyz(1) = xyz(1) + 1/wavelength
	call normvec(xyz,dummy,xyz)
	call vecdotvec(cD,nD,dmin)
	call vecdotvec(xyz,nD,cosang)
	pdist = -dmin/cosang
	call scalevec(xyz,pdist,xyz)
	call addvec(xyz,dble(1),cD,dble(1),xyz)
	
c	print *, xyz
	call matdotvec(dmat,xyz,xyz)
c	print *, xyz
	call matdotvec(omat,xyz,xyz)
c	print *, xyz
	
	call copyvec(xyz,outvec)

	return
	end
	
c ******************************
c Transform a qlab point into an hkl reciprocal lattice point (actually from gonihead coords)
c ******************************
	subroutine qlab2hkl(ubimat,invec,outvec)
	implicit none
	double precision ubimat(3,3),invec(3),outvec(3)
	double precision xyz(3),tmat(3,3)
	integer m
	
	call copyvec(invec,xyz)

c	print *, xyz
	call matdotvec(ubimat,xyz,xyz)
c	print *, xyz

	call copyvec(xyz,outvec)

	return
	end

c ******************************
c Transform an hkl reciprocal space into a qlab point (actually to gonihead coords).
c ******************************
	subroutine hkl2qlab(ubmat,invec,outvec)
	implicit none
	double precision ubmat(3,3),invec(3),outvec(3)
	double precision xyz(3)
	integer m
	
	call copyvec(invec,xyz)

c	print *, xyz
	call matdotvec(ubmat,xyz,xyz)
c	print *, xyz

	call copyvec(xyz,outvec)
	
	return
	end

c ******************************
c gridvoxel = (griddim-1)*[inverse(gridbasis).(HKLvector - gridorigin)]
c ******************************
	subroutine hkl2grid(gridorigin,gridbasisinv,griddim,invec,outvec)
	implicit none
	integer griddim(3)
	double precision gridorigin(3),gridbasisinv(3,3),invec(3),outvec(3)
	double precision xyz(3),tmat(3,3)
	integer i
	
	call copyvec(invec,xyz)

c	print *, xyz
	call addvec(xyz,dble(1),gridorigin,dble(-1),xyz)
c	print *, xyz
	call matdotvec(gridbasisinv,xyz,xyz)
c	print *, xyz
	do i=1,3
		xyz(i) = xyz(i)*dble(max(griddim(i)-1,1))
	end do
c	print *, xyz

	call copyvec(xyz,outvec)

	return
	end

c ******************************
c HKLvector = gridbasis.[gridvoxel/(griddim-1)] + gridorigin
c ******************************
	subroutine grid2hkl(gridorigin,gridbasis,griddim,invec,outvec)
	implicit none
	integer griddim(3)
	double precision gridorigin(3),gridbasis(3,3),invec(3),outvec(3)
	double precision xyz(3)
	integer i
	
	call copyvec(invec,xyz)

c	print *, xyz
	do i=1,3
		xyz(i) = xyz(i)/dble(max(griddim(i)-1,1))
	end do

c	print *, xyz
	call matdotvec(gridbasis,xyz,xyz)
	
c	print *, xyz
	call addvec(xyz,dble(1),gridorigin,dble(1),xyz)
c	print *, xyz
	
	call copyvec(xyz,outvec)
	
	return
	end


c ******************************
c Copy a vector 
c ******************************
	subroutine copyvec(in,out)
	implicit none
	double precision in(3),out(3)
	integer i
	
	do i=1,3
		out(i) = in(i)
	end do
	
	return
	end

c ******************************
c Initialize a vector to zero
c ******************************
	subroutine zerovec(vec)
	implicit none
	double precision vec(3)
	integer i
	
	do i=1,3
		vec(i) = 0
	end do
	
	return
	end

c ******************************
c Add two vectors together using scalar coefficients. 
c ******************************
	subroutine addvec(in1,fac1,in2,fac2,out)
	implicit none
	double precision in1(3),fac1,in2(3),fac2,out(3)
	double precision xyz(3)
	integer i
	
	do i=1,3
		xyz(i) = fac1*in1(i) + fac2*in2(i)
	end do
	call copyvec(xyz,out)
	
	return
	end

c ******************************
c Computes the dot product of two cartesian vectors
c ******************************
	subroutine vecdotvec(in1,in2,dotproduct)
	implicit none
	double precision in1(3),in2(3),dotproduct
	
	dotproduct = in1(1)*in2(1) + in1(2)*in2(2) + in1(3)*in2(3)
	
	return
	end
	
c ******************************
c Compute the cross product of two cartesian vectors
c ******************************
	subroutine crossvec(in1,in2,out)
	implicit none
	double precision in1(3),in2(3),out(3)
	double precision xyz(3)
	
	xyz(1) = in1(2)*in2(3) - in1(3)*in2(2)
	xyz(2) = in1(3)*in2(1) - in1(1)*in2(3)
	xyz(3) = in1(1)*in2(2) - in1(2)*in2(1)
	call copyvec(xyz,out)
	
	return
	end
	
c ******************************
c Multiplies a vector by a scalar factor
c ******************************
	subroutine scalevec(in,fac,out)
	implicit none
	double precision in(3),fac,out(3)
	double precision xyz(3)
	integer i
	
	do i=1,3
		xyz(i) = in(i)*fac
	end do
	call copyvec(xyz,out)
	
	return
	end
	
c ******************************
c Compute the magnitude and the unit direction of an cartesian vector
c ******************************
	subroutine normvec(vec,mag,norm)
	implicit none
	double precision vec(3),mag,norm(3)
	double precision xyz(3)
	integer i

	call vecdotvec(vec,vec,mag)
	mag = sqrt(mag)
	if (mag.gt.0.000001) then
		call scalevec(vec,1/mag,xyz)
	else
		call scalevec(vec,dble(0),xyz)
	end if
	call copyvec(xyz,norm)
	
	return
	end


c ******************************
c Copy a matrix 
c ******************************
	subroutine copymat(in,out)
	implicit none
	double precision in(3,3),out(3,3)
	integer i,j
	
	do i=1,3
		do j=1,3
			out(i,j) = in(i,j)
		end do
	end do
	
	return
	end

c ******************************
c Transpose a matrix
c ******************************
c M(i,j) -> ith row jth column
	subroutine transposemat(inmat,outmat)
	implicit none
	double precision inmat(3,3),outmat(3,3)
	double precision zzz(3,3)
	integer i,j
	
	do i=1,3
		do j=1,3
			zzz(i,j) = inmat(j,i)
		end do
	end do
	call copymat(zzz,outmat)
	
	return
	end

c ******************************
c Initialize a matrix to zero
c ******************************
	subroutine zeromat(mat)
	implicit none
	double precision mat(3,3)
	integer i,j
	
	do i=1,3
		do j=1,3
			mat(i,j) = 0
		end do
	end do
	
	return
	end

c ******************************
c Initialize a matrix to the identity
c ******************************
	subroutine identitymat(mat)
	implicit none
	double precision mat(3,3)
	integer i,j
	
	do i=1,3
		do j=1,3
			if(i.eq.j) then
				mat(i,j) = 1
			else
				mat(i,j) = 0
			end if
		end do
	end do
	
	return
	end

c ******************************
c Matrix transform of a vector
c ******************************
c M(i,j) -> ith row jth column
	subroutine matdotvec(mat,invec,outvec)
	implicit none
	double precision mat(3,3),invec(3),outvec(3)
	double precision xyz(3)
	integer i,j
	
	do i=1,3
		xyz(i) = 0
		do j=1,3
			xyz(i) = xyz(i) + mat(i,j)*invec(j)
		end do
	end do
	call copyvec(xyz,outvec)

	return
	end

c ******************************
c Multiply two matrices
c ******************************
c M(i,j) -> ith row jth column
	subroutine matdotmat(in1,in2,outmat)
	implicit none
	double precision in1(3,3),in2(3,3),outmat(3,3)
	double precision zzz(3,3)
	integer i,j,m
	
	do i=1,3
		do j=1,3
			zzz(i,j) = 0
		end do
	end do

	do i=1,3
		do j=1,3
			do m = 1,3
				zzz(i,j) = zzz(i,j) + in1(i,m)*in2(m,j)
			end do
		end do
	end do
	call copymat(zzz,outmat)
	
	return
	end

c ******************************
c Calculate rotation matrix for right-handed rotation around the positive ("n" = 1,2,3) axis. 
c ******************************
c M(i,j) -> ith row jth column
	subroutine createrotation(axis,angle,mat)
	implicit none
	double precision angle,mat(3,3),cang,sang
	integer axis,i,j
	
	do i=1,3
		do j=1,3
			if (i.eq.j) then
				mat(i,j) = 1
			else
				mat(i,j) = 0
			end if
		end do
	end do
	
	cang = Cos(angle)
	sang = Sin(angle)

	if (axis.eq.1) then
		mat(2,2) = cang
		mat(2,3) = -sang
		mat(3,2) = sang
		mat(3,3) = cang
	else if (axis.eq.2) then
		mat(1,1) = cang
		mat(1,3) = sang
		mat(3,1) = -sang
		mat(3,3) = cang
	else if (axis.eq.3) then
		mat(1,1) = cang
		mat(1,2) = -sang
		mat(2,1) = sang
		mat(2,2) = cang
	end if
	
	return
	end


c ******************************
c Calculate the magnitude of a vector in non-cartesian space defined transform matrix B. 
c ******************************
c M(i,j) -> ith row jth column
	subroutine bmag(bmat,vec,mag)
	implicit none
	double precision bmat(3,3),vec(3),temp(3),mag
	
	call matdotvec(bmat,vec,temp)
	call normvec(temp,mag,temp)
	
	return
	end


c ******************************
c Create a B matrix containing the column basis vectors of the direct unit cell.
c ******************************
c M(i,j) -> ith row jth column
	subroutine createb(unitcell,bstarmat)
	implicit none
	double precision bstarmat(3,3),unitcell(6)
	double precision zzz(3,3),tmat(3,3),alat,blat,clat,alpha,beta,gamma
	
	alat = unitcell(1)
	blat = unitcell(2)
	clat = unitcell(3)
	alpha = unitcell(4)
	beta  = unitcell(5)
	gamma = unitcell(6)
	

	zzz(1,1) = alat
	zzz(2,1) = 0
	zzz(3,1) = 0

	zzz(1,2) = blat*Cos(gamma)
	zzz(2,2) = blat*Sin(gamma)
	zzz(3,2) = 0
	
	zzz(1,3) = clat*Cos(beta)
	zzz(2,3) = clat*(Cos(alpha)-Cos(beta)*Cos(gamma))/Sin(gamma)
	zzz(3,3) = Sqrt(clat**2 - zzz(1,3)**2 - zzz(2,3)**2)

	call invmat(zzz,tmat)
	call transposemat(tmat,bstarmat)

	return
	end
	
	
c***********************************
c Computes the inverse of a matrix
c***********************************
	subroutine invmat(a,ainv)
	implicit none
	integer dim
	parameter (dim=3)
	integer i,j,k,pivot
	double precision a(dim,dim),ainv(dim,dim),b(dim,dim),zzz(dim,dim)
	double precision pivotmag,factor,temp,tolerance
	integer m,n

	tolerance = 1.0E-15
	do j=1,dim
	do k=1,dim
	  b(j,k) = a(j,k)
	  zzz(j,k) = 0.
	  if (j.eq.k) zzz(j,k) = 1.
	end do
	end do

	do 100 i=1,dim
	
	pivot = 0
	do j=i,dim
	  if (abs(b(j,i)).ge.tolerance) then
	      pivotmag = b(j,i)
	      pivot = j
	  end if
	end do
	if (pivot.eq.0) then
	  write(*,*) 'Matrix is singular'
	  return
	end if

	do k=1,dim
	 temp = b(i,k)
	 b(i,k) = b(pivot,k)
	 b(pivot,k) = temp
	 temp = zzz(i,k)
	 zzz(i,k) = zzz(pivot,k)
	 zzz(pivot,k) = temp
	end do

	do k=1,dim
	    b(i,k) = b(i,k)/pivotmag
	    zzz(i,k) = zzz(i,k)/pivotmag
	end do

	do j=1,dim
	  if (j.ne.i) then
	    factor = -b(j,i)
	    do k=1,dim
	      b(j,k) = b(j,k) + factor*b(i,k)
	      zzz(j,k) = zzz(j,k) + factor*zzz(i,k)
	    end do
	  end if
	end do
	
c	write (*,'(A5,9F16.6)') 'amat ',((a(m,n),m=1,dim),n=1,dim)
c	write (*,'(A5,9F16.6)') 'bmat ',((b(m,n),m=1,dim),n=1,dim)
c	write (*,'(A5,9F16.6)') 'ainv ',((zzz(m,n),m=1,dim),n=1,dim)
100	continue

	do j=1,dim
	do k=1,dim
	  ainv(j,k) = zzz(j,k)
	end do
	end do
	
	return
	end

c***********************************
c Computes the inverse of a matrix
c***********************************
	subroutine invmatfour(a,ainv,sing)
	implicit none
	integer dim
	parameter (dim=4)
	integer i,j,k,pivot
	double precision a(dim,dim),ainv(dim,dim),b(dim,dim),zzz(dim,dim)
	double precision pivotmag,factor,temp,tolerance
	integer m,n
	logical sing

	sing = .FALSE.
	tolerance = 1.0E-5
	do j=1,dim
	do k=1,dim
	  b(j,k) = a(j,k)
	  zzz(j,k) = 0.
	  if (j.eq.k) zzz(j,k) = 1.
	end do
	end do

	do 100 i=1,dim
	
	pivot = 0
	do j=i,dim
	  if (abs(b(j,i)).ge.tolerance) then
	      pivotmag = b(j,i)
	      pivot = j
c		print *, "i = ", i, ", j = ", j, ", pivotmag = ", pivotmag, ", pivot = ", pivot
	  end if
	end do
	if (pivot.eq.0) then
	  sing = .TRUE.
	  return
	end if

	do k=1,dim
	 temp = b(i,k)
	 b(i,k) = b(pivot,k)
	 b(pivot,k) = temp
	 temp = zzz(i,k)
	 zzz(i,k) = zzz(pivot,k)
	 zzz(pivot,k) = temp
	end do

	do k=1,dim
	    b(i,k) = b(i,k)/pivotmag
	    zzz(i,k) = zzz(i,k)/pivotmag
	end do

	do j=1,dim
	  if (j.ne.i) then
	    factor = -b(j,i)
	    do k=1,dim
	      b(j,k) = b(j,k) + factor*b(i,k)
	      zzz(j,k) = zzz(j,k) + factor*zzz(i,k)
	    end do
	  end if
	end do
	
c	write (*,'(A5,16F16.6)') 'amat ',((a(m,n),m=1,dim),n=1,dim)
c	write (*,'(A5,16F16.6)') 'bmat ',((b(m,n),m=1,dim),n=1,dim)
c	write (*,'(A5,16F16.6)') 'ainv ',((zzz(m,n),m=1,dim),n=1,dim)
c	write (*,*) ''
100	continue

	do j=1,dim
	do k=1,dim
	  ainv(j,k) = zzz(j,k)
	end do
	end do
	
	return
	end

c***********************************
c Computes the determinant of a matrix
c***********************************
	subroutine determinant(a,det)
	implicit none
	integer dim
	parameter (dim=3)
	double precision a(dim,dim),det

	det = a(1,1)*a(2,2)*a(3,3) + a(1,3)*a(2,1)*a(3,2) + a(1,2)*a(2,3)*a(3,1) - a(1,3)*a(2,2)*a(3,1) - a(1,1)*a(2,3)*a(3,2) - a(1,2)*a(2,1)*a(3,3)
		
	return
	end

c*******************************
c  Function that calculates the largest integer smaller than x
c*******************************
	function myfloor(x)
	implicit none
	double precision x,tolerance
	integer y, myfloor
	
	tolerance = 1E-15
	y = INT(x)
	if ((x.lt.0).and.(ABS(y-x).gt.tolerance)) y = y - 1
	myfloor = y
	
	return
	end

c*******************************
c  Function that calculates the smalles integer larger than x
c*******************************
	function myceiling(x)
	implicit none
	double precision x, tolerance
	integer y, myceiling
	
	tolerance = 1E-15
	y = INT(x)
	if ((x.gt.0).and.(ABS(y-x).gt.tolerance)) y = y + 1
	myceiling = y
	
	return
	end
	
c***********************************
c  Function that calculates the number of non-blank characters in a string preceding the first blank character
c***********************************
	function nchars(teststr)
	implicit none
	character teststr*80
	integer i,nchars,count
		
	count = 0
	do i=1,80
	  if (teststr(i:i).eq.'') then
	    count = i-1
	    goto 100
	  end if
	end do
	
100	nchars = count
	
	end

c******************************
c  Function that converts an double-precision number into a string
c******************************
	function rformit(num,decdigits)
	implicit none
	integer decdigits
	double precision num
	character*80 rformit,str
	character ch
	integer i,dig,temp,ndigits,nchars
	
	temp = ABS(NINT(num*(10.0**decdigits)))
	if (temp.eq.0) then
		rformit = '0'
	else
		ndigits = INT(Log10(dble(temp)))+1
c		print *, "NDIGITS = ",ndigits, ",  DECDIGITS = ",decdigits
		str = ''
		do i=ndigits,1,-1
		  dig = INT(dble(temp)/10.0**(i-1))
		  ch = CHAR(dig+48)
		  temp = temp - dig*10**(i-1)
		  str(ndigits-i+1:ndigits-i+1) = ch
		end do

		if (decdigits > 0) then
			if (decdigits >= ndigits) then
				temp = decdigits-ndigits
				rformit = str(1:ndigits)
				do i=1,temp
					rformit = '0'//rformit
				end do
				rformit = '0.'//rformit
			else
				temp = ndigits-decdigits
				rformit = str(1:temp)//'.'//str(temp+1:ndigits)
			end if
			do i=1,decdigits+1
				temp = nchars(rformit)
				if (rformit(temp:temp) == '0' .or. rformit(temp:temp) == '.') then
					rformit = rformit(1:temp-1)
				end if
			end do
		elseif (decdigits < 0) then
			temp = -decdigits
			rformit = str(1:ndigits)
			do i=1,temp
				rformit = rformit(1:nchars(rformit))//'0'
			end do
		else
			rformit = str(1:ndigits)
		end if

		if (num.lt.0) then
			rformit = '-'//rformit		
		end if
	end if
	
	end

c******************************
c  Function that prepares an axis label from two vectors
c******************************
	function vecstring(vec)
	implicit none
	double precision vec(3)
	character*80 vecstring,rformit,t1,t2,t3
	integer nchars
	
	t1 = rformit(vec(1),2)
	t2 = rformit(vec(2),2)
	t3 = rformit(vec(3),2)
	
	vecstring = '('//t1(1:nchars(t1))//','//t2(1:nchars(t2))//','//t3(1:nchars(t3))//')'

	end
















c ******************************
c The following routines are no longer used
c ******************************


c ******************************
c Transform an hkl point into a grid voxel space by subtracting grid2qshift and then applying the inverse(grid2qmat) transform.
c ******************************
	subroutine qlab2grid(grid2qshift,grid2qinvmat,invec,outvec)
	implicit none
	double precision grid2qshift(3),grid2qinvmat(3,3),invec(3),outvec(3)
	double precision xyz(3),tmat(3,3)
	integer m
	
	call copyvec(invec,xyz)

c	print *, xyz
	call addvec(xyz,dble(1),grid2qshift,dble(-1),xyz)
c	print *, xyz
	call matdotvec(grid2qinvmat,xyz,xyz)
c	print *, xyz

	call copyvec(xyz,outvec)

	return
	end

c ******************************
c Transform an grid voxel to hkl space by applying grid2qmat and then adding grid2qshift.
c ******************************
	subroutine grid2qlab(grid2qshift,grid2qmat,invec,outvec)
	implicit none
	double precision grid2qshift(3),grid2qmat(3,3),invec(3),outvec(3)
	double precision xyz(3)
	integer m
	
	call copyvec(invec,xyz)

c	print *, xyz
	call matdotvec(grid2qmat,xyz,xyz)
	
c	print *, xyz
	call addvec(xyz,dble(1),grid2qshift,dble(1),xyz)
c	print *, xyz
	
	call copyvec(xyz,outvec)
	
	return
	end
