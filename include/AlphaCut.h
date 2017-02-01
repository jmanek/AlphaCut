#include "Mesh.h"
#include "CGAL_Helper.h"
#include "Image.h"
#include "Barycentric.h"
#include <vector>

using namespace std;
class AlphaCut
{
	public:
		AlphaCut();
		static void Process(Mesh &m);
		static void Test(Mesh &m);
        static Point_2 GetIntersectionPoint(Segment_2 seg, Segment_2 e1, Segment_2 e2, Segment_2 e3);
		static vector<HE_Face*> BuildFaces(Mesh &m, const vector<vector<Point_2>> &tris, HE_Face* rootFace, vector<Point_2> rfBC);
	private:
		// Cuts the faces using the given texture
		static void Process(Mesh &m, const Image &image, std::vector<HE_Face*> &faces);
		// Triangulates the output from clipper

};
