#ifndef _MATH3D_H_
#define _MATH3D_H_

#include "MathConstants.h"
//#include "baseMath.h"

class matriz_2x2;
class matriz_4x4;
class matriz_3x3;
class cuaternion;
class vector3;
class vector2;

typedef matriz_4x4 matriz;
typedef matriz_4x4 matrix;
typedef matriz_4x4 matrix_4x4;
typedef vector3 vector;
typedef vector2 texel;
typedef cuaternion vector4;

//#define defaultNumberFormat "%.3e"
constexpr char const *defaultNumberFormat ="%.2f";

class BaseMatrix {
	protected:
		unsigned int nroFilas, nroColumnas;

	public:
		BaseMatrix(unsigned int nroFilas, unsigned int nroColumnas) {
			this->nroFilas = nroFilas;
			this->nroColumnas = nroColumnas;
		}
		unsigned int getNroFilas() const {
			return this->nroFilas;
		}

		unsigned int getNroColumnas() const {
			return this->nroColumnas;
		}
		virtual real &operator()(unsigned int fila, unsigned int columna) = 0;

		virtual real operator()(unsigned int fila, unsigned int columna) const = 0;

		virtual String toString(String format = defaultNumberFormat) const {
			std::string result;

			char temp[500];


			for(unsigned int currentRow = 0; currentRow < this->getNroFilas(); currentRow++) {
				if(currentRow == 0) {
					result += "/";
				} else if(currentRow == this->getNroFilas() - 1) {
					result += "\\";
				} else {
					result += "|";
				}

				for(unsigned int currentColumn = 0; currentColumn < this->getNroColumnas(); currentColumn++) {
					sprintf(temp, format.c_str(), this->operator()(currentRow, currentColumn));
					//sprintf(temp, "%u %u", currentRow, currentColumn);
					result += temp;

					if(currentColumn != this->getNroColumnas() - 1) {
						result += "	";
					}
				}

				//result += " ";

				if(currentRow == 0) {
					result += "\\";
				} else if(currentRow == this->getNroColumnas() - 1) {
					result += "/	";
				} else {
					result += "|";
				}

				result += "\n";
			}

			return result;

		}

		virtual ~BaseMatrix() {};

	};

class matriz_2x2: public BaseMatrix {
	protected:
		union {
				struct {
						real _00, _01;
						real _10, _11;
				};
				real m[4];
		};

		//Factory Methods
	public:
		static const matriz_2x2 matrizRotacion(real angulo);
		static const matriz_2x2 matrizZoom(real x, real y);

	public:
		static matriz_2x2 identidad;

		matriz_2x2();
		matriz_2x2(real _00, real _01, real _10, real _11);
		matriz_2x2(const matriz_2x2 &op1);

		real &operator()(unsigned int fila, unsigned int columna) {
			if (fila > 1 || columna > 1)
				throw InvalidArgumentException("Index Out of Bounds - matriz_2x2::operator(i, j)");

			return m[fila * 2 + columna];
		}

		real operator()(unsigned int fila, unsigned int columna) const {
			if (fila > 1 || columna > 1)
				throw InvalidArgumentException("Index Out of Bounds - matriz_2x2::operator(i, j)");

			return m[fila * 2 + columna];
		}
		const matriz_2x2 operator +(const matriz_2x2 &op1) const;
		const matriz_2x2 operator -(const matriz_2x2 &op1) const;
		const matriz_2x2 operator *(const matriz_2x2 &op1) const;
		const matriz_2x2 operator *(const real &op1) const;
		const vector2 operator *(const vector2 &op1) const;

		void operator +=(const matriz_2x2 &op1);
		void operator -=(const matriz_2x2 &op1);
		void operator *=(const matriz_2x2 &op1);
		void operator *=(const real &op1);

//		void transponer();
//		const matriz_2x2 transpuesta() const;
//
//		void ortoNormalizar();
//		const matriz_2x2 ortoNormalizada() const;
//
//		unsigned char esSingular() const;
//
//		const real determinante() const;
//
//		unsigned char invertir();
//		const matriz_2x2 inversa() const;
};

class matriz_3x3: public BaseMatrix {
		friend class matriz_4x4;
		friend const vector3 operator*(const vector3 &op1, const matriz_3x3 &op2);
		friend const vector3 operator*(const matriz_3x3 &op1, const vector3 &op2);
		friend const matriz_3x3 operator *(real op1, const matriz_3x3 &op2);
	protected:
		union {
				struct {
						real _00, _01, _02;
						real _10, _11, _12;
						real _20, _21, _22;
				};
				real m[9];
		};

	public:
		// Factory methods
		static const matriz_3x3 matrizRotacion(real x, real y, real z);
		static const matriz_3x3 matrizRotacion(const vector3 &angulos);

		/**
		 * Asume que el eje está normalizado
		 */
		static const matriz_3x3 matrizRotacion(real angulo, real x, real y, real z);

		/**
		 * Asume que el eje está normalizado
		 */
		static const matriz_3x3 matrizRotacion(real angulo, const vector3 &eje);
		static const matriz_3x3 matrizRotacion(const cuaternion &rotacion);

		static const matriz_3x3 matrizZoom(real x, real y, real z);
		static const matriz_3x3 matrizZoom(const vector3 &zoom);
	public:
		static matriz_3x3 identidad;

