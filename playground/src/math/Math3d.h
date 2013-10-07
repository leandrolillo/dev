#ifndef _MATH3D_H_
#define _MATH3D_H_

#include "mathConstants.h"
//#include "baseMath.h"
#include <string.h>

class matriz_2x2;
class matriz_4x4;
class matriz_3x3;
class cuaternion;
class vector3;
class vector2;

typedef matriz_4x4 matriz;
typedef vector3 vector;
typedef vector2 texel;
typedef cuaternion vector4;


class matriz_mxn  {
	private:
		real *elementos;
		boolean ownsBuffer;
	protected:
		unsigned int nroFilas, nroColumnas;
	public:
		unsigned int getNroFilas() const {
			return this->nroFilas;
		}

		unsigned int getNroColumnas() const {
			return this->nroColumnas;
		}
		matriz_mxn(unsigned int nroFilas = 0, unsigned int nroColumnas = 0, real *buffer = null) {
			this->nroFilas = nroFilas;
			this->nroColumnas = nroColumnas;
			this->elementos = null;

			ownsBuffer = (buffer == null);
			if(buffer != null) {
				this->elementos = buffer;
			}
			else {
				unsigned int size = getNroColumnas() * this->getNroFilas();
				if (size > 0) {
					this->elementos = new real[size];
					memset(this->elementos, 0, sizeof(real) * size);
				}
			}

		}

		matriz_mxn(const matriz_mxn &op2) {
			ownsBuffer = true;
			this->elementos = null;
			this->nroFilas = this->nroColumnas = 0;
			*this = op2;
		}

		virtual ~matriz_mxn() {
			//if(this->elementos != null) {
			if(ownsBuffer) {
				delete[] this->elementos;
				this->elementos = null;
			}
			//}
		}

		virtual real &operator()(unsigned int fila, unsigned int columna) {
			if (0 <= fila && fila < this->getNroFilas())
				if (0 <= columna && columna < this->getNroColumnas())
					return this->elementos[fila * this->getNroColumnas() + columna];

			throw InvalidArgumentException("Index Out of Bounds - matriz_mxn::operator()");
		}

		virtual const real &operator()(unsigned int fila, unsigned int columna) const {
			if (0 <= fila && fila < this->getNroFilas())
				if (0 <= columna && columna < this->getNroColumnas())
					return this->elementos[fila * this->getNroColumnas() + columna];

			throw InvalidArgumentException("Index Out of Bounds - matriz_mxn::operator()");
		}

		operator const real() const;
		const matriz_mxn &operator =(const matriz_mxn &op2) {
			unsigned int size = op2.getNroColumnas() * op2.getNroFilas();

			if (this->getNroColumnas() != op2.getNroColumnas() || this->getNroFilas() != op2.getNroFilas()) {
				if(this->ownsBuffer) {
					if (this->elementos != null)
						delete[] this->elementos;

					this->nroFilas = op2.getNroFilas();
					this->nroColumnas = op2.getNroColumnas();
					this->elementos = new real[size];
				} else
					throw InvalidArgumentException("Can not resize matrix with external buffer");

			}

			if (size > 0) {
				if (op2.elementos != null)
					memcpy(this->elementos, op2.elementos,
							sizeof(real) * size);
				else
					throw InvalidArgumentException(
							"No se ha asignado memoria para los elementos del operador matricial 2 - matriz_mxn::matriz_mxn(const matriz_mxn &op2)");
			}
			return (*this);
		}

		const matriz_mxn operator +(const matriz_mxn &op1) const;
		const matriz_mxn operator -(const matriz_mxn &op1) const;
		const matriz_mxn operator *(real op1) const;
		const matriz_mxn operator *(const matriz_mxn &op1) const;

//			void trasponer();
//			const matriz_mxn traspuesta() const;

		virtual String toString() const {
			char temp[500];
			std::string result;

			for(unsigned int currentRow = 0; currentRow < this->getNroFilas(); currentRow++) {
				if(currentRow == 0)
				result += "/";
				else if(currentRow == this->getNroFilas() - 1)
				result += "\\";
				else
				result += "|";

				for(unsigned int currentColumn = 0; currentColumn < this->getNroColumnas(); currentColumn++) {
					sprintf(temp, "%.3e", this->operator()(currentRow, currentColumn));
					result += temp;

					if(currentColumn != this->getNroColumnas() - 1)
					result += " ";
				}

				result += " ";

				if(currentRow == 0)
				sprintf(temp, "\\");
				else if(currentRow == this->getNroColumnas() - 1)
				sprintf(temp, "/	");
				else
				sprintf(temp, "|");

				result += temp;
				result += "\n";
			}

			return result;
		}

		static const matriz_mxn identidad(unsigned int n);
	protected:
		virtual real *getElementos()
		{
			return new real[getNroFilas() * getNroColumnas()];
		}

};

class matriz_2x2: public matriz_mxn {
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

		matriz_2x2(real _00, real _01, real _10, real _11) :
				matriz_mxn(2, 2, m) {
			this->_00 = _00;
			this->_01 = _01;
			this->_10 = _10;
			this->_11 = _11;
		}
		matriz_2x2() :
						matriz_mxn(2, 2, m) {
					this->_00 = 1.0;
					this->_01 = 0.0;
					this->_10 = 0.0;
					this->_11 = 1.0;
				}
		matriz_2x2(const matriz_2x2 &op1) :
				matriz_mxn(2, 2, m) {
			*this = op1;
		}

