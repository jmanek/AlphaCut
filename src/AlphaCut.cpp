#include "AlphaCut.h"
#include "Face.h"
#include "Image.h"
#include "Barycentric.h"
#include "Export.h"
#include "CGAL_Helper.h"
#include <map>
#include <algorithm>
#include <string>
#include "Log.h"
#include "Load.h"
#include <stdlib.h>
#include <cstdio>
#include <cassert>
#include <unordered_map>

using namespace std;


void AlphaCut::Process(Mesh &m)
{

    // Create a mapping from material index to faces using that material
    map<int, vector<HE_Face*>> materialMap;
    for (const auto &face : m.GetFaces()) {
        if (materialMap.find (face->materialIndex) == materialMap.end()) {
            materialMap.insert(pair<int, vector<HE_Face*>>(face->materialIndex, vector<HE_Face*>()));
        } 
        materialMap[face->materialIndex].push_back(face);
    }
    // Go through the materials, looking for any with a transparency map

    Log::Debug("Finding transparency maps");
    for (int i = 0; i < m.materials.size(); i++) {
        if (m.materials[i].HasAlphaMap() || m.materials[i].HasDiffuseMap()) {
            Image image = Image(m.materials[i].basePath + (m.materials[i].HasAlphaMap() ? m.materials[i].alphaMap : m.materials[i].diffuseMap));
            if (image.HasAlpha()) {
                Process(m, image, materialMap[i]);
            } else {
                Log::Warn("{} has alpha map with no alpha channel", m.materials[i].name); 
                if (m.materials[i].HasDiffuseMap()) {
                    image = Image(m.materials[i].basePath + m.materials[i].diffuseMap);
                    if (image.HasAlpha()) {
                        Process(m, image, materialMap[i]);
                    }
                }
            }
        } else {
            Log::Debug("No Alpha Map found for {0}", m.materials[i].name);
        }
    }
}
void AlphaCut::Process(Mesh &m, const Image &image, vector<HE_Face*> &faces)
{ 
    float width = (float) image.Width();
    float height = (float) image.Height();
    TextureCoord pBL;
    TextureCoord pBR;
    TextureCoord pTL;
    TextureCoord pTR;
    Barycentric bcBL;
    Barycentric bcBR;
    Barycentric bcTL;
    Barycentric bcTR;
    // A vector of all the transparent pixels
    Log::Debug("Travering image pixels");
    vector<vector<Point_2>> pixels;
    for (int x = 0; x < image.Width(); x++) {
        for (int y = 0; y < image.Height(); y++) {
            // If the pixel is not transparent
            if (!image.IsTransparent(x, y)) {
                //Get the uv coordinates for each corner of the pixel
                // x -> u : x/width
                // y -> v : 1-(y/height)
                Point_2 pBL = Point_2(x/width, 1.0 - ((y + 1)/height));
                Point_2 pBR = Point_2((x + 1)/width, 1.0 - ((y + 1)/height));
                Point_2 pTL = Point_2(x/width, 1.0 - (y/height));
                Point_2 pTR = Point_2((x + 1)/width, 1.0 - (y/height));
                // Create a polygon (square) from the uv coordinates of the pixel
                vector<Point_2> pixel { pBL, pBR, pTR, pTL };
                pixels.push_back(pixel);
            }
        }
    }

    Log::Debug("Clipping mesh with pixels");
    for (const auto &face : faces) {

        TextureCoord vtA = *(face->vtA());
        TextureCoord vtB = *(face->vtB());
        TextureCoord vtC = *(face->vtC());
        Point_2 pvA = Point_2(vtA.x, vtA.y);
        Point_2 pvB = Point_2(vtB.x, vtB.y);
        Point_2 pvC = Point_2(vtC.x, vtC.y);
        vector<Point_2> rfBC = {pvA, pvB, pvC};
        Segment_2 e1 = Segment_2(pvA, pvB);
        Segment_2 e2 = Segment_2(pvB, pvC);
        Segment_2 e3 = Segment_2(pvC, pvA);
        vector<HE_Face*> newFaces;
        unordered_map<string, HE_Vertex*> vMap;
        unordered_map<string, TextureCoord*> tcMap;
        // unordered_map<Point_2, HE_Vertex*> vertMap;
        // for each pixel, we see what parts, if any are inside the triangle
    
        // if it is, we create a new face based on the what part is inside
        for (const auto &p : pixels) {

            bool pBL = CGAL_Helper::PointInTriangle(p[0], pvA, pvB, pvC);
            bool pBR = CGAL_Helper::PointInTriangle(p[1], pvA, pvB, pvC);
            bool pTR = CGAL_Helper::PointInTriangle(p[2], pvA, pvB, pvC);
            bool pTL = CGAL_Helper::PointInTriangle(p[3], pvA, pvB, pvC);
            vector<bool> pInTri = {pBL, pBR, pTR, pTL};
            vector<vector<Point_2>> tris;	
            for (int i = 0; i < 4; i++) {
                // If this corner of the pixel is inside the face, it is part of a triangle:
                //  	 CASES:
                //  	 1. And none of its neighbors are
                //  	 2. And both of its neighbors are
                //  	 3. Only One of its neighbors are and that neighbor's neighbor is
                //		 4. This corner and only one of its neighbors is inside the triangle and
                //			no other corners are inside the triangle 
                // For corner inside the triangle, we need to check its segments to corners not in the triangle 
                // This applies to case 2/3/4
                // Also in case 1, check if corner 'consumes' a vertex of the face
                if (pInTri[i]) {
                    int leftCorner = i == 0 ? 3 : i - 1;
                    int rightCorner = i == 3 ? 0 : i + 1;
                    int farCorner = i == 2 ? 0 : i == 3 ? 1 : i + 2;	

                    // Check case 1
                    if (!pInTri[leftCorner] && !pInTri[rightCorner]) {
                        // Get the edge of the triangle this corner's segments intersect	
                        Segment_2 cL1 = Segment_2(p[i], p[leftCorner]);
                        Segment_2 cL2 = Segment_2(p[i], p[rightCorner]);	
                        Point_2 vB = AlphaCut::GetIntersectionPoint(cL1, e1, e2, e3);
                        Point_2 vC = AlphaCut::GetIntersectionPoint(cL2, e1, e2, e3);
                        tris.push_back({p[i], vC, vB});
                        i++; // since the nextCorner is not valid, we can skip it	

                        // check case 2
                        // Create a triangle out of this corner and its neighbors
                    } else if (pInTri[leftCorner] && pInTri[rightCorner]) {
                        tris.push_back({p[leftCorner], p[i], p[rightCorner]});
                        i++; // We skip the rightCorner, as it's inside the triangle 
                        if (!pInTri[farCorner]) {
                            Segment_2 cL1 = Segment_2(p[leftCorner], p[farCorner]);
                            Segment_2 cL2 = Segment_2(p[rightCorner], p[farCorner]);
                            Point_2 vB = AlphaCut::GetIntersectionPoint(cL1, e1, e2, e3);
                            Point_2 vC = AlphaCut::GetIntersectionPoint(cL2, e1, e2, e3);
                            // We construct two triangles, as the intersection.creates a polygon
                            tris.push_back({p[leftCorner], p[rightCorner], vB});
                            tris.push_back({p[rightCorner], vC, vB});
                            break;
                        }
                        // Check case 3/4
                    } else if ((pInTri[leftCorner] && !pInTri[rightCorner]) || 
                                pInTri[leftCorner] && !pInTri[rightCorner]) { 

                        // case 4
                        if (!pInTri[farCorner]) {
                            if (pInTri[leftCorner]) {
                                Segment_2 cL1 = Segment_2(p[i], p[rightCorner]);
                                Segment_2 cL2 = Segment_2(p[leftCorner], p[farCorner]);
                                Point_2 vB = AlphaCut::GetIntersectionPoint(cL1, e1, e2, e3);
                                Point_2 vC = AlphaCut::GetIntersectionPoint(cL2, e1, e2, e3);
                                // We construct two triangles, as the intersection.creates a polygon
                                tris.push_back({p[i], vB, p[leftCorner]});
                                tris.push_back({p[leftCorner], vB, vC});
                                break; // Since this pixel only contains these triangles, we are done	

                            } else if (pInTri[rightCorner]) {
                                Segment_2 cL1 = Segment_2(p[i], p[leftCorner]);
                                Segment_2 cL2 = Segment_2(p[rightCorner], p[farCorner]);
                                Point_2 vB = AlphaCut::GetIntersectionPoint(cL1, e1, e2, e3);
                                Point_2 vC = AlphaCut::GetIntersectionPoint(cL2, e1, e2, e3);
                                // We construct two triangles, as the intersection.creates a polygon
                                tris.push_back({p[i], vB, p[rightCorner]});
                                tris.push_back({p[rightCorner], vB, vC});
                                break; // Since this pixel only contains these triangles, we are done	

                            }	
                            // case 3
                        } else if(pInTri[leftCorner] && pInTri[farCorner]) {
                            tris.push_back({p[i], p[farCorner], p[leftCorner]});
                            Segment_2 cL1 = Segment_2(p[i], p[rightCorner]);
                            Segment_2 cL2 = Segment_2(p[farCorner], p[rightCorner]);
                            Point_2 vB = AlphaCut::GetIntersectionPoint(cL1, e1, e2, e3);
                            Point_2 vC = AlphaCut::GetIntersectionPoint(cL2, e1, e2, e3);
                            // We construct two triangles, as the intersection.creates a polygon
                            tris.push_back({p[i], vB, p[farCorner]});
                            tris.push_back({p[farCorner], vB, vC});
                            break; // Since this pixel only contains this triangle, we are done	

                        } else if(pInTri[rightCorner] && pInTri[farCorner]) {
                            vector<Point_2> tri {p[i], p[rightCorner], p[farCorner]};
                            tris.push_back(tri);
                            // handle segment from corners to edge
                            Segment_2 cL1 = Segment_2(p[i], p[leftCorner]);
                            Segment_2 cL2 = Segment_2(p[farCorner], p[leftCorner]);
                            Point_2 vB = AlphaCut::GetIntersectionPoint(cL1, e1, e2, e3);
                            Point_2 vC = AlphaCut::GetIntersectionPoint(cL2, e1, e2, e3);
                            // We construct two triangles, as the intersection.creates a polygon
                            tris.push_back({p[i], vB, p[farCorner]});
                            tris.push_back({p[farCorner], vB, vC});
                            break; // Since this pixel only contains this triangle, we are done	
                        }
                    }
                }
            } 
            if (tris.size() > 0) {
                vector<HE_Face*> addedFaces = AlphaCut::BuildFaces(m, tris, face, rfBC);
                for (const auto &f : addedFaces) {
                    newFaces.push_back(f);
                }
            }
        }
        m.DeleteFace(face);
        // m.SimplifyFaces(newFaces);
    }

    m.BuildEdgeConnectivity();	
}

