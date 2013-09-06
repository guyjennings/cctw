c ******************************
c Created for Aug 2013 for Guy Jennings
c g77 -c transform*.f; g77 transform*.o -o transformtest
c ******************************
	program transformtest
	implicit none
	
	character configfilename*20, dummy*120
	integer i, j, frame, row, col, framesize, griddim(3)
	double precision pixelsize, wavelength, dist, unitcell(6), det0(2), xtrans(3), gon(4), orienterrors(4)
	double precision omegacorrection, omegastep, omeganom, omegaangle
	double precision twothetacorrection, twothetanom, twothetaangle
	double precision chicorrection, chinom, chiangle, phicorrection, phinom, phiangle
	double precision gridorigin(3), gridbasis(3,3), gridbasisinv(3,3)
	double precision umat(3,3),uimat(3,3),bmat(3,3),bimat(3,3),ubmat(3,3),ubimat(3,3)
	double precision gmat(3,3),gimat(3,3),dmat(3,3),dimat(3,3),omat(3,3),oimat(3,3),cD(3),nD(3)
	double precision xyz1(3), xyz2(3), xyz3(3), xyz4(3), xyz5(3), xyz6(3), xyz7(3), detpixinitial(2), detpixfinal(2)

	print *, "Enter config filename > "
	read *, configfilename

	open (unit=8,file=configfilename,access="sequential",status='old')
		
		read (8,*) dummy
		read (8,*) frame
	
		read (8,*) dummy
		read (8,*) dummy
		read (8,*) col
	
		read (8,*) dummy
		read (8,*) dummy
		read (8,*) row
		
		read (8,*) dummy
		read (8,*) dummy
		read (8,*) framesize
	
		read (8,*) dummy
		read (8,*) dummy
		read (8,*) pixelsize
	
		read (8,*) dummy
		read (8,*) dummy
		read (8,*) wavelength
	
		read (8,*) dummy
		read (8,*) dummy
		read (8,*) dist
	
		read (8,*) dummy
		read (8,*) dummy
		read (8,*) (unitcell(i), i=1,6)
	
		read (8,*) dummy
		read (8,*) dummy
		do i=1,3
			read (8,*) (ubmat(i,j), j=1,3)
		end do

		read (8,*) dummy
		read (8,*) dummy
		read (8,*) (det0(i), i=1,2)
	
		read (8,*) dummy
		read (8,*) dummy
		read (8,*) (xtrans(i), i=1,3)
	
		read (8,*) dummy
		read (8,*) dummy
		read (8,*) (orienterrors(i), i=1,4)
		
		read (8,*) dummy
		read (8,*) dummy
		read (8,*) twothetacorrection
	
		read (8,*) dummy
		read (8,*) dummy
		read (8,*) twothetanom
	
		read (8,*) dummy
		read (8,*) dummy
		read (8,*) twothetaangle
	
		read (8,*) dummy
		read (8,*) dummy
		read (8,*) omegacorrection
	
		read (8,*) dummy
		read (8,*) dummy
		read (8,*) omegastep
	
		read (8,*) dummy
		read (8,*) dummy
		read (8,*) omeganom

		read (8,*) dummy
		read (8,*) dummy
		read (8,*) omegaangle
	
		read (8,*) dummy
		read (8,*) dummy
		read (8,*) chicorrection
	
		read (8,*) dummy
		read (8,*) dummy
		read (8,*) chinom
	
		read (8,*) dummy
		read (8,*) dummy
		read (8,*) chiangle
	
		read (8,*) dummy
		read (8,*) dummy
		read (8,*) phicorrection
	
		read (8,*) dummy
		read (8,*) dummy
		read (8,*) phinom
	
		read (8,*) dummy
		read (8,*) dummy
		read (8,*) phiangle
	
		read (8,*) dummy
		read (8,*) dummy
		read (8,*) (gridorigin(i), i=1,3)
	
		read (8,*) dummy
		read (8,*) dummy
		do i=1,3
			read (8,*) (gridbasis(i,j), j=1,3)
		end do
		call invmat(gridbasis,gridbasisinv)
	
		read (8,*) dummy
		read (8,*) dummy
		read (8,*) (griddim(i), i=1,3)

	close(8)
	
	gon(1) = twothetaangle                                                               
	gon(2) = omegaangle
	gon(3) = phiangle
	gon(4) = chiangle

	call creatematrices(gon,orienterrors,unitcell,dist,xtrans,umat,uimat,bmat,bimat,ubmat,ubimat,gmat,gimat,dmat,dimat,omat,oimat,cD,nD)
	
	print *, "Frame number: ", frame
	print *, ""

	detpixinitial(1) = col
	detpixinitial(2) = row
		print *, "Detector pixel (column, row): "
		print *, (detpixinitial(j),j=1,2)
		print *, ""
	
	call getdetpos(pixelsize,detpixinitial,det0,xyz1)
		print *, "Detector position (detX,detY,0) relative to zeroed beam spot: "
		print *, (xyz1(j),j=1,3)
		print *, ""

	call pixel2qlab(wavelength,gimat,dimat,oimat,cD,nD,xyz1,xyz2)
		print *, "(qx,qy,qz) in coords of goniometer head: "
		print *, (xyz2(j),j=1,3)
		print *, ""

	call qlab2hkl(ubimat,xyz2,xyz3)
		print *, "reciprocal-lattice vector (h,k,l): "
		print *, (xyz3(j),j=1,3)
		print *, ""

	call hkl2grid(gridorigin,gridbasisinv,griddim,xyz3,xyz4)
		print *, "grid indices (ig,jg,kg): "
		print *, (xyz4(j),j=1,3)
		print *, ""
	
	call grid2hkl(gridorigin,gridbasis,griddim,xyz4,xyz5)
		print *, "reciprocal-lattice vector (h,k,l): "
		print *, (xyz5(j),j=1,3)
		print *, ""

	call hkl2qlab(ubmat,xyz5,xyz6)
		print *, "(qx,qy,qz) in coords of goniometer head: "
		print *, (xyz6(j),j=1,3)
		print *, ""

	call qlab2pixel(wavelength,gmat,dmat,omat,cD,nD,xyz6,xyz7)
		print *, "Detector position (detX,detY,0) relative to zeroed beam spot: "
		print *, (xyz7(j),j=1,3)
		print *, ""

	call getdetpix(pixelsize,detpixfinal,det0,xyz7)
		print *, "Detector pixel (column, row): "
		print *, (detpixfinal(j),j=1,2)
		print *, ""

        print *,"ubimat"
        do i=1,3
                print *,(ubimat(i,j), j=1,3)
        end do

        print *,"umat"
        do i=1,3
                print *,(umat(i,j), j=1,3)
        end do

        print *,"uimat"
        do i=1,3
                print *,(uimat(i,j), j=1,3)
        end do

        print *,"bmat"
        do i=1,3
                print *,(bmat(i,j), j=1,3)
        end do

        print *,"bimat"
        do i=1,3
                print *,(bimat(i,j), j=1,3)
        end do

	stop
	end

