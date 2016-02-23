#include "ParticleSystem.h"

//------------------------------------------------------------------
ParticleSystem::ParticleSystem(int n){
    for(int i=0; i<n; i++){
      particles.push_back(new Particle());
    }
    pos.set(0, 0);
}

//------------------------------------------------------------------
ParticleSystem::ParticleSystem(int n, float ld){
    nParticles = n;
    for(int i=0; i<n; i++){
        particles.push_back(new Particle(ld));
    }
    pos.set(0, 0);
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
void ParticleSystem::updateGravity(ofVec2f g){
    for(int i = 0; i < particles.size(); i++){
        particles[i]->updateGravity(g);
    }
}

//------------------------------------------------------------------
void ParticleSystem::updateWithAngle(){
    float angle = ofNoise(pos.x * 0.1, pos.y * 0.01, 0);
    ofVec2f vel = ofVec2f(ofRandom(-0.1, 0.1),  ofRandom(-0.1, 0.1));
    vel.rotate(angle);
    pos += vel;
}

//------------------------------------------------------------------
void ParticleSystem::updateVelocity(ofVec2f v){
    for(int i = 0; i < particles.size(); i++){
        particles[i]->updateVelocity(v);
    }
}

//------------------------------------------------------------------
void ParticleSystem::setEmitter(float x, float y){
    for(int i = 0; i < particles.size(); i++){
        if(particles[i]->isDead()){
            particles[i]->rebirth(x, y);
        }
    }
    pos.set(x, y);
}

//------------------------------------------------------------------
void ParticleSystem::draw(){
    for(int i = 0; i < particles.size(); i++){
        particles[i]->draw();
    }
}
