#ifndef CYCLONE_PWORLD_H
#define CYCLONE_PWORLD_H

#include "cyclone/pfgen.h"
#include "cyclone/plinks.h"
#include <vector>

namespace cyclone {

    class ParticleWorld {
        protected:
        std::vector<Particle*> particles;
        std::vector<ParticleContactGenerator*> contactGenerators;
        ParticleForceRegistry registry;
        ParticleContactResolver resolver;
        std::vector<ParticleContact> contacts;
        int maxContacts;
        bool calculateIterations;

        public:
        ParticleWorld(int maxContacts, int iterations = 0);

        void startFrame();
        int generateContacts();
        void integrate(real duration);
        void runPhysics(real duration);

        std::vector<Particle*>& getParticles();
        std::vector<ParticleContactGenerator*>& getContactGenerators();
        ParticleForceRegistry& getForceRegistry();
    };

    class GroundContacts : public ParticleContactGenerator {
        std::vector<Particle*>* particles;
        real restitution;

        public:
        void init(std::vector<Particle*>* particles, real restitution = 0.5);
        virtual int addContact(ParticleContact* contact, int limit);
    };

}

#endif