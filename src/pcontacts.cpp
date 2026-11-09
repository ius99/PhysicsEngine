#include "cyclone/pcontacts.h"

using namespace cyclone;

void ParticleContact::resolve(real duration) {
    resolveVelocity(duration);
    resolveInterpenetration(duration);
}

real ParticleContact::calculateSeparatingVelocity() {
    Vector3 relativeVelocity = particle[0]->getVelocity();
    if (particle[1]) {
        relativeVelocity -= particle[1]->getVelocity();
    }
    return relativeVelocity.scalarProduct(contactNormal);
}

void ParticleContact::resolveVelocity(real duration) {
    real separatingVelocity = calculateSeparatingVelocity();

    if (separatingVelocity > 0) {
        return;
    }

    real newSepVelocity = -separatingVelocity * restitution;

    Vector3 accCausedVelocity = particle[0]->getAcceleration();
    if (particle[1]) {
        accCausedVelocity -= particle[1]->getAcceleration();
    }
    real accCausedSepVelocity = accCausedVelocity.scalarProduct(contactNormal) * duration;

    if (accCausedSepVelocity < 0) {
        newSepVelocity += restitution * accCausedSepVelocity;
        if (newSepVelocity < 0) {
            newSepVelocity = 0;
        }
    }

    real deltaVelocity = newSepVelocity - separatingVelocity;

    real totalInverseMass = particle[0]->getInverseMass();
    if (particle[1]) {
        totalInverseMass += particle[1]->getInverseMass();
    }

    if (totalInverseMass <= 0) {
        return;
    }

    real impulse = deltaVelocity / totalInverseMass;
    Vector3 impulsePerIMass = contactNormal * impulse;

    particle[0]->setVelocity(particle[0]->getVelocity() + impulsePerIMass * particle[0]->getInverseMass());

    if (particle[1]) {
        particle[1]->setVelocity(particle[1]->getVelocity() + impulsePerIMass * -particle[1]->getInverseMass());
    }
}

void ParticleContact::resolveInterpenetration(real duration) {
    if (penetration <= 0) {
        return;
    }

    real totalInverseMass = particle[0]->getInverseMass();
    if (particle[1]) {
        totalInverseMass += particle[1]->getInverseMass();
    }

    if (totalInverseMass <= 0) {
        return;
    }

    Vector3 movePerIMass = contactNormal * (penetration / totalInverseMass);

    particleMovement[0] = movePerIMass * particle[0]->getInverseMass();
    if (particle[1]) {
        particleMovement[1] = movePerIMass * -particle[1]->getInverseMass();
    }
    else {
        particleMovement[1].clear();
    }

    particle[0]->setPosition(particle[0]->getPosition() + particleMovement[0]);
    if (particle[1]) {
        particle[1]->setPosition(particle[1]->getPosition() + particleMovement[1]);
    }
}

ParticleContactResolver::ParticleContactResolver(int iterations) {
    this->iterations = iterations;
    iterationsUsed = 0;
}

void ParticleContactResolver::setIterations(int iterations) {
    this->iterations = iterations;
}

void ParticleContactResolver::resolveContacts(ParticleContact* contactArray, int numContacts, real duration) {
    iterationsUsed = 0;

    while (iterationsUsed < iterations) {
        real max = REAL_MAX;
        int maxIndex = numContacts;

        for (int i = 0; i < numContacts; i++) {
            real sepVel = contactArray[i].calculateSeparatingVelocity();
            if (sepVel < max && (sepVel < 0 || contactArray[i].penetration > 0)) {
                max = sepVel;
                maxIndex = i;
            }
        }

        if (maxIndex == numContacts) {
            break;
        }

        for (int i = 0; i < 2; i++) {
            contactArray[maxIndex].particleMovement[i].clear();
        }

        contactArray[maxIndex].resolve(duration);

        Vector3* move = contactArray[maxIndex].particleMovement;
        Particle* moved0 = contactArray[maxIndex].particle[0];
        Particle* moved1 = contactArray[maxIndex].particle[1];

        for (int i = 0; i < numContacts; i++) {
            ParticleContact& c = contactArray[i];

            if (c.particle[0] == moved0) {
                c.penetration -= move[0].scalarProduct(c.contactNormal);
            }
            else if (moved1 && c.particle[0] == moved1) {
                c.penetration -= move[1].scalarProduct(c.contactNormal);
            }

            if (c.particle[1]) {
                if (c.particle[1] == moved0) {
                    c.penetration += move[0].scalarProduct(c.contactNormal);
                }
                else if (moved1 && c.particle[1] == moved1) {
                    c.penetration += move[1].scalarProduct(c.contactNormal);
                }
            }
        }

        iterationsUsed++;
    }
}