		real &operator()(unsigned int fila, unsigned int columna) {
			if (fila > 2 || columna > 2)
				throw InvalidArgumentException("Index Out of Bounds - matriz_3x3::operator(i, j)");

			return m[fila * 3 + columna];
		}

		real operator()(unsigned int fila, unsigned int columna) const {
            if (fila > 2 || columna > 2)
                throw InvalidArgumentException("Index Out of Bounds - matriz_3x3::operator(i, j)");

            return m[fila * 3 + columna];
        }

        const vector fila(unsigned int fila) const;
        const vector columna(unsigned int columna) const;


		matriz_3x3(real d00, real d01, real d02, real d10, real d11, real d12, real d20, real d21, real d22) : BaseMatrix(3, 3) {
	        this->_00 = d00; this->_01 = d01; this->_02 = d02;
	        this->_10 = d10; this->_11 = d11; this->_12 = d12;
	        this->_20 = d20; this->_21 = d21; this->_22 = d22;
	    }

		matriz_3x3(const matriz_3x3 &op1) : BaseMatrix(3, 3) { // Constructor de Copia
	        memcpy(this->m, op1.m, sizeof(this->m));
	    }
		matriz_3x3(void) : BaseMatrix(3, 3) { // matriz Identidad
	        this->_00 = 1.0; this->_01 = 0.0; this->_02 = 0.0;
	        this->_10 = 0.0; this->_11 = 1.0; this->_12 = 0.0;
	        this->_20 = 0.0; this->_21 = 0.0; this->_22 = 1.0;
	    }

		/**
		 * Si no lo dejo, no funciona el operador (matriz_4x4)matriz_3x3. Da error de compilacion por ambiguedad en la línea que llama a la función.
		 */
		matriz_3x3(const matriz_4x4 &op1);

		matriz_3x3 operator=(const matriz_3x3 &op1) {
	        memcpy(this->m, op1.m, sizeof(this->m));

	        return(*this);
	    }

		unsigned char operator==(const matriz_3x3 &op2) const {
	        return((_00 == op2._00 && _01 == op2._01 && _02 == op2._02 &&
	            _10 == op2._10 && _11 == op2._11 && _12 == op2._12 &&
	            _20 == op2._20 && _21 == op2._21 && _22 == op2._22));
	    }

		unsigned char operator!=(const matriz_3x3 &op2) const {
	        return((_00 != op2._00 || _01 != op2._01 || _02 != op2._02 ||
	            _10 != op2._10 || _11 != op2._11 || _12 != op2._12 ||
	            _20 != op2._20 || _21 != op2._21 || _22 != op2._22));
	    }

		const matriz_3x3 operator*(const matriz_3x3 &op2) const {
	        return(matriz_3x3(  (this->_00 * op2._00) + (this->_01 * op2._10) + (this->_02 * op2._20),
	                (this->_00 * op2._01) + (this->_01 * op2._11) + (this->_02 * op2._21),
	                (this->_00 * op2._02) + (this->_01 * op2._12) + (this->_02 * op2._22),

	                (this->_10 * op2._00) + (this->_11 * op2._10) + (this->_12 * op2._20),
	                (this->_10 * op2._01) + (this->_11 * op2._11) + (this->_12 * op2._21),
	                (this->_10 * op2._02) + (this->_11 * op2._12) + (this->_12 * op2._22),

	                (this->_20 * op2._00) + (this->_21 * op2._10) + (this->_22 * op2._20),
	                (this->_20 * op2._01) + (this->_21 * op2._11) + (this->_22 * op2._21),
	                (this->_20 * op2._02) + (this->_21 * op2._12) + (this->_22 * op2._22) ));
	    }

		const matriz_3x3 operator*(real op2) const {
	        return(matriz_3x3(  _00 * op2, _01 * op2, _02 * op2,
	                    _10 * op2, _11 * op2, _12 * op2,
	                    _20 * op2, _21 * op2, _22 * op2));
	    }

		const matriz_3x3 operator+(const matriz_3x3 &op2) const {
	        return(matriz_3x3(  _00 + op2._00, _01 + op2._01, _02 + op2._02,
	                    _10 + op2._10, _11 + op2._11, _12 + op2._12,
	                    _20 + op2._20, _21 + op2._21, _22 + op2._22));
	    }

		const matriz_3x3 operator-(const matriz_3x3 &op2) const {
	        return(matriz_3x3(  _00 - op2._00, _01 - op2._01, _02 - op2._02,
	                    _10 - op2._10, _11 - op2._11, _12 - op2._12,
	                    _20 - op2._20, _21 - op2._21, _22 - op2._22));
	    }


		void operator*=(const matriz_3x3 &op2) {
	        *this = *this * op2;
	    }

		void operator*=(real op2) {
	        *this = *this * op2;
	    }

		void operator+=(const matriz_3x3 &op2) {
	        *this = *this + op2;
	    }

		void operator-=(const matriz_3x3 &op2) {
	        *this = *this - op2;
	    }

		operator real *() const {
	        return((real *)m);
	    }

		operator matriz_4x4() const;

		const matriz_3x3 traspuesta(void) const {
			return(matriz_3x3( 	this->_00, this->_10,	this->_20,
								this->_01, this->_11, this->_21,
								this->_02,	this->_12, this->_22));
		}


