#ifndef __LOADINGSCENE_H__
#define __LOADINGSCENE_H__

#include "axmol.h"

class LoadingScene : public ax::Scene
{   
    public:
        static LoadingScene* create();
        bool init() override;
        void loadAssets();
        void loadingFinished();
};

#endif