#ifndef _Polynomial_h_
#define _Polynomial_h_
/* Polynomial.h
 *
 * Copyright (C) 1993-2011, 2015 David Weenink
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 djmw 20020813 GPL header
 djmw 20110306 Latest modification.
*/

#define FITTER_PARAMETER_FREE 0
#define FITTER_PARAMETER_FIXED 1

#include "SimpleVector.h"
#include "Function.h"
#include "TableOfReal.h"
#include "Graphics.h"
#include "Minimizers.h"
#include "Spectrum.h"
#include "RealTier.h"
#include "SSCP.h"

#define Spline_MAXIMUM_DEGREE 20

#include "Polynomial_def.h"

void FunctionTerms_init (FunctionTerms me, double xmin, double xmax, long numberOfCoefficients);

void FunctionTerms_initFromString (FunctionTerms me, double xmin, double xmax, const char32 *s, int allowTrailingZeros);

autoFunctionTerms FunctionTerms_create (double xmin, double xmax, long numberOfCoefficients);

void FunctionTerms_setDomain (FunctionTerms me, double xmin, double xmax);

void FunctionTerms_setCoefficient (FunctionTerms me, long index, double value);

double FunctionTerms_evaluate (FunctionTerms me, double x);

void FunctionTerms_evaluate_z (FunctionTerms me, dcomplex *z, dcomplex *p);

void FunctionTerms_evaluateTerms (FunctionTerms me, double x, double terms[]);

void FunctionTerms_getExtrema (FunctionTerms me, double x1, double x2, double *xmin, double *ymin, double *xmax, double *ymax);

long FunctionTerms_getDegree (FunctionTerms me);

double FunctionTerms_getMinimum (FunctionTerms me, double x1, double x2);

double FunctionTerms_getXOfMinimum (FunctionTerms me, double x1, double x2);

double FunctionTerms_getMaximum (FunctionTerms me, double x1, double x2);

double FunctionTerms_getXOfMaximum (FunctionTerms me, double x1, double x2);
/*
	Returns minimum and maximum function values (ymin, ymax) in
	interval [x1, x2] and their x-values (xmin, xmax).
	Precondition: [x1, x2] is a (sub)domain
		my xmin <= x1 < x2 <= my xmax
*/

void FunctionTerms_draw (FunctionTerms me, Graphics g, double xmin, double xmax, double ymin, double ymax,
	int extrapolate, int garnish);
/*
	Extrapolate only for functions whose domain is extendable and that can be extrapolated.
	Polynomials can be extrapolated.
	LegendreSeries and ChebyshevSeries cannot be extrapolated.
*/
void FunctionTerms_drawBasisFunction (FunctionTerms me, Graphics g, long index, double xmin, double xmax,
	double ymin, double ymax, int extrapolate, int garnish);

Thing_define (Polynomial, FunctionTerms) {
	// overridden methods:
	public:
		virtual double v_evaluate (double x);
		virtual void v_evaluate_z (dcomplex *z, dcomplex *p);
		virtual void v_evaluateTerms (double x, double terms[]);
		virtual void v_getExtrema (double x1, double x2, double *xmin, double *ymin, double *xmax, double *ymax);
		//virtual long v_getDegree ();   David, is het OK dat deze niet overschreven wordt? Ja
};

autoPolynomial Polynomial_create (double xmin, double xmax, long degree);

autoPolynomial Polynomial_createFromString (double xmin, double xmax, const char32 *s);

void Polynomial_scaleCoefficients_monic (Polynomial me);
/* Make coefficent of leading term 1.0 */

autoPolynomial Polynomial_scaleX (Polynomial me, double xmin, double xmax);
/* x' = (x-location) / scale */

void Polynomial_evaluate_z (Polynomial me, dcomplex *z, dcomplex *p);
/* Evaluate at complex z = x + iy */


double Polynomial_getArea (Polynomial me, double xmin, double xmax);

autoPolynomial Polynomial_getDerivative (Polynomial me);

autoPolynomial Polynomial_getPrimitive (Polynomial me);

void Polynomial_draw (Polynomial me, Graphics g, double xmin, double xmax, double ymin, double ymax, int garnish);

double Polynomial_evaluate (Polynomial me, double x);

void Polynomial_evaluateTerms (Polynomial me, double x, double terms[]);

autoPolynomial Polynomials_multiply (Polynomial me, Polynomial thee);

void Polynomials_divide (Polynomial me, Polynomial thee, autoPolynomial *q, autoPolynomial *r);

Thing_define (LegendreSeries, FunctionTerms) {
	// overridden methods:
	public:
		virtual double v_evaluate (double x);
		virtual void v_evaluateTerms (double x, double terms[]);
		virtual void v_getExtrema (double x1, double x2, double *xmin, double *ymin, double *xmax, double *ymax);
};

autoLegendreSeries LegendreSeries_create (double xmin, double xmax, long numberOfPolynomials);

