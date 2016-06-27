#include<iostream>
#include<unistd.h>
#include"player.h"
#include"sprite.h"
#include"explodingSprite.h"
#include"viewport.h"

Player::~Player(){
  delete strategy;
  delete explosion;
  delete gunshot;
}

void Player::advanceFrame(Uint32 ticks){
  timeSinceLastFrame += ticks;
  if(timeSinceLastFrame >frameInterval){
    if(!revert)
      currentFrame = numberOfFrames/2 + (currentFrame-1) % (numberOfFrames/2);
    else
      currentFrame = (currentFrame+1) % (numberOfFrames/2);
    timeSinceLastFrame=0;
  }
}


Player::Player(const std::string& name,
               const std::vector<Frame*>&f):
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))),
  explosion(NULL),
  gunshot(new BulletPool(this)),
  frames(f), 
  worldWidth(Gamedata::getInstance().getXmlInt("world/width") ),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height") ),
           
  //playerWidth( player.getFrame()->getWidth() ),
  //playerHeight( player.getFrame()->getHeight() ),
  currentFrame(0),
  numberOfFrames(Gamedata::getInstance().getXmlInt(name+"/frames")),
  frameInterval(Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame(0),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  up(false),
  down(false),
  right(false),
  left(false),
  revert(0),
  restricted(false),
  strategy(new PerPixelCollisionStrategy)
{}

void Player::draw() const{
  if(explosion){
    explosion->draw();
    return;
  }
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x,y);
  gunshot->drawBullets();
}

std::list<Bullet*>& Player::getBulletLists() const{
  return gunshot->getBulletList();
}

int Player::getBulletListSize() const{
  return gunshot->getBulletList().size();
}

int Player::getFreeListSize() const{
  return gunshot->getFreeList().size();
}

void Player::reset(){
  string name = getName();
  setPosition(Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                       Gamedata::getInstance().getXmlInt(name+"/startLoc/y")));
  setVelocity(Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                       Gamedata::getInstance().getXmlInt(name+"/speedY")));
  currentFrame = 0;
  restricted = false;
  delete explosion;
  delete gunshot;
  gunshot = new BulletPool(this);
  explosion = NULL;
}

void Player::stop(){
  up = false;
  down = false;
  right = false;
  left = false;
}

void Player::move_right(){
  right = true;
  revert = 0;
}

void Player::move_left(){
  left = true;
  revert = 1;
}

void Player::move_up(){
  up = true;
}

void Player::move_down(){
  down = true;
}

void Player::explode(){
  if(explosion) return;
  Sprite sprite(getName(),getPosition(), getVelocity(), getFrame());
  explosion = new ExplodingSprite(sprite);
}

void Player::shoot(){
  gunshot->shootBullets();
}

void Player::update(Uint32 ticks){
  if(explosion){
    explosion->update(ticks);
    if(explosion->chunkCount() == 0){
      delete explosion;
      explosion = NULL;
    }
    return;
  }
  gunshot->updateList(ticks);
  Vector2f incr(0,0);

  if(up)
    incr += Vector2f(0,Y()<worldHeight - 70 - frameHeight?0:-abs(velocityY())*static_cast<float>(ticks)*0.001);
  if(down)
    incr += Vector2f(0,Y()>worldHeight - frameHeight?0:abs(velocityY()) * static_cast<float>(ticks)*0.001);
  if(right)
    incr += Vector2f(X()>worldWidth-50-frameWidth?0:abs(velocityX())*static_cast<float>(ticks)*0.001,0);
  if(!restricted){
    if(left)
      incr += Vector2f(X()<0?0:-abs(velocityX())*static_cast<float>(ticks)*0.001,0);
  }
  else {
    if(left)
      incr += Vector2f(X()<worldWidth-854?0:-abs(velocityX())*static_cast<float>(ticks)*0.001,0);
  }
  if(up || down || right || left){
    setPosition(getPosition() + incr);
    advanceFrame(ticks);
    
    if(Y()<0){
      velocityY(abs(velocityY()));
    } 
    if(Y() > worldHeight-68-frameHeight){
      velocityY(-abs(velocityY()));
    }
    if((X()<0 || revert ==0) && right){
      revert=0;
      velocityX(abs(velocityX()));
    }
    if((X()>worldWidth-50-frameWidth ||revert==1) && left){
      revert = 1;
      velocityX(-abs(velocityX()));
    }
  }
}
