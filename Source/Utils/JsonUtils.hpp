#include <json.hpp>
#include <optional>
#include <axmol.h>
#include <charconv>

namespace JsonUtils
{
	template<typename T>
	std::optional<T> valueFromObject(const json::Object& obj, std::string_view key) {
		auto it = obj.find(key);
		if (it == obj.end()) return {}; //key does not exist

		try {
			return (*it).second.as<T>();
		}
		catch (...) {
			return {};
		}
	}

	//accepted format = {%f, %f}
	std::optional<ax::Vec2> Vec2FromArrayString(std::string_view data) {
		if (data.empty()) return {};

		constexpr auto npos = std::string_view::npos;

		auto brace = data.find("{");
		if (brace == npos) return {};

		auto numberStart = brace + 1;
		if (numberStart == npos) return {};

		auto comma = data.find(',', numberStart);
		if (comma == npos) return {};

		float x = 0;
		std::string_view first = data.substr(numberStart, comma - numberStart);

		if(std::from_chars(first.data(), first.data() + first.size(), x).ec != std::errc{})
			return {};
		
		comma += 2;
		auto lastBrace = data.find('}', comma);
		if (lastBrace == npos) return {};

		std::string_view second = data.substr(comma, lastBrace - comma);

		float y = 0;
		if(std::from_chars(second.data(), second.data() + second.size(), y).ec != std::errc{})
			return {};

		return ax::Vec2(x, y);
	}

	std::optional<ax::Vec2> Vec2FromJsonObject(const json::Object& obj, std::string_view key) {
		if (auto strRes = valueFromObject<std::string>(obj, key); strRes.has_value()) {
			return Vec2FromArrayString(strRes.value());
		}
		return {};

	}
}