		const real determinante(void) const { //Calcula el determinante de la matriz.
			return(	  this->_00 * this->_11 * this->_22 +
						this->_02 * this->_10 * this->_21 +
						this->_01 * this->_12 * this->_20 -
						this->_02 * this->_11 * this->_20 -
						this->_12 * this->_21 * this->_00 -
						this->_01 * this->_10 * this->_22);
		}

		const matriz_3x3 inversa(void) const {
			real det = this->determinante();
			if(det == 0.0f) {
				throw InvalidArgumentException("Could not calculate inverse of singular matrix (det=0). This is the matrix equivalent of a division by zero");
			} else {
				det = 1.0f / det;

				return matriz_3x3(
					(this->_11 * this->_22 - this->_12 * this->_21) * det, (-this->_01 * this->_22 + this->_02 * this->_21) * det , (this->_01 * this->_12 - this->_11 * this->_02) * det,
					(-this->_10 * this->_22 + this->_12 * this->_20) * det, (this->_00 * this->_22 - this->_02 * this->_20) * det, (-this->_00 * this->_12 + this->_02 * this->_10) * det,
					(this->_10 * this->_21 - this->_20 * this->_11) * det,  (-this->_00 * this->_21 + this->_01 * this->_20) * det, (this->_00 * this->_11 - this->_10 * this->_01) * det
				 );
			 }
		}

//		void transponer(void);
//		const matriz_3x3 transpuesta(void) const;
//
//		void ortonormalizar(void);
//		const matriz_3x3 ortonormalizada(void) const;
//
//		unsigned char esSingular(void);
//		const real determinante(void);
//		unsigned char invertir(void);
//		const matriz_3x3 inversa(void) const;
//
//		void star(const vector3 &op1);
		//real &operator()(int fila, int columna);
};

class matriz_4x4: public BaseMatrix {
		friend class matriz_3x3;
	private:
		union {
				real m[16];
				struct {
						real _00, _01, _02, _03;
						real _10, _11, _12, _13;
						real _20, _21, _22, _23;
						real _30, _31, _32, _33;
				};
		};
	public:
				// Factory Methods
		static const matriz_4x4 matrizTraslacion(const vector3 &desplazamiento);
		static const matriz_4x4 matrizTraslacion(real x, real y, real z);

		static const matriz_4x4 matrizRotacion(real x, real y, real z);
		static const matriz_4x4 matrizRotacion(const vector3 &angulos);
		static const matriz_4x4 matrizRotacion(real angulo, const vector3 &eje);
		static const matriz_4x4 matrizRotacion(real angulo, real x, real y, real z);
		static const matriz_4x4 matrizRotacion(const matriz_3x3 &orientacion);
		static const matriz_4x4 matrizRotacion(const cuaternion &rotacion);

		static const matriz_4x4 matrizZoom(real x, real y, real z);
		static const matriz_4x4 matrizZoom(const vector3 &zoom);

		static const matriz_4x4 matrizBase(const matriz_3x3 &orientacion, const vector3 &posicion);

	public:
		static matriz_4x4 identidad;

		matriz_4x4(void) : BaseMatrix(4, 4) {
			this->_00 = 1.0; this->_01 = 0.0; this->_02 = 0.0; this->_03 = 0.0;
			this->_10 = 0.0; this->_11 = 1.0; this->_12 = 0.0; this->_13 = 0.0;
			this->_20 = 0.0; this->_21 = 0.0; this->_22 = 1.0; this->_23 = 0.0;
			this->_30 = 0.0; this->_31 = 0.0; this->_32 = 0.0; this->_33 = 1.0;
		}

		matriz_4x4(	real _00, real _01, real _02, real _03,
					real _10, real _11, real _12, real _13, real _20, real _21,
					real _22, real _23, real _30, real _31, real _32, real _33) : BaseMatrix(4, 4) {
			this->_00 = _00; this->_01 = _01; this->_02 = _02; this->_03 = _03;
			this->_10 = _10; this->_11 = _11; this->_12 = _12; this->_13 = _13;
			this->_20 = _20; this->_21 = _21; this->_22 = _22; this->_23 = _23;
			this->_30 = _30; this->_31 = _31; this->_32 = _32; this->_33 = _33;
		}

		matriz_4x4(const matriz_4x4 &op1) : BaseMatrix(4, 4) { //Constructor de copia
			memcpy(this->m, op1.m, sizeof(this->m));
		}

		real &operator()(unsigned int fila, unsigned int columna) {
			if (fila > 3 || columna > 3)
				throw InvalidArgumentException("Index Out of Bounds - matriz_4x4::operator(i, j)");
			return m[fila * 4 + columna];
		}
		real operator()(unsigned int fila, unsigned int columna) const {
			if (fila > 3 || columna > 3)
				throw InvalidArgumentException("Index Out of Bounds - matriz_4x4::operator(i, j)");

			return m[fila * 4 + columna];
		}

		const vector4 fila(unsigned int fila) const;
		const vector4 columna(unsigned int columna) const;

		matriz_4x4 operator=(const matriz_4x4 &op1) {
			memcpy(this->m, op1.m, sizeof(this->m));

			return(*this);
		}

