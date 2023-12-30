#ifndef __FORLORN_H__
#define __FORLORN_H__

#include <axmol.h>
#include <cocos2d.h>

#include "AppDelegate.hpp"
#include "Utils/AnimatedSprite.hpp"
#include "Managers/GameSoundManager.hpp"

#include "audio/AudioEngine.h"

namespace forlorn {
    inline const ax::Point getCenterPoint() {
        return ax::Director::getInstance()->getWinSize() / 2;
    }
}

#endif