#ifndef BULLET__H
#define BULLET__H
#include <string>
#include "drawable.h"
#include "collisionStrategy.h"

class Bullet : public Drawable {
public:
  Bullet(const std::string&, 
         const Vector2f& pos, const Vector2f& vel, const Frame*);
  Bullet(const std::string&, const Vector2f& pos, const Vector2f& vel);

  Bullet(const Bullet& s);
  virtual ~Bullet(); 
  Bullet& operator=(const Bullet&);

  virtual const Frame* getFrame() const { return frame; }
  virtual void draw() const;
  bool goneTooFar();
  virtual void update(Uint32 ticks);
  virtual void reset();
  bool collidedWith(const Drawable* d) const;
  void resetBullet(const Vector2f&, const Vector2f&);
  void setFar(bool b) { tooFar = b; }
private:
  const Frame * frame;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  bool tooFar;
  CollisionStrategy * strategy; 
  Vector2f startPos;
};
#endif
