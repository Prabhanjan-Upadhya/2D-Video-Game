#ifndef TWOWAYSPRITE__H
#define TWOWAYSPRITE__H
#include<string>
#include <vector>
#include "drawable.h"

class ExplodingSprite;
class TwoWaySprite : public Drawable {
public:
  TwoWaySprite(const std::string&, const std::vector<Frame*>&);
  virtual ~TwoWaySprite(); 

  virtual void draw() const;
  void explode();
  virtual void reset();
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }
  bool getVisibility() { return visible; }
  void setVisibility(bool b) { visible = b; }
protected:
  ExplodingSprite *explosion;
  const std::vector<Frame *> frames;
  int worldWidth;
  int worldHeight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;
  int revert;
  bool visible;
  void advanceFrame(Uint32 ticks);
  TwoWaySprite(const TwoWaySprite&);
  TwoWaySprite& operator=(const TwoWaySprite&);
};
#endif
