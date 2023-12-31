#ifndef __PLAYSCENE_H__
#define __PLAYSCENE_H__

#include "axmol.h"

class PlayScene : public ax::Node
{   
    public:
        static PlayScene* create(std::string_view levelfile);
        bool init() override;
        bool initWithFile(std::string_view filename);
};

#endif