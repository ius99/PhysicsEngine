#include "cyclone/pworld.h"

using namespace cyclone;

ParticleWorld::ParticleWorld(int maxContacts, int iterations) : resolver(iterations) {
    this->maxContacts = maxContacts;
    contacts.resize(maxContacts);
    calculateIterations = (iterations == 0);
}

void ParticleWorld::startFrame() {
    for (int i = 0; i < particles.size(); i++) {
        particles[i]->clearAccumulator();
    }
}

int ParticleWorld::generateContacts() {
    int limit = maxContacts;
    int used = 0;

    for (int i = 0; i < contactGenerators.size(); i++) {
        if (limit <= 0) {
            break;
        }
        int added = contactGenerators[i]->addContact(&contacts[used], limit);
        used += added;
        limit -= added;
    }

    return used;
}

void ParticleWorld::integrate(real duration) {
    for (int i = 0; i < particles.size(); i++) {
        particles[i]->integrate(duration);
    }
}

void ParticleWorld::runPhysics(real duration) {
    registry.updateForces(duration);

    integrate(duration);

    int usedContacts = generateContacts();

    if (usedContacts > 0) {
        if (calculateIterations) {
            resolver.setIterations(usedContacts * 2);
        }
        resolver.resolveContacts(&contacts[0], usedContacts, duration);
    }
}

std::vector<Particle*>& ParticleWorld::getParticles() {
    return particles;
}

std::vector<ParticleContactGenerator*>& ParticleWorld::getContactGenerators() {
    return contactGenerators;
}

ParticleForceRegistry& ParticleWorld::getForceRegistry() {
    return registry;
}

void GroundContacts::init(std::vector<Particle*>* particles, real restitution) {
    this->particles = particles;
    this->restitution = restitution;
}

int GroundContacts::addContact(ParticleContact* contact, int limit) {
    int count = 0;

    for (int i = 0; i < particles->size(); i++) {
        real y = (*particles)[i]->getPosition().y;

        if (y < 0) {
            contact->contactNormal = Vector3(0, 1, 0);
            contact->particle[0] = (*particles)[i];
            contact->particle[1] = 0;
            contact->penetration = -y;
            contact->restitution = restitution;
            contact++;
            count++;
        }

        if (count >= limit) {
            return count;
        }
    }

    return count;
}