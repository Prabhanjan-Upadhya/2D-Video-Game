#include "twoway.h"
#include "gamedata.h"
#include "explodingSprite.h"


TwoWaySprite::~TwoWaySprite(){
  delete explosion;
}
void TwoWaySprite::advanceFrame(Uint32 ticks) {
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

TwoWaySprite::TwoWaySprite( const std::string& name, 
                          const std::vector<Frame*>& f) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
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
  revert(0),
  visible(true)
{ }

void TwoWaySprite::draw() const { 
  if(explosion){
    explosion->draw();
    return;
  }
  std::cout<< visible << std::endl;
  if(visible){
    Uint32 x = static_cast<Uint32>(X());
    Uint32 y = static_cast<Uint32>(Y());
    frames[currentFrame]->draw(x, y);
  }
}

void TwoWaySprite::reset(){
  string name = getName();
  setPosition(Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                       Gamedata::getInstance().getXmlInt(name+"/startLoc/y")));
  setVelocity(Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                       Gamedata::getInstance().getXmlInt(name+"/speedY")));
  currentFrame=0;
  visible = true;
}

void TwoWaySprite::explode(){
  visible = false;
  if(explosion) 
  return;
  Sprite sprite(getName(),getPosition(), getVelocity(), getFrame());
  explosion = new ExplodingSprite(sprite);
}

void TwoWaySprite::update(Uint32 ticks) { 
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

  advanceFrame(ticks);
  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  } 
}
