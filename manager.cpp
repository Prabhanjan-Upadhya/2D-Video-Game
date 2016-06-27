#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include "vector2f.h"
#include "multisprite.h"
#include "sprite.h"
#include "gamedata.h"
#include "manager.h"
#include "extractSurface.h"
#include "twoway.h"

class ScaledSpriteCompare{
  public:
    bool operator()(const ScaledSprite* lhs, const ScaledSprite* rhs){
      return lhs->getScale() < rhs->getScale();
    }
};

Manager::~Manager() {
  SDL_FreeSurface(RainSurface); 
  std::vector<ScaledSprite*>::const_iterator ptr1 = skdrops.begin();
  while(ptr1 != skdrops.end()){
    delete(*ptr1);
    ++ptr1;
  }


  std::list<Drawable*>::const_iterator ptr = sprites.begin();
  while ( ptr != sprites.end() ) {
    delete (*ptr);
    ++ptr;
  }
  std::list<Drawable*>::const_iterator ptr2 = WeedleSprites.begin();
  while (ptr2 != WeedleSprites.end() ){
    delete(*ptr2);
    ++ptr2;
  }
  delete(bossSprite);
  delete(bossSleep);
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  world1("back1", Gamedata::getInstance().getXmlInt("back1/factor") ),
  world2("back2", Gamedata::getInstance().getXmlInt("back2/factor") ),
  world3("back3", Gamedata::getInstance().getXmlInt("back3/factor") ),
  viewport( Viewport::getInstance() ),
  ff(FrameFactory::getInstance()),
  hud(),
  sound(),
  player("Bike",ff.getFrames("Bike")),
  orbFrame( FrameFactory::getInstance().getFrames("Laprus") ),

  boulderFrame(FrameFactory::getInstance().getFrame("boulder")),
  WeedleFrames(FrameFactory::getInstance().getFrames("Weedle")),
  bossFrames(FrameFactory::getInstance().getFrames("Boss")),
  boss_Sleep(FrameFactory::getInstance().getFrames("Boss_Sleep")),
  RainSurface(io.loadAndSet(Gamedata::getInstance().getXmlStr("Rain/file"),
              Gamedata::getInstance().getXmlBool("Rain/transparency")) ),
  skdrops(),
  sprites(),
  WeedleSprites(),
  bossSprite(),
  bossSleep(),
//  boulder(io.loadAndSet(Gamedata::getInstance().getXmlStr("Boulder/file"),
//         Gamedata::getInstance().getXmlBool("Boulder/transparency")) ),
  currentSprite(),
  
  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") )
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);
  sprites.push_back( new Sprite("boulder",
                   Vector2f(Gamedata::getInstance().getXmlInt("boulder/startLoc/x"),
                            Gamedata::getInstance().getXmlInt("boulder/startLoc/y")),
                   Vector2f(Gamedata::getInstance().getXmlInt("boulder/speedX"),
                            Gamedata::getInstance().getXmlInt("boulder/speedY")),
                    boulderFrame) );
  sprites.push_back( new TwoWaySprite("Laprus", orbFrame) );
  int numberofWeedles = Gamedata::getInstance().getXmlInt("Weedle/number");
  for(int i =0; i<numberofWeedles; i++){
    WeedleSprites.push_back( new MultiSprite("Weedle", WeedleFrames));
  }
  bossSprite = new Enemy("Boss",bossFrames,player);
  bossSleep = new Enemy("Boss_Sleep",boss_Sleep, player);
  //currentSprite = sprites.begin();
  makeDrops();
  viewport.setObjectToTrack(&player);
}

void Manager::makeDrops(){
  unsigned numberofDrops = Gamedata::getInstance().getXmlInt("numberofDrops");
  skdrops.reserve(numberofDrops);
  for(unsigned i=0;i<numberofDrops; ++i){
    skdrops.push_back(new ScaledSprite("Rain", RainSurface));
  }
  sort(skdrops.begin(), skdrops.end(), ScaledSpriteCompare());
}

void Manager::printDrops() const{
  for(unsigned i=0; i< skdrops.size(); ++i){
    std::cout<< skdrops[i]->getScale()<< std::endl;
  }
}

bool showhud = false;
bool showbullethud = false;
bool nocollisioncheck = false;
bool endGameMode = false;
int worldWidth = static_cast<int>(Gamedata::getInstance().getXmlInt("world/width"));

