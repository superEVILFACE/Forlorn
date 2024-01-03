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

void PlayScene::update(float dt)
{
    auto cam = Camera::getDefaultCamera();
    constexpr float moveSpeed = 7.0f;

    if (upPressed) cam->setPositionY(cam->getPositionY() + moveSpeed);
    if (downPressed) cam->setPositionY(cam->getPositionY() - moveSpeed);
    if (leftPressed) cam->setPositionX(cam->getPositionX() - moveSpeed);
    if (rightPressed) cam->setPositionX(cam->getPositionX() + moveSpeed);

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

    auto spawnRes = JsonUtils::Vec2FromArrayString(file["playerSpawn"].as_string());
    if (spawnRes.has_value()) {
        this->spawnPos = spawnRes.value();
    }

    fmt::println("loading spritesheets");
    for (const auto& sheet : file["sheetContainer"].as_object()) {
        auto sheetStr = JsonUtils::valueFromObject<std::string>(sheet.second.as_object(), "texture");
        if (sheetStr.has_value()) {
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile(fmt::format("{}.plist", sheetStr.value()));
        }
    }
    fmt::println("loading finished");


    const json::Object& settingsData = file["settings"].as_object();
    std::string bgImage = JsonUtils::valueFromObject<std::string>(settingsData, "bgImage").value_or("") + ".png";
    Sprite* bg = Sprite::create(bgImage);
    if(bg == nullptr) {
        fmt::println("Failted to create BG with texture: {}", bgImage);
    } else {
        bg->setPosition(Vec2(ForlornUtils::getCenter().x, 545));
        bg->setScale(2.5f);
        this->addChild(bg);
    }

    //Vec2 levelSize = JsonUtils::Vec2FromArrayString(file["levelSize"].as_string()).value_or(Vec2(0, 0));

    for (const auto& block : file["blockContainer"].as_object()) {
        const json::Object& blockData = block.second.as_object();

        auto objType = JsonUtils::valueFromObject<std::string>(blockData, "type");
        if (objType.has_value() && objType.value() == "particle")
            continue;
        //
        //since all fields are strings by default we need to get them from the json as strings and convert them
        //should not result in many allocations due to small string optimization

        //std::string posStr = JsonUtils::valueFromObject<std::string>(blockData, "position").value_or("");
        //Vec2 pos = JsonUtils::Vec2FromArray(posStr).value_or(Vec2(0, 0));
        Vec2 pos = JsonUtils::Vec2FromJsonObject(blockData, "position").value_or(Vec2(0, 0));

        Vec2 flipped = JsonUtils::Vec2FromJsonObject(blockData, "flipped").value_or(Vec2(0, 0));
        bool flipX = flipped.x == 1;
        bool flipY = flipped.y == 1;
        
        std::string animatedStr = JsonUtils::valueFromObject<std::string>(blockData, "animated").value_or("");
        int animated = ForlornUtils::fromString<int>(animatedStr).value_or(0);

        std::string isLightStr = JsonUtils::valueFromObject<std::string>(blockData, "isLight").value_or("");
        int isLight = ForlornUtils::fromString<int>(isLightStr).value_or(0);

        std::string texture = JsonUtils::valueFromObject<std::string>(blockData, "texture").value_or("");
        bool isPNG = texture.substr(texture.size() - 4) == ".png";

        bool spriteSheet = true;

        std::string zStr = JsonUtils::valueFromObject<std::string>(blockData, "zValue").value_or("");
        int zValue = ForlornUtils::fromString<int>(zStr).value_or(0);

        std::string p_uIDStr = JsonUtils::valueFromObject<std::string>(blockData, "p_uID").value_or("");
        int p_uID = ForlornUtils::fromString<int>(p_uIDStr).value_or(0);

        Vec2 scale = JsonUtils::Vec2FromJsonObject(blockData, "scale").value_or(Vec2(0, 0));

        std::string rotationStr = JsonUtils::valueFromObject<std::string>(blockData, "rotation").value_or("");
        float rotation = ForlornUtils::fromString<float>(rotationStr).value_or(0);

        std::string opacityStr = JsonUtils::valueFromObject<std::string>(blockData, "opacity").value_or("");
        float opacityValue = ForlornUtils::fromString<float>(opacityStr).value_or(255);
        float opacity = opacityValue != 255 ? opacityValue * 255 : opacityValue;

        std::string redStr = JsonUtils::valueFromObject<std::string>(blockData, "red").value_or("");
        float redValue = ForlornUtils::fromString<float>(redStr).value_or(255);
        float red = redValue != 255 ? redValue * 255 : redValue;

        std::string greenStr = JsonUtils::valueFromObject<std::string>(blockData, "green").value_or("");
        float greenValue = ForlornUtils::fromString<float>(greenStr).value_or(255);
        float green = greenValue != 255 ? greenValue * 255 : greenValue;

        std::string blueStr = JsonUtils::valueFromObject<std::string>(blockData, "blue").value_or("");
        float blueValue = ForlornUtils::fromString<float>(blueStr).value_or(255);
        float blue = blueValue != 255 ? blueValue * 255 : blueValue;

        Sprite* sprite = nullptr;
        if(animated == 1) {
            if(isPNG)
                sprite = AnimatedSprite::createWithSpriteFrameName(texture);
        }
        else if (spriteSheet) {
            if(isPNG)
                sprite = Sprite::createWithSpriteFrameName(texture);
        } else {
            if(isPNG)
                sprite = Sprite::create(texture);
        }

        if(!sprite) 
        {
            fmt::println("Failed to create sprite with texture: {}", texture);
        } else {
            sprite->setPosition(pos);
            sprite->setFlippedX(flipX);
            sprite->setFlippedY(flipY);
            sprite->setRotation(rotation);
            sprite->setColor(Color3B(red, green, blue));
            if(isLight == 1 && animated == 1)
            {
                sprite->setScale(scale.x, scale.y);
                auto fade1 = FadeTo::create(RandomHelper::random_real(1.0f, 2.0f), opacity / 4);
                auto fade2 = FadeTo::create(RandomHelper::random_real(1.0f, 2.0f), opacity);
                sprite->runAction(RepeatForever::create(Sequence::create(fade1, fade2, nullptr)));
            } else if (isLight == 1)
            {
                sprite->setScale(scale.x, scale.y);
                sprite->setOpacity(opacity);
            } else {
                sprite->setScale(scale.x * 0.5f, scale.y * 0.5f);
                sprite->setOpacity(opacity);
            }
            this->addChild(sprite, p_uID);
            //fmt::println("blockID: {}, texture: {}, animated: {}, spriteSheet: {}, order: {}, pos: [{}, {}], scale: [{}, {}], rotation: {}, flipped: [{}, {}]", block.first, texture, animated, spriteSheet, zValue, pos.x, pos.y, scale.x, scale.y, rotation, flipX, flipY);
        }
        
    }

    auto keyboardListener           = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed  = AX_CALLBACK_2(PlayScene::onKeyPressed, this);
    keyboardListener->onKeyReleased = AX_CALLBACK_2(PlayScene::onKeyReleased, this); 
    _eventDispatcher->addEventListenerWithFixedPriority(keyboardListener, 11);

    scheduleUpdate();

    scheduleOnce([this](float) {
        auto cam = Camera::getDefaultCamera();
        cam->setPosition(this->spawnPos);
        fmt::println("zoom: {}", cam->getZoom());
        cam->setZoom(cam->getZoom() * 0.5);
    }, 0.0f, "set_camera_func");



    return true;
}

void PlayScene::onKeyPressed(ax::EventKeyboard::KeyCode code, ax::Event* event) {

    switch (code) {
    case EventKeyboard::KeyCode::KEY_W: upPressed = true; break;
    case EventKeyboard::KeyCode::KEY_A: leftPressed = true; break;
    case EventKeyboard::KeyCode::KEY_S: downPressed = true; break;
    case EventKeyboard::KeyCode::KEY_D: rightPressed = true; break;
    default: break;
    }
}

void PlayScene::onKeyReleased(ax::EventKeyboard::KeyCode code, ax::Event* event) {
    switch (code) {
    case EventKeyboard::KeyCode::KEY_W: upPressed = false; break;
    case EventKeyboard::KeyCode::KEY_A: leftPressed = false; break;
    case EventKeyboard::KeyCode::KEY_S: downPressed = false; break;
    case EventKeyboard::KeyCode::KEY_D: rightPressed = false; break;
    default: break;
    }

}
