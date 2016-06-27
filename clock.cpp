#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include "clock.h"
#include "gamedata.h"
#include "ioManager.h"

Clock& Clock::getInstance() {
  if ( SDL_WasInit(SDL_INIT_VIDEO) == 0) {
    throw std::string("Must init SDL before Clock");
  }
  static Clock clock; 
  return clock;
}

Clock::Clock() :
  ticks(0),
  started(false), 
  paused(false), 
  sloMo(false),
  frames(0),
  sumOfTicks(SDL_GetTicks()),
  currentTicks(SDL_GetTicks()),
  timeElapsed(0)
  {
  start();
}

Clock::Clock(const Clock& c) :
  ticks(c.ticks),
  started(c.started), 
  paused(c.paused), 
  sloMo(c.sloMo), 
  frames(c.frames),
  sumOfTicks(SDL_GetTicks()),
  currentTicks(SDL_GetTicks()),
  timeElapsed(0)
  {
  start();
}
void Clock::draw() const { 
  /*IOManager::getInstance().
    printMessageValueAt("Seconds: ", getSeconds(), pos[0], pos[1]);
  IOManager::getInstance().
    printMessageValueAt("fps: ", getFps(), pos[0], pos[1]+20);*/
}

void Clock::reset(){
  ticks=0;
  frames=0;
  sumOfTicks=SDL_GetTicks();
  currentTicks=SDL_GetTicks();
  timeElapsed=0;
}
void Clock::update() { 
  if(paused){
    ticks=0;
    return;
  }
  if(sloMo){
    ticks=1;
    ++frames;
    currentTicks = SDL_GetTicks();
    timeElapsed += currentTicks - sumOfTicks;
    sumOfTicks += ticks;
    return;
  }
  ++frames;
  currentTicks = SDL_GetTicks();
  ticks = currentTicks - sumOfTicks;
  sumOfTicks += ticks;
  timeElapsed += ticks;
}

unsigned int Clock::getTicksSinceLastFrame() const {
  return ticks;
}

void Clock::toggleSloMo() {
  sloMo = ! sloMo;
}

int Clock::getFps() const { 
  return (ticks!=0?frames*1000/timeElapsed:0);
}

int Clock::getSeconds() const{
  return timeElapsed/1000;
}

void Clock::start() { 
  started = true; 
  paused = false; 
}

void Clock::pause() { 
  paused = true;
  started = false;
}

void Clock::unpause() { 
  paused = false;
  started = true;
  sumOfTicks =SDL_GetTicks();
  currentTicks= SDL_GetTicks();
}

