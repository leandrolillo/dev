#ifndef MATERIALRESOURCE_H_
#define MATERIALRESOURCE_H_

#include <Resource.h>
#include <Math3d.h>

/**
 * Should this be a resource?
 */
class MaterialResource: public Resource {
private:
	vector ambient;
	vector diffuse;
	vector specular;
	real shininess;
public:
	MaterialResource(const vector &ambient, const vector &diffuse, const vector &specular, real shininess) : Resource(0, "video/material") {
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->shininess = shininess;
	}

	const vector& getAmbient() const {
		return this->ambient;
	}

	const vector& getDiffuse() const {
		return this->diffuse;
	}

	const vector& getSpecular() const {
		return this->specular;
	}

	const real& getShininess() const {
		return this->shininess;
	}

	void setAmbient(const vector &ambient) {
		this->ambient = ambient;
	}

	void setDiffuse(const vector &diffuse) {
		this->diffuse = diffuse;
	}

	void setSpecular(const vector &specular) {
		this->specular = specular;
	}

	void setShininess(real shininess) {
		this->shininess = shininess;
	}
};
#endif
