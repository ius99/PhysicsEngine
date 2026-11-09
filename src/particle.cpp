#include "cyclone/particle.h"
#include <assert.h>

using namespace cyclone;

Particle::Particle() {
    damping = 0.99;
    inverseMass = 1;
}

void Particle::integrate(real duration) {
    if (inverseMass <= 0) {
        return;
    }

    assert(duration > 0);

    position.addScaledVector(velocity, duration);

    Vector3 resultingAcc = acceleration;
    resultingAcc.addScaledVector(forceAccum, inverseMass);

    velocity.addScaledVector(resultingAcc, duration);

    velocity *= real_pow(damping, duration);

    clearAccumulator();
}

void Particle::setMass(real mass) {
    assert(mass != 0);
    inverseMass = ((real)1) / mass;
}

real Particle::getMass() {
    if (inverseMass == 0) {
        return REAL_MAX;
    }
    return ((real)1) / inverseMass;
}

void Particle::setInverseMass(real inverseMass) {
    this->inverseMass = inverseMass;
}

real Particle::getInverseMass() {
    return inverseMass;
}

bool Particle::hasFiniteMass() {
    return inverseMass >= 0;
}

void Particle::setDamping(real damping) {
    this->damping = damping;
}

real Particle::getDamping() {
    return damping;
}

void Particle::setPosition(const Vector3& position) {
    this->position = position;
}

void Particle::setPosition(real x, real y, real z) {
    position.x = x;
    position.y = y;
    position.z = z;
}

Vector3 Particle::getPosition() {
    return position;
}

void Particle::setVelocity(const Vector3& velocity) {
    this->velocity = velocity;
}

void Particle::setVelocity(real x, real y, real z) {
    velocity.x = x;
    velocity.y = y;
    velocity.z = z;
}

Vector3 Particle::getVelocity() {
    return velocity;
}

void Particle::setAcceleration(const Vector3& acceleration) {
    this->acceleration = acceleration;
}

void Particle::setAcceleration(real x, real y, real z) {
    acceleration.x = x;
    acceleration.y = y;
    acceleration.z = z;
}

Vector3 Particle::getAcceleration() {
    return acceleration;
}

void Particle::clearAccumulator() {
    forceAccum.clear();
}

void Particle::addForce(const Vector3& force) {
    forceAccum += force;
}
