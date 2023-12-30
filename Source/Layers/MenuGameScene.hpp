#ifndef __MENUGAMESCENE_H__
#define __MENUGAMESCENE_H__
#include <forlorn.h>

class MenuGameScene : public ax::Scene
{   
    public:
        static MenuGameScene* create();
        bool init() override;
};

#endif