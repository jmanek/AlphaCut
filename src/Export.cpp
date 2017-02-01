#include "Export.h"
#include <vector>
#include <map>
#include <iostream>
#include <limits>
#include <iomanip>
#include <fstream>
#include <cstdio>
#include <cassert>
using namespace std;


void Export::WriteBDM(const string &fp, const vector<vector<Vector2>> &contours)
{
	
	FILE * file = fopen(fp.c_str(), "w");
	fprintf(file, "type 2D\n");
	for (int i = 0; i < contours.size(); i++) {
		fprintf(file, "being poly%u\n", i);
		fprintf(file, "%u \n", contours[i].size());
		for (const auto &v : contours[i]) {
			fprintf(file, "%0.17g %0.17g\n", v.x, v.y);
		}
		fprintf(file, "end poly%u\n\n", i);
	}
	fclose(file);
}


void Export::WriteOBJ(const string &fp, const Mesh &m)
{
	vector<HE_Vertex*> vs = m.GetVertices();
	map<string, int> vMap;
	for (int i = 0; i < vs.size(); i++) {
		vMap.insert(pair<string, int>(vs[i]->ToString(), i));
	}

	vector<TextureCoord*> vts = m.GetTextureCoords();
	map<string, int> vtMap;
	for (int i = 0; i < vts.size(); i++) {
		vtMap.insert(pair<string, int>(vts[i]->ToString(), i));
	}

	// We split the faces into the material that they belong to
	// Each element of faces is a vector of faces using that material
	// Any faces without material are pushed to the end of faces
	// This handles cases of meshes with some faces with no material
	// And meshes with no material
	// For faces with only vertices, each element in faces is a Fx3 vector
	// If there are texture coords, each element is Fx6 v, v, v, vt, vt, vt
	vector<HE_Face*> faces = m.GetFaces();
	vector<vector<vector<int>>> fs(m.materials.size() + 1);
	for (int f = 0; f < faces.size(); f++) {
		// Get the vertices for the face
		vector<int> face {vMap[faces[f]->edge->vA->ToString()] + 1, 
						 vMap[faces[f]->edge->vB->ToString()] + 1, 
						 vMap[faces[f]->edge->prevEdge->vA->ToString()] + 1};
		// If the mesh has texture coords get those as well
		if (!vts.empty()) {
			face.push_back(vtMap[faces[f]->edge->vtA->ToString()] + 1);
			face.push_back(vtMap[faces[f]->edge->vtB->ToString()] + 1);
			face.push_back(vtMap[faces[f]->edge->prevEdge->vtA->ToString()] + 1);
		}
		// The face is placed into fs based on its material index
		if (faces[f]->materialIndex == -1) {
			fs[fs.size()-1].push_back(face);
		} else {
			fs[faces[f]->materialIndex].push_back(face);
		}
	}
	string directory = fp.substr(0, fp.find_last_of("/") + 1);
	string filename = fp.substr(fp.find_last_of("/") + 1);
	string mtlFileName = filename.substr(0, filename.find_last_of(".obj") - 3) + ".mtl";


	FILE * file = fopen(fp.c_str(), "w");
	vector<Material> ms = m.materials;
	fprintf(file, "# %u vertices\n", vs.size());
	if (!vts.empty()) { fprintf(file, "# %u texture coordinates\n", vts.size()); }
	fprintf(file, "# %u faces\n", fs.size());
	if (!m.materials.empty()) { fprintf(file, "mtllib %s\n", mtlFileName.c_str()); }

	for (int i = 0; i < vs.size(); i++) {
		fprintf(file, "v %0.17g %0.17g %0.17g\n", vs[i]->x, vs[i]->y, vs[i]->z);
	}

	if (!vts.empty()) {
		for (int i = 0; i < vts.size(); i++) {
			fprintf(file, "vt %0.17g %0.17g\n", vts[i]->x, vts[i]->y);
		}
	}

	for (int u = 0; u < fs.size(); u++) {
		// check that there are materials
		// and that these faces actually have a material
		if (!ms.empty() && u < ms.size()) { fprintf(file, "usemtl %s\n", ms[u].name.c_str()); }
		for (int v = 0; v < fs[u].size(); v++) {
			if (fs[u][v].size() == 3) {
				fprintf(file, "f %u/%u/%u\n", fs[u][v][0], fs[u][v][1], fs[u][v][2]);
			} else {
				fprintf(file, "f %u/%u %u/%u %u/%u\n", 
					    fs[u][v][0], fs[u][v][3], fs[u][v][1],
					    fs[u][v][4], fs[u][v][2], fs[u][v][5]);
			}
		}
	}
	fclose(file);
	if (!m.materials.empty()) {
		WriteMTL(directory + mtlFileName, m.materials);
	}
}	

void Export::WriteMTL(const string &fp, const vector<Material> &ms)
{
	FILE * file = fopen(fp.c_str(), "w");

	for (const auto &mat : ms)
	{	
		fprintf(file, "newmtl %s\n", mat.name.c_str());
		fprintf(file, "Ka %0.5g %0.5g %0.5g\n", mat.ambient.r, mat.ambient.g, mat.ambient.b);
		fprintf(file, "Kd %0.5g %0.5g %0.5g\n", mat.diffuse.r, mat.diffuse.g, mat.diffuse.b);
		if (mat.ambientMap != "") { fprintf(file, "map_Ka %s\n", mat.ambientMap.c_str()); }
		if (mat.diffuseMap != "") { fprintf(file, "map_Kd %s\n", mat.diffuseMap.c_str()); }
		if (mat.alphaMap != "") { fprintf(file, "map_d %s\n", mat.alphaMap.c_str()); }
		fprintf(file, "\n");
	}


	fclose(file);
}
