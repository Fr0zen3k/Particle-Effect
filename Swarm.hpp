//
//  Swarm.hpp
//  Particle Effect
//
//  Version 1.1
//
//  Created by Jan Kresic on 9/11/19.
//  Copyright © 2019 Jan Kresic. All rights reserved.
//

#ifndef Swarm_hpp
#define Swarm_hpp

#include "Particle.hpp"

namespace jk {
    
    class Swarm {
    public:
        //Fixed number of particles generated on the screen
        const static int NUM_PARTICLES = 1250;
        
    private:
        //Pointer to an array of all particles on the screen
        Particle * m_pParticles;
        
    public:
        //Constructor and destructor dinamically initiate and delete the array of particles
        Swarm();
        ~Swarm();
        
        /*
         Returns a constant pointer to the array of particles.
         The returned pointer cannot be changed, nor can the array of particles be changed trough it.
        */
        const Particle * const getParticles() { return m_pParticles; };
        //Calls the Particle::move() method on all the particles of the array
        void move(int elapsed);
    };
    
}
#endif /* Swarm_hpp */
