#pragma once

#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream

#include <limits>
#include <cmath>
	using namespace std;
	class Vertex 
	{
		public:
			Vertex(){x=0;y=0;z=0;};
			Vertex(double x, double y, double z);



			double x;
			double y;
			double z;	
			string ToString();

			void Normalize();

			static Vertex Up();
			static Vertex Down();
			static Vertex Left();
			static Vertex Right();
			static Vertex Forward();
			static Vertex Backward();
			
			static double Magnitude(Vertex v);
			static Vertex Cross(Vertex v1, Vertex v2);
			static double Dot(Vertex v1, Vertex v2);


			//Overloaded Operators

			bool operator==(const Vertex& v)
			{
				bool isEqual =logically_equal(x,v.x) && logically_equal(y,v.y) && logically_equal(z,v.z);

				return (isEqual);
			}

			bool operator!=(const Vertex& v)
			{
				bool isEqual =logically_equal(x,v.x) && logically_equal(y,v.y) && logically_equal(z,v.z);
				return (!isEqual);
			}
			Vertex operator-(const Vertex& v)
			{
				Vertex difference(0,0,0);
				difference.x = this->x-v.x;
				difference.y = this->y-v.y;
				difference.z = this->z-v.z;

				return difference;
			}

			inline bool logically_equal(double a, double b, double error_factor=1.0)
			{
				return a==b || (float)a ==(float)b;
			 // return a==b || std::abs(a-b)<std::abs(std::min(a,b)) * std::numeric_limits<double>::epsilon()* error_factor;
			}
			//Multiply vertex by scalar.
			Vertex operator*=(const double& d)
			{
				Vertex difference(0,0,0);
				difference.x = this->x*d;
				difference.y = this->y*d;
				difference.z = this->z*d;
				return difference;
			}		
			//Multiply vertex by scalar.
			Vertex operator*(const double& d)
			{
				Vertex difference(0,0,0);
				difference.x = this->x*d;
				difference.y = this->y*d;
				difference.z = this->z*d;
				return difference;
			}		
			//Multiply vertex by scalar.
			Vertex operator/(const double& d)
			{
				Vertex difference(0,0,0);
				difference.x = this->x/d;
				difference.y = this->y/d;
				difference.z = this->z/d;
				return difference;
			}	

			Vertex operator+(const Vertex& v)
			{
				Vertex difference(0,0,0);
				difference.x = this->x+v.x;
				difference.y = this->y+v.y;
				difference.z = this->z+v.z;
				return difference;
			}			

		   Vertex operator-() const 
		    {
				Vertex v;
				v.x = -x;
				v.y = -y;
				v.z =-z;
				return v;
			}
	};
