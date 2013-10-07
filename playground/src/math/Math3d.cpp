#include <string.h>
#include "math3d.h"

//  / / - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// | |  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// | |									Atributos de Clase públicos
// | | - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  \ \  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

matriz_2x2 matriz_2x2::Identidad(1.0f, 0.0f, 0.0f, 1.0f);

matriz_3x3 matriz_3x3::Identidad(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		1.0f);

matriz_4x4 matriz_4x4::Identidad(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

//  / / - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// | |  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// | |									Funciones de la Clase VECTOR2
// | | - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  \ \  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

vector2::vector2() {
	*this = vector2(0.0f, 0.0f);
}
vector2::vector2(real x, real y) {
	this->x = x;
	this->y = y;
}
//vector2::vector2(const vector2 &op1) {
//	*this = op1;
//}

const vector2 vector2::operator +(const vector2 &op1) const {
	return (vector2(this->x + op1.x, this->y + op1.y));
}
const vector2 vector2::operator -(const vector2 &op1) const {
	return (vector2(this->x - op1.x, this->y - op1.y));
}

const vector2 vector2::operator -(void) const {
	return (vector2(-this->x, -this->y));
}

const real vector2::operator *(const vector2 &op1) const {
	return (this->productoEscalar(op1));
}

void vector2::operator +=(const vector2 &op1) {
	this->x += op1.x;
	this->y += op1.y;
}
void vector2::operator -=(const vector2 &op1) {
	this->x -= op1.x;
	this->y -= op1.y;
}
void vector2::operator *=(const real &op1) {
	this->x *= op1;
	this->y *= op1;
}

const vector2 vector2::operator *(const real &op1) const {
	return (vector2(this->x * op1, this->y * op1));
}

const real vector2::modulo() const {
	return (sqrt(this->x * this->x + this->y * this->y));
}

//
//void vector2::normalizar() {
//	real modulo = this->modulo();
//	if(modulo == 0.0f)
//		throw InvalidStateException("Vector module is 0");
//
//	*this = *this * (1.0 / modulo);
//}
//const vector2 vector2::normalizado() const {
//	real modulo = this->modulo();
//	if(modulo == 0.0f)
//		throw InvalidStateException("Vector module is 0");
//
//	return(*this * (1.0 / modulo));
//}
//
//void vector2::perpendicularizar() {
//	real temp = -this->y;
//	this->y = this->x;
//	this->x = temp;
//}
//const vector2 vector2::perpendicularizado() const {
//	return(vector2(-this->y, this->x));
//}
//
//const real vector2::perpDotProduct(const vector2 &op1) const {
//	return(this->perpendicularizado() * op1);
//}
//const real vector2::productoEscalar(const vector2 &op1) const {
//	return(this->x * op1.x + this->y * op1.y);
//}

vector2::operator real *() const {
	return ((real *) this->m);
}

//  / / - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// | |  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// | |									Funciones de la Clase VECTOR3
// | | - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  \ \  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

vector3::vector3(real c_x, real c_y, real c_z) {
	this->x = c_x;
	this->y = c_y;
	this->z = c_z;
}
vector3::vector3(const cuaternion &quat) { // iguala el vector3 a los miembros x, y y z del cuaternion
	this->x = quat.x;
	this->y = quat.y;
	this->z = quat.z;
}
vector3::vector3() {
}

vector3::vector3(const vector3 &op1) {  //constructor de copia
	memcpy(this->m, op1.m, sizeof(this->m));
}

unsigned char vector3::operator==(const vector3 &op2) const {
	return ((x == op2.x && y == op2.y && z == op2.z));
}

unsigned char vector3::operator!=(const vector3 &op2) const {
	return ((x != op2.x || y != op2.y || z != op2.z));
}

const vector3 vector3::operator+(const vector3 &op2) const {
	return (vector3(this->x + op2.x, this->y + op2.y, this->z + op2.z));
}

const vector3 vector3::operator-(const vector3 &op2) const {
	return (vector3(this->x - op2.x, this->y - op2.y, this->z - op2.z));
}
const vector3 vector3::operator-(void) const {
	return (vector3(-this->x, -this->y, -this->z));
}

const vector3 vector3::operator*(real op2) const {
	return (vector3(this->x * op2, this->y * op2, this->z * op2));
}

void vector3::operator +=(const vector3 &op2) {
	this->x += op2.x;
	this->y += op2.y;
	this->z += op2.z;
}

void vector3::operator -=(const vector3 &op2) {
	this->x -= op2.x;
	this->y -= op2.y;
	this->z -= op2.z;
}

void vector3::operator*=(real op2) {
	this->x *= op2;
	this->y *= op2;
	this->z *= op2;
}

const real vector3::operator*(const vector3 &op2) const { // Producto Escalar de vector3es
	return (this->productoEscalar(op2));
}

const vector3 vector3::operator^(const vector3 &op2) const { // Producto Vectorial
	return (productoVectorial(op2));
}

vector3::operator real *() const {
	return ((real *) this->m);
}

//	const real vector3::operator()(int indice) const{
//		return(this->m[indice]);
//	}

const real vector3::modulo(void) const { // Devuelve el módulo del vector3
	return ((real) sqrt(
			(this->x * this->x) + (this->y * this->y) + (this->z * this->z)));
}
//
//	unsigned char vector3::Esta_normalizado(void) const { // Verifica si el módulo del vector3 es uno.
//		return(((this->x * this->x) + (this->y * this->y) + (this->z * this->z)) == 1.0f);
//	}
//
//	const real vector3::Normalizar(void) {		// No se pueden normalizar vector3es cuyo módulo es cero, en caso de intentarse se generará un error de divide by zero.
//		real modulo = this->Modulo();
//
//		if(modulo == 0.0f)
//			throw InvalidStateException("Vector module is 0");
//
//		*this *= (1.0f / modulo);
//
//		return(modulo);
//	}
//
//	const vector3 vector3::Normalizado(void) const {
//		real modulo = this->Modulo();
//
//		if(modulo == 0.0f)
//			throw InvalidStateException("Vector module is 0");
//
//		real oneOverModule = 1.0f / modulo;
//		return(vector3(this->x * oneOverModule, this->y * oneOverModule, this->z * oneOverModule));
//	}
//
//	const matriz_3x3 vector3::Star() const { // devuelve la matriz  "r estrella" con la cual se puede realizar el producto vector3ial.
//		return( matriz_3x3(0.0f,    -this->z,  this->y,
//						   this->z,   0.0f,    -this->x,
//						   -this->y,  this->x,  0.0f));
//	}

//  / / - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// | |  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// | |									Funciones de la MATRIZ_3X3
// | | - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  \ \  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//matriz_2x2::matriz_2x2() :
//		matriz_mxn(2, 2) {
//	this->_00 = 1.0f;
//	this->_01 = 0.0f;
//	this->_10 = 0.0f;
//	this->_11 = 1.0f;
//}
//
const matriz_2x2 matriz_2x2::operator +(const matriz_2x2 &op1) const {
	return (matriz_2x2(this->_00 + op1._00, this->_01 + op1._01,
			this->_10 + op1._10, this->_11 + op1._11));
}
const matriz_2x2 matriz_2x2::operator -(const matriz_2x2 &op1) const {
	return (matriz_2x2(this->_00 - op1._00, this->_01 - op1._01,
			this->_10 - op1._10, this->_11 - op1._11));
}
const matriz_2x2 matriz_2x2::operator *(const matriz_2x2 &op1) const {
	return (matriz_2x2(this->_00 * op1._00 + this->_01 * op1._10,
			this->_00 * op1._01 + this->_01 * op1._11,
			this->_10 * op1._00 + this->_11 * op1._10,
			this->_10 * op1._01 + this->_11 * op1._11));
}

const matriz_2x2 matriz_2x2::operator *(const real &op1) const {
	return (matriz_2x2(this->_00 * op1, this->_01 * op1, this->_10 * op1,
			this->_11 * op1));
}

const vector2 matriz_2x2::operator *(const vector2 &op1) const {
	return (vector2(this->_00 * op1.x + this->_01 * op1.y,
			this->_10 * op1.x + this->_11 * op1.y));
}

void matriz_2x2::operator +=(const matriz_2x2 &op1) {
	this->_00 += op1._00;
	this->_01 += op1._01;
	this->_10 += op1._10;
	this->_11 += op1._11;
}
void matriz_2x2::operator -=(const matriz_2x2 &op1) {
	this->_00 -= op1._00;
	this->_01 -= op1._01;
	this->_10 -= op1._10;
	this->_11 -= op1._11;
}
void matriz_2x2::operator *=(const matriz_2x2 &op1) {
	*this = *this * op1;
}

void matriz_2x2::operator *=(const real &op1) {
	this->_00 *= op1;
	this->_01 *= op1;
	this->_10 *= op1;
	this->_11 *= op1;
}

//void matriz_2x2::transponer() {
//	*this = matriz_2x2(	this->_00, this->_10,
//						this->_01, this->_11);
//}
//const matriz_2x2 matriz_2x2::transpuesta() const {
//	return(matriz_2x2(	this->_00, this->_10,
//						this->_01, this->_11));
//}
//
//void matriz_2x2::ortoNormalizar() {
//}
//const matriz_2x2 matriz_2x2::ortoNormalizada() const {
//	return(*this);
//}
//
//unsigned char matriz_2x2::esSingular() const {
//	return(false);
//}
//
//const real matriz_2x2::determinante() const {
//	return(0.0f);
//}
//
//unsigned char matriz_2x2::invertir() {
//	return(false);
//}
//const matriz_2x2 matriz_2x2::inversa() const {
//	return(*this);
//}
//
//void matriz_2x2::hacerRotacion(real angulo) {
//	real coseno = (real)cos(angulo), seno = (real)sin(angulo);
//
//	this->_00 = coseno; this->_01 = -seno;
//	this->_10 = seno; this->_11 = coseno;
//}
//void matriz_2x2::hacerZoom(real x, real y) {
//	this->_00 = x;
//	this->_11 = y;
//}
//
//void matriz_2x2::cargarIdentidad() {
//	this->_00 = 1.0f; this->_01 = 0.0f;
//	this->_10 = 0.0f; this->_11 = 1.0f;
//
//}

//  / / - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// | |  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// | |									Funciones de la MATRIZ_3X3
// | | - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  \ \  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//	matriz_3x3::matriz_3x3(real x, real y, real z) { //Rotación de Angulos de Euler
//		this->HacerRotacion(x, y, z);
//	}
//
//	matriz_3x3::matriz_3x3(const vector3 &angulos) { //Rotación de Angulos de Euler
//		this->HacerRotacion(angulos);
//	}
//
//	matriz_3x3::matriz_3x3(const vector3 &vec0, const vector3 &vec1, const vector3 &vec2) { // Construye la matriz con los vector3es como columnas
//		this->_00 = vec0.x; this->_01 = vec1.x; this->_02 = vec2.x;
//		this->_10 = vec0.y; this->_11 = vec1.y; this->_12 = vec2.y;
//		this->_20 = vec0.z; this->_21 = vec1.z; this->_22 = vec2.z;
//	}
//
//	matriz_3x3::matriz_3x3(real angulo, real x, real y, real z) { // Rotación de angulo-eje(x, y, z)
//		this->HacerRotacion(angulo, x, y, z);
//	}
//	matriz_3x3::matriz_3x3(real angulo, const vector3 &eje) { // Rotación angulo-eje
//		this->HacerRotacion(angulo, eje);
//	}

//matriz_3x3::matriz_3x3(const matriz_4x4 &op1) :
//		matriz_mxn(3, 3) {// si no lo dejo, no funciona el operador (matriz_4x4)matriz_3x3. Da error de compilación por ambigüedad en la línea que llama a la función.
//	this->_00 = op1._00;
//	this->_01 = op1._01;
//	this->_02 = op1._02;
//	this->_10 = op1._10;
//	this->_11 = op1._11;
//	this->_12 = op1._12;
//	this->_20 = op1._20;
//	this->_21 = op1._21;
//	this->_22 = op1._22;
//}

matriz_3x3 matriz_3x3::operator=(const matriz_3x3 &op1) {
	memcpy(this->m, op1.m, sizeof(this->m));

	return (*this);
}

unsigned char matriz_3x3::operator==(const matriz_3x3 &op2) const {
	return ((_00 == op2._00 && _01 == op2._01 && _02 == op2._02
			&& _10 == op2._10 && _11 == op2._11 && _12 == op2._12
			&& _20 == op2._20 && _21 == op2._21 && _22 == op2._22));
}

unsigned char matriz_3x3::operator!=(const matriz_3x3 &op2) const {
	return ((_00 != op2._00 || _01 != op2._01 || _02 != op2._02
			|| _10 != op2._10 || _11 != op2._11 || _12 != op2._12
			|| _20 != op2._20 || _21 != op2._21 || _22 != op2._22));
}

const matriz_3x3 matriz_3x3::operator*(const matriz_3x3 &op2) const {
	return (matriz_3x3(
			(this->_00 * op2._00) + (this->_01 * op2._10)
					+ (this->_02 * op2._20),
			(this->_00 * op2._01) + (this->_01 * op2._11)
					+ (this->_02 * op2._21),
			(this->_00 * op2._02) + (this->_01 * op2._12)
					+ (this->_02 * op2._22),

			(this->_10 * op2._00) + (this->_11 * op2._10)
					+ (this->_12 * op2._20),
			(this->_10 * op2._01) + (this->_11 * op2._11)
					+ (this->_12 * op2._21),
			(this->_10 * op2._02) + (this->_11 * op2._12)
					+ (this->_12 * op2._22),

			(this->_20 * op2._00) + (this->_21 * op2._10)
					+ (this->_22 * op2._20),
			(this->_20 * op2._01) + (this->_21 * op2._11)
					+ (this->_22 * op2._21),
			(this->_20 * op2._02) + (this->_21 * op2._12)
					+ (this->_22 * op2._22)));
}

const matriz_3x3 matriz_3x3::operator*(real op2) const {
	return (matriz_3x3(_00 * op2, _01 * op2, _02 * op2, _10 * op2, _11 * op2,
			_12 * op2, _20 * op2, _21 * op2, _22 * op2));
}

const matriz_3x3 matriz_3x3::operator+(const matriz_3x3 &op2) const {
	return (matriz_3x3(_00 + op2._00, _01 + op2._01, _02 + op2._02,
			_10 + op2._10, _11 + op2._11, _12 + op2._12, _20 + op2._20,
			_21 + op2._21, _22 + op2._22));
}

const matriz_3x3 matriz_3x3::operator-(const matriz_3x3 &op2) const {
	return (matriz_3x3(_00 - op2._00, _01 - op2._01, _02 - op2._02,
			_10 - op2._10, _11 - op2._11, _12 - op2._12, _20 - op2._20,
			_21 - op2._21, _22 - op2._22));
}

void matriz_3x3::operator*=(const matriz_3x3 &op2) {
	*this = *this * op2;
}

void matriz_3x3::operator*=(real op2) {
	*this = *this * op2;
}

void matriz_3x3::operator+=(const matriz_3x3 &op2) {
	*this = *this + op2;
}

void matriz_3x3::operator-=(const matriz_3x3 &op2) {
	*this = *this - op2;
}

matriz_3x3::operator real *() const {
	return ((real *) m);
}

//matriz_3x3::operator matriz_4x4() const {
//	return (matriz_4x4(_00, _01, _02, 0.0, _10, _11, _12, 0.0, _20, _21, _22,
//			0.0, 0.0, 0.0, 0.0, 1.0));
//}

//	const matriz_3x3 matriz_3x3::HacerRotacion(real x, real y, real z) { //Construye una rotación en ángulos [radianes] de Euler
//		real cx = (real)cos(x), sx = (real)sin(x);
//		real cy = (real)cos(y), sy = (real)sin(y);
//		real cz = (real)cos(z), sz = (real)sin(z);
//
//		this->_00 = (cy * cz);	this->_01 = (sx * sy * cz - cx * sz); 	this->_02 = (cx * sy * cz + sx * sz);
//		this->_10 = (cy * sz); 	this->_11 = (sx * sy * sz + cx * cz); 	this->_12 = (cx * sy * sz - sx * cz);
//		this->_20 = -sy;  	this->_21 = (sx * cy); 			this->_22 = (cx * cy);
//
//		return(*this);
//	}
//
//	const matriz_3x3 matriz_3x3::HacerRotacion(const vector3 &angulos) { // Construye una rotación en ángulos [radianes] de Euler
//		real cx = (real)cos(angulos.x), sx = (real)sin(angulos.x);
//		real cy = (real)cos(angulos.y), sy = (real)sin(angulos.y);
//		real cz = (real)cos(angulos.z), sz = (real)sin(angulos.z);
//
//		this->_00 = (cy * cz);	this->_01 = (sx * sy * cz - cx * sz); 	this->_02 = (cx * sy * cz + sx * sz);
//		this->_10 = (cy * sz); 	this->_11 = (sx * sy * sz + cx * cz); 	this->_12 = (cx * sy * sz - sx * cz);
//		this->_20 = -sy;  	this->_21 = (sx * cy); 			this->_22 = (cx * cy);
//
//		return(*this);
//	}
//
//	const matriz_3x3 matriz_3x3::HacerRotacion(real angulo, real x, real y, real z) { // Construye una rotación angulo [ra dianes] -eje(x, y, z) (debe estar normalizado)
//		real c, s, umc, nxny1cos, nxnz1cos, nynz1cos;
//		c = (real)cos(angulo);
//		s = (real)sin(angulo);
//		umc = (real)(1.0 - c); // umc = Uno Menos Coseno
//
//		nxny1cos = x * y * umc;
//		nxnz1cos = x * z * umc;
//		nynz1cos = y * z * umc;
//
//		this->_00 = (x * x * umc + c);	this->_01 = nxny1cos - s * z;	this->_02 = nxnz1cos + s * y;
//		this->_10 = nxny1cos + s * z;	this->_11 = (y * y * umc) + c;  this->_12 = nynz1cos - s * x;
//		this->_20 = nxnz1cos - s * y;	this->_21 = nynz1cos + s * x;       this->_22 = z * z * umc + c;
//
//		return(*this);
//	}
//
//	const matriz_3x3 matriz_3x3::HacerRotacion(real angulo, const vector3 &eje) { // Construye una rotación angulo [radianes] - eje (debe estar normalizado)
//		return(this->HacerRotacion(angulo, eje.x, eje.y, eje.z));
//	}
//
//	const matriz_3x3 matriz_3x3::HacerZoom(real x, real y, real z) { // Construye una matriz de escala
//		this->_00 = x;
//		this->_11 = y;
//		this->_22 = z;
//
//		return(*this);
//	}
//
//
//	void matriz_3x3::Transponer(void) { // Transpone la matriz
//		const matriz_3x3 temp = *this;
//
//		this->_01 = temp._10; this->_02 = temp._20;
//		this->_10 = temp._01; this->_12 = temp._21;
//		this->_20 = temp._02; this->_21 = temp._12;
//	}
//
//	const matriz_3x3 matriz_3x3::Transpuesta(void) const { //Devuelve una matriz que esla transpuesta a la cual se le aplicó la operación
//		return(matriz_3x3(this->_00, this->_10, this->_20,
//							this->_01, this->_11, this->_21,
//							this->_02, this->_12, this->_22)
//		);
//	}
//
//	void matriz_3x3::Ortonormalizar(void) { //Verifica que los ejes formados por los vector3es columna de la matriz sean perpendiculares entre sí
//		vector3 vec1(this->_00, this->_10, this->_20);
//		vector3 vec2(this->_01, this->_11, this->_21);
//		vector3 vec3;
//
//		vec1.Normalizar();
//
//		vec2 = (vec2 - ((vec2 * vec1) * vec1));
//		vec2.Normalizar();
//
//		vec3 = vec1 ^ vec2;
//
//		this->_00 = vec1.x; this->_10 = vec1.y; this->_20 = vec1.z;
//		this->_01 = vec2.x; this->_11 = vec2.y; this->_21 = vec2.z;
//		this->_02 = vec3.x; this->_12 = vec3.y; this->_22 = vec3.z;
//	}
//
//	const matriz_3x3 matriz_3x3::Ortonormalizada(void) const { // Devuelve una matriz que es la ortonormalizada de la matriz a la cual se le aplicó la operación
//		vector3 vec1(this->_00, this->_10, this->_20);
//		vector3 vec2(this->_01, this->_11, this->_21);
//		vector3 vec3;
//
//		vec1.Normalizar();
//
//		vec2 = (vec2 - ((vec2 * vec1) * vec1));
//		vec2.Normalizar();
//
//		vec3 = vec1 ^ vec2;
//
//		return(matriz_3x3(vec1.x, vec1.y, vec1.z,
//							vec2.x, vec2.y, vec2.z,
//							vec3.x, vec3.y, vec3.z));
//	}
//
//	unsigned char matriz_3x3::EsSingular(void) { // Determina si es singular (o sea no inversible);
//		return(this->Determinante() == 0.0f);
//	}
//
//	const real matriz_3x3::Determinante(void) { //Calcula el determinante de la matriz.
//		return(	  this->_00 * this->_11 * this->_22 +
//					this->_02 * this->_10 * this->_21 +
//					this->_01 * this->_12 * this->_20 -
//					this->_02 * this->_11 * this->_20 -
//					this->_12 * this->_21 * this->_00 -
//					this->_01 * this->_10 * this->_22);
//	}
//
//	unsigned char matriz_3x3::Invertir(void) { //Invierte la matriz mediante el determinante.
//		real det = this->Determinante();
//		if(det == 0.0f)  	return(false);
//		else {
//			det = 1.0f / det;
//
//			*this = matriz_3x3(
//				(this->_11 * this->_22 - this->_12 * this->_21) * det, (-this->_01 * this->_22 + this->_02 * this->_21) * det , (this->_01 * this->_12 - this->_11 * this->_02) * det,
//
//				(-this->_10 * this->_22 + this->_12 * this->_20) * det, (this->_00 * this->_22 - this->_02 * this->_20) * det, (-this->_00 * this->_12 + this->_02 * this->_10) * det,
//
//				(this->_10 * this->_21 - this->_20 * this->_11) * det,  (-this->_00 * this->_21 + this->_01 * this->_20) * det, (this->_00 * this->_11 - this->_10 * this->_01) * det
//			 );
//		 }
//		 return(true);
//	}
//
//	const matriz_3x3 matriz_3x3::Inversa(void) const { //Devuelve una matriz que es la inversa de la matriz a la cual se le aplicó la operación
//		matriz_3x3 temp(*this);
//		if(temp.Invertir()) return(temp);
//		return(*this);
//	}
//
//	void matriz_3x3::CargarIdentidad(void) { //Iguala la matriz a la identidad en espacio R3
//			this->_00 = 1.0; this->_01 = 0.0; this->_02 = 0.0;
//			this->_10 = 0.0; this->_11 = 1.0; this->_12 = 0.0;
//			this->_20 = 0.0; this->_21 = 0.0; this->_22 = 1.0;
//	}
//	void matriz_3x3::Star(const vector3 &op1) { //Iguala la matriz a la matriz op1* (op1 star)
//		this->_00 = 0.0f; this->_01 = -op1.z; this->_02 = op1.y;
//		this->_10 = op1.z; this->_11 = 0.0f; this->_12 = -op1.x;
//		this->_20 = -op1.y; this->_21 = op1.x; this->_22 = 0.0f;
//	}

//  / / - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// | |  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// | |									Funciones FRIEND de las clases  MATRIZ_3X3 y VECTOR
// | | - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  \ \  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const matriz_3x3 operator *(real op1, const matriz_3x3 &op2) {
	return (matriz_3x3(op2._00 * op1, op2._01 * op1, op2._02 * op1,
			op2._10 * op1, op2._11 * op1, op2._12 * op1, op2._20 * op1,
			op2._21 * op1, op2._22 * op1));
}

