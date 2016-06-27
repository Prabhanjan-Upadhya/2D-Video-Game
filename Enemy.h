#ifndef ENEMY__H
#define ENEMY__H
#include<string>
#include <vector>
#include "drawable.h"
#include "player.h"

class Enemy : public Drawable {
public:
  Enemy(const std::string&, const std::vector<Frame*>&, const Player& p);
  virtual ~Enemy(); 

  virtual void draw() const;
  virtual void reset();
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }
  static void incrSafeDistance() { ++safeDistance; }
  static void decrSafeDistance() { --safeDistance; }
  static float getSafeDistance() { return safeDistance; }
  void stop() { setVelocity(Vector2f(0,0)); }
protected:
  enum MODE {NORMAL, EVADE};
  const std::vector<Frame *> frames;
  int worldWidth;
  int worldHeight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;
  const Player& enemy;
  static float safeDistance;
  MODE currentMode;
  void advanceFrame(Uint32 ticks);
  Enemy(const Enemy&);
  Enemy& operator=(const Enemy&);
};
#endif
