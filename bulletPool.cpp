#ifndef BULLETPOOL__H
#define BULLETPOOL__H
#include<iostream>
#include "bulletPool.h"
#include "frameFactory.h"
#include "gamedata.h"

BulletPool::~BulletPool(){
  std::list<Bullet*>::iterator ptr = freeList.begin();
  while(ptr != freeList.end()){
    delete(*ptr);
    ++ptr;
  }
  ptr = bulletList.begin();
  while(ptr != bulletList.end()){
    delete(*ptr);
    ++ptr;
  }
  freeList.clear();
  bulletList.clear();
}

void BulletPool::shootBullets(){
  getBullet()->draw();
}
Bullet* BulletPool::getBullet(){
  if(freeList.empty()){
    bulletList.push_back(new Bullet("bullet",player->getPosition()+Vector2f(25,10),
                         Vector2f(player->velocityX()*3,0),FrameFactory::getInstance().getFrame("bullet")));
  }
  else {
    freeList.back()->resetBullet(player->getPosition()+Vector2f(25,10), Vector2f(player->velocityX()*3,0));
    bulletList.push_back(freeList.back());
    freeList.pop_back();
  }
  return bulletList.back();
}
void BulletPool::drawBullets() {
  std::list<Bullet*>::iterator ptr = bulletList.begin();
  while(ptr != bulletList.end()){
    if(!(*ptr)->goneTooFar()){
      (*ptr)->draw();
    }
    ++ptr;
  }
}

std::list<Bullet*>& BulletPool::getBulletList(){
  return bulletList;
}

std::list<Bullet*>& BulletPool::getFreeList(){
  return freeList;
}
void BulletPool::updateList(Uint32 ticks){
  std::list<Bullet*>::iterator ptr = bulletList.begin();
  while(ptr!=bulletList.end() ){
    (*ptr)->update(ticks);
    if( (*ptr)->goneTooFar()){
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
    }
    else ++ptr;
  }
}
#endif
