//
//  Particle.hpp
//  Particle Effect
//
//  Created by Jan Kresic on 9/11/19.
//  Copyright © 2019 Jan Kresic. All rights reserved.
//

#ifndef Particle_hpp
#define Particle_hpp

namespace jk {
    
    struct Particle {
        //X and Y coordinates.
        double m_x;
        double m_y;
        
        //X and Y speed.
        double m_xSpeed;
        double m_ySpeed;
        
    public:
        /*
          Constructor that procadurally genetares velocity by generating random angle and speed, and initiates
          the particle at the centre of the screen.
        */
        Particle();
        ~Particle();
        
        /*
          Moves the particle by changing m_x and m_y depending on m_xSpeed and m_ySpeed.
          Makes sure to invert x or y speed if the particle touches the edge of the screen, causing
          the particle to bounce.
        */
        void move(int elapsed);
    };
    
}
#endif /* Particle_hpp */
