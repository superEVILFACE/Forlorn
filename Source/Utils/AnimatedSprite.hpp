#ifndef _ANIMATEDSPRITE_H
#define _ANIMATEDSPRITE_H

#include <forlorn.h>
 
class AnimatedSprite : public ax::Sprite
{ 
    public:

        static AnimatedSprite* createWithSpriteFrameName(const std::string& filename) {
            AnimatedSprite* sprite = new AnimatedSprite();
            if (sprite && sprite->initWithSpriteFrameName(filename)) {
                sprite->autorelease();
                return sprite;
            } else {
                delete sprite;
                return nullptr;
            }
        }
        void runAnimation(const std::string& animName, float fps, bool loop) {
            ax::Vector<ax::SpriteFrame*> frames;

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