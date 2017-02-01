#include "CGAL_Helper.h"


bool CGAL_Helper::Intersects(Segment_2 l1, Segment_2 l2, Point_2 &p)
{
    auto  result = intersection(l1, l2);
    if (result) {
        if (const Point_2 *intersectionP = boost::get<Point_2>(&*result)) {
            p = *intersectionP;
            return true;
        }
    }
    return false;
}

bool CGAL_Helper::PointInTriangle(Point_2 p, Point_2 vA, Point_2 vB, Point_2 vC)
{
    Point_2 points[] =  { vA, vB, vC };
    auto result = CGAL::bounded_side_2(points, points+3, p, K());
    if (result == CGAL::ON_UNBOUNDED_SIDE) {
        return false;
    }
    return true;
}	

std::vector<double> CGAL_Helper::GetBarycentric(Point_2 query, Point_2 vA, Point_2 vB, Point_2 vC)
{
    Triangle_coordinates triangle_coordinates(vA, vB, vC);
    auto coords = CGAL::Barycentric_coordinates::compute_triangle_coordinates_2(vA, vB, vC, query, K());
    // auto coords = Triangle_coordinates::compute_triangle_coordinates_2(vA, vB, vC, query);
    std::vector<double> bc = {abs(CGAL::to_double(coords[0])), 
                              abs(CGAL::to_double(coords[1])), 
                              abs(CGAL::to_double(coords[2])) };
    return bc;
}		    
		
