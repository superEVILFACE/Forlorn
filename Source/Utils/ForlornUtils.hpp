
#include <axmol.h>
#include "json.hpp"

namespace ForlornUtils
{
	inline ax::Vec2 getCenter()
	{
		return ax::Director::getInstance()->getWinSize() / 2;
	}

	inline void setCenter(ax::Node* node)
	{
		node->setPosition(ForlornUtils::getCenter());
	}


	inline ax::Vec2 getVec2FromLevel(nlohmann::json data, std::string value) {
		if (data.contains(value)) {
            std::string vec2Str = data[value];
            std::replace(vec2Str.begin(), vec2Str.end(), '{', ' ');
            std::replace(vec2Str.begin(), vec2Str.end(), '}', ' ');
			float xVal, yVal;
            sscanf(vec2Str.c_str(), "%f,%f", &xVal, &yVal);
			return ax::Vec2(xVal, yVal);
        }
		return ax::Vec2(0, 0);
	}
	inline int getIntFromLevel(nlohmann::json data, std::string value) {
		if (data.contains(value)) {
            std::string intStr = data[value];
			int theInt;
            sscanf(intStr.c_str(), "%d", &theInt);
			return theInt;
        }
		return 0;
	}
}