#pragma once
#ifndef SFZ_MATH_MATRIX_HPP
#define SFZ_MATH_MATRIX_HPP

#include <initializer_list>
#include <string>
#include <iostream> // std::ostream

#include "sfz/Assert.hpp"
#include "sfz/math/Vector.hpp"

namespace sfz {

using std::size_t;

/**
 * @brief A mathematical Matrix POD class that imitates a built-in primitive.
 * 
 * It's important to note that the implementation of this Matrix is column-major order (and
 * publically accessable). The reason for this is that this seems to be the most natural way to
 * interact with OpenGL. What might be confusing is that some parts of the interface (most notably
 * the initalizer_list constructor) uses row-major order as it's more natural to write matrices
 * in code that way. Be extra careful when accesing or setting specific values in a matrix.
 *
 * The template is designed to be used with floating point types in first hand. It's possible that
 * using integer types might lead to truncation in some instances.
 *
 * Satisfies the conditions of std::is_pod, std::is_trivial and std::is_standard_layout if used
 * with standard primitives.
 *
 * @param T the element type
 * @param M the amount of rows in the Matrix
 * @param N the amount of columns in the Matrix
 *
 * @author Peter Hillerström
 */
template<typename T, size_t M, size_t N>
struct Matrix final {

	// Public constants
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

	static const Matrix<T,M,N>& ZERO() noexcept;

	// Public members
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

	/**
	 * @brief The internal COLUMN-MAJOR order array of this Matrix.
	 * This is column-major order (i.e. N columns of size M), so the pointer can be supplied to
	 * OpenGL directly with the transpose flag set to GL_FALSE.
	 */
	T mElements[N][M];

	T* glPtr() noexcept { return reinterpret_cast<T*>(mElements); }
	const T* glPtr() const noexcept { return reinterpret_cast<const T*>(mElements); }

	// Constructors & destructors
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

	constexpr Matrix() noexcept = default;
	constexpr Matrix(const Matrix<T,M,N>&) noexcept = default;
	Matrix<T,M,N>& operator= (const Matrix<T,M,N>&) noexcept = default;

	/**
	 * @brief Constructs a matrix with the given elements given in ROW-MAJOR order.
	 * The elements are given in row-major order because it's more natural to write and read a
	 * matrix that way in source. This is however not how the elements will be saved as the
	 * internal representation uses column-major order. Any unspecified elements will be set to 0.
	 * @sfz_assert_debug if any of the lists are larger than the row or column it's trying to fill
	 * @param list the (column) initializer list of (row) initializer lists
	 */
	Matrix(std::initializer_list<std::initializer_list<T>> list) noexcept;

	~Matrix() noexcept = default;

	// Member functions
	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

	/**
	 * @brief General accessor returning the reference to element at the specified location.
	 * @sfz_assert_debug location must be in range
	 * @param i the i:th row
	 * @param j the j:th column
	 * @return reference to element at the specified location
	 */
	T& at(size_t i, size_t j) noexcept;

	/**
	 * @brief Returns the element at the specified location.
	 * @sfz_assert_debug location must be in range
	 * @param i the i:th row
	 * @param j the j:th column
	 * @return the element at the specified location
	 */
	T at(size_t i, size_t j) const noexcept;

	/**
	 * @brief Returns row at the specified location.
	 * @sfz_assert_debug location must be in range
	 * @param i the i:th row
	 * @return the row at the specified location
	 */
	Vector<T,N> rowAt(size_t i) const noexcept;

	/**
	 * @brief Returns column at the specified location.
	 * @sfz_assert_debug location must be in range
	 * @param j the j:th row
	 * @return the column at the specified location
	 */
	Vector<T,M> columnAt(size_t j) const noexcept;

	/**
	 * @brief Assigns value to element at the specified location.
	 * @sfz_assert_debug location must be in range
	 * @param i the i:th row
	 * @param j the j:th column
	 * @param value the value to assign
	 */
	void set(size_t i, size_t j, T value) noexcept;

