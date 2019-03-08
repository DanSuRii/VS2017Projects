#ifndef __TPLVECTORND_H__
#define __TPLVECTORND_H__

#include <vector>
#include <cmath>
#include <numeric>

namespace NS_VECTOR
{
	enum V2D
	{
		_X,
		_Y,
		VECTOR_CNT
	};

#if false
	enum V3D
	{
		EV3_X,
		EV3_Y,
		EV3_Z,
		VECTOR_CNT
	};

#endif // false
}

namespace NS_DP
{
	template< class EType >
	class Vector
	{
	public:
		using T_INTERNAL = double;
		using _MyT = Vector;
		using T_CONT = std::vector<T_INTERNAL>;

		Vector();

		//sets x and y ...N to zero		
		inline void Zero();

		//returns true if both x and y ...N are zero
		inline bool isZero() const;

		// returns the length of the vector
		inline T_INTERNAL Length() const;

		//returns the squared length of the vector ( thereby avoiding the sqrt )
		inline T_INTERNAL LengthSq() const;

		inline void Normalize();

		//returns the dot product of this and v2
		inline T_INTERNAL Dot(const _MyT& v2) const;

		//returns positive if v2 is clockwise of this vector.
		//negative if counterclockwise ( assuming the Y axis is pointing down,
		//X axis to right like a Window app)
		inline int	Sign(const _MyT& v2) const;

		//returns the vector that is perpendicular to this one
		inline _MyT Perp() const;

		//adjusts x and y so that the length of the vector does not exceed max
		inline void Truncate(T_INTERNAL max);

		//returns the distance between this vector and the one passed as a parameter
		inline T_INTERNAL Distance(const _MyT& v2) const;

		//squared version of above
		inline T_INTERNAL DistanceSq(const _MyT &v2) const;

		//returns the vector that is the reverse of this vector
		inline _MyT GetReverse() const;

		//we need some operators
		const _MyT& operator+=(const _MyT& rhs);
		const _MyT& operator-=(const _MyT& rhs);
		const _MyT& operator*=(const _MyT& rhs);
		const _MyT& operator/=(const _MyT& rhs);
		bool operator==(const _MyT& rhs);
		bool operator!=(const _MyT& rhs)const;

	private:
		typename T_CONT _vec = T_CONT(EType::VECTOR_CNT);
	};
	
	
	template<class EType>
	inline Vector<EType>::Vector()
	{
		Zero();
	}

	//sets x and y ...N to zero		
	template<class EType>
	inline void Vector<EType>::Zero() { for (auto& cur : _vec) cur = 0.0; }

	//returns true if both x and y ...N are zero
	template<class EType>
	inline bool Vector<EType>::isZero() const {
		T_INTERNAL accumulator(0.0);
		for (auto cur : _vec) {
			accumulator += cur * cur;
		}
		return accumulator < std::numeric_limits<T_INTERNAL>::min();
	}

	template<class EType>
	inline typename Vector<EType>::T_INTERNAL Vector<EType>::Length() const
	{		
		return std::sqrt(LengthSq());
	}
	
	template<class EType>
	inline typename Vector<EType>::T_INTERNAL Vector<EType>::LengthSq() const
	{
		T_CONT powed;
		std::transform(_vec.begin(), _vec.end(), std::back_inserter(powed)
			, [](T_INTERNAL cur) { return std::pow(cur, 2); });
		return std::accumulate(powed.begin(), powed.end(), -0.0);
	}
	template<class EType>
	inline typename Vector<EType>::T_INTERNAL Vector<EType>::Dot(const _MyT & v2) const
	{
		return std::inner_product(_vec.begin(), _vec.end(), v2.begin(), 0.0);
	}
}

namespace NS_DP
{
	using Vector2D = Vector<NS_VECTOR::V2D>;
	//using Vector3D = Vector<NS_VECTOR::V3D>;
}


#endif // !__TPLVECTORND_H__