autoLegendreSeries LegendreSeries_createFromString (double xmin, double xmax, const char32 *s);

autoLegendreSeries LegendreSeries_getDerivative (LegendreSeries me);

autoPolynomial LegendreSeries_to_Polynomial (LegendreSeries me);

Thing_define (Roots, ComplexVector) {
};

autoRoots Roots_create (long numberOfRoots);

void Roots_fixIntoUnitCircle (Roots me);

void Roots_sort (Roots me);
/* Sort to size of real part a+bi, a-bi*/

dcomplex Roots_evaluate_z (Roots me, dcomplex z);

autoRoots Polynomial_to_Roots_ev (Polynomial me);

long Roots_getNumberOfRoots (Roots me);

void Roots_draw (Roots me, Graphics g, double rmin, double rmax, double imin, double imax,
	const char32 *symbol, int fontSize, int garnish);

dcomplex Roots_getRoot (Roots me, long index);

void Roots_setRoot (Roots me, long index, double re, double im);

autoSpectrum Roots_to_Spectrum (Roots me, double nyquistFrequency, long numberOfFrequencies, double radius);

autoRoots Polynomial_to_Roots (Polynomial me);
/* Find roots of polynomial and polish them */

void Roots_and_Polynomial_polish (Roots me, Polynomial thee);

autoPolynomial Roots_to_Polynomial (Roots me);

autoPolynomial TableOfReal_to_Polynomial (TableOfReal me, long degree, long xcol, long ycol, long scol);

autoLegendreSeries TableOfReal_to_LegendreSeries (TableOfReal me, long numberOfPolynomials, long xcol, long ycol, long scol);

autoSpectrum Polynomial_to_Spectrum (Polynomial me, double nyquistFrequency, long numberOfFrequencies, double radius);

/*
	A ChebyshevSeries p(x) on a domain [xmin,xmax] is defined as the
	following linear combination of Chebyshev polynomials T[k](x') of
	degree k-1 and domain [-1, 1]:
		p(x) = sum (k=1..numberOfCoefficients, c[k]*T[k](x')) - c[1] / 2, where
		x' = (2 * x - xmin - xmax) / (xmax - xmin)
	This is equivalent to:
		p(x) = c[1] /2 + sum (k=2..numberOfCoefficients, c[k]*T[k](x'))
*/
Thing_define (ChebyshevSeries, FunctionTerms) {
	// overridden methods:
	public:
		virtual double v_evaluate (double x);
		virtual void v_evaluateTerms (double x, double terms[]);
		virtual void v_getExtrema (double x1, double x2, double *xmin, double *ymin, double *xmax, double *ymax);
};

autoChebyshevSeries ChebyshevSeries_create (double xmin, double xmax, long numberOfPolynomials);

autoChebyshevSeries ChebyshevSeries_createFromString (double xmin, double xmax, const char32 *s);

autoPolynomial ChebyshevSeries_to_Polynomial (ChebyshevSeries me);

void Spline_init (Spline me, double xmin, double xmax, long degree, long numberOfCoefficients, long numberOfKnots);

long Spline_getOrder (Spline me);

void Spline_drawKnots (Spline me, Graphics g, double xmin, double xmax, double ymin, double ymax, int garnish);

autoSpline Spline_scaleX (Spline me, double xmin, double xmax);
/* scale domain and knots to new domain */

Thing_define (MSpline, Spline) {
	// overridden methods:
	public:
		virtual double v_evaluate (double x);
		virtual void v_evaluateTerms (double x, double terms[]);
};

autoMSpline MSpline_create (double xmin, double xmax, long degree, long numberOfInteriorKnots);

autoMSpline MSpline_createFromStrings (double xmin, double xmax, long degree, const char32 *coef, const char32 *interiorKnots);

Thing_define (ISpline, Spline) {
	// overridden methods:
	public:
		virtual double v_evaluate (double x);
		virtual void v_evaluateTerms (double x, double terms[]);
		virtual long v_getOrder ();
};

autoISpline ISpline_create (double xmin, double xmax, long degree, long numberOfInteriorKnots);

autoISpline ISpline_createFromStrings (double xmin, double xmax, long degree, const char32 *coef, const char32 *interiorKnots);

/****************** fit **********************************************/

void FunctionTerms_and_RealTier_fit (FunctionTerms me, RealTier thee, int freezeCoefficients[], double tol, int ic, autoCovariance *c);

autoPolynomial RealTier_to_Polynomial (RealTier me, long degree, double tol, int ic, autoCovariance *cvm);

autoLegendreSeries RealTier_to_LegendreSeries (RealTier me, long degree, double tol, int ic, autoCovariance *cvm);

autoChebyshevSeries RealTier_to_ChebyshevSeries (RealTier me, long degree, double tol, int ic, autoCovariance *cvm);

#endif /* _Polynomial_h_ */
