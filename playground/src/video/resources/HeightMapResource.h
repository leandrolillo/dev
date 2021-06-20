/*
 * HeightmapResource.h
 *
 *  Created on: Jun 15, 2021
 *      Author: leandro
 */

#ifndef SRC_VIDEO_RESOURCES_HEIGHTMAPRESOURCE_H_
#define SRC_VIDEO_RESOURCES_HEIGHTMAPRESOURCE_H_

#include <Resource.h>
#include <Math3d.h>
#include<Logger.h>
#include<ImageResource.h>

#define max_height 20.0
#define one_over_max_color_over_two 0.000000059604645 // 1.0 / (256.0 * 256.0 * 256.0)

class HeightMapResource: public Resource {
private:
    Logger *logger = LoggerFactory::getLogger("video/HeightMapResource");
    vector voxelSize;
    ImageResource *heightMap;

public:
    /**
     * VoxelSize can be interpreted as the dimensions of each and all individual voxels, or the factor/zoom applied to enlarge the original image dimensions
     */
    HeightMapResource(ImageResource *heightMap, vector voxelSize) : Resource(0, "video/heightmap") {
        this->heightMap = heightMap;
        this->voxelSize = voxelSize;
    }

    real getWidth() const {
        return (real) (this->heightMap->getAncho() - 1) * voxelSize.x;
    }

    real getHeight() const {
        return 2 * voxelSize.y;
    }

    real getDepth() const {
        return (real) (this->heightMap->getAlto() - 1) * voxelSize.z;
    }

    unsigned int getGridWidth() const {
        return this->heightMap->getAncho();
    }

    unsigned int getGridHeight() const {
        return this->heightMap->getAlto();
    }

    vector positionAtGrid(unsigned int i, unsigned int j) const {
        i = std::min(i, this->getGridWidth());
        j = std::min(j, this->getGridHeight());

        return vector((real) i * voxelSize.x, heightAtGrid(i, j), (real) j * voxelSize.z);
    }

    vector normalAtGrid(unsigned int i, unsigned int j) const {
        i = std::min(i, this->getGridWidth());
        j = std::min(j, this->getGridHeight());

        //return vector(0, 1, 0);
        real hL = heightAtGrid(i - 1, j);
        real hR = heightAtGrid(i + 1, j);
        real hD = heightAtGrid(i, j + 1);
        real hU = heightAtGrid(i, j - 1);

        return vector(hL - hR, 2.0, hD - hU).normalizado();
    }

    /**
     * Returns texture coordinates in the range [0, 1] for the given (i, j) coordinates
     */
    vector2 textCoordAtGrid(unsigned int i, unsigned int j) const {
        i = std::min(i, this->getGridWidth());
        j = std::min(j, this->getGridHeight());

        return vector2((real) i / (real) (this->heightMap->getAncho() - 1), (real) j / (real) (this->heightMap->getAlto() - 1));
    }

    /**
     * Returns height in the range [-max_height, max_height] for the given (i, j) coordinates
     */
    real heightAtGrid(unsigned int i, unsigned int j) const {
        i = std::min(i, this->getGridWidth());
        j = std::min(j, this->getGridHeight());

        vector pixel = this->heightMap->getPixel(i, j);

        //        logger->info("<%u, %u) = <%.0f, %.0f, %.0f>", i, j, pixel.x, pixel.y, pixel.z);

        return (pixel.x * pixel.y * pixel.z * one_over_max_color_over_two) * voxelSize.y;
    }

    real heightAt(real x, real z) const {
        unsigned int i = std::max(0, std::min((int)floor(x / voxelSize.x), (int)this->getGridWidth()));
        unsigned int j = std::max(0, std::min((int)floor(z / voxelSize.z), (int)this->getGridHeight()));

        real di = (x - (real) i * voxelSize.x) / voxelSize.x;
        real dj = (z - (real) j * voxelSize.z) / voxelSize.z;

        if (di <= ((real) 1 - dj)) {
            return barycentric(
                    vector(0, heightAtGrid(i, j), 0),
                    vector(1, heightAtGrid(i + 1, j), 0),
                    vector(0, heightAtGrid(i, j + 1), 1),
                    vector2(di, dj));

        } else {
            return barycentric(
                    vector(1, heightAtGrid(i + 1, j), 0),
                    vector(1, heightAtGrid(i + 1, j + 1), 1),
                    vector(0, heightAtGrid(i, j + 1), 1),
                    vector2(di, dj));
        }
    }

    real barycentric(const vector &p1, const vector &p2, const vector &p3, const vector2 &pos) const {
        float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
        float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
        float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
        float l3 = 1.0f - l1 - l2;
        return l1 * p1.y + l2 * p2.y + l3 * p3.y;
    }
};

#endif /* SRC_VIDEO_RESOURCES_HEIGHTMAPRESOURCE_H_ */
