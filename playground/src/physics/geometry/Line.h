/*
 * Line.h
 *
 *  Created on: May 16, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_GEOMETRY_LINE_H_
#define SRC_PHYSICS_GEOMETRY_LINE_H_

#include<Geometry.h>

class Line : public AbstractLine {
    vector origin;

public:
    Line(const vector &origin, const vector &direction) : AbstractLine(direction) {
        setOrigin(origin);
    }

    const vector& getOrigin() const {
        return origin;
    }
    void setOrigin(const vector &origin) {
        this->origin = origin;
    }
};



#endif /* SRC_PHYSICS_GEOMETRY_LINE_H_ */
