#ifndef _MATH3D_H_
	#define _MATH3D_H_
	#include<math.h>
	#include<float.h>
	
	//#include "Math/CMatriz.h"

	//#define matriz matriz_4x4

	typedef float real;


	#define DELTA 0.005

	/*#ifndef real
		#define real float
	#endif
	*/

	#ifndef M_PI	// algunos math.h no lo definen, como el de visual studio.
		#define M_PI 3.14159265358979323846264338327950288419716939937510f
	#endif

	#ifndef _MAX_MINS_
		#define _MAX_MINS_
		#define REAL_MAX FLT_MAX	//Máximo valor positivo
		#define REAL_MIN FLT_MIN	// Mínimo valor POSITIVO
	#endif

	#ifndef radianes
		#define radian(angulo) (angulo * M_PI  / 180.0f)
		#define grado(angulo) (angulo * 180.0f / M_PI)
	#endif
	
	class matriz_2x2;
	class matriz_4x4;
	class matriz_3x3;
	class cuaternion;
	class vector3;
	class vector2;


	//typedef float real;
	typedef matriz_4x4 matriz;
	typedef vector3 vector;
	typedef cuaternion vector4;
	//typedef vector3 vector2;
	//typedef matriz_3x3 matriz_2x2;
	

	typedef struct {
		union {
			real m[2];
			struct {
				real u, v;
			};
			struct {
				real x, y;
			};
		};
	} texel;
	
	class vector2 {
	public:
		union {
			struct {
				real x, y;
			};
			real m[2];
		};

	public:
		friend const vector2 operator*(real op1, const vector2 &op2) {return(vector2(op1 * op2.x, op1 * op2.y));}

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
		void operator /=(const real &op1);
		
		
		const vector2 operator /(const real &op1) const;
		const vector2 operator *(const real &op1) const;
		
		const real modulo() const;
		void normalizar();
		const vector2 normalizado() const;
		
		void perpendicularizar();
		const vector2 perpendicularizado() const;
		
		const real perpDotProduct(const vector2 &op1) const;
		const real productoEscalar(const vector2 &op1) const;
		
		operator real *() const;
		
	};


	class vector3 {
		friend const vector3 operator*(real op1, const vector3 &op2) {
        	        return(vector3(op1 * op2.x, op1 * op2.y, op1 * op2.z));
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
		const vector3 operator/(real op2) const;
		void operator += (const vector3 &op2);
		void operator -= (const vector3 &op2);
		void operator*=(real op2);
		void operator/=(real op2);
		
		const real ProductoEscalar(const vector3 &op2) const;
		const real operator*(const vector3 &op2) const;

		const vector3 ProductoVectorial(const vector3 &op2) const;
		const vector3 operator^(const vector3 &op2) const;

		operator real *() const;
		const real operator() (int indice) const;
		const real Modulo(void) const;
		char *Print(char *adonde) const;

		unsigned char Esta_normalizado(void) const;
		const real Normalizar(void);
		const vector3 Normalizado(void) const;

		const matriz_3x3 Star() const;
		/*operator D3DXVECTOR3();*/
	};
	
	class matriz_2x2 {
	protected:
		union {
			struct {
				real _00, _01;
				real _10, _11;
			};
			real m[4];
		};
	public:
		matriz_2x2();
		matriz_2x2(real _00, real _01, real _10, real _11);
		matriz_2x2(real angulo);
		matriz_2x2(const matriz_2x2 &op1);
		
		const matriz_2x2 operator +(const matriz_2x2 &op1) const;
		const matriz_2x2 operator -(const matriz_2x2 &op1) const;
		const matriz_2x2 operator *(const matriz_2x2 &op1) const;
				
		const matriz_2x2 operator *(const real &op1) const;
		const matriz_2x2 operator /(const real &op1) const;
		
		const vector2 operator *(const vector2 &op1) const;
		
		void operator +=(const matriz_2x2 &op1);
		void operator -=(const matriz_2x2 &op1);
		void operator *=(const matriz_2x2 &op1);
		
		void operator *=(const real &op1);
		void operator /=(const real &op1);
		
		void transponer();
		const matriz_2x2 transpuesta() const;
		
		void ortoNormalizar();
		const matriz_2x2 ortoNormalizada() const;
		
		unsigned char esSingular() const;
		
		const real determinante() const;
		
		unsigned char invertir();
		const matriz_2x2 inversa() const;
		
		void hacerRotacion(real angulo);
		void hacerZoom(real x, real y);
		
		void cargarIdentidad();
	};

	class matriz_3x3 {
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
		static matriz_3x3 Identidad;

		matriz_3x3(	real d00, real d01, real d02,
				real d10, real d11, real d12, 
				real d20, real d21, real d22);
		matriz_3x3(const matriz_3x3 &op1);
		matriz_3x3(void );
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
		void operator*= (const matriz_3x3 &op2);
		void operator*=(real op2);
		void operator+=(const matriz_3x3 &op2);
		void operator-=(const matriz_3x3 &op2);
		operator real *() const;
		operator matriz_4x4 () const;
		
		const matriz_3x3 HacerRotacion(real x, real y, real z);
		const matriz_3x3 HacerRotacion(const vector3 &angulos);
		const matriz_3x3 HacerRotacion(real angulo, real x, real y, real z); // el eje debe estar normalizado
		const matriz_3x3 HacerRotacion(real angulo, const vector3 &eje); // el eje debe estar normalizado
		
		const matriz_3x3 HacerZoom(real x, real y, real z);
		const matriz_3x3 HacerZoom(const vector3 &zoom);
		
		void Transponer(void);
		const matriz_3x3 Transpuesta(void) const;
		
		void Ortonormalizar(void);
		const matriz_3x3 Ortonormalizada(void) const;
		
		unsigned char EsSingular(void);
		const real Determinante(void);
		unsigned char Invertir(void);
		const matriz_3x3 Inversa(void) const;

		void CargarIdentidad(void);
		void Star(const vector3 &op1);
		char *Print(char *adonde) const;
		const vector3 Fila(int nro) const;
		const vector3 Columna(int nro) const;
		const vector3 operator() (int nro) const;
		real &operator()(int fila, int columna);
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
			
			const real Modulo() const;
			
			void Conjugar(void);
			const cuaternion Conjugado(void) const;

			void Invertir(void);
			const cuaternion Invertido(void) const;

			void Normalizar(void);
			const cuaternion Normalizado(void) const;

			void Slerp(const cuaternion &dedonde, const cuaternion &adonde, real t);
			
			const cuaternion HacerRotacion(real angulo, const vector3 &eje);
			const cuaternion HacerRotacion(real x, real y, real z);
			const cuaternion HacerRotacion(const vector3 &angulos);
			
			operator matriz_4x4() const;
			operator matriz_3x3() const;
			const cuaternion AnguloEje(void) const;
			const real operator()(int nro) const;
	};
	
	class matriz_4x4 {
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
			static matriz_4x4 Identidad;
			
			matriz_4x4(	real _00, real _01, real _02, real _03,
						  real _10, real _11, real _12, real _13,
						  real _20, real _21, real _22, real _23,
						  real _30, real _31, real _32, real _33);

			matriz_4x4(const matriz_4x4 &op1);
			matriz_4x4(void);
			matriz_4x4(real x, real y, real z);
			matriz_4x4(const vector3 &op2);
			matriz_4x4(real angulo, real x, real y, real z);
			matriz_4x4(real angulo, const vector3 &eje);
			matriz_4x4(const matriz_3x3 &orientacion, const vector3 &posicion);

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
			matriz_4x4 operator =(const matriz_3x3 &op2);
			
			operator real *() const;
			operator matriz_3x3 () const;
			
			void Trasponer(void);
			const matriz_4x4 Traspuesta(void) const;

			unsigned char EsSingular(void);
			const real Determinante(void);
			unsigned char Invertir(void);
			const matriz_4x4 Inversa(void) const;


			void InvertirTransformacion(void);
			void CargarIdentidad(void);
			/*operator D3DXMATRIX ();*/ 
			const matriz_4x4 HacerTraslacion(const vector3 &desplazamiento);
			const matriz_4x4 HacerTraslacion(real x, real y, real z);
			
			const matriz_4x4 HacerRotacion(real x, real y, real z);
			const matriz_4x4 HacerRotacion(const vector3 &angulos);
			const matriz_4x4 HacerRotacion(real angulo, const vector3 &eje);
			const matriz_4x4 HacerRotacion(real angulo, real x, real y, real z);
			const matriz_4x4 HacerRotacion(const matriz_3x3 &orientacion);

			const matriz_4x4 HacerZoom(real x, real y, real z);
			const matriz_4x4 HacerZoom(const vector3 &zoom);

			char *Print(char *adonde) const;
			const cuaternion Fila(int nro) const;
			const cuaternion Columna(int nro) const;
			const cuaternion operator() (int nro) const;
			real &operator()(int fila, int columna);
	};


/*matriz_3x3 matriz_3x3::Identidad(     1.0f, 0.0f, 0.0f,
										0.0f, 1.0f, 0.0f,
										0.0f, 0.0f, 1.0f);

matriz_4x4 matriz_4x4::Identidad(	1.0f, 0.0f, 0.0f, 0.0f,
									   0.0f, 1.0f, 0.0f, 0.0f,
									   0.0f, 0.0f, 1.0f, 0.0f,
									   0.0f, 0.0f, 0.0f, 1.0f);
*/


#endif
