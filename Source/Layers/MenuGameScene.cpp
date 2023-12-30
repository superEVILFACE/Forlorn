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

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("FrostLevel_Sheet02-hd.plist");
    auto ground1 = Sprite::createWithSpriteFrameName("sn_snowground_big.png");
    ground1->setScale(1.15f);
    ground1->setPosition(Vec2(1045, -15));
    this->addChild(ground1);
    auto ground2 = Sprite::createWithSpriteFrameName("sn_snowground_big.png");
    ground2->setScale(1.15f);
    ground2->setPosition(Vec2(45, -15));
    this->addChild(ground2);

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("CommonSheet_01-hd.plist");
    auto fire = AnimatedSprite::createWithSpriteFrameName("Fire_01_looped_001.png");
    fire->runAnimation("Fire_01_looped_%03d.png", 12.0f, true);
    fire->setPosition(forlorn::getCenterPoint());
    this->addChild(fire);

    scheduleUpdate();
    return true;
}