#pragma once

#include <cassert>
#include <ostream>

namespace space
{
template <unsigned int size, typename T = float>
class Vector
{
  protected:
    T vect_[size];

  public:
    template <typename... Ts>
    Vector(Ts... args)
        : vect_{args...}
    {
        static_assert(sizeof...(Ts) == size,
                      "Vector constructor: wrong number of arguments");
    }

    Vector() = default;

    Vector& operator=(const Vector& vector);
    Vector(const Vector& vector);

    inline Vector& operator*=(T rhs);
    inline Vector operator*(T rhs) const;

    // Perform bitwise multiplication
    inline Vector& operator*=(const Vector& rhs);
    // Perform bitwise multiplication
    inline Vector operator*(const Vector& rhs) const;

    inline Vector& operator/=(T rhs);
    inline Vector operator/(T rhs) const;

    // Perform bitwise division
    inline Vector& operator/=(const Vector& rhs);
    // Perform bitwise division
    inline Vector operator/(const Vector& rhs) const;

    inline Vector& operator+=(const Vector& rhs);
    inline Vector operator+(const Vector& rhs) const;

    inline Vector& operator+=(const T val);
    inline Vector operator+(const T val) const;

    inline Vector& operator-=(const Vector& rhs);
    inline Vector operator-(const Vector& rhs) const;

    inline Vector operator-() const;

    inline T dot(const Vector& rhs) const;

    inline T length() const;

    inline void normalize();
    inline Vector normalized() const;

    inline constexpr const T& operator[](const unsigned int pos) const;

    inline constexpr T& operator[](const unsigned int pos);

    virtual ~Vector() = default;

    friend Vector<3, float> cross_product(const Vector<3, float>& lhs,
                                          const Vector<3, float>& rhs);

    template <unsigned int _size, typename _T>
    friend std::ostream& operator<<(std::ostream& os,
                                    const Vector<_size, _T>& vect);
};

template <unsigned int size, typename T = float>
inline std::ostream& operator<<(std::ostream& os, const Vector<size, T>& vect)
{
    os << "(";
    unsigned int i = 0;
    if (size != 0)
        os << vect.vect_[i];
    i++;
    for (; i < size; i++)
        os << "," << vect.vect_[i];

    os << ")";
    return os;
}

template <unsigned int size, typename T = float>
inline Vector<size, T> operator*(const float scalar,
                                 const Vector<size, T>& vect)
{
    return vect * scalar;
}

using Vector3 = Vector<3, float>;
using Point3 = Vector3;

inline Vector3 cross_product(const Vector3& lhs, const Vector3& rhs);

} // namespace space
#include "vector.hxx"