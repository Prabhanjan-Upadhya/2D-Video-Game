#include <cmath>
#include "sprite.h"
#include "gamedata.h"
#include "explodingSprite.h"

Sprite::~Sprite(){
  delete explosion;
}
Sprite::Sprite(const string& n, const Vector2f& pos, const Vector2f& vel,
               const Frame* frm):
  Drawable(n, pos, vel), 
  explosion(NULL),
  frame( frm ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  visible(true)
{ }

Sprite::Sprite(const Sprite& s) :
  Drawable(s),
  explosion(s.explosion), 
  frame(s.frame),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  visible(s.visible)
{ }

Sprite& Sprite::operator=(const Sprite& rhs) {
  Drawable::operator=( rhs );
  explosion = rhs.explosion;
  frame = rhs.frame;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  visible = rhs.visible;
  return *this;
}

void Sprite::draw() const { 
  if(explosion){
    explosion->draw();
    return;
  }
  if(visible){
    Uint32 x = static_cast<Uint32>(X());
    Uint32 y = static_cast<Uint32>(Y());
    frame->draw(x, y); 
  }
}

void Sprite::reset(){
  string name = getName();
  setPosition(Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                       Gamedata::getInstance().getXmlInt(name+"/startLoc/y")));
  setVelocity(Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                       Gamedata::getInstance().getXmlInt(name+"/speedY")));
  visible=true; 
}

void Sprite::explode(){
  if(explosion) return;
  explosion = new ExplodingSprite(*this);
}

int Sprite::getDistance(const Sprite *obj) const { 
  return hypot(X()-obj->X(), Y()-obj->Y());
}

void Sprite::update(Uint32 ticks) { 
  if(!visible) return;
  if(explosion){
    explosion->update(ticks);
    if(explosion->chunkCount()==0){
      delete explosion;
      explosion = NULL;
    }
    return;
  }
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  Vector2f reset_pos;

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
    
  }
  if ( Y() > worldHeight-frameHeight) {
    //velocityY( -abs( velocityY() ) );
    reset_pos[0] = Gamedata::getInstance().getRandInRange(0, worldWidth);
    reset_pos[1] = 0;
    std::cout<< reset_pos [0] << reset_pos[1] << std::endl;
    setPosition(reset_pos);
  }

  /*if ( X() < 0) {
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  }*/  
}
