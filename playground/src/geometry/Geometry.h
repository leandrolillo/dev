/*
 * Geometry.h
 *
 *  Created on: May 9, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_GEOMETRY_GEOMETRY_H_
#define SRC_PHYSICS_GEOMETRY_GEOMETRY_H_

#include<Math3d.h>
#include<GeometryContact.h>

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
    virtual GeometryContact detectCollision(const Geometry &geometry) const = 0;

    /**
     * "Visit" method in visitor pattern
     */

    virtual bool intersects(const AbstractLine &line) const = 0;
    virtual bool intersects(const AbstractSphere &sphere) const = 0;
    virtual bool intersects(const AbstractPlane &plane) const = 0;

    virtual GeometryContact detectCollision(const AbstractLine &line) const = 0;
    virtual GeometryContact detectCollision(const AbstractSphere &sphere) const = 0;
    virtual GeometryContact detectCollision(const AbstractPlane &plane) const = 0;

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

    void setRadius(real radius) {
        this->radius = radius;
    }

    /**
     * "Accept" method in visitor pattern / double dispatch
     */

    bool intersects(const Geometry &geometry) const {
        return geometry.intersects(*this);
    }

    GeometryContact detectCollision(const Geometry &geometry) const {
        return geometry.detectCollision(*this);
    }

    /**
     * "Visit" methods in visitor pattern / double dispatch
     */

    bool intersects(const AbstractLine &line) const;
    bool intersects(const AbstractSphere &sphere) const;
    bool intersects(const AbstractPlane &plane) const;

    GeometryContact detectCollision(const AbstractLine &line) const;
    GeometryContact detectCollision(const AbstractPlane &plane) const;
    GeometryContact detectCollision(const AbstractSphere &sphere) const;


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

    GeometryContact detectCollision(const Geometry &geometry) const {
        return geometry.detectCollision(*this);
    }


    /**
     * "Visit" methods in visitor pattern / double dispatch
     */
    bool intersects(const AbstractLine &line) const;
    bool intersects(const AbstractSphere &sphere) const;
    bool intersects(const AbstractPlane &plane) const;

    GeometryContact detectCollision(const AbstractLine &line) const;
    GeometryContact detectCollision(const AbstractSphere &sphere) const;
    GeometryContact detectCollision(const AbstractPlane &plane) const;


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

    GeometryContact detectCollision(const Geometry &geometry) const {
        return geometry.detectCollision(*this);
    }


    /**
    * "Visit" methods in visitor pattern / double dispatch
    */
    bool intersects(const AbstractLine &line) const;
    bool intersects(const AbstractSphere &sphere) const;
    bool intersects(const AbstractPlane &plane) const;

    GeometryContact detectCollision(const AbstractLine &line) const;
    GeometryContact detectCollision(const AbstractSphere &sphere) const;
    GeometryContact detectCollision(const AbstractPlane &plane) const;

};

class IntersectionTester {
public:
    static bool lineSphere(const AbstractLine &line, const AbstractSphere &sphere)
    {
       real projection = (sphere.getOrigin() - line.getOrigin()) * line.getDirection();
       vector projectedSphereCenter = line.getOrigin() + line.getDirection() * projection;
       vector lineToSphere = sphere.getOrigin() - projectedSphereCenter;

       return (lineToSphere * lineToSphere <= sphere.getRadius() * sphere.getRadius());
    }

    static bool linePlane(const AbstractLine &line, const AbstractPlane &plane)
    {
       return false;
    }

    static bool lineLine(const AbstractLine &line, const AbstractLine &anotherLine)
    {
       return false;
    }

    static bool sphereSphere(const AbstractSphere &sphere, const AbstractSphere &anotherSphere) {
        vector delta = sphere.getOrigin() - anotherSphere.getOrigin();
        real radiuses = sphere.getRadius() + anotherSphere.getRadius();

        return (delta * delta <= radiuses * radiuses);
    }

    static bool planeSphere(const AbstractPlane &plane, const AbstractSphere &sphere) {
        vector delta = ((sphere.getOrigin() - plane.getOrigin()) * plane.getNormal()) * plane.getNormal();
        return (delta * delta <= sphere.getRadius() * sphere.getRadius());
    }

    static bool planePlane(const AbstractPlane &plane, const AbstractPlane &anotherPlane) {
        return false;
    }

    static GeometryContact lineSphereContact(const AbstractLine &line, const AbstractSphere &sphere)
    {
       return GeometryContact::noContact;
    }

