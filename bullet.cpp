#include <cmath>
#include "bullet.h"
#include "gamedata.h"

Bullet::~Bullet(){
  delete strategy;
}
Bullet::Bullet(const string& n, const Vector2f& pos, const Vector2f& vel,
               const Frame* frm):
  Drawable(n, pos, vel), 
  frame( frm ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  tooFar(false),
  strategy( new PerPixelCollisionStrategy ),
  startPos(pos)
{ }

Bullet::Bullet(const Bullet& s) :
  Drawable(s), 
  frame(s.frame),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  tooFar(false),
  strategy(s.strategy),
  startPos(s.getPosition())
{ }

Bullet& Bullet::operator=(const Bullet& rhs) {
  Drawable::operator=( rhs );
  frame = rhs.frame;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  tooFar = rhs.tooFar;
  strategy = rhs.strategy;
  startPos = rhs.startPos;
  return *this;
}

void Bullet::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y); 
}

bool Bullet::goneTooFar() { 
  if(velocityX() > 0 && (X() - startPos[0] ) >500) {
    tooFar = true;
  }
  else if(velocityX() < 0 && (startPos[0] - X()) > 500){
    tooFar = true;
  }
  else {
    tooFar = false;
  }
  return tooFar;
}
 
void Bullet::reset(){
  string name = getName();
  setPosition(Vector2f(0,0));
  setVelocity(Vector2f(0,0));
}

bool Bullet::collidedWith(const Drawable* d) const{
  return strategy->execute(*this, *d);
}

void Bullet::resetBullet(const Vector2f & pos, const Vector2f & vel){
  velocityX(vel[0]);
  velocityY(vel[1]);
  X(pos[0]);
  Y(pos[1]);
  startPos = pos;
}
void Bullet::update(Uint32 ticks) { 
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  Vector2f reset_pos;

  if ( X() < 0) {
    tooFar = true;
  }
  if ( X() > worldWidth-frameWidth) {
    tooFar = true;
  }  
}
