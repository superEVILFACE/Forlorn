#include <forlorn.h>
#include "MenuGameScene.hpp"

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

    auto bg = Sprite::create("cave_bg_01.png");
    bg->setPosition(Vec2(forlorn::getCenterPoint().x, 545));
    bg->setScale(2.5f);
    this->addChild(bg);

    scheduleUpdate();
    return true;
}