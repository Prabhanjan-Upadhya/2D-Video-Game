#include<SDL.h>
#include "clock.h"
#include "health.h"

class Hud{
  public:  
    Hud();
    ~Hud();
    void drawHUD(SDL_Surface* screen, int x, int y) const;
    void endgameHUD(SDL_Surface* screen, int x, int y) const;
    void drawBulletInfoHUD(SDL_Surface* screen, int x, int y, int bulletsize, int freesize) const;
    void display() const;
    void endgamedisplay() const;
    void lower() { health_bar.decrease(); }  
    int get_Health() const;
    void lower_boss() { health_boss.decrease(); }
    int get_Health_boss() const;
    void reset_healths() { health_bar.reset();
			   health_boss.reset(); }
  private:
    Clock& hclock;
    Health health_bar;
    Health health_boss;
};
