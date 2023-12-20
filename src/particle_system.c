// particle_system.c

#include "particle_system.h"
#include <stdlib.h>

Explosion explosions[MAX_EXPLOSIONS];

void InitializeExplosion(Vector2 position) {
    // Find an available explosion slot
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        if (explosions[i].particles[0].inUse == 0) {
            explosions[i].position = position;
            InitializeParticles(i);  // Initialize particles for this explosion
            break;
        }
    }
}

void InitializeParticles(int explosionIndex) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        explosions[explosionIndex].particles[i].position = explosions[explosionIndex].position;
        explosions[explosionIndex].particles[i].speed.x = (float)(GetRandomValue(-50, 50) / 40.0);
        explosions[explosionIndex].particles[i].speed.y = (float)(GetRandomValue(-50, 50) / 40.0);
        explosions[explosionIndex].particles[i].color = (Color){GetRandomValue(50, 255), GetRandomValue(50, 255), GetRandomValue(50, 255), 255};
        explosions[explosionIndex].particles[i].life = GetRandomValue(20, 60);
        explosions[explosionIndex].particles[i].inUse = 1;
    }
}

void UpdateExplosions(void) {
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        for (int j = 0; j < MAX_PARTICLES; j++) {
            if (explosions[i].particles[j].inUse && explosions[i].particles[j].life > 0) {
                explosions[i].particles[j].position.x += explosions[i].particles[j].speed.x;
                explosions[i].particles[j].position.y += explosions[i].particles[j].speed.y;
                explosions[i].particles[j].life--;

                if (explosions[i].particles[j].life <= 0) {
                    explosions[i].particles[j].inUse = 0;  // Mark particle as not in use when its life is over
                }
            }
        }
    }
}

void DrawExplosions(void) {
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        for (int j = 0; j < MAX_PARTICLES; j++) {
            if (explosions[i].particles[j].inUse && explosions[i].particles[j].life > 0) {
                DrawPixelV(explosions[i].particles[j].position, explosions[i].particles[j].color);
            }
        }
    }
}
