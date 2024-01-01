char take_one(std::string_view& string) {
	if (string.empty()) throw std::runtime_error("eof");
	auto c = string.front();
	string = string.substr(1);
	return c;
}

char peek(std::string_view& string) {
	if (string.empty()) throw std::runtime_error("eof");
	return string.front();
}

bool is_ws(char c) {
	return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

void skip_ws(std::string_view& string) {
	while (!string.empty() && is_ws(peek(string))) {
		take_one(string);
	}
}

std::string_view take_n(std::string_view& string, size_t n) {
	if (string.size() < n) throw std::runtime_error("eof");
	const auto sub = string.substr(0, n);
	string = string.substr(n);
	return sub;
}

using ValuePtr = std::unique_ptr<ValueImpl>;


ValuePtr parse_constant(std::string_view& source) {
	switch (peek(source)) {
		case 't':
			if (take_n(source, 4) != "true"sv) break;
			return std::make_unique<ValueImpl>(Type::Bool, true);
		case 'f':
			if (take_n(source, 5) != "false"sv) break;
			return std::make_unique<ValueImpl>(Type::Bool, false);
		case 'n':
			if (take_n(source, 4) != "null"sv) break;
			return std::make_unique<ValueImpl>(Type::Null, std::monostate{});
		default: break;
	}
	throw std::runtime_error("unknown value");
}

void encode_utf8(std::string& str, int32_t code_point) {
	if (code_point < 0x80) {
		str.push_back(static_cast<char>(code_point));
	} else if (code_point < 0x800) {
		str.push_back(static_cast<char>(0b11000000 | ((code_point & 0b11111'000000) >> 6)));
		str.push_back(static_cast<char>(0b10000000 |  (code_point & 0b00000'111111)));
	} else if (code_point < 0x10000) {
		str.push_back(static_cast<char>(0b11100000 | ((code_point & 0b1111'000000'000000) >> 12)));
		str.push_back(static_cast<char>(0b10000000 | ((code_point & 0b0000'111111'000000) >> 6)));
		str.push_back(static_cast<char>(0b10000000 |  (code_point & 0b0000'000000'111111)));
	} else {
		str.push_back(static_cast<char>(0b11110000 | ((code_point & 0b111'000000'000000'000000) >> 18)));
		str.push_back(static_cast<char>(0b10000000 | ((code_point & 0b000'111111'000000'000000) >> 12)));
		str.push_back(static_cast<char>(0b10000000 | ((code_point & 0b000'000000'111111'000000) >> 6)));
		str.push_back(static_cast<char>(0b10000000 |  (code_point & 0b000'000000'000000'111111)));
	}
}

std::string parse_string(std::string_view& source) {
	take_one(source);
	std::string str;
	while (peek(source) != '"') {
		char c = take_one(source);
		// char is signed, so utf8 high bit bytes will be interpreted as negative,
		// could switch to unsigned char however just checking for c < 0 is easier
		if (c >= 0 && c < 0x20) throw std::runtime_error("invalid string");
		// FIXME: standard should also ignore > 0x10FFFF, however that would require decoding utf-8
		if (c == '\\') {
			switch (take_one(source)) {
				case '"': str.push_back('"'); break;
				case '\\': str.push_back('\\'); break;
				case '/': str.push_back('/'); break;
				case 'b': str.push_back('\b'); break;
				case 'f': str.push_back('\f'); break;
				case 'n': str.push_back('\n'); break;
				case 'r': str.push_back('\r'); break;
				case 't': str.push_back('\t'); break;
				case 'u': {
					const auto take_hex = [&] {
						char c = take_one(source);
						if (c >= '0' && c <= '9') return static_cast<uint32_t>(c - '0');
						else if (c >= 'a' && c <= 'f') return static_cast<uint32_t>(c - 'a' + 10);
						else if (c >= 'A' && c <= 'F') return static_cast<uint32_t>(c - 'A' + 10);
						throw std::runtime_error("invalid hex");
					};
					int32_t value = 0;
					value |= take_hex() << 12;
					value |= take_hex() << 8;
					value |= take_hex() << 4;
					value |= take_hex();
					encode_utf8(str, value);
				} break;
				default: throw std::runtime_error("invalid backslash escape");
			}
		} else {
			str.push_back(c);
		}
	}
	take_one(source);
	return str;
}

ValuePtr parse_number(std::string_view& source) {
	size_t size = 0;
	auto start = source;
	if (peek(source) == '-') {
		take_one(source);
		++size;
	}
	const auto take_digits = [&] {
		bool once = false;
		while (true) {
			char c = peek(source);
			if (c >= '0' && c <= '9') {
				once = true;
				take_one(source);
				++size;
			} else {
				break;
			}
		}
		if (!once) throw std::runtime_error("expected digits");
	};
	if (peek(source) == '0') {
		take_one(source);
		++size;
	} else {
		take_digits();
	}
	if (peek(source) == '.') {
		take_one(source);
		++size;
		take_digits();
	}
	if (peek(source) == 'e' || peek(source) == 'E') {
		take_one(source);
		++size;
		if (peek(source) == '-' || peek(source) == '+') {
			take_one(source);
			++size;
		}
		take_digits();
	}
	#ifndef __cpp_lib_to_chars
		const std::string str(start.substr(0, size));
		// FIXME: std::stod is locale specific, might break on some machines
		return std::make_unique<ValueImpl>(Type::Number, std::stod(str));
	#else
		double value;
		if (auto result = std::from_chars(start.data(), start.data() + size, value); result.ec != std::errc()) {
			throw std::runtime_error("failed to parse number");
		}
		return std::make_unique<ValueImpl>(Type::Number, value);
	#endif
}

ValuePtr parse_element(std::string_view& source);

ValuePtr parse_object(std::string_view& source) {
	take_one(source);
	skip_ws(source);
	Object object;
	if (peek(source) != '}') {
		while (true) {
			skip_ws(source);
			auto key = parse_string(source);
			skip_ws(source);
			
			if (take_one(source) != ':') throw std::runtime_error("missing colon");
			
			auto value = parse_element(source);
			object.insert({key, ValueImpl::as_value(std::move(value))});

			char c = peek(source);
			if (c == ',') {
				take_one(source);
			} else if (c == '}') {
				break;
			} else {
				throw std::runtime_error("expected member");
			}
		}
	}
	take_one(source);
	return std::make_unique<ValueImpl>(Type::Object, object);
}

ValuePtr parse_array(std::string_view& source) {
	take_one(source);
	skip_ws(source);
	Array array;
	if (peek(source) != ']') {
		while (true) {
			array.push_back(ValueImpl::as_value(parse_element(source)));

			char c = peek(source);
			if (c == ',') {
				take_one(source);
			} else if (c == ']') {
				break;
			} else {
				throw std::runtime_error("expected value");
			}
		}
	}
	take_one(source);
	return std::make_unique<ValueImpl>(Type::Array, array);
}

ValuePtr parse_value(std::string_view& source) {
	switch (peek(source)) {
		case 't':
		case 'f':
		case 'n': return parse_constant(source);
		case '"': return std::make_unique<ValueImpl>(Type::String, parse_string(source));
		case '{': return parse_object(source);
		case '[': return parse_array(source);
		case '-':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': return parse_number(source);
		default: throw std::runtime_error("invalid value");
	}
}

ValuePtr parse_element(std::string_view& source) {
	skip_ws(source);
	auto value = parse_value(source);
	skip_ws(source);
	return value;
}

ValuePtr parse_json(std::string_view source) {
	auto value = parse_element(source);
	if (!source.empty()) throw std::runtime_error("expected eof");
	return value;
}
