//
//  Screen.cpp
//  Particle Effect
//
//  Created by Jan Kresic on 9/11/19.
//  Copyright © 2019 Jan Kresic. All rights reserved.
//

#include "Screen.hpp"

namespace jk {
    
    Screen::Screen() :
    m_window(NULL), m_renderer(NULL), m_texture(NULL), m_buffer1(NULL), m_buffer2(NULL) {
        
    }
    
    bool Screen::init() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            return false;
        }
        
        m_window = SDL_CreateWindow("Particle Fire Explosion",
                                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                    SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        
        if (m_window == NULL) {
            SDL_Quit();
            return false;
        }
        
        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);
        m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888,
                                      SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);
        
        if (m_renderer == NULL) {
            SDL_DestroyWindow(m_window);
            SDL_Quit();
            return false;
        }
        
        if (m_texture == NULL) {
            SDL_DestroyRenderer(m_renderer);
            SDL_DestroyWindow(m_window);
            SDL_Quit();
            return false;
        }
        
        m_buffer1 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
        m_buffer2 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
        
        memset(m_buffer1, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
        memset(m_buffer2, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
        
        return true;
    }
    
    void Screen::setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue) {
        
        if(x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
            return;
        }
        
        Uint32 color = 0;
        
        color += red;
        color <<= 8;
        color += green;
        color <<= 8;
        color += blue;
        color <<= 8;
        color += 0xFF;
        
        m_buffer1[(y * SCREEN_WIDTH) + x] = color;
    }
    
    void Screen::update() {
        SDL_UpdateTexture(m_texture, NULL, m_buffer1, SCREEN_WIDTH * sizeof(Uint32));
        SDL_RenderClear(m_renderer);
        SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
        SDL_RenderPresent(m_renderer);
    }
    
    void Screen::boxBlur() {
        Uint32 *temp = m_buffer1;
        m_buffer1 = m_buffer2;
        m_buffer2 = temp;
        
        for(int y = 0; y < SCREEN_HEIGHT; y++) {
            for(int x = 0; x < SCREEN_WIDTH; x++) {
                int R = 0;
                int G = 0;
                int B = 0;
                
                int n = 0;
                
                for(int row = -1; row <= 1; row++) {
                    for(int col = -1; col <= 1; col++) {
                        int curX = x + col;
                        int curY = y + row;
                        
                        if(curX >= 0 && curX < SCREEN_WIDTH && curY >= 0 && curY < SCREEN_HEIGHT) {
                            n++;
                            Uint32 color = m_buffer2[curY * SCREEN_WIDTH + curX];
                            Uint8 red = color >> 24;
                            Uint8 green = color >> 16;
                            Uint8 blue = color >> 8;
                            
                            R += red;
                            G += green;
                            B += blue;
                        }
                    }
                }
                Uint8 red = R / n;
                Uint8 green = G / n;
                Uint8 blue = B / n;
                
                setPixel(x, y, red, green, blue);
            }
        }
    }
    
    bool Screen::processEvents() {
        SDL_Event event;
        
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return false;
            }
        }
        return true;
    }
    
    void Screen::close() {
        delete[] m_buffer1;
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyTexture(m_texture);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    void Screen::clear() {
        for(int y = 0; y < SCREEN_HEIGHT; y++) {
            for(int x = 0; x < SCREEN_WIDTH; x++) {
                setPixel(x, y, 0x00, 0x00, 0x00);
            }
        }
    }
}
