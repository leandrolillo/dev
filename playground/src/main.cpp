#include <iostream>
#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#include <OpenGLRunner.h>
#include <AudioRunner.h>

#include "demo/GeneralDemo.h"
#include "demo/PhysicsDemo.h"
#include "demo/TerrainDemo.h"
#include "demo/ObjDemo.h"
#include "demo/ColDetDemo.h"
#include "demo/RoadFighter.h"

#include "demo/PlaygroundTest.h"

int main(int, char**){
    TestsPlayground *playgroundtests = new TestsPlayground("/Users/leandro/huevadas/projects/dev/media/");
    playgroundtests->run();
    delete playgroundtests;

//    PlaygroundGeneralDemo *playground = new PlaygroundGeneralDemo("/Users/leandro/huevadas/projects/dev/media/");
//    playground->run();
//    delete playground;
//
//    ObjDemoPlayground *objPlayground = new ObjDemoPlayground("/Users/leandro/huevadas/projects/dev/media/");
//    objPlayground->run();
//    delete objPlayground;
//
    CollisionDetectionPlayground *colDetplayground = new CollisionDetectionPlayground("/Users/leandro/huevadas/projects/dev/media/");
    colDetplayground->run();
    delete colDetplayground;

//    PlaygroundTerrainDemo * terrainPlayground = new PlaygroundTerrainDemo("/Users/leandro/huevadas/projects/dev/media/");
//    terrainPlayground->run();
//    delete terrainPlayground;
//
//    PhysicsPlayground *physicsPlayground = new PhysicsPlayground("/Users/leandro/huevadas/projects/dev/media/");
//    physicsPlayground->run();
//    delete physicsPlayground;

//    RoadFighter *roadFighter = new RoadFighter("/Users/leandro/huevadas/projects/dev/media/");
//    roadFighter->run();
//    delete roadFighter;

    printf("done\n");
    return 0;
}

//GeometryResource buildSphereGeometry()
//{
//	GeometryResource resource(0);
//
//	real dFi = radian(10);
//	real dTita = radian(10);
//	real radius = 1;
//
//	real oneOverTwoPi = 1.0f / (2.0f * M_PI);
//	real oneOverPi = 1.0f / M_PI;
//
//	for(real fi = 0; fi < radian(360) - dFi; fi += dFi)
//	{
//		for(real tita = 0; tita < radian(180) - dTita; tita+= dTita)
//		{
//			vector2 texel1 = vector2(fi * oneOverTwoPi,   tita * oneOverPi);
//			vector2 texel2 = vector2(fi * oneOverTwoPi,   (tita + dTita) * oneOverPi);
//			vector2 texel3 = vector2((fi + dFi) * oneOverTwoPi,   tita * oneOverPi);
//			vector2 texel4 = vector2((fi + dFi) * oneOverTwoPi,   (tita + dTita) * oneOverPi);
//
//			vector3 vertex1 = radius * vector3(radius * sin(tita) * sin(fi), radius * cos(tita), radius * sin(tita) * cos(fi));
//			vector3 vertex2 = radius * vector3(radius * sin(tita + dTita) * sin(fi), radius * cos(tita + dTita), radius * sin(tita + dTita) * cos(fi));
//			vector3 vertex3 = radius * vector3(radius * sin(tita) * sin(fi + dFi), radius * cos(tita), radius * sin(tita) * cos(fi + dFi));
//			vector3 vertex4 = radius * vector3(radius * sin(tita + dTita) * sin(fi + dFi), radius * cos(tita + dTita), radius * sin(tita + dTita) * cos(fi + dFi));
//
//			resource.getVertices().push_back(vertex1);
//			resource.getNormals().push_back(VectorUtilities::normalizar(vertex1));
//			resource.getTextureCoordinates().push_back(texel1);
//			resource.getIndices().push_back(resource.getIndices().size());
//			resource.getColors().push_back(vector3(1.0, 1.0, 1.0));
//
//			resource.getVertices().push_back(vertex2);
//			resource.getNormals().push_back(VectorUtilities::normalizar(vertex2));
//			resource.getTextureCoordinates().push_back(texel2);
//			resource.getIndices().push_back(resource.getIndices().size());
//			resource.getColors().push_back(vector3(1.0, 1.0, 1.0));
//
//			resource.getVertices().push_back(vertex3);
//			resource.getNormals().push_back(VectorUtilities::normalizar(vertex3));
//			resource.getTextureCoordinates().push_back(texel3);
//			resource.getIndices().push_back(resource.getIndices().size());
//			resource.getColors().push_back(vector3(1.0, 1.0, 1.0));
//
//			resource.getVertices().push_back(vertex3);
//			resource.getNormals().push_back(VectorUtilities::normalizar(vertex3));
//			resource.getTextureCoordinates().push_back(texel3);
//			resource.getIndices().push_back(resource.getIndices().size());
//			resource.getColors().push_back(vector3(1.0, 1.0, 1.0));
//
//
//			resource.getVertices().push_back(vertex2);
//			resource.getNormals().push_back(VectorUtilities::normalizar(vertex2));
//			resource.getTextureCoordinates().push_back(texel2);
//			resource.getIndices().push_back(resource.getIndices().size());
//			resource.getColors().push_back(vector3(1.0, 1.0, 1.0));
//
//			resource.getVertices().push_back(vertex4);
//			resource.getNormals().push_back(VectorUtilities::normalizar(vertex4));
//			resource.getTextureCoordinates().push_back(texel4);
//			resource.getIndices().push_back(resource.getIndices().size());
//			resource.getColors().push_back(vector3(1.0, 1.0, 1.0));
//		}
//	}
//
//	return resource;
//}

