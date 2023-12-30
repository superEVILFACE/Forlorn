#ifndef __MENUSCENE_H__
#define __MENUSCENE_H__
#include <axmol.h>

class MenuScene : public ax::Scene
{   
    public:
        static MenuScene* create();
        bool init() override;
        void onPlay(Ref* sender);
};

#endif