#include<iostream>
#include<list>
#include"bullet.h"
#include"gamedata.h"

class BulletPool{
  public:
    BulletPool(const Drawable *s): freeList(), bulletList(), player(s) {}
    ~BulletPool();
    Bullet* getBullet();
    void shootBullets();
    void updateList(Uint32);
    void drawBullets();
    std::list<Bullet*>& getBulletList();
    std::list<Bullet*>& getFreeList();
  private:
    std::list<Bullet*> freeList;
    std::list<Bullet*> bulletList;
    const Drawable *player;
    BulletPool(const BulletPool&);
    BulletPool& operator=(const BulletPool&);
};