const vector3 operator*(const vector3 &op1, const matriz_3x3 &op2) {
	return (vector3((op1.x * op2._00) + (op1.y * op2._10) + (op1.z * op2._20),
			(op1.x * op2._01) + (op1.y * op2._11) + (op1.z * op2._21),
			(op1.x * op2._02) + (op1.y * op2._12) + (op1.z * op2._22)));
}

const vector3 operator*(const matriz_3x3 &op1, const vector3 &op2) {
	return (vector3((op1._00 * op2.x) + (op1._01 * op2.y) + (op1._02 * op2.z),
			(op1._10 * op2.x) + (op1._11 * op2.y) + (op1._12 * op2.z),
			(op1._20 * op2.x) + (op1._21 * op2.y) + (op1._22 * op2.z)));
}

//  / / - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// | |  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// | |									Funciones de la Clase MATRIZ_4x4
// | | - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  \ \  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//	matriz_4x4::matriz_4x4(real x, real y, real z) { // Rotacion Angulos (radianes) de Euler
//		this->HacerRotacion(x, y, z);
//					                     this->_03 = 0;
//					                     this->_13 = 0;
//					                     this->_23 = 0;
//		this->_30 = 0; this->_31 = 0; this->_32 = 0; this->_33 = 1;
//	}
//
//	matriz_4x4::matriz_4x4(const vector3 &op2) { //Rotación en angulos (op2.x, op2.y, op2.z) (radianes) de Euler.
//		this->HacerRotacion(op2.x, op2.y, op2.z);
//					                     this->_03 = 0;
//					                     this->_13 = 0;
//					                     this->_23 = 0;
//		this->_30 = 0; this->_31 = 0; this->_32 = 0; this->_33 = 1;
//	}
//
//	matriz_4x4::matriz_4x4(const matriz_3x3 &orientacion, const vector3 &posicion) { //Construye una transformación en sistema de coordenadas ¿mano izquierda? mediante la matriz de 3x3 de orientacion  y el vector3 de posicion.
//		this->_00 = orientacion._00; this->_01 = orientacion._01; this->_02 = orientacion._02; this->_03 = posicion.x;
//		this->_10 = orientacion._10; this->_11 = orientacion._11; this->_12 = orientacion._12; this->_13 = posicion.y;
//		this->_20 = orientacion._20; this->_21 = orientacion._21; this->_22 = orientacion._22; this->_23 = posicion.z;
//		this->_30 = 0.0f; this->_31 = 0.0f; this->_32 = 0.0f; this->_33 = 1.0f;
//		//*this = orientacion;
//		//this->HacerTraslacion(posicion.x, posicion.y, posicion.z);
//		//this->_30 = 0;            this->_31 = 0;           this->_32 = 0;              this->_33 = 1.0;
//	}

