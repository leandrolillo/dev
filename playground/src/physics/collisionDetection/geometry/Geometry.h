/*
 * Geometry.h
 *
 *  Created on: May 9, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_GEOMETRY_GEOMETRY_H_
#define SRC_PHYSICS_GEOMETRY_GEOMETRY_H_

#include<Math3d.h>
#include<collisionDetection/IntersectionTester.h>

class AbstractLine;
class AbstractSphere;
class AbstractPlane;

class Geometry {
public:
    virtual ~Geometry() {}
    virtual const vector& getOrigin() const = 0;

    /**
     * "Accept" method in visitor pattern / double dispatch
     */
    virtual bool intersects(const Geometry &geometry) const = 0;

    /**
     * "Visit" method in visitor pattern
     */

    virtual bool intersects(const AbstractLine &line) const = 0;
    virtual bool intersects(const AbstractSphere &sphere) const = 0;
    virtual bool intersects(const AbstractPlane &plane) const = 0;

    virtual String toString() const {
        return "Geometry";
    }
};

class AbstractSphere: public Geometry {
    real radius;
public:
    AbstractSphere(real radius) {
        this->radius = radius;
    }

    real getRadius() const {
        return this->radius;
    }

    /**
     * "Accept" method in visitor pattern / double dispatch
     */

    bool intersects(const Geometry &geometry) const {
        return geometry.intersects(*this);
    }

    /**
     * "Visit" methods in visitor pattern / double dispatch
     */

    bool intersects(const AbstractLine &line) const;
    bool intersects(const AbstractSphere &sphere) const;
    bool intersects(const AbstractPlane &plane) const;

    String toString() const {
        return "Sphere(origin: " + this->getOrigin().toString("%.2f") + ", radius: " + std::to_string(this->radius) + ")";
    }
};

class AbstractLine: public Geometry {
    vector direction;
public:
    AbstractLine(const vector &direction) {
        this->direction = direction.normalizado();
    }

    const vector& getDirection() const {
        return this->direction;
    }
    /**
     * "Accept" method in visitor pattern / double dispatch
     */
    bool intersects(const Geometry &geometry) const {
        return geometry.intersects(*this);
    }

    /**
     * "Visit" methods in visitor pattern / double dispatch
     */
    bool intersects(const AbstractLine &line) const;
    bool intersects(const AbstractSphere &sphere) const;
    bool intersects(const AbstractPlane &plane) const;

    String toString() const {
        return "Line(origin: " + this->getOrigin().toString("%.2f") + ", dir: " + this->direction.toString("%.2f") + ")";
    }
};


class AbstractPlane: public Geometry {
    vector normal;
public:
    AbstractPlane(const vector &normal) {
        this->normal = normal.normalizado();
    }

    const vector &getNormal() const {
        return this->normal;
    }

    /**
     * "Accept" method in visitor pattern / double dispatch
     */
    bool intersects(const Geometry &geometry) const {
        return geometry.intersects(*this);
    }

    /**
    * "Visit" methods in visitor pattern / double dispatch
    */
    bool intersects(const AbstractLine &line) const;
    bool intersects(const AbstractSphere &sphere) const;
    bool intersects(const AbstractPlane &plane) const;
};

/**
 * Sphere intersection tests
 */

bool AbstractSphere::intersects(const AbstractLine &line) const {
    return IntersectionTester::lineSphere(line.getOrigin(), line.getDirection(), this->getOrigin(), this->getRadius());
    return false;
}

bool AbstractSphere::intersects(const AbstractSphere &sphere) const {
    return IntersectionTester::sphereSphere(sphere.getOrigin(), sphere.getRadius(), this->getOrigin(), this->getRadius());
}

bool AbstractSphere::intersects(const AbstractPlane &plane) const {
    return IntersectionTester::planeSphere(plane.getOrigin(), plane.getNormal(), this->getOrigin(), this->getRadius());
}


/**
 * Line intersection tests
 */
bool AbstractLine::intersects(const AbstractLine &line) const {
    return false;
}

bool AbstractLine::intersects(const AbstractSphere &sphere) const {
    return IntersectionTester::lineSphere(this->getOrigin(), this->getDirection(), sphere.getOrigin(), sphere.getRadius());
}
bool AbstractLine::intersects(const AbstractPlane &plane) const {
    return false;
}


/**
 * Plane intersection tests
 */

bool AbstractPlane::intersects(const AbstractLine &line) const {
    return false;
}

bool AbstractPlane::intersects(const AbstractSphere &sphere) const {
    return IntersectionTester::planeSphere(this->getOrigin(), this->normal, sphere.getOrigin(), sphere.getRadius());
}
bool AbstractPlane::intersects(const AbstractPlane &plane) const {
    return false;
}



#endif /* SRC_PHYSICS_GEOMETRY_GEOMETRY_H_ */
