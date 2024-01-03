#ifndef _FORLORNUTILS_H_
#define _FORLORNUTILS_H_

#include <axmol.h>
#include <optional>
#include <charconv>

namespace ForlornUtils
{
	inline ax::Vec2 getCenter() {
		return ax::Director::getInstance()->getWinSize() / 2;
	}

	inline void setCenter(ax::Node* node) {
		node->setPosition(ForlornUtils::getCenter());
	}

	inline void replaceString(std::string& str, const std::string& pattern, const std::string& replacement) {
		size_t pos = str.find(pattern);
		if (pos != std::string::npos) {
			str.replace(pos, pattern.length(), replacement);
		}
	}

	template<typename T>
	std::optional<T> fromString(std::string_view str) {
		T value;
		if (std::from_chars(str.data(), str.data() + str.size(), value).ec == std::errc()) {
			return value;
		}
		return {};
	}
}
#endif