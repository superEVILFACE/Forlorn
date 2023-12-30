#include "MenuScene.hpp"
#include "MenuGameScene.hpp"
#include "SaveSelectScene.hpp"
#include "Utils/ForlornUtils.hpp"

#include <audio/AudioEngine.h>


USING_NS_AX;

MenuScene* MenuScene::create() {
    auto ret = new MenuScene();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool MenuScene::init()
{
    if (!Scene::init())
        return false;

    auto gameScene = MenuGameScene::create();
    this->addChild(gameScene);

    auto logo = Sprite::create("forlorn_logo_menu.png");
    logo->setPosition(Vec2(ForlornUtils::getCenter().x, 545));
    this->addChild(logo);

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("MenuSheet.plist");
    auto play = Sprite::createWithSpriteFrameName("play_btn.png");
    auto playSelected = Sprite::createWithSpriteFrameName("play_btn.png");
    playSelected->setOpacity(200);
    auto playBtn = MenuItemSprite::create(play, playSelected, AX_CALLBACK_1(MenuScene::onPlay, this));
    playBtn->setPosition(Vec2(ForlornUtils::getCenter().x, 200));

    auto menu = Menu::create();
    menu->setPosition(Vec2::ZERO);
    menu->addChild(playBtn);
    this->addChild(menu);
    scheduleUpdate();
    return true;
}

void MenuScene::onPlay(Ref* sender) {
    AudioEngine::play2d("FL_StartButton.ogg", false);
    auto layer = SaveSelectScene::create();
    auto scene = Scene::create();
    scene->addChild(layer);
    auto transition = TransitionFade::create(0.5f, scene);
    Director::getInstance()->replaceScene(transition);
}