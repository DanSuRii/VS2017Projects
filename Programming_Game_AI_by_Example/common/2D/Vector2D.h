#ifndef __VECTOR_2D_H__
#define __VECTOR_2D_H__

#include <cmath>
#include <iostream>

#include "misc/utils.h"


namespace NS_BUCKLAND
{
	struct Vector2D
	{
		double x;
		double y;

		Vector2D() :x(0.0), y(0.0) {}
		Vector2D(double a, double b) :x(a), y(b) {}

		//sets x and y to zero
		inline void Zero() { x = 0.0; y = 0.0; };

		//returns true if both x and y are zero
		inline bool isZero() const { return (x*x + y * y) < std::numeric_limits<double>::min(); };

		//returns the length of the vector
		inline double Length() const;

		//returns the squared length of the vector ( thereby avoiding the sqrt )
		inline double LengthSq() const;

		inline void Normalize();

		//returns the dot product of this and v2
		inline double Dot(const Vector2D& v2) const;

		//returns positive if v2 is clockwise of this vector,
		//negative if counterclockwise ( assuming the Y axis is pointing down,
		// X axis to right like a window app )
		inline int Sign(const Vector2D& v2) const;

		//returns the vector that is perpendicular to this one
		inline Vector2D Perp() const;

		//adjusts x and y so that the length of the vector does not exceed max
		inline void Truncate(double max);

		//returns the distance between this vector and the one passed as a parameter
		inline double Distance(const Vector2D &v2) const;

		//squared version of above
		inline double DistanceSq(const Vector2D& v2) const;

		//returns the vector that is the reverse of this vector
		inline Vector2D GetReverse() const;

		//we need some operators
		const Vector2D& operator+=(const Vector2D& rhs)
		{
			x += rhs.x;
			y += rhs.y;

			return *this;
		}
		const Vector2D& operator-=(const Vector2D& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;

			return *this;
		}
		const Vector2D& operator*=(const double& rhs)
		{
			x *= rhs;
			y *= rhs;

			return *this;
		}
		const Vector2D& operator/=(const double& rhs)
		{
			x /= rhs;
			y /= rhs;

			return *this;
		}
		bool operator==(const Vector2D& rhs)const
		{
			return (NS_DP::isEqual(x, rhs.x) && NS_DP::isEqual(y, rhs.y));
		}
		bool operator!=(const Vector2D& rhs)const
		{
			return !(*this==rhs);
		}
	};

	/*
	inline Vector2D operator*(const Vector2D &lhs, double rhs);
	inline Vector2D operator*(double lhs, const Vector2D &rhs);
	inline Vector2D operator-(const Vector2D &lhs, const Vector2D& rhs);
	inline Vector2D operator+(const Vector2D &lhs, const Vector2D& rhs);
	inline Vector2D operator/(const Vector2D &lhs, double val);
	std::ostream& operator<<(std::ostream& os, const Vector2D& rhs);
	std::ifstream& operator>>(std::ifstream& is, Vector2D& rhs);
	*/




	double Vector2D::Length() const
	{
		return std::sqrt(LengthSq());
	}
	inline double Vector2D::LengthSq() const
	{
		return (x*x + y * y);
	}
	inline void Vector2D::Normalize()
	{
		double vector_length = Length();

		if (vector_length > std::numeric_limits<double>::epsilon())
		{
			x /= vector_length;
			y /= vector_length;
		}
	}
	inline double Vector2D::Dot(const Vector2D & v2) const
	{
		return x * v2.x + y * v2.y;
	}
	

	//------------------------ Sign ------------------------------------------
	//
	//  returns positive if v2 is clockwise of this vector,
	//  minus if anticlockwise (Y axis pointing down, X axis to right)
	//------------------------------------------------------------------------
	enum { clockwise = 1, anticlockwise = -1 };

	inline int Vector2D::Sign(const Vector2D & v2) const
	{
		if (y*v2.x > x*v2.y) {
			return anticlockwise;
		} else {
			return clockwise;
		}
	}

	inline Vector2D Vector2D::Perp() const
	{
		return Vector2D(-y, x);
	}

	inline void Vector2D::Truncate(double max)
	{
		if (Length() > max)
		{
			Normalize();

			*this *= max;
		}
	}

	inline double Vector2D::Distance(const Vector2D & v2) const
	{
		return sqrt(DistanceSq(v2));
	}

	inline double Vector2D::DistanceSq(const Vector2D & v2) const
	{
		double ySeparation = v2.y - y;
		double xSeparation = v2.x - x;

		return ySeparation*ySeparation + xSeparation * xSeparation;
	}

	inline Vector2D Vector2D::GetReverse() const
	{
		return Vector2D(-x, -y);
	}


}


#endif //__VECTOR_2D_H__
