Object::Object(const Object& object) : m_data(object.m_data) {}
Object::Object(Object&& object) : m_data(std::move(object.m_data)) {}
Object::Object(std::initializer_list<value_type> init) : m_data(init) {}

Object::iterator Object::begin() { return m_data.begin(); }
Object::iterator Object::end() { return m_data.end(); }
Object::const_iterator Object::begin() const { return m_data.begin(); }
Object::const_iterator Object::end() const { return m_data.end(); }
Object::const_iterator Object::cbegin() const { return m_data.cbegin(); }
Object::const_iterator Object::cend() const { return m_data.cend(); }

bool Object::operator==(const Object& other) const {
	for (const auto& [key, value] : *this) {
		if (auto it = other.find(key); it != other.end()) {
			if (value != it->second) return false;
		} else {
			return false;
		}
	}
	return true;
}

bool Object::operator<(const Object& other) const {
	return m_data < other.m_data;
}

bool Object::operator>(const Object& other) const {
	return m_data > other.m_data;
}

Object::iterator Object::find(std::string_view key) {
	auto end = this->end();
	for (auto it = this->begin(); it != end; ++it) {
		if (it->first == key) return it;
	}
	return end;
}

Object::const_iterator Object::find(std::string_view key) const {
	auto end = this->cend();
	for (auto it = this->cbegin(); it != end; ++it) {
		if (it->first == key) return it;
	}
	return end;
}

std::pair<Object::iterator, bool> Object::insert(const Object::value_type& value) {
	if (auto it = this->find(value.first); it != this->end()) {
		return {it, false};
	} else {
		m_data.push_back(value);
		return {--m_data.end(), true};
	}
}

bool Object::contains(std::string_view key) const {
	return this->count(key);
}

size_t Object::count(std::string_view key) const {
	return this->find(key) == this->end() ? 0 : 1;
}

Value& Object::operator[](std::string_view key) {
	if (auto it = this->find(key); it != this->end()) {
		return it->second;
	} else {
		m_data.push_back({std::string(key), Value{}});
		return m_data.back().second;
	}
}
