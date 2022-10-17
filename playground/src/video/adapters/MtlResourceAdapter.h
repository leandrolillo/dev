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

        MaterialCollection *materials = new MaterialCollection();

        String token;
        while ((token = textParser.takeToken()) != FileParser::eof) {
            if (token == "newmtl") {
            	MaterialResource *material = parseMaterial(textParser, textParser.takeLine());
            	material->setFileName(Paths::add(fileParser.getFilename(), material->getName()));

            	this->getResourceManager()->addResource(material);
            	materials->addMaterial(material);
            } else {
                String line = textParser.takeLine().c_str();
                logger->warn("skipping [%s] [%s]", token.c_str(), line.c_str());
            }
        }

        return materials;
    }

    MaterialResource *parseMaterial(TextParser &textParser, const String &name) const {
    	MaterialResource *material = new MaterialResource(name, vector(0.8, 0.8, 0.8), vector(0.8, 0.8, 0.8), vector(0.8, 0.8, 0.8), 1.0);

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
        	} else if (token == "map_Ka") {
        		material->setAmbientTexture(Paths::relative(textParser.getFilename(), textParser.takeLine()));
        	} else if (token == "map_Kd") {
        		material->setDiffuseTexture(Paths::relative(textParser.getFilename(), textParser.takeLine()));
        	} else if (token == "map_Ks") {
        		material->setSpecularTexture(Paths::relative(textParser.getFilename(), textParser.takeLine()));
        	} else if (token == "map_d") {
        		material->setAlphaTexture(textParser.takeLine());
        	} else if (token == "map_bump" || token == "bump") {
        		material->setBumptTexture(Paths::relative(textParser.getFilename(), textParser.takeLine()));
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
