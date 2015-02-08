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
	assert(min[0] < max[0]);
	assert(min[1] < max[1]);
	assert(min[2] < max[2]);
}

AABB::AABB(const vec3f& centerPos, float xExtent, float yExtent, float zExtent) noexcept
{
	assert(xExtent > 0);
	assert(yExtent > 0);
	assert(zExtent > 0);

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