//
//  main.cpp
//  Particle Effect
//
//  Version 1.1
//
//  Created by Jan Kresic on 9/2/19.
//  Copyright © 2019 Jan Kresic. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <cstdlib>

#include <SDL2/SDL.h>

#include "Particle.hpp"
#include "Screen.hpp"
#include "Swarm.hpp"

using namespace std;
using namespace jk;

int main(int argc, const char * argv[]) {
    
    //Seed the random number generation.
    srand(time(NULL) % 1000000);
    
    //Declares the screen.
    Screen screen;
    
    //Screen initiation and check if the screen was succesfully initiated, if not, exit the application.
    if (screen.init() == false) {
        cout << "Error initialising SDL." << endl;
        return 1;
    }
    
    Swarm swarm;
    
    /*
     Procedurally generated factors that determine the speed at which the particle color changes.
     Factors are generated between 0.0003 and 0.0013.
     */
    double randRed = ((1.0 * rand()) / RAND_MAX) * 0.001 + 0.0003;
    double randGreen = ((1.0 * rand()) / RAND_MAX) * 0.001 + 0.0003;
    double randBlue = ((1.0 * rand()) / RAND_MAX) * 0.001 + 0.0003;
    
    int ticks = 0;
    int lastTicks = 0;
    int elapsed = 0;
    
    //Game loop.
    for(long passes = 0; true; passes++) {
        
        lastTicks = ticks;
        ticks = SDL_GetTicks();
        elapsed = ticks - lastTicks;
        
        const Particle * const pParticles = swarm.getParticles();
        
        //Colors calculated trough sin, depending on the factors for each, and time from the SDL start
        Uint8 red = 128 * (sin(randRed * ticks) + 1);
        Uint8 green = 128 * (sin(randGreen * ticks) + 1);
        Uint8 blue = 128 * (sin(randBlue * ticks) + 1);
        
        /*
         Iterate trough all the particles in the swarm and draw them to the screen as pixels on the position of the parcitles, with the color values from the game loop.
         */
        for (int i = 0; i < Swarm::NUM_PARTICLES; i++) {
            Particle particle = pParticles[i];
            
            screen.setPixel(particle.m_x, particle.m_y, red, green, blue);
        }
        
        /*
         Move all the particles in the swarm every frame.
         */
        swarm.move(elapsed);
        
        /*
         Apply the box blur effect each frame, instead of clearing the frame, in order to get more then pure particle movement.
         */
        screen.boxBlur1();
        
        //Draw the screen.
        screen.update();
        
        /*
         Check for messages/events.
         If there was an event to quit the screen, break the game loop and exit the program.
         */
        if (screen.processEvents() == false) {
            break;
        }
        
    }
    
    //Deallocate all the memory and close all the SDL elements.
    screen.close();
    
    return 0;
}
