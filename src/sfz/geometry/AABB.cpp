#include "sfz/geometry/AABB.hpp"

#include "sfz/MSVC12HackON.hpp"

namespace sfz {

// Constructors & destructors
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

AABB::AABB(const vec3f& min, const vec3f& max) noexcept
:
	mMin{min},
	mMax{max}
{
	sfz_assert_debug(min[0] < max[0]);
	sfz_assert_debug(min[1] < max[1]);
	sfz_assert_debug(min[2] < max[2]);
}

AABB::AABB(const vec3f& centerPos, float xExtent, float yExtent, float zExtent) noexcept
{
	sfz_assert_debug(xExtent > 0);
	sfz_assert_debug(yExtent > 0);
	sfz_assert_debug(zExtent > 0);

	vec3f temp = centerPos;
	temp[0] -= xExtent/2.0f;
	temp[1] -= yExtent/2.0f;
	temp[2] -= zExtent/2.0f;
	mMin = temp;

	temp[0] += xExtent;
	temp[1] += yExtent;
	temp[2] += zExtent;
	mMax = temp;
}

// Public member functions
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

std::array<vec3f,8> AABB::corners() const noexcept
{
	std::array<vec3f,8> result;
	this->corners(&result[0]);
	return result;
}

void AABB::corners(vec3f* arrayOut) const noexcept
{
	const vec3f xExtent = vec3f{mMax[0] - mMin[0], 0.0f, 0.0f};
	const vec3f yExtent = vec3f{0.0f, mMax[1] - mMin[1], 0.0f};
	const vec3f zExtent = vec3f{0.0f, 0.0f, mMax[2] - mMin[2]};

	arrayOut[0] = mMin; // Back-bottom-left
	arrayOut[1] = mMin + zExtent; // Front-bottom-left
	arrayOut[2] = mMin + yExtent; // Back-top-left
	arrayOut[3] = mMin + zExtent + yExtent; // Front-top-left
	arrayOut[4] = mMin + xExtent; // Back-bottom-right
	arrayOut[5] = mMin + zExtent + xExtent; // Front-bottom-right
	arrayOut[6] = mMin + yExtent + xExtent; // Back-top-right
	arrayOut[7] = mMax; // Front-top-right
}

size_t AABB::hash() const noexcept
{
	std::hash<vec3f> hasher;
	size_t hash = 0;
	// hash_combine algorithm from boost
	hash ^= hasher(mMin) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
	hash ^= hasher(mMax) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
	return hash;
}

std::string AABB::to_string() const noexcept
{
	std::string str{"Min: "};
	str += mMin.to_string();
	str += "\nMax: ";
	str += mMax.to_string();
	return std::move(str);
}

// Public getters/setters
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

vec3f AABB::position() const noexcept
{
	return mMin + (extents()/2.0f);
}

vec3f AABB::extents() const noexcept
{
	return vec3f{xExtent(), yExtent(), zExtent()};
}

float AABB::xExtent() const noexcept
{
	return mMax[0] - mMin[0];
}

float AABB::yExtent() const noexcept
{
	return mMax[1] - mMin[1];
}

float AABB::zExtent() const noexcept
{
	return mMax[2] - mMin[2];
}

void AABB::min(const vec3f& newMin) noexcept
{
	mMin = newMin;
	sfz_assert_debug(mMin[0] < mMax[0]);
	sfz_assert_debug(mMin[1] < mMax[1]);
	sfz_assert_debug(mMin[2] < mMax[2]);
}

void AABB::max(const vec3f& newMax) noexcept
{
	mMax = newMax;
	sfz_assert_debug(mMin[0] < mMax[0]);
	sfz_assert_debug(mMin[1] < mMax[1]);
	sfz_assert_debug(mMin[2] < mMax[2]);
}

void AABB::position(const vec3f& newCenterPos) noexcept
{
	const vec3f halfExtents{xExtent()/2.0f, yExtent()/2.0f, zExtent()/2.0f};
	mMin = newCenterPos - halfExtents;
	mMax = newCenterPos + halfExtents;
}

void AABB::extents(const vec3f& newExtents) noexcept
{
	sfz_assert_debug(newExtents[0] > 0);
	sfz_assert_debug(newExtents[1] > 0);
	sfz_assert_debug(newExtents[2] > 0);
	const vec3f pos = position();
	const vec3f halfExtents = newExtents/2.0f;
	mMin = pos - halfExtents;
	mMin = pos + halfExtents;
}

void AABB::xExtent(float newXExtent) noexcept
{
	extents(vec3f{newXExtent, yExtent(), zExtent()});
}

void AABB::yExtent(float newYExtent) noexcept
{
	extents(vec3f{xExtent(), newYExtent, zExtent()});
}

void AABB::zExtent(float newZExtent) noexcept
{
	extents(vec3f{xExtent(), yExtent(), newZExtent});
}

// Non-member operators
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

std::ostream& operator<< (std::ostream& ostream, const AABB& aabb) noexcept
{
	return ostream << aabb.to_string();
}

} // namespace sfz

// Specializations of standard library for sfz::AABB
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

namespace std {

size_t hash<sfz::AABB>::operator() (const sfz::AABB& aabb) const noexcept
{
	return aabb.hash();
}

} // namespace std

#include "sfz/MSVC12HackOFF.hpp"