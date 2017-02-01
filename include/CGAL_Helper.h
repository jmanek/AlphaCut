#pragma once

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/intersections.h>
#include <CGAL/Point_3.h>
#include <CGAL/Line_3.h>
#include <CGAL/Line_2.h>
#include <CGAL/Plane_3.h>
#include <CGAL/Triangle_3.h>
#include <CGAL/Polygon_2_algorithms.h>
#include <CGAL/Barycentric_coordinates_2/Triangle_coordinates_2.h>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include "Vector2.h"
#include "Barycentric.h"

#include <vector>

	//Note in order for any of the types in CGAl to be used, a kernal K must be specified,
	////Exact_predicates_exact_constructions_kernel is just one of the models, and is the one used in examples.
	typedef CGAL::Exact_predicates_exact_constructions_kernel K;
	typedef K::Point_3 Point_3;
	typedef K::Line_3 Line_3;
	typedef K::Line_2 Line_2;
	typedef K::Segment_2 Segment_2;
	typedef K::Ray_3 Ray_3;
	typedef K::Plane_3 Plane_3;
	typedef K::Triangle_3 Triangle_3;
	typedef K::Intersect_3 Intersect_3;
	typedef K::Intersect_2 Intersect_2;

	// typedef K::Intersect_2 Intersect_2;
	typedef K::Segment_3 Segment_3;

	typedef K::Ray_2 Ray_2;
	typedef K::Point_2 Point_2;
    typedef CGAL::Barycentric_coordinates::Triangle_coordinates_2<K> Triangle_coordinates;


	class CGAL_Helper
	{
		public:
			// static Triangle_3 ConvertTriangle(Triangle tri);
			// static Point_3 ConvertVertex(Vertex vert);

			// //For Functions that don't require the value of the Intersection
			// static bool Intersects(Plane plane1, Plane plane2); //Converts to CGAL Equiv then performs Calculation

			// static bool Intersects(Plane plane1, Vertex bottomLeft, Vertex topRight, Vertex topLeft);

			// static bool Intersects(Plane_3 cgal_Plane1, Plane_3 cgal_Plane2);
			// static bool Intersects(Line_3 cgal_Line, Plane_3 cgal_Plane);
			static bool Intersects(Segment_2 l1, Segment_2 l2, Point_2 &p);		
            static bool PointInTriangle(Point_2 p, Point_2 vA, Point_2 vB, Point_2 vC);			
            static std::vector<double> GetBarycentric(Point_2 query, Point_2 vA, Point_2 vB, Point_2 vC);		    
            // static Segment_2 GetSegment_2(Point_2 p, Point_2 q);
			/* template<typename T> */
			// static Segment_2 GetSegment_2(T p, T q)
			// {
			//     Point_2 p1 (p.x, p.y);
			//     Point_2 p2 (q.x, q.y);
			//     return Segment_2(p1, p2);
			/* } */;
            template<typename T>
            static Point_2 GetPoint_2(T p)
            {
                Point_2 q (p.x, p.y);
                return q;
            }
			// static Line_2 GetLine_2(Barycentric p, Barycentric q);
			// static Point_2 GetPoint_2(Barycentric p, Barycentric q);
			// static Point_2 GetPoint_2(u p, Barycentric q);
			// static Point_2 GetPoint_2(Barycentric p, Barycentric q);
	};
