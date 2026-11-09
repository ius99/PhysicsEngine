#ifndef CYCLONE_PCONTACTS_H
#define CYCLONE_PCONTACTS_H

#include "cyclone/particle.h"

namespace cyclone {

    class ParticleContact {
        public:
        Particle* particle[2];
        real restitution;
        Vector3 contactNormal;
        real penetration;
        Vector3 particleMovement[2];

        void resolve(real duration);
        real calculateSeparatingVelocity();

        private:
        void resolveVelocity(real duration);
        void resolveInterpenetration(real duration);
    };

    class ParticleContactResolver {
        protected:
        int iterations;
        int iterationsUsed;

        public:
        ParticleContactResolver(int iterations);
        void setIterations(int iterations);
        void resolveContacts(ParticleContact* contactArray, int numContacts, real duration);
    };

    class ParticleContactGenerator {
        public:
        virtual int addContact(ParticleContact* contact, int limit) = 0;
        virtual ~ParticleContactGenerator() {}
    };

}

#endif