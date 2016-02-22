#include "ParticleSystem.h"

//------------------------------------------------------------------
ParticleSystem::ParticleSystem(int n){
    for(int i=0; i<n; i++){
      particles.push_back(new Particle());
    }
}

//------------------------------------------------------------------
ParticleSystem::ParticleSystem(int n, float ld){
    nParticles = n;
    for(int i=0; i<n; i++){
        particles.push_back(new Particle(ld));
    }
}

//------------------------------------------------------------------
void ParticleSystem::update(){
    for(int i = 0; i < particles.size(); i++){
        particles[i]->update();
    }
}

//------------------------------------------------------------------
void ParticleSystem::updateLifedec(float ld){
    for(int i = 0; i < particles.size(); i++){
        particles[i]->updateLifedec(ld);
    }
}

//------------------------------------------------------------------
void ParticleSystem::updatePartSizeref(float ps){
    for(int i = 0; i < particles.size(); i++){
        particles[i]->updatePartSizeref(ps);
    }
}

//------------------------------------------------------------------
void ParticleSystem::setEmitter(float x, float y){
    for(int i = 0; i < particles.size(); i++){
        if(particles[i]->isDead()){
            particles[i]->rebirth(x, y);
        }
    }
}

//------------------------------------------------------------------
void ParticleSystem::draw(){
    for(int i = 0; i < particles.size(); i++){
        particles[i]->draw();
    }
}
