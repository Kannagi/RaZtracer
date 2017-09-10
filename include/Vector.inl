#include <cmath>
#include <cassert>

template <typename T, unsigned int Size>
Vector<T, Size>::Vector(const std::initializer_list<T>& list) {
  assert(("Error: Vector must not be created with less/more values than specified.", Size == list.size()));

  auto element = list.begin();

  for (std::size_t i = 0; i < list.size(); ++i, ++element)
    data[i] = *element;
}

template <typename T, unsigned int Size>
T Vector<T, Size>::dot(const Vector<T, Size>& vec) const {
  float res = 0.f;
  for (std::size_t i = 0; i < data.size(); ++i)
    res += data[i] * vec.getData()[i];
  return res;
}

template <typename T, unsigned int Size>
Vector<T, Size> Vector<T, Size>::cross(const Vector<T, Size>& vec) const {
  static_assert(("Error: Both vectors must be 3 dimensional.", Size == 3));

  Vector<T, Size> res;

  res[0] = data[1] * vec.getData()[2] - data[2] * vec.getData()[1];
  res[1] = -(data[0] * vec.getData()[2] - data[2] * vec.getData()[0]);
  res[2] = data[0] * vec.getData()[1] - data[1] * vec.getData()[0];

  return res;
}

template <typename T, unsigned int Size>
Vector<T, Size> Vector<T, Size>::normalize() const {
  Vector<T, Size> res = *this;

  res /= std::sqrt(dot(*this));

  return res;
}

template <typename T, unsigned int Size>
Vector<T, Size> Vector<T, Size>::operator+(Vector<T, Size> vec) {
  vec += *this;
  return vec;
}

template <typename T, unsigned int Size>
Vector<T, Size> Vector<T, Size>::operator+(float val) {
  Vector<T, Size> res = *this;
  res += val;
  return res;
}

template <typename T, unsigned int Size>
Vector<T, Size> Vector<T, Size>::operator-(Vector<T, Size> vec) {
  vec -= *this;
  return vec;
}

template <typename T, unsigned int Size>
Vector<T, Size> Vector<T, Size>::operator-(float val) {
  Vector<T, Size> res = *this;
  res -= val;
  return res;
}

template <typename T, unsigned int Size>
Vector<T, Size> Vector<T, Size>::operator*(Vector<T, Size> vec) {
  vec *= *this;
  return vec;
}

template <typename T, unsigned int Size>
Vector<T, Size> Vector<T, Size>::operator*(float val) {
  Vector<T, Size> res = *this;
  res *= val;
  return res;
}

template <typename T, unsigned int Size>
Vector<T, Size> Vector<T, Size>::operator/(Vector<T, Size> vec) {
  vec /= *this;
  return vec;
}

template <typename T, unsigned int Size>
Vector<T, Size> Vector<T, Size>::operator/(float val) {
  Vector<T, Size> res = *this;
  res /= val;
  return res;
}

template <typename T, unsigned int Size>
Vector<T, Size>& Vector<T, Size>::operator+=(const Vector<T, Size>& vec) {
  for (std::size_t i = 0; i < data.size(); ++i)
    data[i] += vec.getData()[i];
  return *this;
}

template <typename T, unsigned int Size>
Vector<T, Size>& Vector<T, Size>::operator+=(float val) {
  for (T& it : data)
    it += val;
  return *this;
}

template <typename T, unsigned int Size>
Vector<T, Size>& Vector<T, Size>::operator-=(const Vector<T, Size>& vec) {
  for (std::size_t i = 0; i < data.size(); ++i)
    data[i] -= vec.getData()[i];
  return *this;
}

template <typename T, unsigned int Size>
Vector<T, Size>& Vector<T, Size>::operator-=(float val) {
  for (T& it : data)
    it -= val;
  return *this;
}

template <typename T, unsigned int Size>
Vector<T, Size>& Vector<T, Size>::operator*=(const Vector<T, Size>& vec) {
  for (std::size_t i = 0; i < data.size(); ++i)
    data[i] *= vec.getData()[i];
  return *this;
}

template <typename T, unsigned int Size>
Vector<T, Size>& Vector<T, Size>::operator*=(float val) {
  for (T& it : data)
    it *= val;
  return *this;
}

template <typename T, unsigned int Size>
Vector<T, Size>& Vector<T, Size>::operator/=(const Vector<T, Size>& vec) {
  for (std::size_t i = 0; i < data.size(); ++i)
    data[i] /= vec.getData()[i];
  return *this;
}

template <typename T, unsigned int Size>
Vector<T, Size>& Vector<T, Size>::operator/=(float val) {
  for (T& it : data)
    it /= val;
  return *this;
}
