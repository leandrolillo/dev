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


class HeightMapResource : public Resource
{
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
            return (real)(this->heightMap->getAncho() - 1) * voxelSize.x;
        }

        real getHeight() const {
            return 2 * voxelSize.y;
        }

        real getDepth() const {
            return (real)(this->heightMap->getAlto() - 1) * voxelSize.z;
        }

        unsigned int getGridWidth() const {
            return this->heightMap->getAncho();
        }

        unsigned int getGridHeight() const {
            return this->heightMap->getAlto();
        }

        vector position(unsigned int i, unsigned int j) const {
            i = std::min(i, this->getGridWidth());
            j = std::min(j, this->getGridHeight());

            return vector(  (real)i * voxelSize.x,
                            height(i, j),
                            (real)j * voxelSize.z
                         );
        }

        vector normal(unsigned int i, unsigned int j) const {
            i = std::min(i, this->getGridWidth());
            j = std::min(j, this->getGridHeight());

            //return vector(0, 1, 0);
            real hL = height(i-1 , j);
            real hR = height(i+1 , j);
            real hD = height(i, j+1);
            real hU = height(i, j-1);

            return vector(hL - hR, 2.0, hD - hU).normalizado();
        }

        /**
         * Returns texture coordinates in the range [0, 1] for the given (i, j) coordinates
         */
        vector2 textCoord(unsigned int i, unsigned int j) const {
            i = std::min(i, this->getGridWidth());
            j = std::min(j, this->getGridHeight());

            return vector2((real)i / (real)(this->heightMap->getAncho() - 1), (real)j / (real)(this->heightMap->getAlto() - 1));
        }


        /**
         * Returns height in the range [-max_height, max_height] for the given (i, j) coordinates
         */
        real height(unsigned int i, unsigned int j) const {
            i = std::min(i, this->getGridWidth());
            j = std::min(j, this->getGridHeight());

            vector pixel = this->heightMap->getPixel(i, j);

    //        logger->info("<%u, %u) = <%.0f, %.0f, %.0f>", i, j, pixel.x, pixel.y, pixel.z);


            return (pixel.x * pixel.y * pixel.z * one_over_max_color_over_two) * voxelSize.y;
        }
};


#endif /* SRC_VIDEO_RESOURCES_HEIGHTMAPRESOURCE_H_ */
