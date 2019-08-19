/*
 * mathUtilities.h
 *
 *  Created on: 29/06/2013
 *      Author: Lean
 */

#ifndef MATHUTILITIES_H_
#define MATHUTILITIES_H_

#include "Math3d.h"

class MatrixUtilities
{
		//			virtual unsigned char esSingular() const;
//			virtual const real determinante() const;
//			virtual void transponer();

		void transponer();
		const matriz_2x2 transpuesta() const;

		void ortoNormalizar();
		const matriz_2x2 ortoNormalizada() const;

		unsigned char esSingular() const;

		const real determinante() const;

		unsigned char invertir();
		const matriz_2x2 inversa() const;

};

class VectorUtilities
{
	public:
//		virtual const real modulo() const;

		static vector3 normalizar(const vector3 &operador) {
			real modulo = operador.modulo();

			if(modulo == 0.0f)
				throw InvalidStateException("Vector module is 0");

			return operador * (1.0f / modulo);
		}

};

#endif /* MATHUTILITIES_H_ */
