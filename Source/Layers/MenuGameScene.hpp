#ifndef __MENUGAMESCENE_H__
#define __MENUGAMESCENE_H__
#include <axmol.h>

class MenuGameScene : public ax::Scene
{   
    public:
        static MenuGameScene* create();
        bool init() override;
};

#endif