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

class Line;
class Sphere;
class Plane;

class Geometry {
    vector origin;
public:
    Geometry(const vector &origin) {
        this->origin = origin;
    }

    virtual ~Geometry() {}

    const vector& getOrigin() const {
        return this->origin;
    }

    void setOrigin(const vector &origin) {
        this->origin = origin;
    }

    /**
     * "Accept" method in visitor pattern / double dispatch
     */
    virtual bool intersects(const Geometry &geometry) const = 0;
    virtual GeometryContact detectCollision(const Geometry &geometry) const = 0;

    /**
     * "Visit" method in visitor pattern
     */

    virtual bool intersects(const Line &line) const = 0;
    virtual bool intersects(const Sphere &sphere) const = 0;
    virtual bool intersects(const Plane &plane) const = 0;

    virtual GeometryContact detectCollision(const Line &line) const = 0;
    virtual GeometryContact detectCollision(const Sphere &sphere) const = 0;
    virtual GeometryContact detectCollision(const Plane &plane) const = 0;

    virtual String toString() const {
        return "Geometry";
    }
};

class Sphere: public Geometry {
    real radius;
public:
    Sphere(const vector &origin, real radius) : Geometry(origin) {
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

    bool intersects(const Line &line) const;
    bool intersects(const Sphere &sphere) const;
    bool intersects(const Plane &plane) const;

    GeometryContact detectCollision(const Line &line) const;
    GeometryContact detectCollision(const Plane &plane) const;
    GeometryContact detectCollision(const Sphere &sphere) const;


    String toString() const {
        return "Sphere(origin: " + this->getOrigin().toString("%.2f") + ", radius: " + std::to_string(this->radius) + ")";
    }
};

class Line: public Geometry {
    vector direction;
public:
    Line(const vector &origin, const vector &direction) : Geometry(origin){
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
    bool intersects(const Line &line) const;
    bool intersects(const Sphere &sphere) const;
    bool intersects(const Plane &plane) const;

    GeometryContact detectCollision(const Line &line) const;
    GeometryContact detectCollision(const Sphere &sphere) const;
    GeometryContact detectCollision(const Plane &plane) const;


    String toString() const {
        return "Line(origin: " + this->getOrigin().toString("%.2f") + ", dir: " + this->direction.toString("%.2f") + ")";
    }
};


class Plane: public Geometry {
    vector normal;
public:
    Plane(const vector &origin, const vector &normal) : Geometry(origin) {
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
    bool intersects(const Line &line) const;
    bool intersects(const Sphere &sphere) const;
    bool intersects(const Plane &plane) const;

    GeometryContact detectCollision(const Line &line) const;
    GeometryContact detectCollision(const Sphere &sphere) const;
    GeometryContact detectCollision(const Plane &plane) const;

};

class IntersectionTester {
public:
    static bool lineSphere(const Line &line, const Sphere &sphere)
    {
       real projection = (sphere.getOrigin() - line.getOrigin()) * line.getDirection();
       vector projectedSphereCenter = line.getOrigin() + line.getDirection() * projection;
       vector lineToSphere = sphere.getOrigin() - projectedSphereCenter;

       return (lineToSphere * lineToSphere <= sphere.getRadius() * sphere.getRadius());
    }

    static bool linePlane(const Line &line, const Plane &plane)
    {
       return false;
    }

    static bool lineLine(const Line &line, const Line &anotherLine)
    {
       return false;
    }

    static bool sphereSphere(const Sphere &sphere, const Sphere &anotherSphere) {
        vector delta = sphere.getOrigin() - anotherSphere.getOrigin();
        real radiuses = sphere.getRadius() + anotherSphere.getRadius();

        return (delta * delta <= radiuses * radiuses);
    }

    static bool planeSphere(const Plane &plane, const Sphere &sphere) {
        vector delta = ((sphere.getOrigin() - plane.getOrigin()) * plane.getNormal()) * plane.getNormal();
        return (delta * delta <= sphere.getRadius() * sphere.getRadius());
    }

