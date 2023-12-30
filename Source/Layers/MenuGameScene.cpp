#include "MenuGameScene.hpp"
#include "Utils/AnimatedSprite.hpp"
#include "Utils/ForlornUtils.hpp"
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
    bg->setPosition(Vec2(ForlornUtils::getCenter().x, 545));
    bg->setScale(2.5f);
    this->addChild(bg);

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("CommonSheet_01.plist");
    auto fire1 = AnimatedSprite::createWithSpriteFrameName("Fire_01.png");
    fire1->setPosition(Vec2(431.9, 242.7));
    fire1->setScale(0.69f);
    fire1->runAnimation("Fire_01_looped_%03d.png", 12.0f, true);
    this->addChild(fire1);

    auto fire2 = AnimatedSprite::createWithSpriteFrameName("Fire_01.png");
    fire2->setPosition(Vec2(44.9, 242.7));
    fire2->setScale(0.69f);
    fire2->runAnimation("Fire_01_looped_%03d.png", 12.0f, true);
    this->addChild(fire2);

    scheduleUpdate();
    return true;
}