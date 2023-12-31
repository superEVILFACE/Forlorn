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

    auto bg = Sprite::create("cave_bg_01.png");
    bg->setPosition(Vec2(ForlornUtils::getCenter().x, 545));
    bg->setScale(2.5f);
    this->addChild(bg);

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("CommonSheet_01.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("FrostLevel_Sheet02.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("FrostLevel_Sheet01.plist");

    auto hi = PlayScene::create("LevelCave.plist");
    this->addChild(hi);
    scheduleUpdate();
    return true;
}