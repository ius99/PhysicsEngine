#ifndef CYCLONE_PARTICLE_H
#define CYCLONE_PARTICLE_H

#include "cyclone/core.h"

namespace cyclone {

    class Particle {
        protected:
        Vector3 position;
        Vector3 velocity;
        Vector3 acceleration;
        Vector3 forceAccum;
        real damping;
        real inverseMass;

        public:
        Particle();

        void integrate(real duration);

        void setMass(real mass);
        real getMass();
        void setInverseMass(real inverseMass);
        real getInverseMass();
        bool hasFiniteMass();

        void setDamping(real damping);
        real getDamping();

        void setPosition(const Vector3& position);
        void setPosition(real x, real y, real z);
        Vector3 getPosition();

        void setVelocity(const Vector3& velocity);
        void setVelocity(real x, real y, real z);
        Vector3 getVelocity();

        void setAcceleration(const Vector3& acceleration);
        void setAcceleration(real x, real y, real z);
        Vector3 getAcceleration();

        void clearAccumulator();
        void addForce(const Vector3& force);
    };

}

#endif