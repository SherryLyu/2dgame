#include <string>
#include <iostream>
#include <ctime>
#include "sound.h"
using std::string;

SDLSound::~SDLSound() {
  Mix_HaltMusic();
  Mix_FreeMusic(music);
  for (unsigned int i = 0; i < sounds.size(); ++i) {
    Mix_FreeChunk(sounds[i]);
  }
  Mix_CloseAudio();
}

SDLSound::SDLSound() : 
  volume(SDL_MIX_MAXVOLUME), 
  currentSound(-1), 
  music(NULL),
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
  music = Mix_LoadMUS("sound/OldeTimey.mp3");
  if (!music) throw string("Couldn't load OldeTimey.mp3")+Mix_GetError();

  startMusic();

  sounds.push_back( Mix_LoadWAV("sound/girlhit.wav") );
  sounds.push_back( Mix_LoadWAV("sound/explosion.wav") );

  for (unsigned int i = 0; i < sounds.size(); ++i) channels.push_back(i);
}


SDLSound::SDLSound(const SDLSound& s) :
  volume( s.volume ), 
  currentSound( s.currentSound ), 
  music( s.music ),
  audioRate( s.audioRate ), 
  audioChannels( s.audioChannels ), 
  audioBuffers( s.audioBuffers ),
  sounds( s.sounds ),
  channels( s.channels ) 
  { }

SDLSound& SDLSound::operator=(const SDLSound& s) {
  volume = ( s.volume ); 
  currentSound = ( s.currentSound ); 
  music = ( s.music );
  audioRate = ( s.audioRate ); 
  audioChannels = ( s.audioChannels ); 
  audioBuffers = ( s.audioBuffers );
  sounds = ( s.sounds );
  channels = ( s.channels );
  return *this;
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
     Mix_PlayChannel(-1, sounds[index], 0);
}

void SDLSound::startMusic() {
  Mix_VolumeMusic(volume);
  Mix_PlayMusic(music, -1);
}

void SDLSound::stopMusic() {
  Mix_HaltMusic();
  Mix_FreeMusic(music);
}

