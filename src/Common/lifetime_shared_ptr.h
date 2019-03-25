#pragma once

#include <memory>
#include <type_traits>

template <class T>
class lifetime_shared_ptr {
public:
	lifetime_shared_ptr(T& t) : this_ptr(t) {}
	operator std::shared_ptr<T>() {
		return std::shared_ptr(this_ptr);
	}
	operator std::weak_ptr<T>() {
		return std::weak_ptr(this_ptr);
	}

	const std::shared_ptr<T> this_ptr;
};