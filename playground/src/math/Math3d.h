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

		virtual String toString() const {
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
					sprintf(temp, "%.3e", this->operator()(currentRow, currentColumn));
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
		static matriz_2x2 Identidad;

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
		static const matriz_3x3 matrizRotacion(real angulo, real x, real y, real z); // el eje debe estar normalizado
		static const matriz_3x3 matrizRotacion(real angulo, const vector3 &eje); // el eje debe estar normalizado
		static const matriz_3x3 matrizRotacion(const cuaternion &rotacion);

		static const matriz_3x3 matrizZoom(real x, real y, real z);
		static const matriz_3x3 matrizZoom(const vector3 &zoom);
	public:
		static matriz_3x3 Identidad;

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
		matriz_3x3(real d00, real d01, real d02, real d10, real d11, real d12, real d20, real d21, real d22);
		matriz_3x3(const matriz_3x3 &op1);
		matriz_3x3(void);
		matriz_3x3(real x, real y, real z);
		matriz_3x3(const vector3 &angulos);
		matriz_3x3(const vector3 &vec0, const vector3 &vec1, const vector3 &vec2);
		matriz_3x3(real angulo, real x, real y, real z);
		matriz_3x3(real angulo, const vector3 &eje);
		matriz_3x3(const matriz_4x4 &op1);

		matriz_3x3 operator=(const matriz_3x3 &op1);
		unsigned char operator==(const matriz_3x3 &op2) const;
		unsigned char operator!=(const matriz_3x3 &op2) const;
		const matriz_3x3 operator*(const matriz_3x3 &op2) const;
		const matriz_3x3 operator*(real op2) const;
		const matriz_3x3 operator+(const matriz_3x3 &op2) const;
		const matriz_3x3 operator-(const matriz_3x3 &op2) const;
		void operator*=(const matriz_3x3 &op2);
		void operator*=(real op2);
		void operator+=(const matriz_3x3 &op2);
		void operator-=(const matriz_3x3 &op2);
		operator real *() const;
		operator matriz_4x4() const;

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
		real &operator()(int fila, int columna);
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
		static matriz_4x4 Identidad;

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


		matriz_4x4 operator=(const matriz_4x4 &op1) {
			memcpy(this->m, op1.m, sizeof(this->m));

			return(*this);
		}

		const matriz_4x4 operator*(const matriz_4x4 &op2) const const {
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
		vector3(real c_x, real c_y, real c_z);
		vector3(const cuaternion &quat);
		vector3(void);
		vector3(const vector3 &op1);
		unsigned char operator==(const vector3 &op2) const;
		unsigned char operator!=(const vector3 &op2) const;
		const vector3 operator+(const vector3 &op2) const;
		const vector3 operator-(const vector3 &op2) const;
		const vector3 operator-(void) const;
		const vector3 operator*(real op2) const;
		void operator +=(const vector3 &op2);
		void operator -=(const vector3 &op2);
		void operator*=(real op2);

		const real operator*(const vector3 &op2) const;
		const vector3 operator^(const vector3 &op2) const;

		const real modulo() const;

		operator real *() const;
//		const real operator() (int indice) const;
//		const real modulo(void) const;
//
//		const real normalizar(void);
//		const vector3 normalizado(void) const;
//
//		const matriz_3x3 star() const;
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
						real w, x, y, z;
				};
				real m[4];
		};

		cuaternion(real new_w, real new_x, real new_y, real new_z);
		cuaternion(real new_x, real new_y, real new_z);
		cuaternion(const cuaternion &dedonde, const cuaternion &adonde, real t);
		cuaternion(const vector3 &op1);
		cuaternion(void);
		cuaternion(const cuaternion &op1);
		const cuaternion operator+(const cuaternion &op2) const;
		const cuaternion operator-(const cuaternion &op2) const;
		const cuaternion operator-(void) const;
		const cuaternion operator*(const real op2) const;
		void operator *=(const real op2);
		const cuaternion operator*(const cuaternion &op2) const;
		operator real *() const;

		const real modulo() const;
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
		static const cuaternion cuaternionRotacion(real angulo, const vector3 &eje);
		static const cuaternion cuaternionRotacion(real x, real y, real z);
		static const cuaternion cutaernionRotacion(const vector3 &angulos);
		static const cuaternion cuaternionAnguloEje(real angulo, const vector3 &eje);
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
