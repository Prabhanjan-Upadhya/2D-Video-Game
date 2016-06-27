#ifndef PLAYER__H
#define PLAYER__H

#include <string>
#include <vector>
#include "drawable.h"
#include "bulletPool.h"

class ExplodingSprite;
class Player : public Drawable{
  public:
    Player(const std::string& name, const std::vector<Frame*>&);
    virtual ~Player();
    
    virtual void draw() const;
    virtual void update(Uint32 ticks);
    void move_up();
    void move_down();
    void move_left();
    void move_right();
    void set_restricted() {restricted=true;}
    void stop();
     /*Drawable* getSprite() { return &player; }
    Vector2f getPlayerPos() const { return player.getPosition(); }
    Vector2f getPlayerVel() const { return player.getVelocity(); }
    float getX() const { return player.X(); }
    float getY() const { return player.Y(); }*/
    int getFrameWidth() const { return frameWidth; }
    int getFrameHeight() const { return frameHeight; }
    
    virtual const Frame* getFrame() const{
      return frames[currentFrame];
    }
    virtual void reset();
    void explode();
    void shoot();
    std::list<Bullet*>& getBulletLists() const;
    int getFreeListSize() const;
    int getBulletListSize() const;
   
    bool collidedWith(Drawable* d)const{
      return strategy->execute(*this, *d);
    } 
   
  private:
    ExplodingSprite *explosion;
    BulletPool *gunshot;
    const std::vector<Frame*> frames;
    int worldWidth;
    int worldHeight;
    unsigned currentFrame;
    unsigned numberOfFrames;
    unsigned frameInterval;
    float timeSinceLastFrame;
    int frameWidth;
    int frameHeight;
    //float playerWidth;
    //float playerHeight;
    bool up;
    bool down;
    bool right;
    bool left;
    int revert;
    bool restricted;
    CollisionStrategy * strategy;
    void advanceFrame(Uint32 ticks);
    Player(const Player&);
    Player& operator=(const Player&);
};
#endif