		const matriz_4x4 operator*(const matriz_4x4 &op2) const {
			return(matriz_4x4(	(this->_00 * op2._00) + (this->_01 * op2._10) + (this->_02 * op2._20) + (this->_03 * op2._30),
						(this->_00 * op2._01) + (this->_01 * op2._11) + (this->_02 * op2._21) + (this->_03 * op2._31),
						(this->_00 * op2._02) + (this->_01 * op2._12) + (this->_02 * op2._22) + (this->_03 * op2._32),
						(this->_00 * op2._03) + (this->_01 * op2._13) + (this->_02 * op2._23) + (this->_03 * op2._33),

						(this->_10 * op2._00) + (this->_11 * op2._10) + (this->_12 * op2._20) + (this->_13 * op2._30),
						(this->_10 * op2._01) + (this->_11 * op2._11) + (this->_12 * op2._21) + (this->_13 * op2._31),
						(this->_10 * op2._02) + (this->_11 * op2._12) + (this->_12 * op2._22) + (this->_13 * op2._32),
						(this->_10 * op2._03) + (this->_11 * op2._13) + (this->_12 * op2._23) + (this->_13 * op2._33),

						(this->_20 * op2._00) + (this->_21 * op2._10) + (this->_22 * op2._20) + (this->_23 * op2._30),
						(this->_20 * op2._01) + (this->_21 * op2._11) + (this->_22 * op2._21) + (this->_23 * op2._31),
						(this->_20 * op2._02) + (this->_21 * op2._12) + (this->_22 * op2._22) + (this->_23 * op2._32),
						(this->_20 * op2._03) + (this->_21 * op2._13) + (this->_22 * op2._23) + (this->_23 * op2._33),

						(this->_30 * op2._00) + (this->_31 * op2._10) + (this->_32 * op2._20) + (this->_33 * op2._30),
						(this->_30 * op2._01) + (this->_31 * op2._11) + (this->_32 * op2._21) + (this->_33 * op2._31),
						(this->_30 * op2._02) + (this->_31 * op2._12) + (this->_32 * op2._22) + (this->_33 * op2._32),
						(this->_30 * op2._03) + (this->_31 * op2._13) + (this->_32 * op2._23) + (this->_33 * op2._33)));
		}

		const matriz_4x4 operator *(real op2) const {
			return(matriz_4x4(	_00 * op2, _01 * op2, _02 * op2, _03 * op2,
						_10 * op2, _11 * op2, _12 * op2, _13 * op2,
						_20 * op2, _21 * op2, _22 * op2, _23 * op2,
						_30 * op2, _31 * op2, _32 * op2, _33 * op2));
		}

		const vector3 operator*(const vector3 &op1) const;
		const vector4 operator*(const vector4 &op1) const;

		const matriz_4x4 operator +(const matriz_4x4 &op2) const {
			return(matriz_4x4(	_00 + op2._00, _01 + op2._01, _02 + op2._02, _03 + op2._03,
								_10 + op2._10,  _11 + op2._11,   _12 + op2._12,   _13 + op2._13,
								_20 + op2._20, _21 + op2._21, _22 + op2._22, _23 + op2._23,
								_30 + op2._30, _31 + op2._31, _32 + op2._32, _33 + op2._33));
		}
		const matriz_4x4 operator -(const matriz_4x4 &op2) const {
			return(matriz_4x4(	_00 - op2._00, _01 - op2._01, _02 - op2._02, _03 - op2._03,
						_10 - op2._10, _11 - op2._11, _12 - op2._12, _13 - op2._13,
						_20 - op2._20, _21 - op2._21, _22 - op2._22, _23 - op2._23,
						_30 - op2._30, _31 - op2._31, _32 - op2._32, _33 - op2._33));
		}
		void operator*=(const matriz_4x4 &op2) {
			*this = *this * op2;
		}
		void operator *=(real op2) {
			*this = *this * op2;
		}
		void operator +=(const matriz_4x4 &op2) {
			*this =  *this + op2;
		}
		void operator -=(const matriz_4x4 &op2) {
			*this = *this - op2;
		}
		matriz_4x4 operator =(const matriz_3x3 &op2) {
			this->_00 = op2._00;  this->_01 = op2._01; this->_02 = op2._02;
			this->_10 = op2._10;   this->_11 = op2._11;   this->_12 = op2._12;
			this->_20 = op2._20;  this->_21 = op2._21; this->_22 = op2._22;

			return(*this);
		}

		operator real *() const {
			return((real *)m);
		}

		operator matriz_3x3() const {	//Funciona solamente si dejo un constructor matriz_3x3(matriz_4x4 op1) en la clase matriz_3x3
			return(matriz_3x3(	_00, _01, _02,
								_10, _11, _12,
								_20, _21, _22));
		}

//			void trasponer(void);
		const matriz_4x4 traspuesta(void) const {
			return(matriz_4x4( 	this->_00, this->_10,	this->_20,	this->_30,
								this->_01, this->_11, this->_21,	this->_31,
								this->_02,	this->_12, this->_22, this->_32,
								this->_03, this->_13,	this->_23, this->_33));
		}

