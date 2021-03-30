/*
 * ArcBall.h
 *
 *  Created on: Mar 30, 2021
 *      Author: leandro
 */

#ifndef SRC_VIDEO_ARCBALL_H_
#define SRC_VIDEO_ARCBALL_H_

#include<Math3d.h>

class ArcBall {
private:
	bool isDragging = false;
	vector2 startPosition;
	vector2 endPosition;
public:

	void startDrag(const vector2 &position) {
		this->isDragging = true;
		this->startPosition = position;
		printf("start position: %s\r", position.toString().c_str());

	}

	cuaternion drag(const vector2 &delta) {
		if(this->isDragging) {
			this->endPosition = this->startPosition + delta;
			printf("dragging delta: %s\r", delta.toString().c_str());
		}

		return cuaternion();
	}

	cuaternion endDrag(const vector2 &position) {
		if(this->isDragging) {
			this->endPosition = position;
			printf("end position: %s\r", position.toString().c_str());
		}
		this->isDragging = false;

		return cuaternion();
	}

	cuaternion getRotation(unsigned int height, unsigned int width) const {
		return cuaternion();
	}
};


#endif /* SRC_VIDEO_ARCBALL_H_ */
