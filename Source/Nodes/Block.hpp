#ifndef FORLORN_BLOCK
#define FORLORN_BLOCK

#include "axmol.h"
#include <json.hpp>


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
    static Block* create(const json::Object& obj);

    //ints
    BlockType _objectType = BlockType::UNDEFINED;
    int _p_uID = 0; //correct zOrder?
   
    //floats
    float rotation = 0.0f;


    //bools
    bool _animated = false;
    bool _isLight = false;



    bool init(const json::Object& obj);
    void setScale(const ax::Vec2& s); //helper
};


#endif