/* Process stars.dat data
 * This program reads the stars.dat file and draws a map of the sky.
 * The output is a PostScript file called sky.ps
 * input parameters:
 * d - focal distance, 0 < d < 100.
 * (ra, dec) - this pair of numbers specify the direction to look at,
 * so that is particular location is at the center of plot.
 * Wang Jian-Sheng 9 Sep 2002 
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

/* First we define some utility functions, no all are used.
 */
/* Convert from celestial coordinates, Right Ascension in 
 * hour, min, sec, and DEClination with sign in degree, min, and sec,
 * to two spherical polar coordinates in radian.
 * Note that 0 <= theta < M_PI, 0 <= phi < 2*M_PI.
 * Return values are pointers.
 */
void celestial_to_spherical(double RAhr, double RAmin, double RAsec, int sign,
                            double DECdeg, double DECmin, double DECsec, 
                            double *theta, double *phi)
{
   *theta = (M_PI/180.0)*(90.0 - sign*(DECdeg + DECmin/60.0 + DECsec/3600.0));
   *phi = 2.0*M_PI*(RAhr + RAmin/60.0 + RAsec/3600.0)/24.0;
}

/* this is the inverse of the above function 
 */
void spherical_to_celestial(double theta, double phi,
		         double *RAhr, double *RAmin, double *RAsec, int *sign,
                         double *DECdeg, double *DECmin, double *DECsec) 
{
   double ra, dec; 

   ra = 24.0*phi/(2.0*M_PI);
   *RAhr = (int) ra;
   ra = (ra- (*RAhr))*60.0;
   *RAmin  = (int) ra;
   ra = (ra-(*RAmin))*60.0;
   *RAsec = ra;
   dec = 90.0 - (theta*180/M_PI);
   if(dec < 0) {
      *sign = -1;
      dec = -dec;
   } else {
      *sign = +1;
   }
   *DECdeg = (int) dec;
   dec = (dec-(*DECdeg))*60.0;
   *DECmin = (int) dec;
   dec = (dec-(*DECmin))*60.0;
   *DECsec = dec;
}

/*  transform from spherical polar coordinates to cartesian coordinates.
 */
spherical_to_cartesian(double r, double theta, double phi, 
		       double *x, double *y, double *z)
{
   *x = r*sin(theta)*cos(phi);
   *y = r*sin(theta)*sin(phi);
   *z = r*cos(theta);
}

/* The rotate() function take a point (x,y,z) and find its new coordinates
 * (x1,y1,z1).  The rotation is specified by two angles theta and phi, such
 * that z1 axis is in the direction specified by the polar angles
 * theta and phi.  * This is not the most general rotation.  
 * The rotation is defined as follows: 
 * [x1, y1, z1]^T = R_y(theta) R_z(phi) [x,y,z]^T
 * i.e., first rotate about z-axis by angle phi
 *            [ cos(phi)   sin(phi)  0 ]
 * R_z(phi) = [-sin(phi)   cos(phi)  0 ]
 *            [  0          0        1 ]
 * then rotate by angle theta about (new) y
 *              [ cos(theta)   0    -sin(theta) ]
 * R_y(theta) = [    0         1        0       ]
 *              [ sin(theta)   0     cos(theta) ]
 */
void rotate(double theta, double phi, 
            double x, double y, double z,
            double *x1, double *y1, double *z1)
{
   double xp, yp, zp;

   xp =  x*cos(phi) + y*sin(phi);
   yp = -x*sin(phi) + y*cos(phi);
   zp = z;
   *x1 =  xp*cos(theta) - zp*sin(theta);
   *y1 = yp;
   *z1 = xp*sin(theta) + zp*cos(theta);
}

/* The image() function maps a three-dimensional point (x,y,z)
 * into a point in surface at z = 0.  The (x1,y1) is obtained from
 * the projection  with focal point at (0,0,-d), i.e., the (x1,y1)
 * is a point such that the straight line from (x,y,z)
 * to the focal point (0,0,-d) intersects the z=0 plane.
 * The reason for flip x and y and minus sign is to make an image
 * that is consistent with the convention that up is north,
 * right is west for the sky. 
 */
image(double d, double x, double y, double z, double *x1, double *y1)
{
   double lambda;

   lambda = d/(z+d);
   *y1 = -x*lambda;
   *x1 = -y*lambda;
}

