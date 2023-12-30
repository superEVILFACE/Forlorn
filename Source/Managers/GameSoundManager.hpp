#ifndef __GAMESOUND_MANAGER_H__
#define __GAMESOUND_MANAGER_H__

#include <axmol.h>

class GameSoundManager : public ax::Node
{   
public:
    static GameSoundManager* sharedManager();
    
    void playBackgroundMusic(const char* file, bool loop = true, float volume = 1.0f);
    void stopBackgroundMusic();
    
    void playEffect(const char* file, float volume = 1.0f);
};

#endif