#include<iostream>
#include "ioManager.h"
#include "aaline.h"
#include "hud.h"
#include "gamedata.h"

const int HUD_WIDTH = 300;
const int HUD_HEIGHT = 280;

Hud::Hud():
hclock(Clock::getInstance()),
health_bar(30,90,200,200,14,1,(SDL_MapRGB(IOManager::getInstance().getScreen()->format, 0xff,0,0)),1000),
health_boss(30,160,200,200,14,1,(SDL_MapRGB(IOManager::getInstance().getScreen()->format, 0,0xff,0)),1000)
{}

Hud::~Hud(){
}

void Hud::drawHUD( SDL_Surface* screen, int x, int y) const{
  const Uint32 RED = SDL_MapRGB(screen->format, 0xff, 0, 0);
  Draw_AALine(screen, x, y+HUD_HEIGHT/2, x+HUD_WIDTH, y+HUD_HEIGHT/2,
              HUD_HEIGHT, 0xff , 0xff, 0xff, 0xff/2);
  Draw_AALine(screen, x, y, x+HUD_WIDTH, y, RED);
  Draw_AALine(screen, x, y, x, y+HUD_HEIGHT, RED);
  Draw_AALine(screen, x+HUD_WIDTH, y , x+HUD_WIDTH , y+HUD_HEIGHT, RED);
  Draw_AALine(screen, x, y+HUD_HEIGHT, x+HUD_WIDTH, y+HUD_HEIGHT, RED);
  display();  
}
void Hud::endgameHUD( SDL_Surface* screen, int x, int y) const{
  const Uint32 RED = SDL_MapRGB(screen->format, 0xff, 0, 0);
  Draw_AALine(screen, x, y+HUD_HEIGHT/2, x+HUD_WIDTH, y+HUD_HEIGHT/2,
              HUD_HEIGHT, 0xff , 0xff, 0xff, 0xff/2);
  Draw_AALine(screen, x, y, x+HUD_WIDTH, y, RED);
  Draw_AALine(screen, x, y, x, y+HUD_HEIGHT, RED);
  Draw_AALine(screen, x+HUD_WIDTH, y , x+HUD_WIDTH , y+HUD_HEIGHT, RED);
  Draw_AALine(screen, x, y+HUD_HEIGHT, x+HUD_WIDTH, y+HUD_HEIGHT, RED);
  endgamedisplay();  
}

void Hud::drawBulletInfoHUD(SDL_Surface* screen, int x, int y, int bulletsize, int freesize) const{
  Draw_AALine(screen, x, y+HUD_HEIGHT/4, x+HUD_WIDTH/2, y+HUD_HEIGHT/4, HUD_HEIGHT/2, 0xff, 0xff, 0xff, 0xff/2);
  const Uint32 RED = SDL_MapRGB(screen->format, 0xff, 0, 0);
  IOManager::getInstance().printMessageValueAt("Active Bullets: ", bulletsize,x,y);
  IOManager::getInstance().printMessageValueAt("Free Bullets: ", freesize, x,y+20);
  Draw_AALine(screen, x, y, x+HUD_WIDTH/2, y, RED);
  Draw_AALine(screen, x, y+HUD_HEIGHT/2, x+HUD_WIDTH/2, y+HUD_HEIGHT/2, RED);
  Draw_AALine(screen, x, y, x, y+HUD_HEIGHT/2, RED);
  Draw_AALine(screen, x+HUD_WIDTH/2, y, x+HUD_WIDTH/2, y+HUD_HEIGHT/2, RED);
}

void Hud::display() const{
  IOManager::getInstance().printMessageValueAt("Seconds: ", hclock.getSeconds(), 30, 20);
  IOManager::getInstance().printMessageValueAt("fps: ", hclock.getFps(), 30, 40);
  IOManager::getInstance().printMessageAt("Player's health",30, 60);
  health_bar.draw();
  health_boss.draw();
  IOManager::getInstance().printMessageAt("Boss's health",30,130);
  IOManager::getInstance().printMessageAt("Press 'w,a,s,d' to move", 30, 190);
  IOManager::getInstance().printMessageAt("Press 'g' to turn on god mode", 30, 210);
  IOManager::getInstance().printMessageAt("Press 'F1' to toggle HUD", 30, 230);
  IOManager::getInstance().printMessageAt("Press 'F2' for bullet information", 30, 250);
  IOManager::getInstance().printMessageAt("Press 'F4' to generate frames", 30, 270);
}

void Hud::endgamedisplay()const{
  IOManager::getInstance().printMessageAt("End of the game!",30,20);
  IOManager::getInstance().printMessageAt("Press 'r' to restart the game.",30,40);
  IOManager::getInstance().printMessageAt("Or Enjoy the high energy music!",30,60);
  
}

int Hud::get_Health() const{
  return health_bar.getLength();
}

int Hud::get_Health_boss() const{
  return health_boss.getLength();
}
