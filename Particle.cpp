//
//  Particle.cpp
//  Particle Effect
//
//  Created by Jan Kresic on 9/11/19.
//  Copyright © 2019 Jan Kresic. All rights reserved.
//

#include "Particle.hpp"
#include <stdlib.h>
#include "Screen.hpp"

namespace jk {
    
    Particle::Particle(): m_x(Screen::SCREEN_WIDTH / 2), m_y(Screen::SCREEN_HEIGHT / 2) {
        double speed = ((4.5 * rand())/RAND_MAX);
        double angle = 2 * M_PI * ((1.0 * rand())/RAND_MAX);
        speed *= speed;
        m_xSpeed = speed * cos(angle);
        m_ySpeed = speed * sin(angle);
    }
    
    Particle::~Particle() {

    }
    
    void Particle::move() {
        
        if(m_x < 1 || m_x > Screen::SCREEN_WIDTH - 2) {
            m_xSpeed = -m_xSpeed;
        }
        
        if(m_y < 1 || m_y > Screen::SCREEN_HEIGHT - 3) {
            m_ySpeed = -m_ySpeed;
        }
        
        m_x += m_xSpeed;
        m_y += m_ySpeed;
    }
}