matriz_4x4 matriz_4x4::operator=(const matriz_4x4 &op1) {
	memcpy(this->m, op1.m, sizeof(this->m));

	return (*this);
}

const matriz_4x4 matriz_4x4::operator*(const matriz_4x4 &op2) const {
	return (matriz_4x4(
			(this->_00 * op2._00) + (this->_01 * op2._10)
					+ (this->_02 * op2._20) + (this->_03 * op2._30),
			(this->_00 * op2._01) + (this->_01 * op2._11)
					+ (this->_02 * op2._21) + (this->_03 * op2._31),
			(this->_00 * op2._02) + (this->_01 * op2._12)
					+ (this->_02 * op2._22) + (this->_03 * op2._32),
			(this->_00 * op2._03) + (this->_01 * op2._13)
					+ (this->_02 * op2._23) + (this->_03 * op2._33),

			(this->_10 * op2._00) + (this->_11 * op2._10)
					+ (this->_12 * op2._20) + (this->_13 * op2._30),
			(this->_10 * op2._01) + (this->_11 * op2._11)
					+ (this->_12 * op2._21) + (this->_13 * op2._31),
			(this->_10 * op2._02) + (this->_11 * op2._12)
					+ (this->_12 * op2._22) + (this->_13 * op2._32),
			(this->_10 * op2._03) + (this->_11 * op2._13)
					+ (this->_12 * op2._23) + (this->_13 * op2._33),

			(this->_20 * op2._00) + (this->_21 * op2._10)
					+ (this->_22 * op2._20) + (this->_23 * op2._30),
			(this->_20 * op2._01) + (this->_21 * op2._11)
					+ (this->_22 * op2._21) + (this->_23 * op2._31),
			(this->_20 * op2._02) + (this->_21 * op2._12)
					+ (this->_22 * op2._22) + (this->_23 * op2._32),
			(this->_20 * op2._03) + (this->_21 * op2._13)
					+ (this->_22 * op2._23) + (this->_23 * op2._33),

			(this->_30 * op2._00) + (this->_31 * op2._10)
					+ (this->_32 * op2._20) + (this->_33 * op2._30),
			(this->_30 * op2._01) + (this->_31 * op2._11)
					+ (this->_32 * op2._21) + (this->_33 * op2._31),
			(this->_30 * op2._02) + (this->_31 * op2._12)
					+ (this->_32 * op2._22) + (this->_33 * op2._32),
			(this->_30 * op2._03) + (this->_31 * op2._13)
					+ (this->_32 * op2._23) + (this->_33 * op2._33)));
}

