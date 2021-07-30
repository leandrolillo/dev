/*
 * CollisionDetector.h
 *
 *  Created on: Apr 5, 2021
 *      Author: leandro
 */

#ifndef SRC_PHYSICS_COLLISIONDETECTOR_H_
#define SRC_PHYSICS_COLLISIONDETECTOR_H_

#define sphere_radius 0.1

#include<vector>
#include<ParticleContact.h>
#include<Particle.h>

#include <Geometry.h>
#include <IntersectionTester.h>


class CollisionDetector {
    std::vector <const Geometry *>scenery;
    std::unique_ptr<IntersectionTester> intersectionTester;
public:
    CollisionDetector() {
        this->intersectionTester = std::unique_ptr<IntersectionTester>(new IntersectionTester());
    }
    virtual ~CollisionDetector() {
    }

    void setIntersectionTester(IntersectionTester *intersectionTester) {
        this->intersectionTester.reset(intersectionTester);
    }

    void addScenery(const Geometry *scenery) {
        this->scenery.push_back(scenery);
    }

    virtual std::vector<ParticleContact>detectCollisions(const std::vector<Particle *> &particles) const {
            std::vector<ParticleContact> contacts;

            for(std::vector<Particle *>::const_iterator iteratorA = particles.begin(); iteratorA != particles.end(); iteratorA++) {
                Particle *particleA = *iteratorA;
                if(particleA->getStatus() && particleA->getGeometry() != null) {
                    for(std::vector<const Geometry *>::const_iterator sceneryIterator = scenery.begin();
                            sceneryIterator != scenery.end();
                            sceneryIterator ++)  {

                        std::vector<GeometryContact> pairContacts = intersectionTester->detectCollision(*particleA->getGeometry(), **sceneryIterator);
                        if(!pairContacts.empty()) {
                            std::transform(pairContacts.begin(), pairContacts.end(), std::back_inserter(contacts),
                                    [&particleA](GeometryContact pairContact) -> ParticleContact {
                                    return ParticleContact(particleA,
                                            null,
                                            pairContact.getIntersection(),
                                            pairContact.getNormal(),
                                            pairContact.getRestitution(),
                                            pairContact.getPenetration());
                            });

                            particleA->onCollision(contacts.back());
                        }
                    }

                    for(std::vector<Particle *>::const_iterator iteratorB = iteratorA+1; iteratorB != particles.end(); iteratorB++) {
                        Particle *particleB = *iteratorB;
                        if(particleB->getStatus() && particleB->getGeometry() != null) {
                            std::vector<GeometryContact> pairContacts = intersectionTester->detectCollision(*particleA->getGeometry(), *particleB->getGeometry());
                            if(!pairContacts.empty()) {
                                std::transform(pairContacts.begin(), pairContacts.end(), std::back_inserter(contacts),
                                        [&particleA, &particleB](GeometryContact pairContact) -> ParticleContact {
                                        return ParticleContact(particleA,
                                                particleB,
                                                pairContact.getIntersection(),
                                                pairContact.getNormal(),
                                                pairContact.getRestitution(),
                                                pairContact.getPenetration());
                                });

                                particleA->onCollision(contacts.back());
                                particleB->onCollision(contacts.back());
                            }
                        }
                    }
                }
            }

            return contacts;

        }
};


#endif /* SRC_PHYSICS_COLLISIONDETECTOR_H_ */
