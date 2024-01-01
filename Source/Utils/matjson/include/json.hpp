#pragma once

#include <string_view>
#include <optional>
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>

#ifdef MAT_JSON_DYNAMIC
	#if defined(_WIN32) && !defined(__CYGWIN__)
		#ifdef MAT_JSON_EXPORTING
			#define MAT_JSON_DLL __declspec(dllexport)
		#else
			#define MAT_JSON_DLL __declspec(dllimport)
		#endif
	#else
		#ifdef MAT_JSON_EXPORTING
			#define MAT_JSON_DLL [[gnu::visibility("default")]]
		#else
			#define MAT_JSON_DLL
		#endif
	#endif
#else
	#define MAT_JSON_DLL
#endif

namespace json {
	enum class Type {
		Object,
		Array,
		String,
		Number,
		Bool,
		Null,
	};

	class ValueImpl;

	class Value;

	using Array = std::vector<Value>;

	class Object;

	using JsonException = std::runtime_error;

	// Specialize this class and implement the following methods (not all required)
	// static T from_json(const json::Value&);
	// static json::Value to_json(const T&);
	// static bool is_json(const json::Value&);
	template <class T>
	struct Serialize;

	static constexpr int NO_INDENTATION = 0;
	static constexpr int TAB_INDENTATION = -1;

	class MAT_JSON_DLL Value final {
		std::unique_ptr<ValueImpl> m_impl;
		friend ValueImpl;
		Value(std::unique_ptr<ValueImpl>);
	public:
		Value();
		Value(std::string value);
		Value(const char* value);
		Value(double value);
		Value(bool value);
		Value(Object value);
		Value(Array value);
		Value(std::nullptr_t);
		template <class T>
		requires std::is_integral_v<T>
		Value(T value) : Value(static_cast<double>(value)) {}

		Value(const Value&);
		Value(Value&&);
		~Value();

		Value& operator=(Value);

		template <class T>
		requires requires(T value) { Serialize<std::decay_t<T>>::to_json(value); }
		Value(T&& value) : Value(Serialize<std::decay_t<T>>::to_json(std::forward<T>(value))) {}

		template <class T>
		// Prevents implicit conversion from pointer to bool
		Value(T*) = delete;

		static Value from_str(std::string_view source);

		std::optional<std::reference_wrapper<Value>> try_get(std::string_view key);
		std::optional<std::reference_wrapper<const Value>> try_get(std::string_view key) const;
		std::optional<std::reference_wrapper<Value>> try_get(size_t index);
		std::optional<std::reference_wrapper<const Value>> try_get(size_t index) const;

		Value& operator[](std::string_view key);
		const Value& operator[](std::string_view key) const;
		Value& operator[](size_t index);
		const Value& operator[](size_t index) const;

		void set(std::string_view key, Value value);

		Type type() const;

		bool as_bool() const;
		std::string as_string() const;
		int as_int() const;
		double as_double() const;

		const Object& as_object() const;
		Object& as_object();
		
		const Array& as_array() const;
		Array& as_array();

		bool operator==(const Value&) const;
		bool operator<(const Value&) const;
		bool operator>(const Value&) const;

		bool is_null() const { return type() == Type::Null; }
		bool is_string() const { return type() == Type::String; }
		bool is_number() const { return type() == Type::Number; }
		bool is_bool() const { return type() == Type::Bool; }
		bool is_array() const { return type() == Type::Array; }
		bool is_object() const { return type() == Type::Object; }

		bool contains(std::string_view key) const;
		size_t count(std::string_view key) const;

		// Use json::NO_INDENTATION for a compact json, json::TAB_INDENTATION for tabs,
		// otherwise specifies the amount of spaces
		std::string dump(int indentation_size = 4) const;

		template <class T>
		decltype(auto) as() const {
			if constexpr (std::is_same_v<T, bool>) {
				return as_bool();
			} else if constexpr (std::is_integral_v<T>) {
				return as_int();
			} else if constexpr (std::is_floating_point_v<T>) {
				return as_double();
			} else if constexpr (requires(const Value& json) { Serialize<T>::from_json(json); }) {
				return Serialize<T>::from_json(*this);
			} else if constexpr (std::is_same_v<T, Array>) {
				return as_array();
			} else if constexpr (std::is_same_v<T, Object>) {
				return as_object();
			} else if constexpr (std::is_constructible_v<std::string, T>) {
				return as_string();
			} else if constexpr (std::is_same_v<T, Value>) {
				return *this;
			} else {
				static_assert(!std::is_same_v<T, T>, "no conversion found from json::Value to T");
			}
		}

		template <class T>
		decltype(auto) as() {
			if constexpr (std::is_same_v<T, Array>) {
				return as_array();
			} else if constexpr (std::is_same_v<T, Object>) {
				return as_object();
			} else {
				return static_cast<const Value*>(this)->as<T>();
			}
		}

		template <class T>
		bool is() const {
			if constexpr (requires(const Value& json) { Serialize<T>::is_json(json); }) {
				return Serialize<T>::is_json(*this);
			}
			switch (type()) {
				case Type::Array: return std::is_same_v<T, Array>;
				case Type::Object: return std::is_same_v<T, Object>;
				case Type::String: return std::is_constructible_v<std::string, T>;
				case Type::Number: return std::is_integral_v<T> || std::is_floating_point_v<T>;
				case Type::Bool: return std::is_same_v<T, bool>;
				case Type::Null: return false;
			}
			return false;
		}

		template <class T, class Key>
		decltype(auto) get(Key&& key_or_index) const {
			return this->operator[](std::forward<Key>(key_or_index)).template as<T>();
		}

		template <class T, class Key>
		decltype(auto) get(Key&& key_or_index) {
			return this->operator[](std::forward<Key>(key_or_index)).template as<T>();
		}
	};

	class MAT_JSON_DLL Object final {
		using value_type = std::pair<std::string, Value>;
		using iterator = typename std::vector<value_type>::iterator;
		using const_iterator = typename std::vector<value_type>::const_iterator;
		std::vector<value_type> m_data;
	public:
		Object() = default;
		Object(const Object&);
		Object(Object&&);

		template <class It>
		Object(It first, It last) : m_data(first, last) {}

		Object(std::initializer_list<value_type> init);

		size_t size() const { return m_data.size(); }
		bool empty() const { return m_data.empty(); }

		Value& operator[](std::string_view key);

		iterator begin();
		iterator end();

		const_iterator begin() const;
		const_iterator end() const;

		const_iterator cbegin() const;
		const_iterator cend() const;

		iterator find(std::string_view key);
		const_iterator find(std::string_view key) const;

		std::pair<iterator, bool> insert(const value_type& value);
		size_t count(std::string_view key) const;
		bool contains(std::string_view key) const;

		bool operator==(const Object& other) const;
		bool operator<(const Object&) const;
		bool operator>(const Object&) const;
	};

	inline Value parse(std::string_view source) {
		return Value::from_str(source);
	}
}

template <>
struct std::hash<json::Value> {
	MAT_JSON_DLL std::size_t operator()(json::Value const& value) const;
};