void Manager::checkForCollisions(){
  std::list<Drawable*>::const_iterator sprite = sprites.begin();
  std::list<Bullet*>::iterator ptr = player.getBulletLists().begin();
  std::list<Drawable*>::const_iterator sprite1 = WeedleSprites.begin(); 
  
  while(ptr!=player.getBulletLists().end()){
    while(sprite!=sprites.end()){
      if((*ptr)->collidedWith(*sprite)){
        if(TwoWaySprite* spr = dynamic_cast<TwoWaySprite*>(*sprite)){
	  if(spr->getVisibility()){
            spr->explode();
            (*ptr)->setFar(true);
	    sound[2];
          }
        }
      }
      ++sprite;
    }
    while(sprite1!=WeedleSprites.end()){
      if((*ptr)->collidedWith(*sprite1)){
        if(MultiSprite* spr = dynamic_cast<MultiSprite*>(*sprite1)){
          if(spr->getVisibility()){
            spr->explode();
            (*ptr)->setFar(true);
	    sound[2];
	  }
        }
      }
      ++sprite1;
    }
    if((*ptr)->collidedWith(bossSprite)){
      //if(Enemy* epr = dynamic_cast<Enemy*>(*bossSprite)){
        if(hud.get_Health_boss()<=0){
          endGameMode = true;
          sound[2];
          sound.winMusic();
          bossSleep->setVelocity(Vector2f(0,0)); 
          bossSprite->setVelocity(Vector2f(0,0));
        }
	else hud.lower_boss();
      
    }
    ++ptr;
  }
  sprite = sprites.begin();
  sprite1 = WeedleSprites.begin();
  
  if(!nocollisioncheck){
    while(sprite != sprites.end()){
      if(player.collidedWith(*sprite)){
        TwoWaySprite* tmspr = dynamic_cast<TwoWaySprite *>(*sprite);
        if(tmspr->getVisibility()){
	  if(hud.get_Health() <= 0){
            player.explode();
 	    sound[2];
            hud.reset_healths();
          }
          else hud.lower();
        }
      }
      ++sprite;
    }
    while(sprite1 != WeedleSprites.end()){
      if(player.collidedWith(*sprite1)){
        MultiSprite* mspr = dynamic_cast<MultiSprite *>(*sprite1);
        if(mspr->getVisibility()){
          if(hud.get_Health() <=0){
            player.explode();
            sound[2];
            hud.reset_healths();
          }
        else
          hud.lower();
        }
      }
      ++sprite1;
    }
    if(player.collidedWith(bossSprite)){  
        if(hud.get_Health()<=0){
          player.explode();
          sound[2];
          hud.reset_healths();
        }
    else hud.lower();
    }
  }
  return;
}


void Manager::draw() const {
  world1.draw();
  std::vector<ScaledSprite*>::const_iterator sptr = skdrops.begin();
  int count =0;
  while(sptr!= skdrops.end() && count < (int)skdrops.size()/3){
    (*sptr)->draw();
    ++sptr;
    count++;
  }
  world2.draw();
  while(sptr!= skdrops.end() && count < (int)(skdrops.size()/2)){
    (*sptr)->draw();
    ++sptr;
    count++;
  }
  world3.draw();
  while(sptr!= skdrops.end() && count < (int)(2*skdrops.size()/3)){
    (*sptr)->draw();
    ++sptr;
    count++;
  }
  std::list<Drawable*>::const_iterator ptr = sprites.begin();
  while ( ptr != sprites.end() ) {
    (*ptr)->draw();
    ++ptr;
  }

  ptr = WeedleSprites.begin();
  while ( ptr!= WeedleSprites.end() ){
    (*ptr)->draw();
    ++ptr;
  }
  player.draw();
  if(hud.get_Health_boss()<=0){
    bossSleep->draw();
  }
  else
    bossSprite->draw();
  //boulder.draw();
  
  io.printMessageAt(title, 10, 450);
  
  viewport.draw();
 
  int startX = 10;
  int startY = 10;
  Uint8 initial_time = Gamedata::getInstance().getXmlInt("HUD/initial_time");
  if(clock.getSeconds() <= initial_time){
    hud.drawHUD(screen, startX, startY);
  }
  if(clock.getSeconds() > 3 && showhud){
    hud.drawHUD(screen, startX, startY);
  }
  if(endGameMode){
    showhud=false;
    showbullethud = false;
    hud.endgameHUD(screen, startX, startY);
  }
  if(showbullethud){
    hud.drawBulletInfoHUD(screen, 650, 50, player.getBulletListSize(), player.getFreeListSize());
  }
  /*if(player.X() >= worldWidth-200){
    sound.winMusic();
  }*/

  SDL_Flip(screen);
}

