/*
 * MtlResourceAdapter.h
 *
 *  Created on: Oct 5, 2022
 *      Author: leandro
 */

#ifndef SRC_VIDEO_ADAPTERS_MTLRESOURCEADAPTER_H_
#define SRC_VIDEO_ADAPTERS_MTLRESOURCEADAPTER_H_

#include<ResourceAdapter.h>
#include<MaterialResource.h>
#include<TextParser.h>

class MtlResourceAdapter: public ResourceAdapter {
public:
    MtlResourceAdapter() {
        logger = LoggerFactory::getLogger(typeid(*this).name());
        this->addSupportedMimeType("model/mtl");
    }

    virtual Resource *load(FileParser &fileParser, const String &mimeType) const override {
        TextParser textParser(fileParser);

        std::vector<MaterialResource *>materials;

        String token;
        while ((token = textParser.takeToken()) != FileParser::eof) {
            if (token == "newmtl") {
            	materials.push_back(parseMaterial(textParser, textParser.takeLine()));
            } else {
                String line = textParser.takeLine().c_str();
                logger->warn("skipping [%s] [%s]", token.c_str(), line.c_str());
            }
        }

        if(materials.empty()) {
        	return null;
        }

        return *(materials.begin());
    }

    MaterialResource *parseMaterial(TextParser &textParser, String name) const {
    	MaterialResource *material = new MaterialResource(vector(0.8, 0.8, 0.8), vector(0.8, 0.8, 0.8), vector(0.8, 0.8, 0.8), 1.0);

        String token;
        while ((token = textParser.takeToken()) != FileParser::eof) {
        	if (token == "Ka") {
        		material->setAmbient(vector(textParser.readReal(), textParser.readReal(), textParser.readReal()));
        	} else if (token == "Ks") {
        		material->setSpecular(vector(textParser.readReal(), textParser.readReal(), textParser.readReal()));
        	} else if (token == "Kd") {
        		material->setDiffuse(vector(textParser.readReal(), textParser.readReal(), textParser.readReal()));
        	} else if (token == "Ke") {
        		material->setEmissive(vector(textParser.readReal(), textParser.readReal(), textParser.readReal()));
        	} else if (token == "Ns") {
        		material->setShininess(textParser.readReal());
        	} else if (token == "Tr") {
        		material->setAlpha(textParser.readReal());
        	} else if (token == "d") {
        		material->setAlpha(1.0 - textParser.readReal());
        	} else if (token == "map_Kd") {
        		String imageName = textParser.takeLine();
        		ImageResource *imageResource = (ImageResource *)this->getResourceManager()->load(Paths::add(Paths::getDirname(textParser.getFilename()), imageName));
        		if(imageResource == null) {
        			logger->warn("Could not read texture [%s] referenced from [%s]", imageName.c_str(), textParser.getFilename().c_str());
        		}
        	} else {
                String line = textParser.takeLine().c_str();
                logger->warn("skipping [%s] [%s]", token.c_str(), line.c_str());
            }


        	if(textParser.peekToken() == "newmtl") {
        		break;
        	}
        }

        return material;
    }
};



#endif /* SRC_VIDEO_ADAPTERS_MTLRESOURCEADAPTER_H_ */
