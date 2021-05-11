/*
 * IntersectionTester.h
 *
 *  Created on: May 11, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_COLLISIONDETECTION_INTERSECTIONTESTER_H_
#define SRC_PHYSICS_COLLISIONDETECTION_INTERSECTIONTESTER_H_

#include<Math3d.h>

class IntersectionTester {
public:
    static bool areIntersecting(const vector &lineOrigin, const vector &lineDirection, const vector &sphereOrigin, real sphereRadius)
    {
       real projection = (sphereOrigin - lineOrigin) * lineDirection;
       vector projectedSphereCenter = lineOrigin + lineDirection * projection;
       vector lineToSphere = sphereOrigin - projectedSphereCenter;

       return (lineToSphere * lineToSphere <= sphereRadius * sphereRadius);
    }

    static bool areIntersecting(const vector &sphereOrigin, real radius, const vector &anotherSphereOrigin, real anotherSphereRadius) {
        vector delta = sphereOrigin - anotherSphereOrigin;
        real radiuses = radius + anotherSphereRadius;

        return (delta * delta <= radiuses * radiuses);

    }
};




#endif /* SRC_PHYSICS_COLLISIONDETECTION_INTERSECTIONTESTER_H_ */