const matriz_4x4 matriz_4x4::operator *(real op2) const {
	return (matriz_4x4(_00 * op2, _01 * op2, _02 * op2, _03 * op2, _10 * op2,
			_11 * op2, _12 * op2, _13 * op2, _20 * op2, _21 * op2, _22 * op2,
			_23 * op2, _30 * op2, _31 * op2, _32 * op2, _33 * op2));
}

const vector3 matriz_4x4::operator*(const vector3 &op1) const {
	return (vector3(_00 * op1.x + _01 * op1.y + _02 * op1.z + _03,
			_10 * op1.x + _11 * op1.y + _12 * op1.z + _13,
			_20 * op1.x + _21 * op1.y + _22 * op1.z + _23));
}

const matriz_4x4 matriz_4x4::operator +(const matriz_4x4 &op2) const {
	return (matriz_4x4(_00 + op2._00, _01 + op2._01, _02 + op2._02,
			_03 + op2._03, _10 + op2._10, _11 + op2._11, _12 + op2._12,
			_13 + op2._13, _20 + op2._20, _21 + op2._21, _22 + op2._22,
			_23 + op2._23, _30 + op2._30, _31 + op2._31, _32 + op2._32,
			_33 + op2._33));
}

const matriz_4x4 matriz_4x4::operator -(const matriz_4x4 &op2) const {
	return (matriz_4x4(_00 - op2._00, _01 - op2._01, _02 - op2._02,
			_03 - op2._03, _10 - op2._10, _11 - op2._11, _12 - op2._12,
			_13 - op2._13, _20 - op2._20, _21 - op2._21, _22 - op2._22,
			_23 - op2._23, _30 - op2._30, _31 - op2._31, _32 - op2._32,
			_33 - op2._33));
}

