#pragma once

#include <json.hpp>
#include <optional>
#include <axmol.h>
#include <charconv>
#include <type_traits>
#include "ForlornUtils.hpp"

namespace JsonUtils
{
	//accepted format = {%f, %f}
	inline std::optional<ax::Vec2> Vec2FromArrayString(std::string_view data) {
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

		if (std::from_chars(first.data(), first.data() + first.size(), x).ec != std::errc{})
			return {};

		comma += 2;
		auto lastBrace = data.find('}', comma);
		if (lastBrace == npos) return {};

		std::string_view second = data.substr(comma, lastBrace - comma);

		float y = 0;
		if (std::from_chars(second.data(), second.data() + second.size(), y).ec != std::errc{})
			return {};

		return ax::Vec2(x, y);
	}

	template<typename T>
	std::optional<T> fromObject(const json::Object& obj, std::string_view key)
	{
		auto it = obj.find(key);
		if (it == obj.end()) return {}; //key does not exist

		try
		{
			if constexpr (std::is_integral_v<T> || std::is_floating_point_v<T> || std::is_same_v<T, bool>)
			{
				if (auto& value = (*it).second; value.is_string())
					return ForlornUtils::fromString<T>(value.as_string());
				else
					return {};
			}
			else if constexpr (std::is_same_v<T, ax::Vec2>)
			{
				if (auto& value = (*it).second; value.is_string())
					return JsonUtils::Vec2FromArrayString(value.as_string());
				else
					return {};
			}
			else
			{
				return (*it).second.as<T>();
			}
		}
		catch (...)
		{
			return {};
		}
	}
}