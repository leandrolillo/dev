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

	#include "exceptions/InvalidArgumentException.h"
	#include "exceptions/InvalidStateException.h"
	#include "exceptions/InvalidOperationException.h"
	#include "core/javaLike.h"

	typedef float real;

	#define DELTA 0.005

	#ifndef M_PI	// algunos math.h no lo definen, como el de visual studio.
		#define M_PI 3.14159265358979323846264338327950288419716939937510f
	#endif

	#ifndef _MAX_MINS_
		#define _MAX_MINS_
		#define REAL_MAX FLT_MAX	//Máximo valor positivo
		#define REAL_MIN FLT_MIN	// Mínimo valor POSITIVO
	#endif

	#ifndef radian
		#define radian(angulo) (angulo * M_PI  / 180.0f)
		#define grado(angulo) (angulo * 180.0f / M_PI)
	#endif

	#define THRESHOLD 0.00000000000000000000000009
	#define equalsZero(r) (-(real)THRESHOLD <= r && r <= (real)THRESHOLD)

#endif /* MATHCONSTANTS_H_ */
