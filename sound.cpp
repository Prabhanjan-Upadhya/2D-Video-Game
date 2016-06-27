#include <string>
#include <iostream>
#include <ctime>
#include "sound.h"
using std::string;

SDLSound::~SDLSound() {
  std::cout << "Cleaning up sounds ..." << std::endl;
  std::clock_t start = std::clock();
  Mix_HaltMusic();
  for (unsigned int i = 0; i<music.size(); ++i){
    Mix_FreeMusic(music[i]);
  }
  for (unsigned int i = 0; i < sounds.size(); ++i) {
    Mix_FreeChunk(sounds[i]);
  }
  Mix_CloseAudio();
  std::clock_t duration = std::clock() - start;
  std::cout << "Clean up took " << duration << " ticks\n";
}

SDLSound::SDLSound() : 
  volume(SDL_MIX_MAXVOLUME/4), 
  currentSound(-1), 
  music(),
  audioRate(22050), 
  audioChannels(2), 
  audioBuffers(4096),
  sounds(),
  channels() 
{
  if(Mix_OpenAudio(audioRate, MIX_DEFAULT_FORMAT, audioChannels, 
                   audioBuffers)){
    throw string("Unable to open audio!");
  }
  music.reserve(10);
  sounds.reserve(15);
  music.push_back( Mix_LoadMUS("sound/Background.mp3") );
  music.push_back( Mix_LoadMUS("sound/PuliUrumudhu.mp3") );
  // Need to install midi to play the following:
  // music = Mix_LoadMUS("sound/ballad2.mid");
  if (!music[0]) throw string("Couldn't load Background.mp3")+Mix_GetError();
  if (!music[1]) throw string("Couldn't load PuliUrumudhu.mp3")+Mix_GetError();

  //startMusic();
  sounds.push_back( Mix_LoadWAV("sound/Boom.wav") );
  sounds.push_back( Mix_LoadWAV("sound/Gun.wav") );
  sounds.push_back( Mix_LoadWAV("sound/Thump.wav") );
  sounds.push_back( Mix_LoadWAV("sound/phaser.wav") );
  sounds.push_back( Mix_LoadWAV("sound/Putt1.wav") );
  sounds.push_back( Mix_LoadWAV("sound/explosion.wav") );
  sounds.push_back( Mix_LoadWAV("sound/cannon.wav") );
  sounds.push_back( Mix_LoadWAV("sound/Larc.wav") );
  sounds.push_back( Mix_LoadWAV("sound/Throw.wav") );
  for (unsigned int i = 0; i < sounds.size(); ++i) channels.push_back(i);
}

void SDLSound::toggleMusic() {
  if( Mix_PausedMusic() ) { 
    Mix_ResumeMusic(); 
  } 
  else { 
    Mix_PauseMusic(); 
  } 
}

void SDLSound::operator[](int index) {
  if (currentSound >= 0) Mix_HaltChannel(currentSound);
  currentSound = index;
  Mix_VolumeChunk(sounds[index], volume);
  channels[index] = 
     Mix_PlayChannel(channels[index], sounds[index], 0);
}

void SDLSound::startMusic() {
  Mix_VolumeMusic(volume);
  Mix_PlayMusic(music[0], -1);
}

void SDLSound::winMusic()const {
  Mix_HaltMusic();
  Mix_VolumeMusic(volume);
  Mix_PlayMusic(music[1], -1);
}

void SDLSound::stopMusic() {
  Mix_HaltMusic();
  Mix_FreeMusic(music[0]);
  Mix_FreeMusic(music[1]);
}