// Intersects a segment with the edges of a triangle, sets intersection to the intersection point, if found
// If there is no intersection it was raise an error
Point_2 AlphaCut::GetIntersectionPoint(Segment_2 seg, Segment_2 e1, Segment_2 e2, Segment_2 e3)
{
    Point_2 intersection;
    bool intersects = false;
    if (CGAL_Helper::Intersects(seg, e1, intersection)) {
        intersects = true;
    } else if (CGAL_Helper::Intersects(seg, e2, intersection)) {
        intersects = true;
    } else if (CGAL_Helper::Intersects(seg, e3, intersection)) {
        intersects = true;
    }
    assert(intersects);
    return intersection;
}

vector<HE_Face*> AlphaCut::BuildFaces(Mesh &m, const vector<vector<Point_2>> &tris, HE_Face* rootFace, vector<Point_2> rfBC)
{
    vector<Face> faces;
    vector<vector<double>> vertices;
    vector<vector<double>> textureCoords;
    HE_Vertex rfVa = *(rootFace->vA());
    HE_Vertex rfVb = *(rootFace->vB());
    HE_Vertex rfVc = *(rootFace->vC());
    for (const auto &tri : tris) {
        string triA = to_string(CGAL::to_double(tri[0].x())) + to_string(CGAL::to_double(tri[0].y()));
        string triB = to_string(CGAL::to_double(tri[1].x())) + to_string(CGAL::to_double(tri[1].y()));
        string triC = to_string(CGAL::to_double(tri[2].x())) + to_string(CGAL::to_double(tri[2].y()));
        Face face = Face();
        face.materialIndex = rootFace->materialIndex;
        TextureCoord* vtA;
        // if (tcMap.find(triA) != tcMap.end()) {
            // vtA = tcMap[triA];
        // } else {
            vtA = m.Allocate_TextureCoord(CGAL::to_double(tri[0].x()), CGAL::to_double(tri[0].y()));
            // tcMap.insert(make_pair(triA, vtA));
        // }
        face.vtA = textureCoords.size();
        face.vtB = face.vtA + 1;
        face.vtC = face.vtA + 2;
        face.vA = vertices.size();
        face.vB = face.vA + 1;
        face.vC = face.vA + 2;
        vector<double> bcVa = CGAL_Helper::GetBarycentric(tri[0], rfBC[0], rfBC[1], rfBC[2]);
        vector<double> bcVb = CGAL_Helper::GetBarycentric(tri[1], rfBC[0], rfBC[1], rfBC[2]);
        vector<double> bcVc = CGAL_Helper::GetBarycentric(tri[2], rfBC[0], rfBC[1], rfBC[2]);
        HE_Vertex vA = rfVa*bcVa[0] + rfVb*bcVa[1] + rfVc*bcVa[2];
        HE_Vertex vB = rfVa*bcVb[0] + rfVb*bcVb[1] + rfVc*bcVb[2];
        HE_Vertex vC = rfVa*bcVc[0] + rfVb*bcVc[1] + rfVc*bcVc[2];
        vertices.push_back({vA.x, vA.y, vA.z});
        vertices.push_back({vB.x, vB.y, vB.z});
        vertices.push_back({vC.x, vC.y, vC.z});
        textureCoords.push_back({CGAL::to_double(tri[0].x()), CGAL::to_double(tri[0].y())});
        textureCoords.push_back({CGAL::to_double(tri[1].x()), CGAL::to_double(tri[1].y())});
        textureCoords.push_back({CGAL::to_double(tri[2].x()), CGAL::to_double(tri[2].y())});
        faces.push_back(face);
    }
    return m.AddGeometry(vertices, faces, textureCoords);
}


    // Make sure the barycentric coords can correctly recreate the geometry
    void AlphaCut::Test(Mesh &m)
    {
        vector<HE_Face*> faces = m.GetFaces();
        for (const auto &face : faces) {
            Log::Debug("f");
            TextureCoord vtA = *(face->edge->vtA);
            TextureCoord vtB = *(face->edge->vtB);
            TextureCoord vtC = *(face->edge->nextEdge->vtB);
            Barycentric bcVa = Barycentric::Get(*(face->vtA()), *(face->vtA()), *(face->vtB()), *(face->vtC()));
            Barycentric bcVb = Barycentric::Get(*(face->vtB()), *(face->vtA()), *(face->vtB()), *(face->vtC()));
            Barycentric bcVc = Barycentric::Get(*(face->vtC()), *(face->vtA()), *(face->vtB()), *(face->vtC()));
            if (!bcVa.IsValid() || !bcVb.IsValid() || !bcVc.IsValid()) {
                Log::Error("Barycentric coordinates wrong");
            }
            HE_Vertex vA = bcVa.Cartesian(*(face->vA()), *(face->vB()), *(face->vC()));
            HE_Vertex vB = bcVb.Cartesian(*(face->vA()), *(face->vB()), *(face->vC()));
            HE_Vertex vC = bcVc.Cartesian(*(face->vA()), *(face->vB()), *(face->vC()));
            Log::File("\n\nOriginal:");
            Log::File("{}", face->vA()->ToString());
            Log::File("New");
            Log::File("{}", vA.ToString());
            Log::File("Original:");
            Log::File("{}", face->vB()->ToString());
            Log::File("New");
            Log::File("{}", vB.ToString());
            Log::File("Original:");
            Log::File("{}", face->vC()->ToString());
            Log::File("New");
            Log::File("{}", vC.ToString());
        }

    }

