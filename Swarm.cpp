//
//  Swarm.cpp
//  Particle Effect
//
//  Version 1.1
//
//  Created by Jan Kresic on 9/11/19.
//  Copyright © 2019 Jan Kresic. All rights reserved.
//

#include "Swarm.hpp"

namespace jk {
    
    Swarm::Swarm() {
        m_pParticles = new Particle[NUM_PARTICLES];
        
    }
    
    Swarm::~Swarm() {
        delete [] m_pParticles;
    }
    
    void Swarm::move(int elapsed) {
        for(int i = 0; i < NUM_PARTICLES; i++) {
            m_pParticles[i].move(elapsed);
        }
    }
    
}