void matriz_4x4::operator*=(const matriz_4x4 &op2) {
	*this = *this * op2;
}

void matriz_4x4::operator *=(real op2) {
	*this = *this * op2;
}

void matriz_4x4::operator +=(const matriz_4x4 &op2) {
	*this = *this + op2;
}

void matriz_4x4::operator -=(const matriz_4x4 &op2) {
	*this = *this - op2;
}

//matriz_4x4 matriz_4x4::operator =(const matriz_3x3 &op2) {
//	this->_00 = op2._00;
//	this->_01 = op2._01;
//	this->_02 = op2._02;
//	this->_10 = op2._10;
//	this->_11 = op2._11;
//	this->_12 = op2._12;
//	this->_20 = op2._20;
//	this->_21 = op2._21;
//	this->_22 = op2._22;
//
//	return (*this);
//}

matriz_4x4::operator real *() const {
	return ((real *) m);
}

//matriz_4x4::operator matriz_3x3() const {//Funciona solamente si dejo un constructor matriz_3x3(matriz_4x4 op1) en la clase matriz_3x3
//	return (matriz_3x3(_00, _01, _02, _10, _11, _12, _20, _21, _22));
//}
//	void matriz_4x4::Trasponer(void) { //Transpone la matriz
//		matriz_4x4 tmp = *this;
//
//							this->_01 = tmp._10;	this->_02 = tmp._20;	this->_03 = tmp._30;
//		this->_10 = tmp._01;						this->_12 = tmp._21;	this->_13 = tmp._31;
//		this->_20 = tmp._02;	this->_21 = tmp._12;						this->_23 = tmp._32;
//		this->_30 = tmp._03;	this->_31 = tmp._13;	this->_32 = tmp._23;
//
//	}
//	const matriz_4x4 matriz_4x4::Traspuesta(void) const { //Devuelve una matriz que es la transpuesta de aquella sobre la cual se aplicó la operación
//		return(matriz_4x4( this->_00, this->_10,	this->_20,	this->_30,
//							this->_01, this->_11, this->_21,	this->_31,
//							this->_02,	this->_12, this->_22, this->_32,
//							this->_03, this->_13,	this->_23, this->_33));
//	}
//
//	void matriz_4x4::InvertirTransformacion(void) { //Invierte la matriz asumiendo que esta es actualmente una transformación válida
//		matriz_4x4 tmp = *this;
//
//							this->_01 = tmp._10;	this->_02 = tmp._20;	this->_03 = - tmp._00 * tmp._03  - tmp._10 * tmp._13 - tmp._20 * tmp._23;
//		this->_10 = tmp._01;						this->_12 = tmp._21;	  this->_13 = - tmp._01 * tmp._03   - tmp._11 * tmp._13	 - tmp._21 * tmp._23;
//		this->_20 = tmp._02;	this->_21 = tmp._12;						this->_23 = - tmp._02 * tmp._03  - tmp._12 * tmp._13 - tmp._22 * tmp._23;
//																     this->_33 = 1.0f;
//	}
//
//	unsigned char matriz_4x4::EsSingular(void) { // verifica si la matriz es singular (o sea no invertible)
//		return(false);
//	}
//	const real matriz_4x4::Determinante(void) { // Calcula el determinante de la matriz
//		return(0.0f);
//	}
//	const matriz_4x4 matriz_4x4::Inversa(void) const { //Devuelve la matriz que es la inversa de aquella sobre la cual se aplicó la operación
//		return(*this);
//	}
//	unsigned char matriz_4x4::Invertir(void) { // Invierte la matriz.
//		return(true);
//	}
//
//	void matriz_4x4::CargarIdentidad(void) { // Iguala la matriz a la identidad en R4
//		this->_00 = 1.0; this->_01 = 0.0; this->_02 = 0.0; this->_03 = 0.0;
//		this->_10 = 0.0; this->_11 = 1.0; this->_12 = 0.0; this->_13 = 0.0;
//		this->_20 = 0.0; this->_21 = 0.0; this->_22 = 1.0; this->_23 = 0.0;
//		this->_30 = 0.0; this->_31 = 0.0; this->_32 = 0.0; this->_33 = 1.0;
//	}
//
//	const matriz_4x4 matriz_4x4::HacerTraslacion(const vector3 &desplazamiento) { // Iguala la matriz a una traslación de desplazamiento.x, desplazamiento.y y desplazamiento.z
//		this->_03 = desplazamiento.x;
//		this->_13  = desplazamiento.y;
//		this->_23 = desplazamiento.z;
//
//		return(*this);
//	}
//
//	const matriz_4x4 matriz_4x4::HacerTraslacion(real x, real y, real z) { // Iguala la matriz a una traslación de x, y y z
//		_03 = x;
//		_13 = y;
//		_23 = z;
//
//		return(*this);
//	}
//
//	const matriz_4x4 matriz_4x4::HacerRotacion(real x, real y, real z) { // Iguala la matriz a una rotación de angulos (radianes) de euler
//		real cx = (real)cos(x), sx = (real)sin(x);
//		real cy = (real)cos(y), sy = (real)sin(y);
//		real cz = (real)cos(z), sz = (real)sin(z);
//
//		this->_00 = (cy * cz);	this->_01 = (sx * sy * cz - cx * sz); 	this->_02 = (cx * sy * cz + sx * sz);
//		this->_10 = (cy * sz); 	this->_11 = (sx * sy * sz + cx * cz); 	this->_12 = (cx * sy * sz - sx * cz);
//		this->_20 = -sy;  	this->_21 = (sx * cy); 			this->_22 = (cx * cy);
//
//		return(*this);
//	}
//
//	const matriz_4x4 matriz_4x4::HacerRotacion(real angulo, real x, real y, real z) {// Rotación en angulo (radian) - eje (x, y, z) (debe estar normalizado)
//		real c, s, umc, nxny1cos, nxnz1cos, nynz1cos;
//		c = (real)cos(angulo);
//		s = (real)sin(angulo);
//		umc = (real)(1.0 - c); // umc = Uno Menos Coseno
//
//		nxny1cos = x * y * umc;
//		nxnz1cos = x * z * umc;
//		nynz1cos = y * z * umc;
//
//		this->_00 = (x * x * umc + c);	this->_01 = nxny1cos - s * z;	this->_02 = nxnz1cos + s * y;
//		this->_10 = nxny1cos + s * z;	this->_11 = (y * y * umc) + c;  this->_12 = nynz1cos - s * x;
//		this->_20 = nxnz1cos - s * y;	this->_21 = nynz1cos + s * x;       this->_22 = z * z * umc + c;
//
//		return(*this);
//	}
//
//	const matriz_4x4 matriz_4x4::HacerRotacion(real angulo, const vector3 &eje) {	// Rotación en ángulo (radianes) - Eje (debe estar normalizado
//		return(this->HacerRotacion(angulo, eje.x, eje.y, eje.z));
//	}
//	const matriz_4x4 matriz_4x4::HacerRotacion(const vector3 &angulos) { // Rotación en ángulos (radianes) de Euler
//		return(this->HacerRotacion(angulos.x, angulos.y, angulos.z));
//	}
//	const matriz_4x4 matriz_4x4::HacerRotacion(const matriz_3x3 &orientacion) { //Iguala la parte de la orientacion de la transformación a la orientación dada en forma de matriz de 3x3
//		this->_00 = orientacion._00;	this->_01 = orientacion._01; 	this->_02 = orientacion._02;
//		this->_10 = orientacion._10; 	this->_11 = orientacion._11; 	this->_12 = orientacion._12;
//		this->_20 = orientacion._20;  this->_21 = orientacion._21;this->_22 = orientacion._22;
//
//		return(*this);
//	}
//
//	const matriz_4x4 matriz_4x4::HacerZoom(real x, real y, real z) { //Construye una matriz de Escala
//		this->_00 = x;
//		this->_11 = y;
//		this->_22 = z;
//		this->_33 = 1.0;
//
//		return(*this);
//	}
//
//	const matriz_4x4 matriz_4x4::HacerZoom(const vector3 &zoom) { // Construye una matriz de escala
//		this->_00 = zoom.x;
//		this->_11 = zoom.y;
//		this->_22 = zoom.z;
//		this->_33 = 1.0;
//
//		return(*this);
//	}

