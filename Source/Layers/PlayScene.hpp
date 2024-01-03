#ifndef __PLAYSCENE_H__
#define __PLAYSCENE_H__

#include "axmol.h"
#include <json.hpp>


class PlayScene : public ax::Node
{   
public:
    bool upPressed = false;
    bool downPressed = false;
    bool leftPressed = false;
    bool rightPressed = false;
    ax::Vec2 spawnPos = { 0,0 };
    ax::Label* tempStatusLabel = nullptr;

    static PlayScene* create(std::string_view levelFileName);

    bool init() override;
    void update(float dt) override;

    bool initWithFile(std::string_view levelFileName);
    void initialLoading(std::string_view levelFileName);
    void readPlist(const json::Value& level);
    


    void initVars();
    void createPlayer(); //TODO
    void createUI(); //TODO
    void loadBlocks(const json::Object& blockContainer);
    void createBackground(); //TODO



    void onKeyPressed(ax::EventKeyboard::KeyCode code, ax::Event* event);
    void onKeyReleased(ax::EventKeyboard::KeyCode code, ax::Event* event);
};

#endif