		const real determinante(void) const { //Calcula el determinante de la matriz.
            return(_00 * (_11 * _22 * _33 + _12 * _23 * _31 + _13 * _21 * _32
                            - _13 * _22 * _31 - _12 * _21 * _33 - _11 * _23 * _32)
                   - _10 * (_01 * _22 * _33 + _02 * _23 * _31 + _03 * _21 * _32
                            - _03 * _22 * _31 - _03 * _21 * _33 - _01 * _23 * _32)
                   + _20 * (_01 * _12 * _33 + _02 * _13 * _31 + _03 * _11 * _32
                            - _03 * _12 * _31 - _02 * _11 * _33 - _01 * _13 * _32)
                   - _30 * (_01 * _12 * _23 + _02 * _13 * _21 + _03 * _11 * _22
                            - _03 * _12 * _21 - _02 * _11 * _23 - _01 * _13 * _22)

            );
        }

        const matriz_4x4 inversa(void) const {
            real invDet = this->determinante();
            if(invDet == 0.0f) {
                throw InvalidArgumentException("Could not calculate inverse of singular matrix (det=0). This is the matrix equivalent of a division by zero");
            } else {
                invDet = (real)1 / invDet;

                return matriz_4x4(
                    (_11 * _22 * _33 + _12 * _23 * _31 + _13 * _21 * _32 - _13 * _22 * _31 - _12 * _21 * _33 - _11 * _23 * _32) * invDet,
                    (- _01 * _22 * _33 - _02 * _23 * _31 - _03 * _21 * _32 + _03 * _22 * _31 + _02 * _21 * _33 + _01 * _23 * _32) * invDet,
                    (_01 * _12 * _33 + _02 * _13 * _31 + _03 * _11 * _32 - _03 * _12 * _31 - _02 * _11 * _33 - _01 * _13 * _32) * invDet,
                    (- _01 * _12 * _23 - _02 * _13 * _21 - _03 * _11 * _22 + _03 * _12 * _21 + _02 * _11 * _23 + _01 * _13 * _22) * invDet,

                    (- _10 * _22 * _33 - _12 * _23 * _30 - _13 * _20 * _32 + _13 * _22 * _30 + _12 * _20 * _33 + _10 * _23 * _32) * invDet,
                    (_00 * _22 * _33 + _02 * _23 * _30 + _03 * _20 * _32 - _03 * _22 * _30 - _02 * _20 * _33 - _00 * _23 * _32) * invDet,
                    (- _00 * _12 * _33 - _02 * _13 * _30 - _03 * _10 * _32 + _03 * _12 * _30 + _02 * _10 * _33 + _00 * _13 * _32) * invDet,
                    (_00 * _12 * _23 + _02 * _13 * _20 + _03 * _10 * _22 - _03 * _12 * _20 - _02 * _10 * _23 - _00 * _13 * _22) * invDet,

                    (_10 * _21 * _33 + _11 * _23 * _30 + _13 * _20 * _31 - _13 * _21 * _30 - _11 * _20 * _33 - _10 * _23 * _31) * invDet,
                    (- _00 * _21 * _33 - _01 * _23 * _30 - _03 * _20 * _31 + _03 * _21 * _30 + _01 * _20 * _33 + _00 * _23 * _31) * invDet,
                    (_00 * _11 * _33 + _01 * _12 * _30 + _03 * _10 * _31 - _03 * _11 * _30 - _01 * _10 * _33 - _00 * _12 * _31) * invDet,
                    (- _00 * _11 * _23 - _01 * _13 * _20 - _03 * _10 * _21 + _03 * _11 * _20 + _01 * _10 * _23 + _00 * _13 * _21) * invDet,

                    (-_10 * _21 * _32 - _11 * _22 * _30 - _12 * _20 * _31 + _12 * _21 * _30 + _11 * _20 * _32 + _10 * _22 * _31) * invDet,
                    (_00 * _21 * _32 + _01 * _22 * _30 + _02 * _20 * _31 - _02 * _21 * _30 - _01 * _20 * _32 - _00 * _22 * _31) * invDet,
                    (- _00 * _11 * _32 - _01 * _12 * _30 - _02 * _10 * _31 + _02 * _11 * _30 + _01 * _10 * _32 + _00 * _12 * _31) * invDet,
                    (_00 * _11 * _22 + _01 * _12 * _20 + _02 * _10 * _21 - _02 * _11 * _20 - _01 * _10 * _22 - _00 * _12 * _21) * invDet
                 );
             }
        }


		//
//			unsigned char esSingular(void);
//			const real determinante(void);
//			unsigned char invertir(void);
//			const matriz_4x4 inversa(void) const;
//
//
//			void invertirTransformacion(void);
};

class matriz_mxn: public BaseMatrix {
	private:
		real *elementos;

	public:

		// TODO: Warning: this matrix was initially implemented with indexes 1..height instead of 0..height-1
		real &operator()(unsigned int fila, unsigned int columna) {
			if (0 <= fila && fila < this->getNroFilas())
				if (0 <= columna && columna < this->getNroColumnas())
					return this->elementos[fila * this->getNroColumnas() + columna];

			throw InvalidArgumentException("Index Out of Bounds - matriz_mxn::operator()");
		}

		real operator()(unsigned int fila, unsigned int columna) const {
			if (0 <= fila && fila < this->getNroFilas())
				if (0 <= columna && columna < this->getNroColumnas())
					return this->elementos[fila * this->getNroColumnas() + columna];

			throw InvalidArgumentException("Index Out of Bounds - matriz_mxn::operator()");
		}