//  / / - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// | |  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// | |									Funciones de la Clase CUATERNION											     
// | | - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  \ \  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
cuaternion::cuaternion(real new_w, real new_x, real new_y, real new_z) {
	this->w = new_w;
	this->x = new_x;
	this->y = new_y;
	this->z = new_z;
}

//	cuaternion::cuaternion(real new_x, real new_y, real new_z) { // Construye una rotación en ángulos(radianes) de Euler
//		this->HacerRotacion(new_x, new_y, new_z);
//	}
//
//	cuaternion::cuaternion(const cuaternion &dedonde, const cuaternion &adonde, real t) { // Spherical Linear intERPolation
//		this->Slerp(dedonde, adonde, t);
//	}
cuaternion::cuaternion(const vector3 &op1) { // copia los atributos x, y y z a los del cuaternion, dejando w = 0
	this->w = 0.0;
	this->x = op1.x;
	this->y = op1.y;
	this->z = op1.z;
}
cuaternion::cuaternion(void) {
	this->w = 1.0;
	this->x = 0.0;
	this->y = 0.0;
	this->z = 0.0;
}

cuaternion::cuaternion(const cuaternion &op1) {
	memcpy(this->m, op1.m, sizeof(this->m));
}

const cuaternion cuaternion::operator+(const cuaternion &op2) const {
	return (cuaternion(this->w + op2.w, this->x + op2.x, this->y + op2.y,
			this->z + op2.z));
}

const cuaternion cuaternion::operator-(const cuaternion &op2) const {
	return (cuaternion(this->w - op2.w, this->x - op2.x, this->y - op2.y,
			this->z - op2.z));
}
const cuaternion cuaternion::operator-(void) const {
	return (cuaternion(-this->w, -this->x, -this->y, -this->z));
}

const cuaternion cuaternion::operator*(const real op2) const {
	return (cuaternion(this->w * op2, this->x * op2, this->y * op2,
			this->z * op2));
}
void cuaternion::operator *=(const real op2) {
	this->x *= op2;
	this->y *= op2;
	this->z *= op2;
	this->w *= op2;
}

const cuaternion cuaternion::operator*(const cuaternion &op2) const {
	return (cuaternion(
			this->w * op2.w - this->x * op2.x - this->y * op2.y
					- this->z * op2.z,	//componente W
			this->w * op2.x + this->x * op2.w + this->y * op2.z
					- this->z * op2.y,	//componente X
			this->w * op2.y + this->y * op2.w + this->z * op2.x
					- this->x * op2.z,		//componente Y
			this->w * op2.z + this->z * op2.w + this->x * op2.y
					- this->y * op2.x));		//componente Z
}

cuaternion::operator real *() const {
	return ((real *) m);
}

