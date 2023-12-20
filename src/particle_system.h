// particle_system.h

#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <raylib.h>

#define MAX_PARTICLES 40
#define MAX_EXPLOSIONS 5

typedef struct {
    Vector2 position;
    Vector2 speed;
    Color color;
    int life;
    int inUse;  // Flag to indicate whether the particle is in use
} Particle;

typedef struct {
    Vector2 position;
    Particle particles[MAX_PARTICLES];
} Explosion;

void InitializeExplosion(Vector2 position);
void InitializeParticles(int explosionIndex);
void UpdateExplosions(void);
void DrawExplosions(void);

#endif // PARTICLE_SYSTEM_H
