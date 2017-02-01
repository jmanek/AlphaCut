#pragma once

#include "Vector2.h"
#include <string>       // std::string
#include "assert.h"
	class Barycentric
	{
		public:
			Barycentric() : u{-1.0}, v{-1.0}, w{-1.0} {};
			Barycentric(double u, double v, double w);
			double u;
			double v; 
			double w;
			std::string ToString();
			// Returns the barycentric coordinate of the point
			// Assumes the point is inside the triangle
			// If it's not the coordinates will not be correct
			template<typename T>
			static Barycentric Get(const T &point, const T &vA, const T &vB, const T &vC)
			{
				Barycentric bc = Barycentric();
				T v0 = vB - vA;
				T v1 = vC - vA;
				T v2 = point - vA;

				float d00 = T::Dot(v0, v0);
				// d00 = abs(d00) if abs(d00) < 0.000001 else d00
				float d01 = T::Dot(v0, v1);
				// d01 = abs(d01) if abs(d01) < 0.000001 else d01
				float d11 = T::Dot(v1, v1);
				// d11 = abs(d11) if abs(d11) < 0.000001 else d11
				float d20 = T::Dot(v2, v0);
				// d20 = abs(d20) if abs(d20) < 0.000001 else d20
				float d21 = T::Dot(v2, v1);
				// d21 = abs(d21) if abs(d21) < 0.000001 else d21
				float denom = d00 * d11 - d01 * d01;

				assert(denom != 0.0);

				bc.v = (d11 * d20 - d01 * d21) / denom;
				bc.w = (d00 * d21 - d01 * d20) / denom;
				bc.u = 1.0 - bc.v - bc.w;
				// v = abs(v) if abs(v) < 0.000001 else v
				// w = abs(w) if abs(w) < 0.000001 else w
				// u = abs(u) if abs(u) < 0.000001 else u
				return bc;
			}
			// Same as above, but convert points of different type to T
			template<typename A, typename T>
			static Barycentric Get(const A &point, const T &vA, const T &vB, const T &vC)
			{
				T p = T(point);
				return Get(p, vA, vB, vC);
			}
			// Gets the point inside triangle of the barycentric coord
			template<typename T>
			T Cartesian(T vA, T vB, T vC)
			{
				// assert(u != -1.0 && v != 1.0 && w != -1.0);
				return (vA * u) + (vB * v) + (vC * w);
			}

			// Returns true if the barycentric coordinates are valid
			// non-negative and almost equal to 1
			bool IsValid();
	};
