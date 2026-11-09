#include "cyclone/pfgen.h"

using namespace cyclone;

void ParticleForceRegistry::add(Particle* particle, ParticleForceGenerator* fg) {
    ParticleForceRegistration registration;
    registration.particle = particle;
    registration.fg = fg;
    registrations.push_back(registration);
}

void ParticleForceRegistry::remove(Particle* particle, ParticleForceGenerator* fg) {
    for (int i = 0; i < registrations.size(); i++) {
        if (registrations[i].particle == particle && registrations[i].fg == fg) {
            registrations.erase(registrations.begin() + i);
            return;
        }
    }
}

void ParticleForceRegistry::removeParticle(Particle* particle) {
    for (int i = registrations.size() - 1; i >= 0; i--) {
        if (registrations[i].particle == particle) {
            registrations.erase(registrations.begin() + i);
        }
    }
}

void ParticleForceRegistry::clear() {
    registrations.clear();
}

void ParticleForceRegistry::updateForces(real duration) {
    for (int i = 0; i < registrations.size(); i++) {
        registrations[i].fg->updateForce(registrations[i].particle, duration);
    }
}

ParticleGravity::ParticleGravity(const Vector3& gravity) {
    this->gravity = gravity;
}

void ParticleGravity::updateForce(Particle* particle, real duration) {
    if (!particle->hasFiniteMass()) {
        return;
    }
    particle->addForce(gravity * particle->getMass());
}

ParticleDrag::ParticleDrag(real k1, real k2) {
    this->k1 = k1;
    this->k2 = k2;
}

void ParticleDrag::updateForce(Particle* particle, real duration) {
    Vector3 force = particle->getVelocity();

    real dragCoeff = force.magnitude();
    dragCoeff = k1 * dragCoeff + k2 * dragCoeff * dragCoeff;

    force.normalize();
    force *= -dragCoeff;
    particle->addForce(force);
}

ParticleSpring::ParticleSpring(Particle* other, real springConstant, real restLength) {
    this->other = other;
    this->springConstant = springConstant;
    this->restLength = restLength;
}

void ParticleSpring::updateForce(Particle* particle, real duration) {
    Vector3 force = particle->getPosition();
    force -= other->getPosition();

    real magnitude = force.magnitude();
    magnitude = real_abs(magnitude - restLength);
    magnitude *= springConstant;

    force.normalize();
    force *= -magnitude;
    particle->addForce(force);
}

ParticleAnchoredSpring::ParticleAnchoredSpring(const Vector3& anchor, real springConstant, real restLength) {
    this->anchor = anchor;
    this->springConstant = springConstant;
    this->restLength = restLength;
}

Vector3 ParticleAnchoredSpring::getAnchor() {
    return anchor;
}

void ParticleAnchoredSpring::setAnchor(const Vector3& anchor) {
    this->anchor = anchor;
}

void ParticleAnchoredSpring::updateForce(Particle* particle, real duration) {
    Vector3 force = particle->getPosition();
    force -= anchor;

    real magnitude = force.magnitude();
    magnitude = (restLength - magnitude) * springConstant;

    force.normalize();
    force *= magnitude;
    particle->addForce(force);
}

ParticleBungee::ParticleBungee(Particle* other, real springConstant, real restLength) {
    this->other = other;
    this->springConstant = springConstant;
    this->restLength = restLength;
}

void ParticleBungee::updateForce(Particle* particle, real duration) {
    Vector3 force = particle->getPosition();
    force -= other->getPosition();

    real magnitude = force.magnitude();
    if (magnitude <= restLength) {
        return;
    }

    magnitude = springConstant * (restLength - magnitude);

    force.normalize();
    force *= magnitude;
    particle->addForce(force);
}

ParticleBuoyancy::ParticleBuoyancy(real maxDepth, real volume, real waterHeight, real liquidDensity) {
    this->maxDepth = maxDepth;
    this->volume = volume;
    this->waterHeight = waterHeight;
    this->liquidDensity = liquidDensity;
}

void ParticleBuoyancy::updateForce(Particle* particle, real duration) {
    real depth = particle->getPosition().y;

    if (depth >= waterHeight + maxDepth) {
        return;
    }

    Vector3 force(0, 0, 0);

    if (depth <= waterHeight - maxDepth) {
        force.y = liquidDensity * volume;
    }
    else {
        force.y = liquidDensity * volume * (waterHeight + maxDepth - depth) / (2 * maxDepth);
    }

    particle->addForce(force);
}