#ifndef _ANIMATEDSPRITE_H
#define _ANIMATEDSPRITE_H

#include <axmol.h>
 
class AnimatedSprite : public ax::Sprite
{ 
    public:

        static AnimatedSprite* createWithSpriteFrameName(const std::string& filename, float fps = 12.0f, bool loop = true) {
            AnimatedSprite* sprite = new AnimatedSprite();
            
            if (sprite && sprite->initWithSpriteFrameName(filename)) {
                sprite->autorelease();
                sprite->runAnimation(filename, fps, loop);
                return sprite;
            } else {
                delete sprite;
                return nullptr;
            }
        }
        void runAnimation(std::string animName, float fps, bool loop) {
            stopAllActions();
            ax::Vector<ax::SpriteFrame*> frames;

            if(animName.find("_01.") != std::string::npos)
                animName.replace(animName.find("_01."), 4, "_%02d.");
            else if (animName.find("_001.") != std::string::npos)
                animName.replace(animName.find("_001."), 5, "_%03d.");

            int frameIndex = 1;
            while (true) {
                const std::string frameName = ax::StringUtils::format(animName.c_str(), frameIndex);
                ax::SpriteFrame* frame = ax::SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);

                if (frame) {
                    frames.pushBack(frame);
                    frameIndex++;
                } else {
                    break;
                }
            }

            ax::Animation* animation = ax::Animation::createWithSpriteFrames(frames, 1.0f / fps);
            ax::Animate* animate = ax::Animate::create(animation);
            if (loop) {
                runAction(ax::RepeatForever::create(animate));
            } else {
                runAction(animate);
            }
        };
};

#endif