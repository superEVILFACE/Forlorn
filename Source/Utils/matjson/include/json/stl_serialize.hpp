#pragma once

#include "../json.hpp"
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <algorithm>

namespace json {
	// allow converting parsing JSON directly to STL containers for convenience

	template <class T>
	struct Serialize<std::optional<T>> {
		static std::optional<T> from_json(Value const& value)
			requires requires(Value const& value) {
				value.template as<T>();
			}
		{
			if (!value.is_null()) {
				return value.template as<T>();
			}
			return std::nullopt;
		}

		static Value to_json(std::optional<T> const& value)
			requires requires(T const& value) {
				Value(value);
			}
		{
			if (value.has_value()) {
				return Value(value.value());
			}
			return Value(nullptr);
		}
	};

	template <class T>
	struct Serialize<std::vector<T>> {
		static std::vector<T> from_json(Value const& value)
			requires requires(Value const& value) {
				value.template as<T>();
			}
		{
			std::vector<T> res;
            res.reserve(value.as_array().size());
            std::transform(
                value.as_array().begin(), value.as_array().end(),
                std::back_inserter(res),
                [](Value const& value) -> T {
                    return value.template as<T>();
                }
            );
			return res;
		}

		static Value to_json(std::vector<T> const& value)
			requires requires(T const& value) {
				Value(value);
			}
		{
			Array res;
			res.reserve(value.size());
            std::transform(
                value.begin(), value.end(),
                std::back_inserter(res),
                [](T const& value) -> Value {
                    return Value(value);
                }
            );
			return res;
		}
	};

	template <class T>
	struct Serialize<std::unordered_set<T>> {
		static std::unordered_set<T> from_json(Value const& value)
			requires requires(Value const& value) {
				value.template as<std::decay_t<T>>();
			}
		{
			std::unordered_set<T> res;
            res.reserve(value.as_array().size());
            std::transform(
                value.as_array().begin(), value.as_array().end(),
                std::inserter(res, res.begin()),
                [](Value const& value) -> T {
                    return value.template as<T>();
                }
            );
			return res;
		}

		static Value to_json(std::unordered_set<T> const& value)
			requires requires(T const& value) {
				Value(value);
			}
		{
			Array res;
			res.reserve(value.size());
            std::transform(
                value.begin(), value.end(),
                std::back_inserter(res),
                [](T const& value) -> Value {
                    return Value(value);
                }
            );
			return res;
		}
	};

	template <class T>
	struct Serialize<std::set<T>> {
		static std::set<T> from_json(Value const& value)
			requires requires(Value const& value) {
				value.template as<std::decay_t<T>>();
			}
		{
			std::set<T> res;
            std::transform(
                value.as_array().begin(), value.as_array().end(),
                std::inserter(res, res.begin()),
                [](Value const& value) -> T {
                    return value.template as<T>();
                }
            );
			return res;
		}

		static Value to_json(std::set<T> const& value)
			requires requires(T const& value) {
				Value(value);
			}
		{
			Array res;
			res.reserve(value.size());
            std::transform(
                value.begin(), value.end(),
                std::back_inserter(res),
                [](T const& value) -> Value {
                    return Value(value);
                }
            );
			return res;
		}
	};

	template <class T>
	struct Serialize<std::map<std::string, T>> {
		static std::map<std::string, T> from_json(Value const& value)
			requires requires(Value const& value) {
				value.template as<std::decay_t<T>>();
			}
		{
			std::map<std::string, T> res;
			auto obj = value.as_object();
			for (auto& [k, v] : obj) {
				res.insert({ k, v.template as<std::decay_t<T>>() });
			}
			return res;
		}

		static Value to_json(std::map<std::string, T> const& value)
			requires requires(T const& value) {
				Value(value);
			}
		{
			Object res;
			for (auto& [k, v] : value) {
				res[k] = (Value(v));
			}
			return res;
		}
	};

	template <class T>
	struct Serialize<std::unordered_map<std::string, T>> {
		static std::unordered_map<std::string, T> from_json(Value const& value)
			requires requires(Value const& value) {
				value.template as<std::decay_t<T>>();
			}
		{
			std::unordered_map<std::string, T> res;
			auto obj = value.as_object();
			for (auto& [k, v] : obj) {
				res.insert({ k, v.template as<std::decay_t<T>>() });
			}
			return res;
		}

		static Value to_json(std::unordered_map<std::string, T> const& value)
			requires requires(T const& value) {
				Value(value);
			}
		{
			Object res;
			for (auto& [k, v] : value) {
				res[k] = (Value(v));
			}
			return res;
		}
	};
}
