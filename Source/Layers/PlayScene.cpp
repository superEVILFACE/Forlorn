#include "PlayScene.hpp"
#include "Utils/ForlornUtils.hpp"
#include "Utils/AnimatedSprite.hpp"
#include "Utils/json.hpp"
using json = nlohmann::json;

USING_NS_AX;

PlayScene* PlayScene::create(std::string_view levelfile) {
    auto level = new PlayScene();
    if (level->initWithFile(levelfile))
    {
        level->autorelease();
        return level;
    }
    return nullptr;
}

bool PlayScene::init() {
    if (!PlayScene::init())
        return false;

    scheduleUpdate();
    return true;
}

bool PlayScene::initWithFile(std::string_view filename) {
    ValueMap levelData = FileUtils::getInstance()->getValueMapFromFile(filename);
    ValueMap blockContainer = levelData["blockContainer"].asValueMap();
    for (const auto& pair : blockContainer) {
        int blockId = std::stoi(pair.first);
        ValueMap blockData = pair.second.asValueMap();

        bool animated = blockData["animated"].asString() == "1";
        bool isSpriteSheet = blockData["spriteSheet"].asBool();
        Vec2 position = PointFromString(blockData["position"].asString());
        std::string scale = blockData["scale"].asString();
        float scaleX, scaleY;
        sscanf(scale.c_str(), "{%f, %f}", &scaleX, &scaleY);
        float rotation = blockData["rotation"].asFloat();
        Sprite* sprite;
        if(animated) {
            sprite = AnimatedSprite::createWithSpriteFrameName(blockData["texture"].asString());
        } else if(isSpriteSheet) {
            sprite = Sprite::createWithSpriteFrameName(blockData["texture"].asString());
        } else {
            sprite = Sprite::create(blockData["texture"].asString());
        }
        sprite->setTag(blockId);
        sprite->setPosition(position);
        sprite->setScale(scaleX, scaleY);
        if(blockData["rotation"].asBool())
            sprite->setRotation(rotation);
        this->addChild(sprite);
        AXLOG("Sprite added\n details: id: %d, scale: {%f, %f}, rotation: %f", blockId, scaleX, scaleY, rotation);
    }
    return true;
}