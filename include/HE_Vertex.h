#pragma once

#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream

#include <limits>
#include <cmath>
#include <vector>
//Cyclic Dependency
// #include "HE_Face.h"
// #include "HE_Edge.h"

	class HE_Face; //Foreward Decleration to avoid Cyclic Dependency

	using namespace std;
	class HE_Vertex
	{


		public: 
			HE_Vertex() {};
			HE_Vertex(double x, double y, double z);

			//The ID of Edges to be incremented each time a new EDGE is created
			static long unsigned int lastID;

			//Book Keeping
			long unsigned int uniqueID=0;

			double x;
			double y;
			double z;
			void AssignFace(HE_Face* face);

			// void AssignEdge(const shared_ptr<HE_Edge>& edge)

			//List of Faces linked to this Vertex
			std::vector<HE_Face*> faces;
			//List of Edges linked to this Vertex
			// std::vector<shared_ptr<HE_Edge*>> edgeList;
			static double Distance(HE_Vertex v1, HE_Vertex v2)
			{
				return sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z, 2));
			}
			bool operator==(const HE_Vertex& v)
			{
				bool isEqual =logically_equal(x,v.x) && logically_equal(y,v.y) && logically_equal(z,v.z);

				return (isEqual);
			}

			bool operator!=(const HE_Vertex& v)
			{
				bool isEqual =logically_equal(x,v.x) && logically_equal(y,v.y) && logically_equal(z,v.z);
				return (!isEqual);
			}
						inline bool logically_equal(double a, double b, double error_factor=1.0)
			{
				return a==b || (float)a ==(float)b;
			 // return a==b || std::abs(a-b)<std::abs(std::min(a,b)) * std::numeric_limits<double>::epsilon()* error_factor;
			}
			HE_Vertex operator-(const HE_Vertex& v)
			{
				HE_Vertex difference(0,0,0);
				difference.x = this->x-v.x;
				difference.y = this->y-v.y;
				difference.z = this->z-v.z;

				return difference;
			}
			HE_Vertex operator+(const HE_Vertex& v)
			{
				HE_Vertex difference(0,0,0);
				difference.x = this->x+v.x;
				difference.y = this->y+v.y;
				difference.z = this->z+v.z;
				return difference;
			}	

			//Multiply vertex by scalar.
			HE_Vertex operator*=(const double& d)
			{
				HE_Vertex difference(0,0,0);
				difference.x = this->x*d;
				difference.y = this->y*d;
				difference.z = this->z*d;
				return difference;
			}		
			//Multiply vertex by scalar.
			HE_Vertex operator*(const double& d)
			{
				HE_Vertex difference(0,0,0);
				difference.x = this->x*d;
				difference.y = this->y*d;
				difference.z = this->z*d;
				return difference;
			}		
			//Multiply vertex by scalar.
			HE_Vertex operator/(const double& d)
			{
				HE_Vertex difference(0,0,0);
				difference.x = this->x/d;
				difference.y = this->y/d;
				difference.z = this->z/d;
				return difference;
			}	

	static double Magnitude(HE_Vertex v)
	{
		return 	sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	}
	void Normalize()
	{
		double magnitude = HE_Vertex::Magnitude(*this);
		if( magnitude == 1.0)
			return; //Vector is already Normalized.
		this->x /= magnitude;
		this->y /= magnitude;
		this->z /= magnitude;
	}
			static HE_Vertex Cross(HE_Vertex v1, HE_Vertex v2)
			{
				HE_Vertex v;
				v.x = (v1.y * v2.z) - (v1.z * v2.y);
				v.y = (v1.x * v2.z) - (v1.z * v2.x);
				v.z = (v1.x * v2.y) - (v1.y * v2.x);

				return v;
			}
			static double Dot(HE_Vertex v1, HE_Vertex v2)
			{
				//a · b = ax × bx + ay × by + az * bz
				}

		   HE_Vertex operator-() const 
		    {
				HE_Vertex v;
				v.x = -x;
				v.y = -y;
				v.z =-z;
				return v;
			}
			string ToString()
			{
			    stringstream sstream;
				sstream << "("<< x<<","<<y<<","<<z<<")";
				return sstream.str();
			}
	};
