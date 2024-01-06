#include "Block.hpp"
#include <json.hpp>
#include "JsonUtils.hpp"
#include "CustomFormatters.hpp"
#include "ForlornUtils.hpp"



USING_NS_AX;

Block* Block::create(const json::Object& obj, bool isBG)
{
    auto ret = new Block();
    if (ret && ret->init(obj, isBG))
    {
        ret->autorelease();
        return ret;
    }
    AX_SAFE_DELETE(ret);
    return nullptr;
}

bool Block::isSheetHD(std::string_view sheet)
{
    constexpr const char* no_hd[] =
    {
        "ForestSheet_MG", "lightSheet", "FrostLevel_MGSheet", "FlameVillageMG"
    };

    for (const auto& nohdsheet : no_hd)
        if (sheet == nohdsheet)
            return false;

    return true;
}

//!!TODO RE!!
//ax::Vec2 getParallaxPositionFrom(const ax::Vec2& inputPoint, std::string_view type)
//{
//    float x = inputPoint.x;
//    ax::Vec2 retstr;
//    if (type != "bg")
//    {
//        if (type == "mg")
//        {
//            retstr.x = 1.5;
//            retstr.y = 1.0;
//        }
//        else if (type == "fg")
//        {
//            retstr.x = 1.5;
//            retstr.y = 1.0;
//        }
//        else
//        {
//            // Handle other cases if needed
//            retstr.x = 0.0;
//            retstr.y = 0.0;
//        }
//    }
//    else
//    {
//        retstr.x = 0.2f;  // Example values, you can adjust as needed
//        retstr.y = 0.3f;  // Example values, you can adjust as needed
//    }
//
//    float v13 = 1.0 - retstr.x;
//    float v14 = 1.0 - retstr.y;
//
//    auto winSize = Director::getInstance()->getWinSize();
//    retstr.x = inputPoint.x + v14 * winSize.width / 2;
//    retstr.y = inputPoint.y + v13 * winSize.height / 2;
//
//    return retstr;
//}



bool Block::init(const json::Object& obj, bool isBG)
{
    if (auto type = JsonUtils::fromObject<std::string>(obj, "type"))
    {
        if (type.value() == "particle" || type.value() == "animated")
            return false;
    }

    _animated = JsonUtils::fromObject<bool>(obj, "animated").value_or(false);
    _isLight = JsonUtils::fromObject<bool>(obj, "isLight").value_or(false);


    if (auto spriteSheet = JsonUtils::fromObject<std::string>(obj, "spriteSheet"))
    {
        auto texture = JsonUtils::fromObject<std::string>(obj, "texture");
        if(_animated && !_isLight) {
            runAnimation(texture.value(), 12.0f, true); //keep loop true for now
        }
        if (!texture || !initWithSpriteFrameName(texture.value()))
        {
            return false;
        }
    }

    if (auto position = JsonUtils::fromObject<Vec2>(obj, "position"))
    {
        setPosition(position.value());
    }
    else
    {
        return false;
    }

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
        if (auto spritesheet = JsonUtils::fromObject<std::string>(obj, "spriteSheet"))
        {
            setScale(Block::isSheetHD(spritesheet.value()) ? scale.value() * 0.5f : scale.value());
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

//TODO: handle startAnim/endAnim if needed
void Block::runAnimation(std::string_view animName, float fps, bool loop)
{
    if(currentAnim)
        stopAction(currentAnim);

    constexpr auto npos = std::string_view::npos;
    auto point = animName.find_last_of('.');
    if (point == npos) return;


    std::string_view rawName = ForlornUtils::substrOffset(animName, 0, point - 1);
    int i = 1;
    ax::Vector<ax::SpriteFrame*> frames;
    while (auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(fmt::format("{}_looped_{:03}.png", rawName, i)))
    {
        frames.pushBack(frame);
        i += 1;
    }

    ax::Animation* animation = ax::Animation::createWithSpriteFrames(frames, 1.0f / fps);
    if (!animation)
    {
        AX_SAFE_DELETE(animation);
        return;
    }

    animation->setLoops(UINT_MAX);

    currentAnim = ax::Animate::create(animation);
    if (!currentAnim)
    {
        AX_SAFE_DELETE(currentAnim);
        return;
    }

    runAction(currentAnim);
};

