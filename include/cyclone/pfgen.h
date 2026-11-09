#ifndef CYCLONE_PFGEN_H
#define CYCLONE_PFGEN_H

#include "cyclone/particle.h"
#include <vector>

namespace cyclone {

    class ParticleForceGenerator {
        public:
        virtual void updateForce(Particle* particle, real duration) = 0;
        virtual ~ParticleForceGenerator() {}
    };

    class ParticleForceRegistry {
        protected:
        struct ParticleForceRegistration {
            Particle* particle;
            ParticleForceGenerator* fg;
        };

        std::vector<ParticleForceRegistration> registrations;

        public:
        void add(Particle* particle, ParticleForceGenerator* fg);
        void remove(Particle* particle, ParticleForceGenerator* fg);
        void removeParticle(Particle* particle);
        void clear();
        void updateForces(real duration);
    };

    class ParticleGravity : public ParticleForceGenerator {
        Vector3 gravity;

        public:
        ParticleGravity(const Vector3& gravity);
        virtual void updateForce(Particle* particle, real duration);
    };

    class ParticleDrag : public ParticleForceGenerator {
        real k1;
        real k2;

        public:
        ParticleDrag(real k1, real k2);
        virtual void updateForce(Particle* particle, real duration);
    };

    class ParticleSpring : public ParticleForceGenerator {
        Particle* other;
        real springConstant;
        real restLength;

        public:
        ParticleSpring(Particle* other, real springConstant, real restLength);
        virtual void updateForce(Particle* particle, real duration);
    };

    class ParticleAnchoredSpring : public ParticleForceGenerator {
        protected:
        Vector3 anchor;
        real springConstant;
        real restLength;

        public:
        ParticleAnchoredSpring(const Vector3& anchor, real springConstant, real restLength);
        Vector3 getAnchor();
        void setAnchor(const Vector3& anchor);
        virtual void updateForce(Particle* particle, real duration);
    };

    class ParticleBungee : public ParticleForceGenerator {
        Particle* other;
        real springConstant;
        real restLength;

        public:
        ParticleBungee(Particle* other, real springConstant, real restLength);
        virtual void updateForce(Particle* particle, real duration);
    };

    class ParticleBuoyancy : public ParticleForceGenerator {
        real maxDepth;
        real volume;
        real waterHeight;
        real liquidDensity;

        public:
        ParticleBuoyancy(real maxDepth, real volume, real waterHeight, real liquidDensity = 1000.0);
        virtual void updateForce(Particle* particle, real duration);
    };

}

#endif