		matriz_mxn();
		matriz_mxn(unsigned int nroFilas, unsigned int nroColumnas);
		matriz_mxn(const matriz_mxn &op2);//Constructor de copia (se necesitan cuando se maneja memoria din�micamente reservada)
		virtual ~matriz_mxn();

		//operator const real() const;
		const matriz_mxn &operator =(const matriz_mxn &op1);
		const matriz_mxn operator +(const matriz_mxn &op1) const;
		const matriz_mxn operator -(const matriz_mxn &op1) const;
		const matriz_mxn operator *(real op1) const;
		const matriz_mxn operator *(const matriz_mxn &op1) const;

//			void trasponer();
//			const matriz_mxn traspuesta() const;

		static const matriz_mxn identidad(unsigned int n);


};

class vector2 {
	public:
		union {
				struct {
						real x, y;
				};
				struct {
						real u, v;
				};

				real m[2];
		};

	public:
		friend const vector2 operator*(real op1, const vector2 &op2) {
			return (vector2(op1 * op2.x, op1 * op2.y));
		}

	public:
		vector2();
		vector2(real x, real y);
		//vector2(const vector2 &op1);

		const vector2 operator +(const vector2 &op1) const;
		const vector2 operator -(const vector2 &op1) const;
		const vector2 operator-(void) const;

		const real operator *(const vector2 &op1) const;
		const real operator ^(const vector2 &op1) const;

		void operator +=(const vector2 &op1);
		void operator -=(const vector2 &op1);
		void operator *=(const real &op1);

		const vector2 operator *(const real &op1) const;

		const real modulo() const;

//		void normalizar();
//		const vector2 normalizado() const;
//
//		void perpendicularizar();
//		const vector2 perpendicularizado() const;
//
//		const real perpDotProduct(const vector2 &op1) const;
//		const real productoEscalar(const vector2 &op1) const;

		operator real *() const;

		String toString(String numberFormat = defaultNumberFormat) const {
            char temp[256];

            sprintf(temp, ("<" + numberFormat + ", " + numberFormat + ">").c_str(), this->x, this->y);

            return String(temp);
        }

	private:
		const real productoEscalar(const vector2 &op1) const {
			return(this->x * op1.x + this->y * op1.y);
		}


};

class vector3 {
		friend const vector3 operator*(real op1, const vector3 &op2) {
			return (vector3(op1 * op2.x, op1 * op2.y, op1 * op2.z));
		}

		friend const vector3 operator*(const vector3 &op1, const matriz_3x3 &op2);
		friend const vector3 operator*(const matriz_3x3 &op1, const vector3 &op2);

	public:
		union {
				struct {
						real x, y, z;
				};
				real m[3];
		};

	public:
		vector3() {
		}
		vector3(real c_x , real c_y, real c_z) {
			this->x = c_x;
			this->y = c_y;
			this->z = c_z;
		}

		vector3(const vector3 &op1) {  //constructor de copia
			memcpy(this->m, op1.m, sizeof(this->m));
		}

		unsigned char operator==(const vector3 &op2) const {
			return((x == op2.x && y == op2.y && z == op2.z));
		}

		unsigned char operator!=(const vector3 &op2) const {
			return((x != op2.x || y != op2.y || z != op2.z));
		}

		const vector3 operator+(const vector3 &op2) const {
			return (vector3(this->x + op2.x, this->y + op2.y, this->z + op2.z));
		}

		const vector3 operator-(const vector3 &op2) const {
			return (vector3(this->x - op2.x, this->y - op2.y, this->z - op2.z));
		}
		const vector3 operator-(void) const {
			return(vector3(-this->x, -this->y, -this->z));
		}

		const vector3 operator*(real op2) const {
			return(vector3(this->x * op2, this->y * op2, this->z * op2));
		}

		void operator += (const vector3 &op2) {
			this->x += op2.x;
			this->y += op2.y;
			this->z += op2.z;
		}

		void operator -= (const vector3 &op2) {
			this->x -= op2.x;
			this->y -= op2.y;
			this->z -= op2.z;
		}

		void operator*=(real op2) {
		       this->x *= op2;
		       this->y *= op2;
		       this->z *= op2;
		}

		const real operator*(const vector3 &op2) const { // Producto Escalar de vector3es
			return(this->productoEscalar(op2));
		}

		const vector3 operator^(const vector3 &op2) const { // Producto Vectorial
			return(productoVectorial(op2));
		}

		const real modulo(void) const { // Devuelve el modulo del vector3
			return((real)sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z)));
		}

		operator real *() const {
			return((real *)this->m);
		}

//		const real operator() (int indice) const;
//		const real modulo(void) const;
//
//		const real normalizar(void);
		const vector3 normalizado(void) const {
			real modulo = this->modulo();

			if(modulo == 0.0f)
				throw InvalidStateException("Vector module is 0");

			real oneOverModule = (real)1 / modulo;
			return(vector3(this->x * oneOverModule, this->y * oneOverModule, this->z * oneOverModule));

		}
