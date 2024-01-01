#ifndef __LOADINGSCENE_H__
#define __LOADINGSCENE_H__

#include "axmol.h"

class LoadingScene : public ax::Scene
{   
    public:
        size_t loadedResources = 0;
        ax::Label* progressLabel = nullptr;

        static LoadingScene* create();
        bool init() override;
        void loadAssets();
        void loadingFinished();
        void updateLabel();
};

#endif