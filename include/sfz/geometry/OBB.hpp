#pragma once
#ifndef SFZ_GEOMETRY_OBB_HPP
#define SFZ_GEOMETRY_OBB_HPP

#include <string>
#include <array>
#include <iostream> // std::ostream
#include <functional> // std::hash

#include "sfz/Assert.hpp"
#include "sfz/math/MathHelpers.hpp"
#include "sfz/math/Vector.hpp"
#include "sfz/math/Matrix.hpp"
#include "sfz/geometry/AABB.hpp"

namespace sfz {

using std::array;

/**
 * @brief Class representing an Oriented Bounding Box.
 * @author Peter Hillerström
 */
class OBB final {
public:
	// Constructors & destructors
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

	inline OBB() noexcept = default;

	inline OBB(const vec3& center, const array<vec3,3>& axes, const vec3& extents) noexcept;

	inline OBB(const vec3& center, const vec3& xAxis, const vec3& yAxis, const vec3& zAxis,
	           const vec3& extents) noexcept;

	inline OBB(const vec3& center, const vec3& xAxis, const vec3& yAxis, const vec3& zAxis,
	           float xExtent, float yExtent, float zExtent) noexcept;

	explicit inline OBB(const AABB& aabb) noexcept;

	// Public member functions
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

	inline std::array<vec3,8> corners() const noexcept;
	inline void corners(vec3* arrayOut) const noexcept;
	inline vec3 closestPoint(const vec3& point) const noexcept;
	inline OBB transformOBB(const mat4& transform) const noexcept;

	inline size_t hash() const noexcept;
	inline std::string to_string() const noexcept;

	// Public getters/setters
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

	inline vec3 position() const noexcept { return mCenter; }
	inline const array<vec3,3>& axes() const noexcept { return mAxes; }
	inline vec3 xAxis() const noexcept { return mAxes[0]; }
	inline vec3 yAxis() const noexcept { return mAxes[1]; }
	inline vec3 zAxis() const noexcept { return mAxes[2]; }
	inline vec3 extents() const noexcept { return mHalfExtents * 2.0f; }
	inline float xExtent() const noexcept { return mHalfExtents[0] * 2.0f; }
	inline float yExtent() const noexcept { return mHalfExtents[1] * 2.0f; }
	inline float zExtent() const noexcept { return mHalfExtents[2] * 2.0f; }
	inline vec3 halfExtents() const noexcept { return mHalfExtents; }
	inline float halfXExtent() const noexcept { return mHalfExtents[0]; }
	inline float halfYExtent() const noexcept { return mHalfExtents[1]; }
	inline float halfZExtent() const noexcept { return mHalfExtents[2]; }

	inline void position(const vec3& newCenterPos) noexcept { mCenter = newCenterPos; }
	inline void axes(const array<vec3,3>& newAxes) noexcept { mAxes = newAxes; }
	inline void xAxis(const vec3& newXAxis) noexcept { mAxes[0] = newXAxis; }
	inline void yAxis(const vec3& newYAxis) noexcept { mAxes[1] = newYAxis; }
	inline void zAxis(const vec3& newZAxis) noexcept { mAxes[2] = newZAxis; }
	inline void extents(const vec3& newExtents) noexcept;
	inline void xExtent(float newXExtent) noexcept;
	inline void yExtent(float newYExtent) noexcept;
	inline void zExtent(float newZExtent) noexcept;
	inline void halfExtents(const vec3& newHalfExtents) noexcept;
	inline void halfXExtent(float newHalfXExtent) noexcept;
	inline void halfYExtent(float newHalfYExtent) noexcept;
	inline void halfZExtent(float newHalfZExtent) noexcept;

private:
	// Private functions
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

	inline void ensureCorrectAxes() const noexcept;
	inline void ensureCorrectExtents() const noexcept;

	// Private members
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

	vec3 mCenter;
	array<vec3,3> mAxes;
	vec3 mHalfExtents;
};

// Non-member operators
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

inline std::ostream& operator<< (std::ostream& ostream, const OBB& obb) noexcept;

} // namespace sfz

// Specializations of standard library for sfz::OBB
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

namespace std {

template<>
struct hash<sfz::OBB> {
	inline size_t operator() (const sfz::OBB& obb) const noexcept;
};

} // namespace std
#include "sfz/geometry/OBB.inl"
#endif