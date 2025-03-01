#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "headers/Circle.h"
#include <iostream>
#include "headers/Timestep.h"
#include "sysinfoapi.h"

int main(int argc, char* argv[]){
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	const float GRAVITY = -9.81f; // Adjust gravity as needed

	int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	if (result < 0) {
		SDL_Log("SDL_INIT error: %s", SDL_GetError());
		return -1;
	}
	SDL_Init(SDL_INIT_AUDIO);
	window = SDL_CreateWindow("test", 640, 640, 0);
	if (window == NULL){
		SDL_Log("SDL_CreateWindow error: %s", SDL_GetError());
		return -2;
	}

	renderer = SDL_CreateRenderer(window, NULL);
	if (renderer == NULL){
		SDL_Log("SDL_CreateRenderer error: %s", SDL_GetError());
		return -3;
	}

	std::cout << "Hi6" << std::endl;
	

	Circle ball;
	ball.point = {300, 300, 0};
	ball.radius = 200;
	ball.gravity = GRAVITY;
	ball.vel = {0, 0, 0};

	Circle smallBall;
	smallBall.point = {300, 300, 0};
	smallBall.radius = 25;
	smallBall.gravity = GRAVITY;
	smallBall.vel = {1500, 2000, 0};

	Uint64 lastPerformanceCounter = SDL_GetPerformanceCounter();
	Uint64 performanceFrequency = SDL_GetPerformanceFrequency();
	
	SDL_Event event;
	int quit = 0;
	while(!quit){
		Uint64 currentPerformanceCounter = SDL_GetPerformanceCounter();
		float deltaTime = (float)(currentPerformanceCounter - lastPerformanceCounter) / (float)performanceFrequency;
		lastPerformanceCounter = currentPerformanceCounter;
		const float MIN_DELTA_TIME = 0.001f; // 1ms minimum
		const float MAX_DELTA_TIME = 0.05f;  // 50ms maximum (prevents large jumps if game pauses)
		if (deltaTime < MIN_DELTA_TIME) 
			deltaTime = MIN_DELTA_TIME;
		if (deltaTime > MAX_DELTA_TIME) 
			deltaTime = MAX_DELTA_TIME;
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_EVENT_QUIT:
					SDL_Log("SDL Event Quit");
					quit = 1;
					break;
				case SDL_EVENT_KEY_DOWN:
					if (event.key.key == SDLK_ESCAPE){
						SDL_Log("SDL Event Quit");
						quit = 1;
						
					}
			}
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
		drawCircle(renderer, ball);
		
		
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
		drawCircleFilled(renderer, smallBall);
		if (circlevsCircleOptimizedInverted(ball, smallBall)){
			// Small ball is INSIDE the big circle, normal movement
			movement(smallBall, deltaTime);
		} else {
			// Small ball is escaping the boundary, resolve collision
			resolveCircle2(smallBall, ball, deltaTime);
	
		}
		
		SDL_RenderPresent(renderer);
		std::cout << "Small Ball Position: (" << smallBall.point.x << ", " << smallBall.point.y << ")\n";
        std::cout << "Delta Time: " << deltaTime << "s\n"; 

	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}