// Move this to IOManager
void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}

void Manager::switchSprite() {
  ++currentSprite;
  if ( currentSprite == sprites.end() ) {
    currentSprite = sprites.begin();
  }
  viewport.setObjectToTrack(*currentSprite);
}

void Manager::update() {
  clock.update();
  Uint32 ticks = clock.getTicksSinceLastFrame();

  std::list<Drawable*>::const_iterator ptr = sprites.begin();
  while ( ptr != sprites.end() ) {
    (*ptr)->update(ticks);
    ++ptr;
  }
  ptr = WeedleSprites.begin();
  while ( ptr!= WeedleSprites.end() ) {
    (*ptr)->update(ticks);
    ++ptr;
  }
  std::vector<ScaledSprite*>::const_iterator sptr = skdrops.begin();
  while(sptr!=skdrops.end()){
    (*sptr)->update(ticks);
    ++sptr;
  }
  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }
  world1.update();
  world2.update();
  world3.update();
  player.update(ticks);
  player.stop();
  if(player.X()>=worldWidth-854){
    viewport.setObjectToTrack(sprites.front());
    player.set_restricted();
  }
  bossSprite->update(ticks); 
  bossSleep->update(ticks);
  viewport.update();   // always update viewport last

}

void Manager::reset(){
  endGameMode = false;
  player.reset();
  clock.reset();
  sound.startMusic();
  nocollisioncheck = false;
  bossSprite->reset();
  bossSleep->reset();
  hud.reset_healths();
  std::list<Drawable*>::iterator itr = sprites.begin();
  while(itr!=sprites.end()){
    (*itr)->reset();
    ++itr;
  }
  itr = WeedleSprites.begin();
  while(itr!=WeedleSprites.end()){
    (*itr)->reset();
    ++itr;
  }
  viewport.setObjectToTrack(&player);
}

void Manager::play() {
  SDL_Event event;
  bool done = false;
  clock.start();
  
  bool IsFrameCapped = Gamedata::getInstance().getXmlBool("framesAreCapped");
  unsigned int FrameCap = Gamedata::getInstance().getXmlInt("frameCap");

  sound.startMusic();
    while ( not done ) {
      Uint32 currentTicks = SDL_GetTicks();
          Uint8 *keystate = SDL_GetKeyState(NULL);
          if (keystate[SDLK_a]){
            player.move_left();
          }
          if (keystate[SDLK_d]){
            player.move_right();
          }
          if (keystate[SDLK_w]){
            player.move_up();
          }
          if (keystate[SDLK_s]){
            player.move_down();
          }
          if ( (keystate[SDLK_w] && keystate[SDLK_s]) || ( keystate[SDLK_a] && keystate[SDLK_d])||endGameMode ){
            player.stop();
          }
    

    while ( SDL_PollEvent(&event) ) {
      Uint8 *keystate = SDL_GetKeyState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_q) {
          done = true;
          break;
        }
        if(keystate[SDLK_x]){
          player.explode();
        }
        if(keystate[SDLK_SPACE] & !endGameMode){
          player.shoot();
	  sound[0];
        }
        if ( keystate[SDLK_p]) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if(keystate[SDLK_r]){
          reset();
        }
	if (keystate[SDLK_g]){
	  nocollisioncheck = !nocollisioncheck;
	}
        if (keystate[SDLK_l]) {
          //clock.toggleSloMo();
        }
        if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
        if (keystate[SDLK_F1]){
          if(!showhud){
            showhud = true;
          }
          else showhud = false;
        }
        if (keystate[SDLK_F2]){
          if(!showbullethud){
            showbullethud = true;
          }
          else showbullethud=false;
        }
      }
    }
    if(IsFrameCapped){
      if(1000/FrameCap > (SDL_GetTicks() - currentTicks)){
        SDL_Delay((1000/FrameCap) - (SDL_GetTicks() - currentTicks));
      }
    }
    draw();
    checkForCollisions();
    update();
  }
}
