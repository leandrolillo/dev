/*
 * RenderableAABB.h
 *
 *  Created on: Jul 8, 2021
 *      Author: leandro
 */

#ifndef SRC_VIDEO_RENDERERS_GEOMETRY_DRAWABLEHIERARCHY_H_
#define SRC_VIDEO_RENDERERS_GEOMETRY_DRAWABLEHIERARCHY_H_

#include<Geometry.h>
#include<DefaultRenderer.h>

class DrawableHierarchy: public HierarchicalGeometry, public Drawable {
public:
    DrawableHierarchy(Geometry *boundingVolume) : HierarchicalGeometry(boundingVolume) {
    }

    void draw(DefaultRenderer &renderer) const override {
        vector halfSizes = ((AABB &)this->getBoundingVolume()).getHalfSizes();

        renderer.drawBox(matriz::matrizTraslacion(this->getOrigin()),
                (real)2 * halfSizes.x,
                (real)2 * halfSizes.y,
                (real)2 * halfSizes.z);

        for(auto &child : this->getChildren()) {
            renderer.drawSphere(matriz::matrizTraslacion(child->getOrigin()), ((Sphere &)*child.get()).getRadius());
        }
    }
};

#endif /* SRC_VIDEO_RENDERERS_GEOMETRY_DRAWABLEAABB_H_ */