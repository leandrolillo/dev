/*
 * Resource.cpp
 *
 *  Created on: Aug 15, 2022
 *      Author: leandro
 */

#include "Resource.h"

#include "ResourceAdapter.h"

Resource::~Resource() {
	logger->debug("Destroying Resource %s", this->toString().c_str());

	if(this->adapter != null) {
		this->adapter->dispose(this);
	}
}
