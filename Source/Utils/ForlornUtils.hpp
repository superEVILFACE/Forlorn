
#include <axmol.h>

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

	inline ax::FontFNT* getFont(std::string_view fntfile)
	{
		ax::FontFNT* (*fn1)(std::string_view);
		fn1 = ax::FontFNT::create;
		return fn1(fntfile);
	}
}