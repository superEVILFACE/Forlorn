#include <forlorn.h>
USING_NS_AX;

static GameSoundManager* s_SharedManager = nullptr;

GameSoundManager* GameSoundManager::sharedManager()
{
    if (!s_SharedManager)
        s_SharedManager = new GameSoundManager;

    return s_SharedManager;
}

void GameSoundManager::playBackgroundMusic(const char* file, bool loop, float volume) {
    if(file) AudioEngine::play2d(file, loop, volume);
    else AXLOG("Sound file not found");
}

void GameSoundManager::stopBackgroundMusic() {
    AudioEngine::stop(1);
}

void GameSoundManager::playEffect(const char* file, float volume) {
    if(file) AudioEngine::play2d(file, false, volume);
    else AXLOG("Sound file not found");
}