    **************************************************************
                                HOMEST
                              version 1.4
                          By Manolis Lourakis

                     Institute of Computer Science
            Foundation for Research and Technology - Hellas
                       Heraklion, Crete, Greece
    **************************************************************


GENERAL
This is homest, a copylefted C library for non-linear, robust homography
estimation from matched image point features. homest requires my levmar
Levenberg-Marquardt non-linear least squares library and LAPACK/BLAS, available
from http://www.ics.forth.gr/~lourakis/levmar and http://www.netlib.org/clapack,
respectively. Also, it implements robust regression techniques for coping with
outliers. Note that homest does *not* include any means for detecting and
matching point features between images. Such functionality can be supplied by
other software such as D. Lowe's SIFT or (in some cases) S. Birchfield's KLT.

Briefly, the approach implemented by homest is the following:

1) Normalization of point coordinates to improve conditioning as
   described in
   R.I. Hartley "In Defense of the Eight-Point Algorithm",
   IEEE Trans. on PAMI, Vol. 19, No. 6, pp. 580-593, June 1997.

2) Least Median of Squares (LMedS) linear fitting to detect outliers,
   see P.J. Rousseeuw, "Least Median of Squares Regression", Journal
   of the American Statistics Association, Vol. 79, No. 388, pp. 871-880,
   Dec. 1984. To ensure adequate spatial distribution of point quadruples
   over the image, LMedS random sampling employs the bucketing technique
   proposed in
   Z. Zhang, R. Deriche, O. Faugeras, Q.T. Luong, 
   "A Robust Technique for Matching Two Uncalibrated Images Through the
   Recovery of the Unknown Epipolar Geometry", INRIA RR-2273, May 1994 

3) Non-linear refinement of the linear homography estimate by minimizing either:
   * the homographic transfer error in the second image
   * the symmetric homographic transfer error between both images
   * the Sampson error (see P.D. Sampson, "Fitting Conic Sections to ``very
     scattered'' Data: An Iterative Refinement of the Bookstein Algorithm",
     CGIP, Vol. 18, Is. 1, pp. 97-108, Jan. 1982.)
   * the reprojection error

   The minimization is performed using the Levenberg-Marquardt algorithm, see
   M.I.A. Lourakis, "levmar: Levenberg-Marquardt Nonlinear Least Squares
   Algorithms in C/C++", http://www.ics.forth.gr/~lourakis/levmar/

Apart from fully projective homographies, homest can also estimate
affine homographies. Affine homographies have their third row fixed to
[0, 0, 1] and arise when the image displacements come from a small
image region or a large focal length lens is being used. When
estimating affine homographies, the third step of the approach
described above is omitted: Non-linear refinement is unnecessary
for them because in that case, the algebraic error equals the
geometric one.

For more details on homographies, projective geometry and such,
refer to R. Hartley and A. Zisserman, "Multiple View Geometry in
Computer Vision", Cambridge University Press, 2000-3


COMPILATION
 - Non-linear minimization in homest requires the levmar library. Thus,
   before building it, you should make sure that levmar and LAPACK/BLAS
   are installed on your system. Detailed installation instructions can be
   found in levmar's distribution at http://www.ics.forth.gr/~lourakis/levmar

 - The preferred way to build homest is through the CMake cross-platform
   build system. The included CMakeLists.txt file can be used to generate
   makefiles for Unix systems or project files for Windows systems.
   CMakeLists.txt defines some configuration variables that control
   certain aspects of homest and can be modified from CMake's user
   interface. The values of these variables are automatically propagated
   to homest.h after CMake runs.  More information on how to use CMake
   can be found at http://www.cmake.org

 - homest can also be built using the supplied makefiles. Platform-specific
   instructions are given below. Before compiling, you should edit the
   appropriate makefile to specify the location of your compiled levmar and
   LAPACK/BLAS libraries. You also might consider changing a few configuration
   options found at the top of homest.h. See the accompanying comments for
   more details.

   -- On a Linux/Unix system, typing "make" will build both levmar and the
      demo program using gcc. Alternatively, if Intel's C++ compiler is
      installed, it can be used by typing "make -f Makefile.icc".

   -- Under Windows and if Visual C is installed & configured for command line
      use, type "nmake /f Makefile.vc" in a cmd window to build levmar and the
      demo program. In case of trouble, read the comments on top of Makefile.vc

MATLAB INTERFACE
homest's distribution includes a matlab mex interface.
See the 'matlab' subdirectory for more information and examples of use.

USE
homest() is the main routine for estimating a (projective) homography. See the
comments in homest.c for an explanation of its arguments; homest_demo.c illustrates
an example of using homest() with matched point pairs that are read from a text
file. An example of such point pairs is included in subdirectory test.
An affine homography can be estimated with the routine homestaff(), which is
employed when homest_demo is invoked with the -a option.

Typing 
./homest_demo  test/matches.txt 

should produce output similar to

Homography estimation using 933 image matches

Estimated homography [147 outliers, 15.76%]

-0.4737507 -0.1677093 20.95481 
0.09925273 -0.5048342 -82.8415 
-0.0001052896 1.037155e-05 -0.5395166 

Elapsed time: 0.07 seconds, 70.00 msecs

Homography RMS and RMedS errors for input points: 33.3516 0.467221
        25%, 50% and 75% quartiles: 0.273253 0.467221 0.911758

--------------------------------------------------------------------------

The affine case:
./homest_demo -a test/matches.txt

Affine homography estimation using 933 image matches

Estimated affine homography [133 outliers, 14.26%]

0.7534839 0.3028768 -16.3913 
-0.240202 0.8849388 167.6426 
0 0 1 

Elapsed time: 0.01 seconds, 10.00 msecs

Homography RMS and RMedS errors for input points: 33.2287 4.11127
        25%, 50% and 75% quartiles: 2.59884 4.11127 7.67183


CONTACT
Send your comments/bug reports to lourakis **at** ics.forth.gr
