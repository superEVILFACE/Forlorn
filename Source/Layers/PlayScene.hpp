#ifndef __PLAYSCENE_H__
#define __PLAYSCENE_H__

#include "axmol.h"

class PlayScene : public ax::Node
{   
    public:
        bool upPressed = false;
        bool downPressed = false;
        bool leftPressed = false;
        bool rightPressed = false;
        ax::Vec2 spawnPos = { 0,0 };

        static PlayScene* create(std::string_view levelfile);
        bool init() override;
        void update(float dt) override;
        bool initWithFile(std::string_view filename);
        void onKeyPressed(ax::EventKeyboard::KeyCode code, ax::Event* event);
        void onKeyReleased(ax::EventKeyboard::KeyCode code, ax::Event* event);

};

#endif