int main()
{
   int sign, i, no;
   double RAhr, RAmin, RAsec, DECdeg, DECmin, DECsec, Vmag;
   double ra, r, theta, phi, x, y, z, xi, yi;
   double d, theta0, phi0, x1, y1, z1;
   double theta1, phi1, xold, yold;
   char s3[10], str[100];
   FILE *fp, *fpout;

   printf("enter focal length d\n");
   scanf("%lf", &d);
   if(d<= 0.0) {
      d = 1.0;
   }
   printf("enter viewing direction in RA hour and DEC degree\n");
   scanf("%lf%lf", &RAhr, &DECdeg);
   if(DECdeg < 0) {
      sign = -1;
      DECdeg = - DECdeg;
   } else {
      sign = +1;
   }
   celestial_to_spherical(RAhr, 0.0, 0.0, sign, DECdeg, 0.0, 0.0,
  	                  &theta0, &phi0);

   fp = fopen("stars.dat", "r");
   assert(fp != NULL);
   fpout = fopen("sky.ps", "w");
   assert(fpout != NULL);
   fprintf(fpout, "%%!PS\n");
   fprintf(fpout, "/star {\n");
   fprintf(fpout, "/r exch def\n");
   fprintf(fpout, "/y exch def\n");
   fprintf(fpout, "/x exch def\n");
   fprintf(fpout, "x y r 0 360 arc fill\n");
   fprintf(fpout, "} def\n");
   fprintf(fpout, "295 400 translate\n");
   fprintf(fpout, "280 280 scale\n");
   fprintf(fpout, "0 0 0 setrgbcolor\n");
   fprintf(fpout, "0 0 1 0 360 arc fill\n");
   fprintf(fpout, "0 0 1 setrgbcolor\n");
   fprintf(fpout, "0.001 setlinewidth\n");

   /* draw hour and declination lines */
   for(ra = 0.0; ra < 23; ra += 2.0) {
      xold = -2.0;  /* impossible value */
      yold = -2.0;
      phi = ra*2.0*M_PI/24.0;
      for(theta = 0.0; theta < M_PI; theta += M_PI/200) {
         spherical_to_cartesian(1.0, theta, phi, &x, &y, &z);
         rotate(theta0, phi0, x, y, z, &x1, &y1, &z1);
         if(z1 > 0) {
            image(d, x1, y1, z1, &xi, &yi);
	    if(fabs(xi-xold) + fabs(yi-yold) > 0.1) {
               fprintf(fpout, "%g   %g   moveto\n", xi, yi);
	    } else {
               fprintf(fpout, "%g   %g   lineto\n", xi, yi);
	    }
	    xold = xi;
	    yold = yi;
         }
      }
      fprintf(fpout, "stroke\n");
   }
   for(theta = 0.0; theta < M_PI-0.01; theta += M_PI/6) {
      xold = -2.0;  
      yold = -2.0;
      for(phi = 0.0; phi < 2.0*M_PI; phi += 2.0*M_PI/200) {
         spherical_to_cartesian(1.0, theta, phi, &x, &y, &z);
         rotate(theta0, phi0, x, y, z, &x1, &y1, &z1);
         if(z1 > 0) {
            image(d, x1, y1, z1, &xi, &yi);
	    if(fabs(xi-xold) + fabs(yi-yold) > 0.1) {
               fprintf(fpout, "%g   %g   moveto\n", xi, yi);
	    } else {
               fprintf(fpout, "%g   %g   lineto\n", xi, yi);
	    }
	    xold = xi;
	    yold = yi;
         }
      }
      fprintf(fpout, "stroke\n");
   }
   fprintf(fpout, "1 1 1 setrgbcolor\n");

   i = 0;
   while(fgets(str, 99, fp) != NULL) {
      if(str[0] == '#') { continue; }
      if( sscanf(str, "%d%lf%lf%lf%s%lf%lf%lf%lf",
		   &no, &RAhr, &RAmin, &RAsec,
		   s3, &DECdeg, &DECmin, &DECsec, &Vmag) <= 0 ) {
         break;
      }
      if (s3[0] == '+') { 
         sign = 1;
      } else if (s3[0] == '-') {
         sign = -1;
      } else {       /* ignore nova data */
         continue;     
      }

      celestial_to_spherical(RAhr, RAmin, RAsec, sign, DECdeg, DECmin, DECsec,
		             &theta, &phi);
      spherical_to_cartesian(1.0, theta, phi, &x, &y, &z);
      rotate(theta0, phi0, x, y, z, &x1, &y1, &z1);
      if(z1 > 0) {
         image(d, x1, y1, z1, &xi, &yi);
	 r = 0.0004*(6.5-Vmag)*(6.5-Vmag);
         fprintf(fpout, "%g   %g   %g   star\n", xi, yi, r);
      }

      ++i;
   }
   fprintf(fpout, "showpage\n");
   fclose(fp);
   fclose(fpout);
    
   return 0;
}