		~matriz_2x2() {}

		real &operator()(unsigned int fila, unsigned int columna) {
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
	protected:
			real *getElementos() {
				return m;
			}
};

class matriz_3x3: public matriz_mxn {
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

		matriz_3x3(real d00, real d01, real d02, real d10, real d11,
				real d12, real d20, real d21, real d22) :
				matriz_mxn(3, 3) {
			this->_00 = d00;
			this->_01 = d01;
			this->_02 = d02;
			this->_10 = d10;
			this->_11 = d11;
			this->_12 = d12;
			this->_20 = d20;
			this->_21 = d21;
			this->_22 = d22;
		}
		matriz_3x3(const matriz_3x3 &op1) :
				matriz_mxn(3, 3) { // Constructor de Copia
			memcpy(this->m, op1.m, sizeof(this->m));
		}
		matriz_3x3(void) :
				matriz_mxn(3, 3) { // matriz Identidad
			this->_00 = 1.0;
			this->_01 = 0.0;
			this->_02 = 0.0;
			this->_10 = 0.0;
			this->_11 = 1.0;
			this->_12 = 0.0;
			this->_20 = 0.0;
			this->_21 = 0.0;
			this->_22 = 1.0;
		}
		//matriz_3x3(const matriz_4x4 &op1);
		~matriz_3x3() {}

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
		//operator matriz_4x4() const;

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
	protected:
			real *getElementos() {
				return m;
			}
};

class matriz_4x4: matriz_mxn {
		friend class matriz_3x3;
	private:
		union {
				real m[16];
				struct {
						real _00, _10, _20, _30;
						real _01, _11, _21, _31;
						real _02, _12, _22, _32;
						real _03, _13, _23, _33;
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

	public:
		static matriz_4x4 Identidad;

		matriz_4x4(real _00, real _01, real _02, real _03, real _10, real _11, real _12, real _13, real _20, real _21,
				real _22, real _23, real _30, real _31, real _32, real _33);
		matriz_4x4(const matriz_4x4 &op1);
		matriz_4x4(void);
		~matriz_4x4() {}

		matriz_4x4 operator=(const matriz_4x4 &op1);
		const matriz_4x4 operator*(const matriz_4x4 &op2) const;
		const matriz_4x4 operator *(real op2) const;
		const vector3 operator*(const vector3 &op1) const;
		const matriz_4x4 operator +(const matriz_4x4 &op2) const;
		const matriz_4x4 operator -(const matriz_4x4 &op2) const;
		void operator*=(const matriz_4x4 &op2);
		void operator *=(real op2);
		void operator +=(const matriz_4x4 &op2);
		void operator -=(const matriz_4x4 &op2);
		//matriz_4x4 operator =(const matriz_3x3 &op2);

		operator real *() const;
		//operator matriz_3x3() const;

//			void trasponer(void);
//			const matriz_4x4 traspuesta(void) const;
//
//			unsigned char esSingular(void);
//			const real determinante(void);
//			unsigned char invertir(void);
//			const matriz_4x4 inversa(void) const;
//
//
//			void invertirTransformacion(void);
	protected:
			real *getElementos() {
				return m;
			}

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
		vectorN() : matriz_mxn(0, 0)
		{
		}

		vectorN(unsigned int longitud, bool vectorFila = true) :
				matriz_mxn(1, longitud) {
//				if(!vectorFila)
//					this->hacerVectorColumna();
		}
		vectorN(const matriz_mxn &op1) :
				matriz_mxn(op1) {
		}

		vectorN(const vector2 &op1) : matriz_mxn(1, 2)
		{
			vectorN::operator()(0) = op1.x;
			vectorN::operator()(1) = op1.y;
		}
//			vectorN(const vectorN &op1) : matriz_mxn((matriz_mxn &)op1), BaseVector(op1.getLength())
//			{
//			}

				unsigned int getLength() const {
			if (this->getNroColumnas() > this->getNroFilas())
				return getNroColumnas();

			return getNroFilas();
		}
		virtual real &operator()(unsigned int index) {
			if (this->getNroFilas() == 1)
				return matriz_mxn::operator ()(0, index);

			return matriz_mxn::operator ()(index, 0);
		}

		const vectorN &operator =(const vectorN &op1) {
			if (((const matriz_mxn &) op1).getNroColumnas() == 1 || ((const matriz_mxn &) op1).getNroFilas() == 1) {
				this->matriz_mxn::operator=(op1);
				return *this;
			} else
				throw InvalidArgumentException(
						"Una de las dimensiones debe ser igual a 1 (¿tratando de asignar una matriz a un vector?) - vectorN::operator =");
		}

		real punto(vectorN &operador) {
			if (this->getLength() == operador.getLength()) {
				real resultado = 0;

				for (unsigned int index = 1; index <= this->getLength(); index++)
					resultado += vectorN::operator ()(index) * operador(index);

				return resultado;
			} else
				throw InvalidArgumentException("Los vectores deben ser del mismo tamaño - vectorN::punto(...)");
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
