#pragma once

template<typename T> class Vector2D
{
public:
	T X;
	T Y;

	Vector2D();
	Vector2D(const T& v);
	template<typename O> Vector2D(const Vector2D<O>& other);
	Vector2D(const T& x, const T& y);

	template<typename O> Vector2D& operator=(const Vector2D<O>& other);



	const Vector2D& operator+() const;
	Vector2D operator-() const;

	template<typename O> Vector2D operator+(const Vector2D<O>& other) const;
	template<typename O> Vector2D operator-(const Vector2D<O>& other) const;
	template<typename O> T operator*(const Vector2D<O>& other) const;

	Vector2D operator*(T s) const;
	Vector2D operator/(T s) const;

	template<typename O> Vector2D& operator+=(const Vector2D<O>& other);
	template<typename O> Vector2D& operator-=(const Vector2D<O>& other);
	template<typename O> T operator*=(const Vector2D<O>& other);

	Vector2D& operator*=(T s);
	Vector2D& operator/=(T s);

	double Length() const;
	Vector2D Normalize() const;
};

template<typename T> Vector2D<T> operator*(T o, const Vector2D<T>& t) { return t*o; }

template<typename T> bool operator==(const Vector2D<T>& f, const Vector2D<T>& t) { return f.X==t.X && f.Y==t.Y; }

template<typename T> double abs(const Vector2D<T>& t) { return t.Length(); }

#ifndef _VECTOR_2D_IMPL
#include "Vector2D.cpp"
#endif