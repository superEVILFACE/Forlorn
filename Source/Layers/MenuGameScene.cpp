#include "MenuGameScene.hpp"
#include "Utils/AnimatedSprite.hpp"
#include "Utils/ForlornUtils.hpp"
#include "PlayScene.hpp"
USING_NS_AX;

MenuGameScene* MenuGameScene::create() {
    auto ret = new MenuGameScene();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool MenuGameScene::init()
{
    if (!Scene::init())
        return false;

    auto titleBG = PlayScene::create("LevelCave.json");
    this->addChild(titleBG);
    scheduleUpdate();
    return true;
}