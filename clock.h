#include <SDL.h>
#include <string>
#include <deque>
#include "vector2f.h"

class Manager;

class Clock {
public:
  static Clock& getInstance();  // This class is a Singleton
  unsigned int getTicksSinceLastFrame() const;
  unsigned int getTicksSinceInit() const { return sumOfTicks; }
  int getSeconds() const;

  void toggleSloMo();
  bool isStarted() const { return started; }
  bool isPaused() const  { return paused;  }
  int getFps() const;

  void start();
  void pause();
  void unpause();
  void draw() const;  // For debugging
  void update();
  void reset();

private:
  unsigned int ticks;
  bool started;
  bool paused;
  bool sloMo;
  unsigned int frames;
  unsigned int sumOfTicks;
  unsigned int currentTicks;
  unsigned timeElapsed;


  Clock();
  Clock(const Clock&);
  Clock&operator=(const Clock&);
  friend class Manager;
};
