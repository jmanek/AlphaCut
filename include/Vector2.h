#pragma once

#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream
#include <vector>
#include <limits>
#include <cmath>
	using namespace std;
	class Vector2 
	{
		public:
			Vector2() : x{0}, y{0} {}
			Vector2(double x, double y);

			Vector2(vector<double> v);


			double x;
			double y;
			string ToString() const;

			void Normalize();

			static double Magnitude(Vector2 v);
			static double Magnitude(Vector2 v1, Vector2 v2);			
			static Vector2 Cross(Vector2 v1, Vector2 v2);
			static double Dot(Vector2 v1, Vector2 v2);
			static Vector2 SegmentIntersection(Vector2 v1A, Vector2 v1B, Vector2 v2A, Vector2 v2B);
			static bool IsAlongNormal(Vector2 normal, Vector2 point);

			//Overloaded Operators

			bool operator==(const Vector2& v)
			{
				bool isEqual =logically_equal(x,v.x) && logically_equal(y,v.y);

				return (isEqual);
			}

			bool operator!=(const Vector2& v)
			{
				bool isEqual =logically_equal(x,v.x) && logically_equal(y,v.y);
				return (!isEqual);
			}
			Vector2 operator-(const Vector2& v)
			{
				Vector2 difference(0,0);
				difference.x = this->x-v.x;
				difference.y = this->y-v.y;
				return difference;
			}

			inline bool logically_equal(double a, double b, double error_factor=1.0)
			{
				return a==b || (float)a ==(float)b;
			 // return a==b || std::abs(a-b)<std::abs(std::min(a,b)) * std::numeric_limits<double>::epsilon()* error_factor;
			}
			//Multiply Vector2 by scalar.
			Vector2 operator*=(const double& d)
			{
				Vector2 difference(0,0);
				difference.x = this->x*d;
				difference.y = this->y*d;
				return difference;
			}		
			//Multiply Vector2 by scalar.
			Vector2 operator*(const double& d)
			{
				Vector2 difference(0,0);
				difference.x = this->x*d;
				difference.y = this->y*d;
				return difference;
			}	
			Vector2 operator/=(const double& d)
			{
				Vector2 difference(0,0);
				difference.x = this->x/d;
				difference.y = this->y/d;
				return difference;
			}					
			Vector2 operator/(const double& d)
			{
				Vector2 difference(0,0);
				difference.x = this->x/d;
				difference.y = this->y/d;
				return difference;
			}	
			Vector2 operator+(const Vector2& v)
			{
				Vector2 difference(0,0);
				difference.x = this->x+v.x;
				difference.y = this->y+v.y;
				return difference;
			}	
			Vector2 operator+=(const Vector2& v)
			{
				Vector2 difference(0,0);
				difference.x = this->x+v.x;
				difference.y = this->y+v.y;
				return difference;
			}	

		   Vector2 operator-=(const Vector2& v) const 
		    {
				Vector2 difference(0,0);
				difference.x = this->x-v.x;
				difference.y = this->y-v.y;
				return difference;
			}
			template<typename T>
		   T operator-(const T& v) const 
		    {
				T difference(0,0);
				difference.x = this->x-v.x;
				difference.y = this->y-v.y;
				return difference;
			}	
		   template<typename T>
		   T operator-() const 
		    {
		      T v;
		      v.x = -x;
		      v.y = -y;
		      return v;
			}
	};
