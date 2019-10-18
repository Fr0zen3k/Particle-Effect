//
//  Screen.cpp
//  Particle Effect
//
//  Version 1.1
//
//  Created by Jan Kresic on 9/11/19.
//  Copyright © 2019 Jan Kresic. All rights reserved.
//

#include "Screen.hpp"

using namespace std;

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
    
    void Screen::boxBlur1() {
        long curTime = SDL_GetTicks();
        Uint32 *temp = m_buffer1;
        m_buffer1 = m_buffer2;
        m_buffer2 = temp;
        
        int n;
        int redBuffer[3] = {0};
        int greenBuffer[3] = {0};
        int blueBuffer[3] = {0};
        
        for(int y = 0; y < SCREEN_HEIGHT; y++) {
            for(int x = 0; x < SCREEN_WIDTH; x++) {
                int R = 0;
                int G = 0;
                int B = 0;
                
                if(y == 0 || y == SCREEN_HEIGHT - 1) {
                    if(x == 0 || x == SCREEN_WIDTH - 1) {
                        n = 4;
                    }
                    else {
                        n = 6;
                    }
                }
                else {
                    if(x == 0 || x == SCREEN_WIDTH - 1) {
                        n = 6;
                    }
                    else {
                        n = 9;
                    }
                }
                
                if(x == 0) {
                    redBuffer[0] = 0;
                    redBuffer[1] = 0;
                    greenBuffer[0] = 0;
                    greenBuffer[1] = 0;
                    blueBuffer[0] = 0;
                    blueBuffer[1] = 0;
                    
                    //calculate buffer 0 and 1 for each color
                    for(int i = -1; i <= 1; i++) {
                        int curY = y + i;
                        
                        if(curY >= 0 && curY < SCREEN_HEIGHT) {
                            Uint32 color1 = m_buffer2[curY * SCREEN_WIDTH + x];
                            Uint8 red1 = color1 >> 24;
                            Uint8 green1 = color1 >> 16;
                            Uint8 blue1 = color1 >> 8;
                            
                            redBuffer[0] += red1;
                            greenBuffer[0] += green1;
                            blueBuffer[0] += blue1;
                            
                            Uint32 color2 = m_buffer2[curY * SCREEN_WIDTH + x + 1];
                            Uint8 red2 = color2 >> 24;
                            Uint8 green2 = color2 >> 16;
                            Uint8 blue2 = color2 >> 8;
                            
                            redBuffer[1] += red2;
                            greenBuffer[1] += green2;
                            blueBuffer[1] += blue2;
                        }
                    }
                }
                else if(x == 1) {
                    redBuffer[2] = 0;
                    greenBuffer[2] = 0;
                    blueBuffer[2] = 0;

                    //calculate buffer 2 for each color
                    for(int i = -1; i <= 1; i++) {
                        int curY = y + i;
                        
                        if(curY >= 0 && curY < SCREEN_HEIGHT) {
                            Uint32 color = m_buffer2[curY * SCREEN_WIDTH + x + 1];
                            Uint8 red = color >> 24;
                            Uint8 green = color >> 16;
                            Uint8 blue = color >> 8;
                            
                            redBuffer[2] += red;
                            greenBuffer[2] += green;
                            blueBuffer[2] += blue;
                        }
                    }
                }
                else if(x == SCREEN_WIDTH - 1) {
                    redBuffer[(x - 2) % 3] = 0;
                    greenBuffer[(x - 2) % 3] = 0;
                    blueBuffer[(x - 2) % 3] = 0;
                }
                else {
                    //rotation of buffers regarding the x in question
                    redBuffer[(x - 2) % 3] = 0;
                    greenBuffer[(x - 2) % 3] = 0;
                    blueBuffer[(x - 2) % 3] = 0;
                    
                    for(int i = -1; i <= 1; i++) {
                        int curY = y + i;
                        
                        if(curY >= 0 && curY < SCREEN_HEIGHT) {
                            Uint32 color = m_buffer2[curY * SCREEN_WIDTH + x + 1];
                            Uint8 red = color >> 24;
                            Uint8 green = color >> 16;
                            Uint8 blue = color >> 8;
                            
                            redBuffer[(x - 2) % 3] += red;
                            greenBuffer[(x - 2) % 3] += green;
                            blueBuffer[(x - 2) % 3] += blue;
                        }
                    }
                }
                
                R = (redBuffer[0] + redBuffer[1] + redBuffer[2]) / n;
                G = (greenBuffer[0] + greenBuffer[1] + greenBuffer[2]) / n;
                B = (blueBuffer[0] + blueBuffer[1] + blueBuffer[2]) / n;
                
                setPixel(x, y, R, G, B);
            }
        }
        cout << SDL_GetTicks() - curTime << endl;
    }

    void Screen::boxBlur2() {
        long curTime = SDL_GetTicks();
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
        
        cout << SDL_GetTicks() - curTime << endl;
    }
    
    bool Screen::processEvents() {
        SDL_Event event;
        
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
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