//
//		const matriz_3x3 star() const;

		String toString(String numberFormat = defaultNumberFormat) const {
			char temp[256];

			sprintf(temp, ("<" + numberFormat + ", " + numberFormat + ", " + numberFormat + ">").c_str(), this->x, this->y, this->z);

			return String(temp);
		}
	private:
		const real productoEscalar(const vector3 &op2) const {
			return((this->x * op2.x) + (this->y * op2.y) + (this->z * op2.z));
		}

		const vector3 productoVectorial(const vector3 &op2) const {
			return(vector3(	(this->z * op2.y - this->y * op2.z),
					(this->x * op2.z - this->z * op2.x),
					(this->y * op2.x - this->x * op2.y)));
		}
};

class cuaternion {
	public:
		union {
				struct {
						real x, y, z, w;
				};
				real m[4];
		};

		cuaternion(void) {
			this->x = 0.0;
			this->y = 0.0;
			this->z = 0.0;
            this->w = 1.0;
		}

		cuaternion(real new_x, real new_y, real new_z, real new_w) {
			this->x = new_x;
			this->y = new_y;
			this->z = new_z;
            this->w = new_w;
		}
		cuaternion(const vector3 &op1) { // copia los atributos x, y y z a los del cuaternion, dejando w = 0
			this->x = op1.x;
			this->y = op1.y;
			this->z = op1.z;
            this->w = 0.0;
		}

		cuaternion(const cuaternion &op1) {
			memcpy(this->m, op1.m, sizeof(this->m));
		}
		const cuaternion operator+(const cuaternion &op2) const {
			return(cuaternion(this->x + op2.x, this->y + op2.y, this->z + op2.z, this->w + op2.w));
		}

		const cuaternion operator-(const cuaternion &op2) const{
			return(cuaternion(this->x - op2.x, this->y - op2.y, this->z - op2.z, this->w - op2.w));
		}
		const cuaternion operator-(void) const {
			return(cuaternion(-this->x, -this->y, -this->z, -this->w));
		}

		const cuaternion operator*(const real op2) const {
			return(cuaternion(this->x * op2, this->y * op2, this->z * op2, this->w * op2));
		}
		void operator *=(const real op2) {
			this->x *= op2;
			this->y *= op2;
			this->z *= op2;
			this->w *= op2;
		}

		const cuaternion operator*(const cuaternion &op2) const {
			return(cuaternion(
			        this->w * op2.x + this->x * op2.w + this->y * op2.z - this->z * op2.y,
                    this->w * op2.y + this->y * op2.w + this->z * op2.x - this->x * op2.z,
					this->w * op2.z + this->z * op2.w + this->x * op2.y - this->y * op2.x,
					this->w * op2.w - this->x * op2.x - this->y * op2.y - this->z * op2.z
					));
		}

		operator real *() const {
			return((real *)m);
		}

		const real modulo() const { // Devuelve el m�dulo del cuaternion
			return((real)sqrt(this->w * this->w + this->x * this->x   +   this->y * this->y   +   this->z * this->z));
		}

		const cuaternion normalizado(void) const { // Devuelve el cuaternion normalizado
			real modulo = this->modulo();

			if(modulo == (real)0)
				throw InvalidStateException("Cuaternion module is 0");

			real unosobremodulo = (real)1 / modulo;

			return(*this * unosobremodulo);
		}

		operator vector3() const {
			return vector3(this->x, this->y, this->z);
		}

		operator matriz_4x4() const { //Obtiene la matriz de 3x3 correspondiente al cuaternion sobre el cual se aplica la operaci�n.
			real y2 = y * y, x2 = x * x, z2 = z * z;
			real xy = x * y;
			real xz = x * z;
			real wx = x * w;
			real yz = y * z;
			real wy = y * w;
			real wz = w * z;
			return(matriz_4x4(1.0f - 2.0f * (y2 + z2),  2.0f * (xy - wz),              2.0f * (xz + wy),		 0.0f,
							  2.0f * (xy + wz),            1.0f - 2.0f * (x2 + z2),   2.0f * (yz - wx),		0.0f,
							  2.0f * (xz - wy),            2.0f * (yz +  wx),             1.0f - 2.0f * (x2 + y2),	0.0f,
							  0.0f,				      0.0f,				    0.0f,			         1.0f
			));
		}
		operator matriz_3x3() const { // obtiene la matriz de 4x4 que corresponde al cuaternion
			real y2 = y * y, x2 = x * x, z2 = z * z;
			real xy = x * y;
			real xz = x * z;
			real wx = x * w;
			real yz = y * z;
			real wy = y * w;
			real wz = w * z;
			return(matriz_3x3( 1.0f - 2.0f * (y2 + z2),   2.0f * (xy - wz),             2.0f * (xz + wy),
								2.0f * (xy + wz),            1.0f - 2.0f * (x2 + z2),	   2.0f * (yz - wx),
								2.0f * (xz - wy),            2.0f * (yz +  wx),             1.0f - 2.0f * (x2 + y2)
			));
		}

		String toString(const String format = defaultNumberFormat) const {
			char temp[256];

			sprintf(temp, ("<" + format + ", " + format + ", " + format + ", " + format + ">").c_str(), this->x, this->y, this->z, this->w);

			return String(temp);
		}
//
//			void conjugar(void);
//			const cuaternion conjugado(void) const;
//
//			void invertir(void);
//			const cuaternion invertido(void) const;
//
//			void normalizar(void);
//			const cuaternion normalizado(void) const;
//
//			void slerp(const cuaternion &dedonde, const cuaternion &adonde, real t);
//
//			operator matriz_4x4() const;
//			operator matriz_3x3() const;
//			const real operator()(int nro) const;

