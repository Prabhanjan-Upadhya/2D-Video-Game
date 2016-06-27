#include "multisprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "explodingSprite.h"

MultiSprite::~MultiSprite(){
  delete explosion;
}
void MultiSprite::advanceFrame(Uint32 ticks) {
    timeSinceLastFrame += ticks;
    if (timeSinceLastFrame > frameInterval) {
      if(velocityX()==0){
        currentFrame = currentFrame;
      }
      else if(velocityX()<0){
        currentFrame = (currentFrame+1) % (numberOfFrames/2);
      }
      else 
        currentFrame = (currentFrame+1) % (numberOfFrames/2) + numberOfFrames/2;    
      timeSinceLastFrame = 0;
    }
}

MultiSprite::MultiSprite( const std::string& name, 
                          const std::vector<Frame*>& f) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getRandInRange(0, 3840-854),
                    Gamedata::getInstance().getRandInRange(480-170, 480)), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  explosion(NULL),
  frames( f ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  visible(true)
{ }

void MultiSprite::draw() const {
  if(explosion){
    explosion->draw();
    return;
  }
  if(visible){ 
    Uint32 x = static_cast<Uint32>(X());
    Uint32 y = static_cast<Uint32>(Y());
    frames[currentFrame]->draw(x, y);
  }
}

void MultiSprite::explode(){
  visible = false;
  if(explosion) return;
  Sprite sprite(getName(), getPosition(), getVelocity(), getFrame());
  explosion = new ExplodingSprite(sprite);
}

void MultiSprite::reset(){
  string name = getName();
  setPosition(Vector2f(Gamedata::getInstance().getRandInRange(0, worldWidth-854),
              Gamedata::getInstance().getRandInRange(worldHeight-170, worldHeight)));
  setVelocity(Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                       Gamedata::getInstance().getXmlInt(name+"/speedY")));
  currentFrame = 0;
  visible = true;
  
}
void MultiSprite::update(Uint32 ticks) { 
  if(explosion){
    explosion->update(ticks);
    if(explosion->chunkCount() == 0){
      delete explosion;
      explosion = NULL;
    }
    return;
  }
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  advanceFrame(ticks);
  Vector2f reset_pos = Vector2f(0,0);
  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-170-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    //velocityX( abs( velocityX() ) );
    reset_pos[0] = Gamedata::getInstance().getRandInRange(0,(worldWidth-854));
    reset_pos[1] = Gamedata::getInstance().getRandInRange((worldHeight-170), worldHeight);
    setPosition(reset_pos);
  }
  if ( X() > worldWidth-854-frameWidth) {
    velocityX( -abs( velocityX() ) );
  } 
}