    static GeometryContact linePlaneContact(const AbstractLine &line, const AbstractPlane &plane) {
        return GeometryContact::noContact;
    }

    static GeometryContact lineLineContact(const AbstractLine &line, const AbstractLine &anotherLine) {
        return GeometryContact::noContact;
    }

    static GeometryContact sphereSphereContact(const AbstractSphere &sphereA, const AbstractSphere &sphereB) {
        vector delta = sphereB.getOrigin() - sphereA.getOrigin();
        real radiuses = sphereA.getRadius() + sphereB.getRadius();

        if(delta * delta <= radiuses * radiuses) {
            real distance = delta.modulo();
            vector normal = delta * (1.0 / distance);
            real penetration = radiuses - distance;
            vector intersection = sphereA.getOrigin() + (normal * sphereA.getRadius());
            return GeometryContact(&sphereA, &sphereB, intersection, normal, 0.8f,  penetration);
        }

        return GeometryContact::noContact;
    }

    static GeometryContact planeSphereContact(const AbstractPlane &plane, const AbstractSphere &sphere) {
        real distance = (sphere.getOrigin() - plane.getOrigin()) * plane.getNormal();

        if(distance <= sphere.getRadius()) {
            vector delta = distance * plane.getNormal();
            real penetration = sphere.getRadius() - distance;
            vector intersection = sphere.getOrigin() - (plane.getNormal() * sphere.getRadius());

            return GeometryContact(&plane, &sphere, intersection, plane.getNormal(), 0.8f, penetration);
        }

        return GeometryContact::noContact;
    }

    static GeometryContact planePlaneContact(const AbstractPlane &plane, const AbstractPlane &anotherPlane) {
        return GeometryContact::noContact;
    }


};

/**
 * Sphere intersection tests
 */

bool AbstractSphere::intersects(const AbstractLine &line) const {
    return IntersectionTester::lineSphere(line, *this);
}

bool AbstractSphere::intersects(const AbstractSphere &sphere) const {
    return IntersectionTester::sphereSphere(sphere, *this);
}

bool AbstractSphere::intersects(const AbstractPlane &plane) const {
    return IntersectionTester::planeSphere(plane, *this);
}

GeometryContact AbstractSphere::detectCollision(const AbstractLine &line) const {
    return IntersectionTester::lineSphereContact(line, *this);
}
GeometryContact AbstractSphere::detectCollision(const AbstractSphere &sphere) const {
    return IntersectionTester::sphereSphereContact(sphere, *this);
}
GeometryContact AbstractSphere::detectCollision(const AbstractPlane &plane) const {
    return IntersectionTester::planeSphereContact(plane, *this);
}


/**
 * Line intersection tests
 */
bool AbstractLine::intersects(const AbstractLine &line) const {
    return IntersectionTester::lineLine(*this, line);
}

bool AbstractLine::intersects(const AbstractSphere &sphere) const {
    return IntersectionTester::lineSphere(*this, sphere);
}
bool AbstractLine::intersects(const AbstractPlane &plane) const {
    return IntersectionTester::linePlane(*this, plane);
}

GeometryContact AbstractLine::detectCollision(const AbstractLine &line) const {
    return IntersectionTester::lineLineContact(*this, line);
}
GeometryContact AbstractLine::detectCollision(const AbstractSphere &sphere) const {
    return IntersectionTester::lineSphereContact(*this, sphere);
}
GeometryContact AbstractLine::detectCollision(const AbstractPlane &plane) const {
    return IntersectionTester::linePlaneContact(*this, plane);
}


/**
 * Plane intersection tests
 */

bool AbstractPlane::intersects(const AbstractLine &line) const {
    return IntersectionTester::linePlane(line, *this);
}

bool AbstractPlane::intersects(const AbstractSphere &sphere) const {
    return IntersectionTester::planeSphere(*this, sphere);
}
bool AbstractPlane::intersects(const AbstractPlane &plane) const {
    return IntersectionTester::planePlane(*this, plane);
}

GeometryContact AbstractPlane::detectCollision(const AbstractLine &line) const {
    return IntersectionTester::linePlaneContact(line, *this);
}
GeometryContact AbstractPlane::detectCollision(const AbstractSphere &sphere) const {
    return IntersectionTester::planeSphereContact(*this, sphere);
}
GeometryContact AbstractPlane::detectCollision(const AbstractPlane &plane) const {
    return IntersectionTester::planePlaneContact(*this, plane);
}





#endif /* SRC_PHYSICS_GEOMETRY_GEOMETRY_H_ */
