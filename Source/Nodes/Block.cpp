#include "Block.hpp"
#include <json.hpp>
#include "JsonUtils.hpp"
#include "CustomFormatters.hpp"


USING_NS_AX;

Block* Block::create(const json::Object& obj)
{
    auto ret = new Block();
    if (ret && ret->init(obj))
    {
        ret->autorelease();
        return ret;
    }
    AX_SAFE_DELETE(ret);
    return nullptr;
}

bool Block::init(const json::Object& obj)
{
    if (auto type = JsonUtils::fromObject<std::string>(obj, "type"))
    {
        if (type.value() == "particle" || type.value() == "animated")
            return false;
    }


    if (auto spriteSheet = JsonUtils::fromObject<std::string>(obj, "spriteSheet"))
    {
        auto texture = JsonUtils::fromObject<std::string>(obj, "texture");
        if (!texture || !initWithSpriteFrameName(texture.value())) {
            return false;
        }
    }


    if (auto position = JsonUtils::fromObject<Vec2>(obj, "position")) {
        setPosition(position.value());
    }
    else {
        return false;
    }


    _animated = JsonUtils::fromObject<bool>(obj, "animated").value_or(false);
    _isLight = JsonUtils::fromObject<bool>(obj, "isLight").value_or(false);
    _p_uID = JsonUtils::fromObject<int>(obj, "p_uID").value_or(0);
    
    if (auto flipped = JsonUtils::fromObject<Vec2>(obj, "flipped"))
    {
        setFlippedX(flipped.value().x == 1.0f);
        setFlippedY(flipped.value().y == 1.0f);
    }

    if(auto opacity = JsonUtils::fromObject<float>(obj, "opacity")) setOpacity(opacity.value() * 255);
    if (auto rotation = JsonUtils::fromObject<float>(obj, "rotation")) setRotation(rotation.value());


    if (auto red = JsonUtils::fromObject<float>(obj, "red"))
    {
        auto green = JsonUtils::fromObject<float>(obj, "green");
        auto blue = JsonUtils::fromObject<float>(obj, "blue");
        if (green && blue) {
            setColor(ForlornUtils::getColor3B(red.value() * 255, green.value() * 255, blue.value() * 255));
        }
    }

    if (auto scale = JsonUtils::fromObject<Vec2>(obj, "scale"))
    {
        if (_isLight || _animated) {
            setScale(scale.value());
        }
        else {
            setScale(scale.value() * 0.5f);
        }
    }

    if (_isLight && _animated)
    {
        auto fade1 = FadeTo::create(RandomHelper::random_real(1.0f, 2.0f), getOpacity() / 4);
        auto fade2 = FadeTo::create(RandomHelper::random_real(1.0f, 2.0f), getOpacity());
        this->runAction(RepeatForever::create(Sequence::create(fade1, fade2, nullptr)));
    }

    return true;

}

void Block::setScale(const ax::Vec2& s)
{
    setScaleX(s.x);
    setScaleY(s.y);
}

