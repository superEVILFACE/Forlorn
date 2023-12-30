#include <forlorn.h>
#include "SaveSelectScene.hpp"
#include "MenuScene.hpp"

USING_NS_AX;

SaveSelectScene* SaveSelectScene::create() {
    auto ret = new SaveSelectScene();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool SaveSelectScene::init()
{
    if (!Scene::init())
        return false;

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("MenuSheet.plist");
    auto back = Sprite::createWithSpriteFrameName("back_btn.png");
    auto backBtn = MenuItemSprite::create(back, back, AX_CALLBACK_1(SaveSelectScene::onBack, this));
    backBtn->setPosition(Vec2(89, 670));

    auto menu = Menu::create();
    menu->setPosition(Vec2(0,0));
    menu->addChild(backBtn);
    this->addChild(menu);

    scheduleUpdate();
    return true;
}

void SaveSelectScene::onBack(Ref* sender) {
    auto layer = MenuScene::create();
    auto scene = Scene::create();
    scene->addChild(layer);
    auto transition = TransitionFade::create(0.5f, scene);
    Director::getInstance()->replaceScene(transition);
}