#include <forlorn.h>
#include "MenuScene.hpp"
#include "MenuGameScene.hpp"

USING_NS_AX;

bool MenuScene::init()
{
    if (!Scene::init())
        return false;

    auto gameScene = MenuGameScene::create();
    this->addChild(gameScene);

    auto logo = Sprite::create("forlorn_logo_menu-hd.png");
    logo->setPosition(Vec2(forlorn::getCenterPoint().x, 545));
    this->addChild(logo);

    scheduleUpdate();
    return true;
}