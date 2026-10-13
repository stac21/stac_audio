#pragma once

#include <type_traits>

namespace dsp {
template<typename _sample_t>
struct Frame {
	using sample_type = _sample_t;

	sample_type left_sample = {};
	sample_type right_sample = {};

	template<typename _Number> requires std::is_arithmetic_v<_Number>
	Frame& operator*=(const _Number f) {
		this->left_sample *= f;
		this->right_sample *= f;

		return *this;
	}
	template<typename _Number> requires std::is_arithmetic_v<_Number>
	friend Frame operator*(const _Number lhs, const Frame& rhs) {
		return rhs * lhs;
	}
	template<typename _Number> requires std::is_arithmetic_v<_Number>
	friend Frame operator*(Frame lhs, const _Number rhs) {
		lhs *= rhs;

		return lhs;
	}

	template<typename _Number> requires std::is_arithmetic_v<_Number>
	Frame& operator/=(const _Number f) {
		this->left_sample /= f;
		this->right_sample /= f;

		return *this;
	}
	template<typename _Number> requires std::is_arithmetic_v<_Number>
	friend Frame operator/(const _Number lhs, const Frame& rhs) {
		return rhs * lhs;
	}
	template<typename _Number> requires std::is_arithmetic_v<_Number>
	friend Frame operator/(Frame lhs, const _Number rhs) {
		lhs *= rhs;

		return lhs;
	}

	template<typename _Number> requires std::is_arithmetic_v<_Number>
	Frame& operator-=(const Frame& other) {
		this->left_sample -= other.left_sample;
		this->right_sample -= other.right_sample;

		return *this;
	}
	template<typename _Number> requires std::is_arithmetic_v<_Number>
	friend Frame operator-(const _Number lhs, const Frame& rhs) {
		return rhs * lhs;
	}
	template<typename _Number> requires std::is_arithmetic_v<_Number>
	friend Frame operator-(Frame lhs, const _Number rhs) {
		lhs *= rhs;

		return lhs;
	}


	template<typename _Number> requires std::is_arithmetic_v<_Number>
	Frame& operator+=(const Frame& other) {
		this->left_sample += other.left_sample;
		this->right_sample += other.right_sample;

		return *this;
	}
	template<typename _Number> requires std::is_arithmetic_v<_Number>
	friend Frame operator+(const _Number lhs, const Frame& rhs) {
		return rhs * lhs;
	}
	template<typename _Number> requires std::is_arithmetic_v<_Number>
	friend Frame operator+(Frame lhs, const _Number rhs) {
		lhs *= rhs;

		return lhs;
	}
};
}
