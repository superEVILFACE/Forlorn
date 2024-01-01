#include "PlayScene.hpp"
#include "ForlornUtils.hpp"
#include "JsonUtils.hpp"
#include "AnimatedSprite.hpp"
#include <fmt/format.h>
#include <json.hpp>


USING_NS_AX;

PlayScene* PlayScene::create(std::string_view levelfile) {
    auto level = new PlayScene();
    if (level && level->initWithFile(levelfile))
    {
        level->autorelease();
        return level;
    }
    return nullptr;
}

bool PlayScene::init() {
    if (!Node::init())
        return false;

    //scheduleUpdate();
    return true;
}

bool PlayScene::initWithFile(std::string_view filename) {
    std::string levelData = FileUtils::getInstance()->getStringFromFile(filename);
    //json levelJson = json::parse(levelData);
    //json blockContainer = levelJson["blockContainer"];
    //for (auto& block : blockContainer.items()) {
    //    std::string blockId = block.key();
    //    json blockData = block.value();
    //
    //    int animated = ForlornUtils::getIntFromLevel(blockData, "animated");
    //    Vec2 position = ForlornUtils::getVec2FromLevel(blockData, "position");
    //    Vec2 scale = ForlornUtils::getVec2FromLevel(blockData, "scale");
    //    AXLOG("Block Details: id: %d, animated: %d, position, {%f, %f}, scale: {%f, %f}", std::stoi(blockId), animated, position.x, position.y, scale.x, scale.y);
    //}

    json::Value file = json::parse(levelData);

    for (const auto& block : file["blockContainer"].as_object()) {
        const json::Object& blockData = block.second.as_object();

        //since all fields are strings by default we need to get them from the json as strings and convert them
        //should not result in many allocations due to small string optimization

        //std::string posStr = JsonUtils::valueFromObject<std::string>(blockData, "position").value_or("");
        //Vec2 pos = JsonUtils::Vec2FromArray(posStr).value_or(Vec2(0, 0));
        Vec2 pos = JsonUtils::Vec2FromJsonObject(blockData, "position").value_or(Vec2(0, 0));
        
        std::string animatedStr = JsonUtils::valueFromObject<std::string>(blockData, "animated").value_or("");
        int animated = ForlornUtils::fromString<int>(animatedStr).value_or(0);

        Vec2 scale = JsonUtils::Vec2FromJsonObject(blockData, "scale").value_or(Vec2(0, 0));

        fmt::println("blockID: {}, pos: [{}, {}], animated: {}, scale: [{}, {}]", block.first, pos.x, pos.y, animated, scale.x, scale.y);
        
    }
    return true;
}