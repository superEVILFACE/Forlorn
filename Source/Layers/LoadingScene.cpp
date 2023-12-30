#include "LoadingScene.hpp"
#include "MenuScene.hpp"
#include "Utils/ForlornUtils.hpp"

USING_NS_AX;

LoadingScene* LoadingScene::create() {
    auto ret = new LoadingScene();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool LoadingScene::init()
{
    if (!Scene::init())
        return false;

    auto banner = Sprite::create("Default.png");
    return true;
}

constexpr static auto fonts = std::to_array<const char*>({
    "bigFont.fnt", "softFont.fnt", "cloisterFont.fnt", "chatFont.fnt",
    "game.fnt", "gameFnt.fnt", "goldFont.fnt"
});

void LoadingScene::loadAssets() {
    for(auto font : fonts) {
        ForlornUtils::getFont(font);
    }
}

void LoadingScene::loadingFinished() {
    auto layer = MenuScene::create();
    auto scene = Scene::create();
    scene->addChild(layer);
    auto transition = TransitionFade::create(0.5f, scene);
    Director::getInstance()->replaceScene(transition);
}