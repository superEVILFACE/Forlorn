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
    std::string levelData = FileUtils::getInstance()->getStringFromFile(filename);
    json levelJson = json::parse(levelData);
    json blockContainer = levelJson["blockContainer"];
    for (auto& block : blockContainer.items()) {
        std::string blockId = block.key();
        json blockData = block.value();

        int animated = ForlornUtils::getIntFromLevel(blockData, "animated");
        Vec2 position = ForlornUtils::getVec2FromLevel(blockData, "position");
        Vec2 scale = ForlornUtils::getVec2FromLevel(blockData, "scale");
        AXLOG("Block Details: id: %d, animated: %d, position, {%f, %f}, scale: {%f, %f}", std::stoi(blockId), animated, position.x, position.y, scale.x, scale.y);
    }
    return true;
}