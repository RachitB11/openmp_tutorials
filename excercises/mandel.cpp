/*
**  PROGRAM: Mandelbrot area
**
**  PURPOSE: Program to compute the area of a  Mandelbrot set.
**           Correct answer should be around 1.510659.
**           WARNING: this program may contain errors
**
**  USAGE:   Program runs without input ... just run the executable
**
**  HISTORY: Written:  (Mark Bull, August 2011).
**           Changed "comples" to "d_comples" to avoid collsion with
**           math.h complex type (Tim Mattson, September 2011)
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

# define NPOINTS 1000
# define MAXITER 1000

void testpoint(struct d_complex& c, int& numoutside);

struct d_complex{
   double r;
   double i;
};


int main(){
   int i, j;
   double area, error, eps  = 1.0e-5;

   struct d_complex c;
   int numoutside = 0;

//   Loop over grid of points in the complex plane which contains the Mandelbrot set,
//   testing each point to see whether it is inside or outside the set.

// Lets walk through this
// - The parallel for only means i will be made private but not j
// - Lets set the default to none so that no assumptions were made about shared/private
// - Add a reduction because you want a copy of numoutside for each thread that
// you want to aggregate at the end and store in global variable. You could have
// also set numoutside as atomic
// - i is private because of the for construct but j is not!! Privatize it.
// - firstprivate eps becuase we want to use the global value of eps
#pragma omp parallel for default(none) reduction(+:numoutside) private(j,c) firstprivate(eps)
   for (i=0; i<NPOINTS; i++) {
     for (j=0; j<NPOINTS; j++) {
       c.r = -2.0+2.5*(double)(i)/(double)(NPOINTS)+eps;
       c.i = 1.125*(double)(j)/(double)(NPOINTS)+eps;
       testpoint(c,numoutside);
     }
   }

// Calculate area of set and error estimate and output the results
area=2.0*2.5*1.125*(double)(NPOINTS*NPOINTS-numoutside)/(double)(NPOINTS*NPOINTS);
   error=area/(double)NPOINTS;

   printf("Area of Mandlebrot set = %12.8f +/- %12.8f\n",area,error);
   printf("Correct answer should be around 1.510659\n");

}

void testpoint(struct d_complex& c, int& numoutside){

// Does the iteration z=z*z+c, until |z| > 2 when point is known to be outside set
// If loop count reaches MAXITER, point is considered to be inside the set

       struct d_complex z;
       int iter;
       double temp;

       z=c;
       for (iter=0; iter<MAXITER; iter++){
         temp = (z.r*z.r)-(z.i*z.i)+c.r;
         z.i = z.r*z.i*2+c.i;
         z.r = temp;
         if ((z.r*z.r+z.i*z.i)>4.0) {
           numoutside++;
           break;
         }
       }

}
