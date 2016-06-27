#ifndef MANAGER__H
#define MANAGER__H

#include <list>
#include <vector>
#include <SDL.h>
#include "ioManager.h"
#include "player.h"
#include "world.h"
#include "viewport.h"
#include "frameFactory.h"
#include "hud.h"
#include "scaledSprite.h"
#include "sound.h"
#include "sprite.h"
#include "Enemy.h"

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();
  void switchSprite();
  void makeDrops();
  void printDrops() const;
  void resetgame();
  void pause () { clock.pause(); }
  void unpause() { clock.unpause(); }
private:
  const bool env;
  const IOManager& io;
  Clock& clock;

  SDL_Surface * const screen;
  World world1;
  World world2;
  World world3;
  Viewport& viewport;
  FrameFactory& ff;
  Hud hud;
  SDLSound sound;
  Player player; 
  std::vector<Frame*> orbFrame;
  Frame* boulderFrame;
  std::vector<Frame*> WeedleFrames;
  std::vector<Frame*> bossFrames;
  std::vector<Frame*> boss_Sleep;
  SDL_Surface *RainSurface;
  std::vector<ScaledSprite*>skdrops;
  std::list<Drawable*> sprites;
  std::list<Drawable*> WeedleSprites;
  Drawable *bossSprite;
  Drawable *bossSleep;
//  const Sprite *boulder;
  std::list<Drawable*>::iterator currentSprite;

  bool makeVideo;
  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;
  void draw() const;
  void checkForCollisions();
  void update();
  void reset();
  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
};
#endif
