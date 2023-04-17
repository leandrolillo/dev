/*
 * mathConstants.h
 *
 *  Created on: 29/06/2013
 *      Author: Lean
 */

#ifndef MATHCONSTANTS_H_
#define MATHCONSTANTS_H_

	#include<math.h>
	#include<float.h>
	#include<string>

	#include "../exceptions/InvalidArgumentException.h"
	#include "../exceptions/InvalidStateException.h"
	#include "../exceptions/InvalidOperationException.h"
	
	typedef float real;

	#ifndef _REAL_POW
		#define _REAL_POW
		#define powr powf
	#endif

	constexpr real DELTA = (real)0.005;
	constexpr real one_over_rand_max = ((real)1 / (real)RAND_MAX);

	#ifndef M_PI	// algunos math.h no lo definen, como el de visual studio.
		#define M_PI 3.14159265358979323846264338327950288419716939937510f
	#endif

	#ifndef _MAX_MINS_
		#define _MAX_MINS_
		constexpr real REAL_MAX = FLT_MAX;	//Maximo valor positivo
		constexpr real REAL_MIN = FLT_MIN;	// Minimo valor POSITIVO
	#endif

	#ifndef radian
		constexpr real two_pi = (real)2 * M_PI;
		constexpr real pi_over_180 = M_PI  / 180.0;
		constexpr real one_over_pi_over_180 = 180.0 / M_PI;

		/*
		 * 180º = PI radians / 360º = 2 * PI radians
		 */
		#define radian(angulo) (angulo * pi_over_180)
		#define grado(angulo) (angulo * one_over_pi_over_180)
	#endif

	#define THRESHOLD 0.00000000000000000000000009
	#define equalsZero(r) (-(real)THRESHOLD <= r && r <= (real)THRESHOLD)
#endif /* MATHCONSTANTS_H_ */
