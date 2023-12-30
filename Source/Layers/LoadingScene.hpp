#ifndef __LOADINGSCENE_H__
#define __LOADINGSCENE_H__
#include <forlorn.h>

class LoadingScene : public ax::Scene
{   
    public:
        static LoadingScene* create();
        bool init() override;
        void loadAssets();
        void loadingFinished();
};

#endif