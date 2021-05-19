#ifndef MATERIALRESOURCE_H_
#define MATERIALRESOURCE_H_

#include <Resource.h>
#include <Math3d.h>

/**
 * Should this be a resource?
 */
class MaterialResource: public Resource {
private:
	vector4 ambient;
	vector4 diffuse;
	vector4 specular;
	real shininess;
public:
	MaterialResource(const vector &ambient, const vector &diffuse, const vector &specular, real shininess) : Resource(0, "video/material") {
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->shininess = shininess;
	}

	MaterialResource(const vector4 &ambient, const vector4 &diffuse, const vector4 &specular, real shininess) : Resource(0, "video/material") {
	        this->ambient = ambient;
	        this->diffuse = diffuse;
	        this->specular = specular;
	        this->shininess = shininess;
	    }

	const vector4& getAmbient() const {
		return this->ambient;
	}

	const vector4& getDiffuse() const {
		return this->diffuse;
	}

	const vector4& getSpecular() const {
		return this->specular;
	}

	const real& getShininess() const {
		return this->shininess;
	}

	void setAmbient(const vector4 &ambient) {
		this->ambient = ambient;
	}

	void setDiffuse(const vector4 &diffuse) {
		this->diffuse = diffuse;
	}

	void setSpecular(const vector4 &specular) {
		this->specular = specular;
	}

	void setShininess(real shininess) {
		this->shininess = shininess;
	}
};
#endif
