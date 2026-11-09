#include "raylib.h"
#include "cyclone/pworld.h"
#include <vector>

Vector3 toRay(cyclone::Vector3 v) {
    return { (float)v.x, (float)v.y, (float)v.z };
}

void removeFromWorld(cyclone::ParticleWorld& world, cyclone::Particle* p) {
    std::vector<cyclone::Particle*>& all = world.getParticles();
    for (int i = 0; i < all.size(); i++) {
        if (all[i] == p) {
            all.erase(all.begin() + i);
            return;
        }
    }
}

int main() {
    InitWindow(1280, 720, "PhysicsEngine");
    SetTargetFPS(60);

    Camera3D camera = { 0 };
    camera.position = { 22.0f, 12.0f, 22.0f };
    camera.target = { 0.0f, 3.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    cyclone::ParticleWorld world(200);

    cyclone::GroundContacts ground;
    ground.init(&world.getParticles(), 0.6);
    world.getContactGenerators().push_back(&ground);

    cyclone::Particle bob;
    bob.setMass(1.0);
    bob.setPosition(9, 8, 0);
    bob.setAcceleration(0, -9.81, 0);
    bob.setDamping(0.999);
    world.getParticles().push_back(&bob);

    cyclone::ParticleRodConstraint pendulum;
    pendulum.particle = &bob;
    pendulum.anchor = cyclone::Vector3(5, 8, 0);
    pendulum.length = 4;
    world.getContactGenerators().push_back(&pendulum);

    cyclone::Particle bellA;
    cyclone::Particle bellB;
    bellA.setMass(1.0);
    bellB.setMass(1.0);
    bellA.setAcceleration(0, -9.81, 0);
    bellB.setAcceleration(0, -9.81, 0);
    bellA.setDamping(0.99);
    bellB.setDamping(0.99);
    bellA.setPosition(-6, 0.5, 4);
    bellB.setPosition(-4, 0.5, 4);
    world.getParticles().push_back(&bellA);
    world.getParticles().push_back(&bellB);

    cyclone::ParticleRod dumbbell;
    dumbbell.particle[0] = &bellA;
    dumbbell.particle[1] = &bellB;
    dumbbell.length = 2;
    world.getContactGenerators().push_back(&dumbbell);

    std::vector<cyclone::Particle*> projectiles;
    int maxProjectiles = 20;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) {
            if (projectiles.size() >= maxProjectiles) {
                removeFromWorld(world, projectiles[0]);
                delete projectiles[0];
                projectiles.erase(projectiles.begin());
            }

            cyclone::Particle* p = new cyclone::Particle();
            p->setMass(2.0);
            p->setPosition(-10, 1, 0);
            p->setVelocity(8, 12, 0);
            p->setAcceleration(0, -9.81, 0);
            p->setDamping(0.99);
            projectiles.push_back(p);
            world.getParticles().push_back(p);
        }

        if (IsKeyPressed(KEY_D)) {
            bellA.setVelocity(4, 10, 0);
            bellB.setVelocity(2, 12, 0);
        }

        if (IsKeyPressed(KEY_P)) {
            bob.setPosition(9, 8, 0);
            bob.setVelocity(0, 0, 0);
        }

        cyclone::real duration = GetFrameTime();
        if (duration > 0) {
            world.startFrame();
            world.runPhysics(duration);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        DrawGrid(40, 1.0f);

        DrawCube(toRay(pendulum.anchor), 0.4f, 0.4f, 0.4f, DARKGRAY);
        DrawLine3D(toRay(pendulum.anchor), toRay(bob.getPosition()), BLACK);
        DrawSphere(toRay(bob.getPosition()), 0.4f, BLUE);

        DrawLine3D(toRay(bellA.getPosition()), toRay(bellB.getPosition()), BLACK);
        DrawSphere(toRay(bellA.getPosition()), 0.35f, DARKGREEN);
        DrawSphere(toRay(bellB.getPosition()), 0.35f, DARKGREEN);

        for (int i = 0; i < projectiles.size(); i++) {
            DrawSphere(toRay(projectiles[i]->getPosition()), 0.3f, RED);
        }
        EndMode3D();

        DrawText("SPACE fire ball   D toss dumbbell   P reset pendulum", 10, 10, 20, DARKGRAY);
        DrawText(TextFormat("Balls: %d", (int)projectiles.size()), 10, 40, 20, DARKGRAY);

        EndDrawing();
    }

    for (int i = 0; i < projectiles.size(); i++) {
        delete projectiles[i];
    }

    CloseWindow();
    return 0;
}