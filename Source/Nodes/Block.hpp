#ifndef FORLORN_BLOCK
#define FORLORN_BLOCK

#include "axmol.h"
#include <json.hpp>
#include "../Utils/ForlornUtils.hpp"


enum BlockType : int
{
    UNDEFINED,
    DISABLED,
    PARTICLE,
    CURVED
};

//struct BlockData
//{
//    static BlockData fromObject(const json::Object&);
//
//    float rotation = 0.f;
//    float blockFriction = 0.f;
//
//
//    int zValue = 0;
//    int p_uID = 0;
//    int spawnedByTrigger = 0;
//    int mType = 0;
//
//    bool flippedX = false;
//    bool flippedY = false;
//    bool relativePosition = false;
//    bool invisible = false;
//    bool activateByTrigger = false;
//    bool animated = false;
//    bool trapControlled = false;
//    bool passable = false;
//    bool darken = false;
//    bool isLight = false;
//    bool disableSound = false;
//    bool sleep = false;
//    bool skipReset = false;
//    bool lockRotation = false;
//
//    std::string sheet;
//    std::string texture;
//    std::string customSound;
//
//};


//group class members by TYPE 
//always add a default value for built in types

struct Block : public ax::Sprite
{
    static Block* create(const json::Object& obj, bool isBG);

    //ints
    BlockType _objectType = BlockType::UNDEFINED;
    int _p_uID = 0; //correct zOrder?
   
    //floats
    float rotation = 0.0f;


    //bools
    bool _animated = false;
    bool _isLight = false;



    bool init(const json::Object& obj, bool isBG);
    void setScale(const ax::Vec2& s); //helper

    ax::Action* currentAnim;
    void runAnimation(std::string animName, float fps, bool loop)
    {
        stopAction(currentAnim);
        ax::Vector<ax::SpriteFrame*> frames;

        std::string loopedAnim = animName;
        if (loopedAnim.find_last_of(".") != std::string::npos)
            loopedAnim.insert(loopedAnim.find_last_of("."), "_looped_001");

        std::string loopedAnimName = loopedAnim;
            
        ForlornUtils::replaceString(animName, "_01.", "_%02d.");
        ForlornUtils::replaceString(animName, "_001.", "_%03d.");
        ForlornUtils::replaceString(loopedAnimName, "_01.", "_%02d.");
        ForlornUtils::replaceString(loopedAnimName, "_001.", "_%03d.");

        int frameIndex = 1;
        while (true) 
        {
            std::string frameName = ax::StringUtils::format((loopedAnim.find("_looped_001.") != std::string::npos) ? loopedAnimName.c_str() : animName.c_str(), frameIndex);
            ax::SpriteFrame* frame = ax::SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);

            if (frame) 
            {
                frames.pushBack(frame);
                frameIndex++;
            } 
            else {
                break;
            }
        }

        ax::Animation* animation = ax::Animation::createWithSpriteFrames(frames, 1.0f / fps);
        ax::Animate* animate = ax::Animate::create(animation);
        currentAnim = loop ? ax::RepeatForever::create(animate) : static_cast<ax::Action*>(animate);
        runAction(currentAnim);
    };
};


#endif