	/**
	 * @brief Assigns vector the specified row.
	 * @sfz_assert_debug location must be in range
	 * @param i the i:th row
	 * @param row the row vector to set
	 */
	void setRow(size_t i, const Vector<T,N>& row) noexcept;

	/**
	 * @brief Assigns vector the specified column.
	 * @sfz_assert_debug location must be in range
	 * @param j the j:th column
	 * @param column the column vector to set
	 */
	void setColumn(size_t j, const Vector<T,M>& column) noexcept;

	/**
	 * @brief Assigns each element in the matrix with the specified value.
	 */
	void fill(const T value) noexcept;

	/**
	 * @brief Element-wise multiplication
	 * @return the resulting matrix
	 */
	Matrix<T,M,N> elemMult(const Matrix<T,M,N>& other) const noexcept;

	Matrix<T,N,M> transpose() const noexcept;

	size_t hash() const noexcept;

	std::string to_string() const noexcept;
};

// Operators (arithmetic & assignment)
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

template<typename T, size_t M, size_t N>
Matrix<T,M,N>& operator+= (Matrix<T,M,N>& lhs, const Matrix<T,M,N>& rhs) noexcept;

template<typename T, size_t M, size_t N>
Matrix<T,M,N>& operator-= (Matrix<T,M,N>& lhs, const Matrix<T,M,N>& rhs) noexcept;

template<typename T, size_t M, size_t N>
Matrix<T,M,N>& operator*= (Matrix<T,M,N>& lhs, T rhs) noexcept;

template<typename T, size_t N>
Matrix<T,N,N>& operator*= (Matrix<T,N,N>& lhs, const Matrix<T,N,N>& rhs) noexcept;

// Operators (arithmetic)
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

template<typename T, size_t M, size_t N>
Matrix<T,M,N> operator+ (const Matrix<T,M,N>& lhs, const Matrix<T,M,N>& rhs) noexcept;

template<typename T, size_t M, size_t N>
Matrix<T,M,N> operator- (const Matrix<T,M,N>& lhs, const Matrix<T,M,N>& rhs) noexcept;

template<typename T, size_t M, size_t N>
Matrix<T,M,N> operator- (const Matrix<T,M,N>& matrix) noexcept;

template<typename T, size_t M, size_t N, size_t P>
Matrix<T,M,P> operator* (const Matrix<T,M,N>& lhs, const Matrix<T,N,P>& rhs) noexcept;

template<typename T, size_t M, size_t N>
Vector<T,M> operator* (const Matrix<T,M,N>& lhs, const Vector<T,N>& rhs) noexcept;

template<typename T, size_t M, size_t N>
Matrix<T,M,N> operator* (const Matrix<T,M,N>& lhs, T rhs) noexcept;

template<typename T, size_t M, size_t N>
Matrix<T,M,N> operator* (T lhs, const Matrix<T,M,N>& rhs) noexcept;

// Operators (comparison)
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

template<typename T, size_t M, size_t N>
bool operator== (const Matrix<T,M,N>& lhs, const Matrix<T,M,N>& rhs) noexcept;

template<typename T, size_t M, size_t N>
bool operator!= (const Matrix<T,M,N>& lhs, const Matrix<T,M,N>& rhs) noexcept;

// Operators (other)
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

template<typename T, size_t M, size_t N>
std::ostream& operator<< (std::ostream& ostream, const Matrix<T,M,N>& matrix) noexcept;

// Standard typedefs
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

using mat2f = Matrix<float,2,2>;
using mat2i = Matrix<int,2,2>;

using mat3f = Matrix<float,3,3>;
using mat3i = Matrix<int,3,3>;

using mat4f = Matrix<float,4,4>;
using mat4i = Matrix<int,4,4>;

} // namespace sfz

// Specializations of standard library for sfz::Matrix
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

namespace std {

template<typename T, size_t M, size_t N>
struct hash<sfz::Matrix<T,M,N>> {
	size_t operator() (const sfz::Matrix<T,M,N>& matrix) const noexcept;
};

} // namespace std
#include "sfz/math/Matrix.inl"
#include "sfz/math/MatrixSupport.hpp"
#endif
