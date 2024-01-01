#include "LoadingScene.hpp"
#include "MenuScene.hpp"
#include "Utils/ForlornUtils.hpp"
#include <fmt/format.h>


constexpr static auto sprites = std::to_array<const char*>({
    "BatSpriteSheet.png", "bigFont.png", "BK_Sheet.png", "Blacksmith_Sheet.png", "bluearrow.png",
    "bluesquare.png", "bossHealthBar.png", "BOWSpriteSheet.png", "BubbleSheet.png",
    "BunnyBossSheet.png", "buttonBG2.png", "cave_bg_01.png", "chain001.png", "chain01.png",
    "character_shadow.png", "chatFont.png", "chatIcon.png", "cloisterFont.png", "colorSquare.png",
    "CommonSheet_01.png", "Default.png", "Default@2x.png", "Effects_spritesheet_01.png", "FireflySheet.png",
    "FireSheet_01.png", "Fire_Main_Character_spritesheet_01.png", "FlameVillageMG.png",
    "FlameVillageSheet01.png", "FlameVillageSheet02.png", "FlameVillageSheet03.png", "FlameVillageSheet04.png",
    "FL_Map_World.png", "ForestSheet_01.png", "ForestSheet_02.png", "ForestSheet_03.png", "ForestSheet_MG.png",
    "forest_bg_01.png", "forlorn_logo_menu.png", "fps_images.png", "FrostLevel_MGSheet.png", "FrostLevel_Sheet01.png",
    "FrostLevel_Sheet02.png", "Frost_Main_Character_spritesheet_01.png", "game.png", "gameFnt.png",
    "GateKeeper_Sheet.png", "goldFont.png", "Golem_Sheet.png", "green_arrow.png", "heartPiece.png",
    "Icon-72.png", "Icon-Small-50.png", "Icon-Small.png", "Icon-Small@2x.png", "Icon.png", "Icon@2x.png",
    "ImpNPCSheet.png", "iTunesArtwork.png", "Knight_Sheet.png", "LargeCreature_Sheet.png", "laserTest.png",
    "lava2.png", "lightSheet.png", "light_b.png", "light_r.png", "Ling_Sheet.png", "MenuSheet.png",
    "particleImgSheet.png", "poison1.png", "rope.png", "SBTShark_Sheet.png", "shopUI_forest.png",
    "shopUI_frost.png", "shopUI_village.png", "SkelChamp_Sheet.png", "Skeleling_Sheet.png", "Skeleton_Sheet.png",
    "Skull_Sheet.png", "sliderBar.png", "slidergroove.png", "sliderthumb.png", "sliderthumbsel.png",
    "softFont.png", "square01_001.png", "square02_001.png", "StoneFish_Sheet.png", "sunTexture.png", "tar01.png",
    "timer_innerCircle.png", "timer_outerCircle.png", "toxic1.png", "TS_Sheet.png", "tutorial_attack.png",
    "tutorial_item.png", "tutorial_movable01.png", "tutorial_movable02.png", "tutorial_stance.png",
    "tutorial_switch.png", "tutorial_sword.png", "UISheet.png", "VillageMG.png", "VillageSheet01.png",
    "VillageSheet02.png", "VillageSheet03.png", "VillageSheet04.png", "village_bg_01.png", "village_bg_02.png",
    "water5.png", "Witch_Sheet.png", "ZombieBunny_Sheet.png" });


constexpr static auto plists = std::to_array<const char*>({
    "CommonSheet_01.plist", "FrostLevel_Sheet02.plist", "FrostLevel_Sheet01.plist", "LevelCave.plist",
    "MenuSheet.plist"
    });

constexpr static auto fonts = std::to_array<const char*>({
    "bigFont.fnt", "softFont.fnt", "cloisterFont.fnt", "chatFont.fnt",
    "game.fnt", "gameFnt.fnt", "goldFont.fnt"
    });


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


    Vec2 center = ForlornUtils::getCenter();

    if (auto banner = Sprite::create("Default.png")) {
        banner->setPosition(Vec2(center.x, center.y - 10));
        banner->setScale(1.35);
        this->addChild(banner);
    }

    if (progressLabel = Label::createWithBMFont("bigFont.fnt", "")) {
        progressLabel->setPosition({ center.x, center.y - 250 });
        updateLabel();
        addChild(progressLabel);
    }

    this->runAction(Sequence::create(CallFunc::create([this]() { this->loadAssets(); }), nullptr));
    return true;
}

void LoadingScene::loadAssets() {

    auto* textureCache = Director::getInstance()->getTextureCache();
    for (auto sprite : sprites) {
        textureCache->addImageAsync(sprite, [this](ax::Texture2D*) { loadedResources++; updateLabel(); });
    }

    for(auto font : fonts) {
        ax::Label::createWithBMFont("", font);
        loadedResources++;
        updateLabel();
    }

    auto* frameCache = SpriteFrameCache::getInstance();
    for (auto plist : plists) {
        frameCache->addSpriteFramesWithFile(plist);
        loadedResources++;
        updateLabel();
    }
}

void LoadingScene::loadingFinished() {
    auto layer = MenuScene::create();
    auto scene = Scene::create();
    scene->addChild(layer);
    Director::getInstance()->replaceScene(scene);
}

void LoadingScene::updateLabel()
{
    constexpr size_t total = fonts.size() + sprites.size() + plists.size();

    float percentage = (static_cast<float>(loadedResources) / total) * 100;
    progressLabel->setString(fmt::format("{}%", static_cast<int>(percentage)));

    if(static_cast<int>(percentage) == 100)
        loadingFinished();
}