const real cuaternion::modulo() const { // Devuelve el módulo del cuaternion
	return ((real) sqrt(
			this->w * this->w + this->x * this->x + this->y * this->y
					+ this->z * this->z));
}
//
//	void cuaternion::Conjugar() { // Conjuga el cuaternion
//		this->x = -this->x;
//		this->y = -this->y;
//		this->z = -this->z;
//	}
//	const cuaternion cuaternion::Conjugado() const { //Devuelve el cuaternion conjugado a aquel sobre el cual se invocó la operación
//		return(cuaternion(this->w, -this->x, -this->y, -this->z));
//	}
//
//	void cuaternion::Normalizar(void) { //Normaliza el cuaternion
//		real modulo = this->Modulo();
//		real unosobremodulo;
//
//		if(modulo == 0.0f)
//			throw InvalidStateException("Vector module is 0");
//
//		unosobremodulo = 1.0f / modulo;
//		*this = *this * unosobremodulo;
//	}
//
//	const cuaternion cuaternion::Normalizado(void) const { // Devuelve el cuaternion normalizado
//		real modulo = this->Modulo();
//
//		if(modulo == 0.0f)
//			throw InvalidStateException("Vector module is 0");
//
//		real unosobremodulo = 1.0f / modulo;
//
//		return(*this * unosobremodulo);
//	}
//
//	void cuaternion::Invertir(void) { //Invierte el cuaternion
//	}
//	const cuaternion cuaternion::Invertido(void) const { //Devuelve el cuaternion que es el inverso de aquel sobre el cual se aplicó la operación
//		return(*this);
//	}
//
//	void cuaternion::Slerp(const cuaternion &dedonde, const cuaternion &adonde, real t) { // Interpola mediante el método SLERP (Spherical Linear intERPolation).
//		real sen, cos, omega, escala_0, escala_1;
//
//		cos = dedonde.x * adonde.x + dedonde.y * adonde.y + dedonde.z * adonde.z + dedonde.w * adonde.w;
//		if((1.0f + cos) > DELTA) {
//			if((1.0f - cos) > DELTA) {
//				omega = (real)acos(cos);
//				sen = (real)sin(omega);
//				escala_0 = (real)sin((1.0f - t) * omega)  / sen;
//				escala_1 = (real)sin(t * omega) / sen;
//			}
//			else {
//				escala_0 = 1.0f - t;
//				escala_1 = t;
//			}
//
//			*this = (  (dedonde * escala_0)  +  (adonde * escala_1)  );
//		}
//		else {
//			escala_0 = (real)sin((1.0f - t) * (M_PI / 2.0f));
//			escala_1 = (real)sin(t * (M_PI/2.0f));
//
//			this->w =  ( (escala_0 * dedonde.w) - (escala_1 * adonde.z) );
//			this->x = ( (escala_0 * dedonde.x) - (escala_1 * adonde.y) );
//			this->y = ( (escala_0 * dedonde.y) + (escala_1 * adonde.x) );
//			this->z = ( (escala_0 * dedonde.z) - (escala_1 * adonde.w) );
//		}
//	}
//
//	const cuaternion cuaternion::HacerRotacion(real angulo, const vector3 &eje) { // Construye una rotación ángulo (radianes), eje (normalizado)
//		this->w = (real)cos(angulo * 0.5);
//		this->x = eje.x * (real)sin(angulo * 0.5);
//		this->y = eje.y * (real)sin(angulo * 0.5);
//		this->z = eje.z * (real)sin(angulo * 0.5);
//
//		return(*this);
//	}
//
//	const cuaternion cuaternion::HacerRotacion(real x, real y, real z) {	// Construye una rotación de ángulos (radianes) de Euler
//		double	ex, ey, ez;
//		double	cr, cp, cy, sr, sp, sy, cpcy, spsy;		// temp vars in roll,pitch yaw
//
//		ex = x / 2.0;	// convert to rads and half them
//		ey = y / 2.0;
//		ez = z / 2.0;
//
//		cr = cos(ex); sr = sin(ex);
//		cp = cos(ey); sp = sin(ey);
//		cy = cos(ez); sy = sin(ez);
//
//		cpcy = cp * cy;
//		spsy = sp * sy;
//
//		this->w = float(cr * cpcy + sr * spsy);
//		this->x = float(sr * cpcy - cr * spsy);
//		this->y = float(cr * sp * cy + sr * cp * sy);
//		this->z = float(cr * cp * sy - sr * sp * cy);
//
//		Normalizar();
//
//		return(*this);
//	}
//
//	const cuaternion cuaternion::HacerRotacion(const vector3 &angulos) { // Construye una rotación de ángulos (radianes) de Euler.
//		return(this->HacerRotacion(angulos.x, angulos.y, angulos.z));
//	}
//
//	cuaternion::operator matriz_4x4() const { //Obtiene la matriz de 3x3 correspondiente al cuaternion sobre el cual se aplica la operación.
//		real y2 = y * y, x2 = x * x, z2 = z * z;
//		real xy = x * y;
//		real xz = x * z;
//		real wx = x * w;
//		real yz = y * z;
//		real wy = y * w;
//		real wz = w * z;
//		return(matriz_4x4(1.0f - 2.0f * (y2 + z2),  2.0f * (xy - wz),              2.0f * (xz + wy),		 0.0f,
//						  2.0f * (xy + wz),            1.0f - 2.0f * (x2 + z2),   2.0f * (yz - wx),		0.0f,
//						  2.0f * (xz - wy),            2.0f * (yz +  wx),             1.0f - 2.0f * (x2 + y2),	0.0f,
//						  0.0f,				      0.0f,				    0.0f,			         1.0f
//		));
//	}
//	cuaternion::operator matriz_3x3() const { // obtiene la matriz de 4x4 que corresponde al cuaternion
//		real y2 = y * y, x2 = x * x, z2 = z * z;
//		real xy = x * y;
//		real xz = x * z;
//		real wx = x * w;
//		real yz = y * z;
//		real wy = y * w;
//		real wz = w * z;
//		return(matriz_3x3( 1.0f - 2.0f * (y2 + z2),   2.0f * (xy - wz),             2.0f * (xz + wy),
//							2.0f * (xy + wz),            1.0f - 2.0f * (x2 + z2),	   2.0f * (yz - wx),
//							2.0f * (xz - wy),            2.0f * (yz +  wx),             1.0f - 2.0f * (x2 + y2)
//		));
//	}
//
//	const cuaternion cuaternion::AnguloEje() const { // devuelve una rotación angulo(radianes) - eje(normalizado) codificada en un cuaternion cuyo w es el ángulo y cuyos x, y y z forman el vector3.
//		real escala, omega;
//
//		omega = (real)acos(this->w);
//		escala = (real)sin(    omega    );
//		if(escala == 0.0) {
//			return(cuaternion(	0.0,
//			      0.0,
//			      1.0,
//			      0.0
//		));
//
//		}
//		return(cuaternion(	((omega * 360.0f) / M_PI),
//						      (this->x / escala),
//						      (this->y / escala),
//						      (this->z / escala)
//		));
//	}
//
//	const real cuaternion::operator()(int nro) const {
//		return(this->m[nro]);
//	}


