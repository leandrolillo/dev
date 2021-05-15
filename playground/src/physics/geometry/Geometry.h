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

class Line;
class Sphere;
class Plane;

class Geometry {
public:

    /**
     * "Accept" method in visitor pattern / double dispatch
     */
    virtual bool intersects(const Geometry &geometry) const = 0;

    /**
     * "Visit" method in visitor pattern
     */

    virtual bool intersects(const Line &line) const = 0;
    virtual bool intersects(const Sphere &sphere) const = 0;
    virtual bool intersects(const Plane &plane) const = 0;

    virtual String toString() const {
        return "Geometry";
    }
};

class Sphere: public Geometry {
    real radius;
    vector origin;
public:
    Sphere(const vector &origin, real radius) {
        this->origin = origin;
        this->radius = radius;
    }

    void setOrigin(const vector &origin) {
        this->origin = origin;
    }

    const vector& getOrigin() const {
        return this->origin;
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

    bool intersects(const Line &line) const;
    bool intersects(const Sphere &sphere) const;
    bool intersects(const Plane &plane) const;

    String toString() const {
        return "Sphere(origin: " + this->origin.toString("%.2f") + ", radius: " + std::to_string(this->radius) + ")";
    }
};

class Line: public Geometry {
    vector origin;
    vector direction;
public:
    Line(const vector &origin, const vector &direction) {
        this->origin = origin;
        this->direction = direction.normalizado();
    }

    const vector& getOrigin() const {
        return this->origin;
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
    bool intersects(const Line &line) const;
    bool intersects(const Sphere &sphere) const;
    bool intersects(const Plane &plane) const;

    String toString() const {
        return "Line(origin: " + this->origin.toString("%.2f") + ", dir: " + this->direction.toString("%.2f") + ")";
    }
};


class Plane: public Geometry {
    vector origin;
    vector normal;
public:
    Plane(const vector &origin, const vector &normal) {
        this->origin = origin;
        this->normal = normal.normalizado();
    }

    const vector& getOrigin() const {
        return this->origin;
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
    bool intersects(const Line &line) const;
    bool intersects(const Sphere &sphere) const;
    bool intersects(const Plane &plane) const;
};

/**
 * Sphere intersection tests
 */

bool Sphere::intersects(const Line &line) const {
    return IntersectionTester::lineSphere(line.getOrigin(), line.getDirection(), this->getOrigin(), this->getRadius());
    return false;
}

bool Sphere::intersects(const Sphere &sphere) const {
    return IntersectionTester::sphereSphere(sphere.getOrigin(), sphere.getRadius(), this->getOrigin(), this->getRadius());
}

bool Sphere::intersects(const Plane &plane) const {
    return IntersectionTester::planeSphere(plane.getOrigin(), plane.getNormal(), this->getOrigin(), this->getRadius());
}


/**
 * Line intersection tests
 */
bool Line::intersects(const Line &line) const {
    return false;
}

bool Line::intersects(const Sphere &sphere) const {
    return IntersectionTester::lineSphere(this->getOrigin(), this->getDirection(), sphere.getOrigin(), sphere.getRadius());
}
bool Line::intersects(const Plane &plane) const {
    return false;
}


/**
 * Plane intersection tests
 */

bool Plane::intersects(const Line &line) const {
    return false;
}

bool Plane::intersects(const Sphere &sphere) const {
    return IntersectionTester::planeSphere(this->origin, this->normal, sphere.getOrigin(), sphere.getRadius());
}
bool Plane::intersects(const Plane &plane) const {
    return false;
}



#endif /* SRC_PHYSICS_GEOMETRY_GEOMETRY_H_ */
