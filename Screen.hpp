//
//  Screen.hpp
//  Particle Effect
//
//  Created by Jan Kresic on 9/11/19.
//  Copyright © 2019 Jan Kresic. All rights reserved.
//

#ifndef Screen_hpp
#define Screen_hpp

#include <SDL2/SDL.h>

namespace jk {
    
    class Screen {
    public:
        //Constants holding the dimensions of the screen in pixels
        const static int SCREEN_WIDTH = 1600;
        const static int SCREEN_HEIGHT = 900;
        
    private:
        SDL_Window *m_window;
        SDL_Renderer *m_renderer;
        SDL_Texture *m_texture;
        Uint32 *m_buffer1;
        Uint32 *m_buffer2;
        
    public:
        //Initiates all variables to NULL
        Screen();
        
        /*
         Method used to start the screen.
         The method starts SDL, as well as SDL_video, creates the texture and renderer, as well as dinamically initiating the buffers holding color information for all the pixels on the screen, and setting them to 0x00000000 (RGBA black).
         Returns false if one of the SDL components failed to initiate, true otherwise.
         */
        bool init();
        
        /*
         Updates m_texture with m_buffer1, updates m_renderer with m_texture, and renders the renderer on the screen.
         */
        void update();
        
        /*
         Sets a pixel in m_buffer1 representing the pixel on (x, y) on the screen to given red, green and blue value.
         */
        void setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue);
        
        /*
         Uses the SDL Event system to check if the screen was quit.
         Returns false if the screen was quit, true otherwise.
         */
        bool processEvents();
        
        /*
         Closes all the SDL elements and deallocates the memory used by the two buffers.
         */
        void close();
        
        /*
         Sets all the values in the primary buffer to 0x00000000 (RBGA black).
         */
        void clear();
        
        /*
         Implements the box blur effect algorithm to each pixel of the screen.
         The algorithm takes the values from m_buffer1, puts it into m_buffer2, and calculates a new pixel values in m_buffer1 based on tha last frame.
         The algoritm iterates trough all the adjacent pixels for each pixel on the screen and sets the pixel value to the average color between the adjacent 9(or less) pixels.
         */
        void boxBlur();
    };
    
}
#endif /* Screen_hpp */
