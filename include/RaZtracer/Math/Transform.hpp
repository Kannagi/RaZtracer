#pragma once

#ifndef RAZTRACER_TRANSFORM_HPP
#define RAZTRACER_TRANSFORM_HPP

#include "Matrix.hpp"
#include "Vector.hpp"

class Transform {
public:
  explicit Transform(const Vec3f& position = Vec3f(0.f), const Mat4f& rotation = Mat4f::identity(), const Vec3f& scale = Vec3f(1.f))
    : m_position{ position }, m_rotation{ rotation }, m_scale{ scale } {}

  const Vec3f& getPosition() const { return m_position; }
  Vec3f& getPosition() { return m_position; }
  const Mat4f& getRotation() const { return m_rotation; }
  Mat4f& getRotation() { return m_rotation; }
  const Vec3f& getScale() const { return m_scale; }
  Vec3f& getScale() { return m_scale; }

  void translate(float x, float y, float z);
  template <typename T, std::size_t Size> void translate(const Vector<T, Size>& values) { translate(values[0], values[1], values[2]); }
  void rotate(float angle, float x, float y, float z);
  void scale(float x, float y, float z);
  void scale(float val) { scale(val, val, val); }
  Mat4f computeTransformMatrix() const;

protected:
  Vec3f m_position;
  Mat4f m_rotation;
  Vec3f m_scale;
};

#endif // RAZTRACER_TRANSFORM_HPP
