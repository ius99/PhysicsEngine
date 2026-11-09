#ifndef CYCLONE_PLINKS_H
#define CYCLONE_PLINKS_H

#include "cyclone/pcontacts.h"

namespace cyclone {

    class ParticleLink : public ParticleContactGenerator {
        public:
        Particle* particle[2];

        protected:
        real currentLength();

        public:
        virtual int addContact(ParticleContact* contact, int limit) = 0;
    };

    class ParticleCable : public ParticleLink {
        public:
        real maxLength;
        real restitution;

        virtual int addContact(ParticleContact* contact, int limit);
    };

    class ParticleRod : public ParticleLink {
        public:
        real length;

        virtual int addContact(ParticleContact* contact, int limit);
    };

    class ParticleConstraint : public ParticleContactGenerator {
        public:
        Particle* particle;
        Vector3 anchor;

        protected:
        real currentLength();

        public:
        virtual int addContact(ParticleContact* contact, int limit) = 0;
    };

    class ParticleCableConstraint : public ParticleConstraint {
        public:
        real maxLength;
        real restitution;

        virtual int addContact(ParticleContact* contact, int limit);
    };

    class ParticleRodConstraint : public ParticleConstraint {
        public:
        real length;

        virtual int addContact(ParticleContact* contact, int limit);
    };

}

#endif