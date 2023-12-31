#ifndef __LOADINGSCENE_H__
#define __LOADINGSCENE_H__

#include "axmol.h"

class LoadingScene : public ax::Scene
{   
    public:
        size_t loadedFonts = 0;
        size_t loadedSprites = 0;
        size_t loadedPlists = 0;
        ax::Label* progressLabel = nullptr;

        static LoadingScene* create();
        bool init() override;
        void update(float dt) override;
        void loadAssets();
        void loadingFinished();
};

#endif