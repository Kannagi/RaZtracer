#pragma once

#ifndef RAZTRACER_QUATERNION_HPP
#define RAZTRACER_QUATERNION_HPP

#include "Matrix.hpp"
#include "Vector.hpp"

template <typename T = float>
class Quaternion {
  static_assert(std::is_floating_point<T>::value, "Error: Quaternion's type must be floating point.");

public:
  Quaternion(T angle, Vec3f axes);
  Quaternion(T angle, float axisX, float axisY, float axisZ) : Quaternion(angle, Vec3f({ axisX, axisY, axisZ })) {}

  Mat4f computeMatrix() const;

private:
  T m_real {};
  Vector<T, 3> m_complexes {};
};

#include "Quaternion.inl"

#endif // RAZTRACER_QUATERNION_HPP