		//Factory methods
		static const cuaternion cuaternionRotacion(real angulo, const vector3 &eje) {
			return cuaternion(
					eje.x * (real)sin(angulo * 0.5),
					eje.y * (real)sin(angulo * 0.5),
					eje.z * (real)sin(angulo * 0.5),
					(real)cos(angulo * 0.5));
		}
		static const cuaternion cuaternionRotacion(real x, real y, real z) {
			real	ex, ey, ez;
			real	cr, cp, cy, sr, sp, sy, cpcy, spsy;		// temp vars in roll,pitch yaw

			ex = x * (real)0.5;	// convert to rads and half them
			ey = y * (real)0.5;
			ez = z * (real)0.5;

			cr = cos(ex); sr = sin(ex);
			cp = cos(ey); sp = sin(ey);
			cy = cos(ez); sy = sin(ez);

			cpcy = cp * cy;
			spsy = sp * sy;

			return cuaternion(
					real(sr * cpcy - cr * spsy),
					real(cr * sp * cy + sr * cp * sy),
					real(cr * cp * sy - sr * sp * cy),
					real(cr * cpcy + sr * spsy)).normalizado();
		}
		static const cuaternion cuaternionRotacion(const vector3 &angulos) {
			return cuaternionRotacion(angulos.x, angulos.y, angulos.z);
		}
//		static const cuaternion cuaternionAnguloEje(real angulo, const vector3 &eje) {
//
//		}

		static const cuaternion slerp(const cuaternion &dedonde, const cuaternion &adonde, real t) { // Interpola mediante el m�todo SLERP (Spherical Linear intERPolation).
			real sen, cos, omega, escala_0, escala_1;

			cos = dedonde.x * adonde.x + dedonde.y * adonde.y + dedonde.z * adonde.z + dedonde.w * adonde.w;
			if((1.0f + cos) > DELTA) {
				if((1.0f - cos) > DELTA) {
					omega = (real)acos(cos);
					sen = (real)sin(omega);
					escala_0 = (real)sin((1.0f - t) * omega)  / sen;
					escala_1 = (real)sin(t * omega) / sen;
				}
				else {
					escala_0 = 1.0f - t;
					escala_1 = t;
				}

				return (  (dedonde * escala_0)  +  (adonde * escala_1)  );
			}
			else {
				escala_0 = (real)sin((1.0f - t) * (M_PI / 2.0f));
				escala_1 = (real)sin(t * (M_PI/2.0f));

				return cuaternion (
						(escala_0 * dedonde.x) - (escala_1 * adonde.y),
						(escala_0 * dedonde.y) + (escala_1 * adonde.x),
						(escala_0 * dedonde.z) - (escala_1 * adonde.w),
						(escala_0 * dedonde.w) - (escala_1 * adonde.z));
			}
		}
};

class vectorN: public matriz_mxn {
	public:
		vectorN(unsigned int longitud, bool vectorFila = true) :
				matriz_mxn(1, longitud) {
//				if(!vectorFila)
//					this->hacerVectorColumna();
		}
		vectorN(const matriz_mxn &op1) :
				matriz_mxn(op1) {
		}
//			vectorN(const vectorN &op1) : matriz_mxn((matriz_mxn &)op1), BaseVector(op1.getLength())
//			{
//			}

				unsigned int getLength() const {
			if (this->getNroColumnas() > this->getNroFilas())
				return getNroColumnas();

			return getNroFilas();
		}
		virtual real &operator()(unsigned int fila, unsigned int columna) {
			throw InvalidOperationException("Vector doesn't support this method");
		}
		virtual real &operator()(unsigned int index) {
			if (this->getNroFilas() == 1)
				return ((BaseMatrix *) this)->operator ()(1, index);

			return ((BaseMatrix *) this)->operator ()(index, 1);
		}

		const vectorN &operator =(const vectorN &op1) {
			if (((const matriz_mxn &) op1).getNroColumnas() == 1 || ((const matriz_mxn &) op1).getNroFilas() == 1) {
				this->matriz_mxn::operator=(op1);
				return *this;
			} else
				throw InvalidArgumentException(
						"Una de las dimensiones debe ser igual a 1 (�tratando de asignar una matriz a un vector?) - vectorN::operator =");
		}

		real punto(vectorN &operador) {
			if (this->getLength() == operador.getLength()) {
				real resultado = 0;

				for (unsigned int index = 1; index <= this->getLength(); index++)
					resultado += vectorN::operator ()(index) * operador(index);

				return resultado;
			} else
				throw InvalidArgumentException("Los vectores deben ser del mismo tama�o - vectorN::punto(...)");
		}

//			bool esVectorColumna() const
//			{
//				return this->getNroColumnas() < this->getNroFilas();
//			}
//
//			void hacerVectorColumna()
//			{
//				if(!this->esVectorColumna())
//					this->matriz_mxn::trasponer();
//			}
//			void hacerVectorFila()
//			{
//				if(this->esVectorColumna())
//					this->matriz_mxn::trasponer();
//			}
	private:
};

#endif