//real &matriz_mxn::operator ()(unsigned int fila, unsigned int columna)
//{
//	if(0 < fila && fila <= this->getNroFilas())
//		if(0 < columna && columna <= this->getNroColumnas())
//			return this->elementos[(fila - 1) * this->getNroColumnas() + (columna - 1)];
//
//	throw InvalidArgumentException("Index Out of Bounds - matriz_mxn::operator()");
//}
//
//const real &matriz_mxn::operator ()(unsigned int fila, unsigned int columna) const
//{
//	if(0 < fila && fila <= this->getNroFilas())
//		if(0 < columna && columna <= this->getNroColumnas())
//			return this->elementos[(fila - 1) * this->getNroColumnas() + (columna - 1)];
//
//	throw InvalidArgumentException("Index Out of Bounds - matriz_mxn::operator() const");
//}
//const matriz_mxn matriz_mxn::operator +(const matriz_mxn &op1) const {
//	matriz_mxn respuesta(this->getNroFilas(), this->getNroColumnas());
//
//	if (this->getNroColumnas() == op1.getNroColumnas()
//			&& this->getNroFilas() == op1.getNroFilas())
//		for (unsigned short i = 0; i < this->getNroFilas(); i++)
//			for (unsigned short j = 0; j < this->getNroColumnas(); j++)
//				respuesta(i, j) = (*(matriz_mxn *) this)(i, j)
//						+ ((matriz_mxn &) op1)(i, j);
//	else
//		throw InvalidArgumentException(
//				"Las dimensiones de las matrices son erróneas - matriz_mxn::operator +");
//
//	return (respuesta);
//}
//void matriz_mxn::operator += (const matriz_mxn &op1)
//{
//	if(this->getNroFilas() == op1.getNroFilas() && this->getNroColumnas() == op1.getNroColumnas())
//		for(unsigned short i = 1; i <= this->getNroFilas(); i++)
//			for(unsigned short j = 1; j <= this->getNroColumnas(); j++)
//				(*this)(i, j) += ((matriz_mxn &)op1)(i, j);
//	else throw InvalidArgumentException("Las dimensiones de las matrices son erróneas - matriz_mxn::operator +=");
//}

//const matriz_mxn matriz_mxn::operator -(const matriz_mxn &op1) const {
//	matriz_mxn respuesta(this->getNroFilas(), this->getNroColumnas());
//
//	if (this->getNroFilas() == op1.getNroFilas()
//			&& this->getNroColumnas() == op1.getNroColumnas())
//		for (unsigned short i = 0; i < this->getNroFilas(); i++)
//			for (unsigned short j = 0; j < this->getNroColumnas(); j++)
//				respuesta(i, j) = (*(matriz_mxn *) this)(i, j)
//						- ((matriz_mxn &) op1)(i, j);
//	else
//		throw InvalidArgumentException(
//				"Las dimensiones de las matrices son erróneas - matriz_mxn::operator -");
//
//	return (respuesta);
//}
//void matriz_mxn::operator -= (const matriz_mxn &op1)
//{
//	if(this->getNroFilas() == op1.getNroFilas() && this->getNroColumnas() == op1.getNroColumnas())
//		for(unsigned short i = 1; i <= this->getNroFilas(); i++)
//			for(unsigned short j = 1; j <= this->getNroColumnas(); j++)
//				(*this)(i, j) -= ((matriz_mxn &)op1)(i, j);
//	else throw InvalidArgumentException("Las dimensiones de las matrices son erróneas - matriz_mxn::operator -=");
//}
//const matriz_mxn matriz_mxn::operator / (real op1) const
//{
//	matriz_mxn respuesta(this->getNroFilas(), this->getNroColumnas());
//
//	for(unsigned short i = 1; i <= this->getNroFilas(); i++)
//		for(unsigned short j = 1; j <= this->getNroColumnas(); j++)
//			respuesta(i, j) = (*(matriz_mxn *)this)(i, j) / op1;
//
//	return(respuesta);
//}
//void matriz_mxn::operator /= (real op1)
//{
//	for(unsigned short i = 1; i <= this->getNroFilas(); i++)
//		for(unsigned short j = 1; j <= this->getNroColumnas(); j++)
//			(*this)(i, j) /= op1;
//}
//const matriz_mxn matriz_mxn::operator *(real op1) const {
//	matriz_mxn respuesta(this->getNroFilas(), this->getNroColumnas());
//
//	for (unsigned short i = 0; i < this->getNroFilas(); i++)
//		for (unsigned short j = 0; j < this->getNroColumnas(); j++)
//			respuesta(i, j) = (*(matriz_mxn *) this)(i, j) * op1;
//
//	return (respuesta);
//}
////void matriz_mxn::operator *= (real op1)
//{
//	for(unsigned short i = 1; i <= this->getNroFilas(); i++)
//		for(unsigned short j = 1; j <= this->getNroColumnas(); j++)
//			(*this)(i, j) *= op1;
//}
//const matriz_mxn matriz_mxn::operator *(const matriz_mxn &op1) const {
//	matriz_mxn resultado(this->getNroFilas(), op1.getNroColumnas());
//
//	if (this->getNroColumnas() == op1.getNroFilas()) {
//		for (unsigned short i = 0; i < this->getNroFilas(); i++)
//			for (unsigned short j = 0; j < op1.getNroColumnas(); j++) {
//				resultado(i, j) = 0.0f;
//				for (unsigned short k = 0; k < this->getNroColumnas(); k++)
//					resultado(i, j) += (*(matriz_mxn *) this)(i, k)
//							* ((matriz_mxn &) op1)(k, j);
//			}
//	} else
//		throw InvalidArgumentException(
//				"Las dimensiones de las matrices son erróneas - matriz_mxn::operator *");
//	return (resultado);
//
//}
//void matriz_mxn::operator *=(const matriz_mxn &op1)
//{
//	matriz_mxn temp(*this);
//
//	if(this->getNroColumnas() == op1.getNroFilas()) {
//		for(unsigned short i = 1; i <= this->getNroFilas(); i++)
//			for(unsigned short j = 1; j <= this->getNroColumnas(); j++) {
//				(*this)(i, j) = 0.0f;
//				for(unsigned short k = 1; k <= this->getNroColumnas(); k++)
//					(*this)(i, j) += temp(i, k) * ((matriz_mxn &)op1)(k, j);
//			}
//	}
//	else throw InvalidArgumentException("Las dimensiones de las matrices son erróneas - matriz_mxn::operator *=");
//}
//
//void matriz_mxn::trasponer()
//{
//	matriz_mxn temp =*this;
//	unsigned short tmp;
//
//	tmp = this->getNroFilas();
//	this->nroFilas = this->getNroColumnas();
//	this->nroColumnas = tmp;
//
//	for(unsigned short i = 1; i <= temp.getNroFilas(); i++)
//		for(unsigned short j = 1; j <= temp.getNroColumnas(); j++)
//			(*this)(j, i) = temp(i, j);
//
//}
//const matriz_mxn matriz_mxn::traspuesta() const
//{
//	matriz_mxn resultado(this->getNroColumnas(), this->getNroFilas());
//
//	for(unsigned short i = 1; i <= this->getNroFilas(); i++)
//		for(unsigned short j = 1; j <= this->getNroColumnas(); j++)
//			resultado(j, i) = (*(matriz_mxn *)this)(i, j);
//
//	return(resultado);
//}

//const matriz_mxn matriz_mxn::identidad(unsigned int n) {
//	matriz_mxn identity(n, n);
//
//	for (unsigned int i = 0; i < n; i++)
//		for (unsigned int j = 0; j < n; j++) {
//			if (i == j)
//				identity(i, j) = 1.0;
//			else
//				identity(i, j) = 0.0;
//		}
//
//	return identity;
//}

//matriz_mxn::operator const real() const {
//	if (this->getNroColumnas() == 1 && this->getNroFilas() == 1)
//		return operator()(0, 0);
//	else
//		throw InvalidArgumentException(
//				"Invalid Cast - matriz_mxn::operator(real)(...)");
//}
