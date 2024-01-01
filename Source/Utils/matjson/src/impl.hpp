#include <variant>

class json::ValueImpl {
	Type m_type;
	std::variant<std::monostate, std::string, double, bool, Array, Object> m_value;
public:
	template <class T>
	ValueImpl(Type type, T value) : m_type(type), m_value(value) {}
	ValueImpl(const ValueImpl&) = default;
	Type type() const { return m_type; }

	static Value as_value(std::unique_ptr<ValueImpl> impl) {
		return Value(std::move(impl));
	}

	bool as_bool() const { return std::get<bool>(m_value); }
	std::string as_string() const { return std::get<std::string>(m_value); }
	double as_double() const { return std::get<double>(m_value); }
	Object& as_object() { return std::get<Object>(m_value); }
	Array& as_array() { return std::get<Array>(m_value); }
};