    static bool planePlane(const Plane &plane, const Plane &anotherPlane) {
        return false;
    }

    static GeometryContact lineSphereContact(const Line &line, const Sphere &sphere)
    {
       return GeometryContact::noContact;
    }

    static GeometryContact linePlaneContact(const Line &line, const Plane &plane) {
        return GeometryContact::noContact;
    }

    static GeometryContact lineLineContact(const Line &line, const Line &anotherLine) {
        return GeometryContact::noContact;
    }

    static GeometryContact sphereSphereContact(const Sphere &sphereA, const Sphere &sphereB) {
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

    static GeometryContact planeSphereContact(const Plane &plane, const Sphere &sphere) {
        real distance = (sphere.getOrigin() - plane.getOrigin()) * plane.getNormal();

        if(distance <= sphere.getRadius()) {
            vector delta = distance * plane.getNormal();
            real penetration = sphere.getRadius() - distance;
            vector intersection = sphere.getOrigin() - (plane.getNormal() * sphere.getRadius());

            return GeometryContact(&plane, &sphere, intersection, plane.getNormal(), 0.8f, penetration);
        }

        return GeometryContact::noContact;
    }

    static GeometryContact planePlaneContact(const Plane &plane, const Plane &anotherPlane) {
        return GeometryContact::noContact;
    }


};

/**
 * Sphere intersection tests
 */

bool Sphere::intersects(const Line &line) const {
    return IntersectionTester::lineSphere(line, *this);
}

bool Sphere::intersects(const Sphere &sphere) const {
    return IntersectionTester::sphereSphere(sphere, *this);
}

bool Sphere::intersects(const Plane &plane) const {
    return IntersectionTester::planeSphere(plane, *this);
}

GeometryContact Sphere::detectCollision(const Line &line) const {
    return IntersectionTester::lineSphereContact(line, *this);
}
GeometryContact Sphere::detectCollision(const Sphere &sphere) const {
    return IntersectionTester::sphereSphereContact(sphere, *this);
}
GeometryContact Sphere::detectCollision(const Plane &plane) const {
    return IntersectionTester::planeSphereContact(plane, *this);
}


/**
 * Line intersection tests
 */
bool Line::intersects(const Line &line) const {
    return IntersectionTester::lineLine(*this, line);
}

bool Line::intersects(const Sphere &sphere) const {
    return IntersectionTester::lineSphere(*this, sphere);
}
bool Line::intersects(const Plane &plane) const {
    return IntersectionTester::linePlane(*this, plane);
}

GeometryContact Line::detectCollision(const Line &line) const {
    return IntersectionTester::lineLineContact(*this, line);
}
GeometryContact Line::detectCollision(const Sphere &sphere) const {
    return IntersectionTester::lineSphereContact(*this, sphere);
}
GeometryContact Line::detectCollision(const Plane &plane) const {
    return IntersectionTester::linePlaneContact(*this, plane);
}


/**
 * Plane intersection tests
 */

bool Plane::intersects(const Line &line) const {
    return IntersectionTester::linePlane(line, *this);
}

bool Plane::intersects(const Sphere &sphere) const {
    return IntersectionTester::planeSphere(*this, sphere);
}
bool Plane::intersects(const Plane &plane) const {
    return IntersectionTester::planePlane(*this, plane);
}

GeometryContact Plane::detectCollision(const Line &line) const {
    return IntersectionTester::linePlaneContact(line, *this);
}
GeometryContact Plane::detectCollision(const Sphere &sphere) const {
    return IntersectionTester::planeSphereContact(*this, sphere);
}
GeometryContact Plane::detectCollision(const Plane &plane) const {
    return IntersectionTester::planePlaneContact(*this, plane);
}





#endif /* SRC_PHYSICS_GEOMETRY_GEOMETRY_H_ */
