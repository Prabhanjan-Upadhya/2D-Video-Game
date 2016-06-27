#include <cmath>
#include "Enemy.h"
#include "gamedata.h"


Enemy::~Enemy(){

}

float distance(float x1, float y1, float x2, float y2){
  float x=x1-x2;
  float y=y1-y2;
  return hypot(x,y);
}


void Enemy::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  Uint32 frameRev = numberOfFrames/2;
  Vector2f vec = getVelocity();

  if (timeSinceLastFrame > frameInterval) {
    if(vec[0] >0){
      currentFrame = (currentFrame+1)%frameRev + frameRev;
    } else{
        currentFrame = (currentFrame+1) % frameRev;
      } 
    timeSinceLastFrame = 0;
  }
}

Enemy::Enemy( const std::string& name, 
                          const std::vector<Frame*>& f, const Player& p) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  frames( f ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  enemy(p),
  currentMode(NORMAL)
{ }

void Enemy::draw() const { 
    Uint32 x = static_cast<Uint32>(X());
    Uint32 y = static_cast<Uint32>(Y());
    frames[currentFrame]->draw(x,y);
}

void Enemy::reset(){
  string name = getName();
  setPosition(Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                       Gamedata::getInstance().getXmlInt(name+"/startLoc/y")));
  setVelocity(Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                       Gamedata::getInstance().getXmlInt(name+"/speedY")));
  currentFrame=0;
}

void Enemy::update(Uint32 ticks) { 
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  advanceFrame(ticks);
  float x=X()+getFrame()->getWidth()/2;
  float y=Y()+getFrame()->getHeight()/2;
  float ex= enemy.X()+enemy.getFrame()->getWidth()/2;
  float ey = enemy.Y()+enemy.getFrame()->getHeight()/2;
  float distanceToEnemy = ::distance(x,y,ex,ey);
  if(currentMode == NORMAL){
    if(distanceToEnemy < safeDistance) currentMode = EVADE;
  }
  else if(currentMode == EVADE){
    if (distanceToEnemy < safeDistance) currentMode = NORMAL;
    else{
      if(x < ex) velocityX(abs(velocityX()));
      if(x > ex) velocityX(-abs(velocityX()));
      if(y < ey) velocityY(-abs(velocityY()));
      if(y > ey) velocityY(abs(velocityY()));
    }
  }
  /*if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }*/

  if ( X() < worldWidth-854) {
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  } 
}
