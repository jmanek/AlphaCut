#include "Load.h" 
#include "Log.h" 
#include "Mesh.h"
#include "Material.h" 
#include "Face.h" 
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <utility>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#define VERTICES_PER_FACE 3
#define LINEMAX 4096
using namespace std;

Mesh Load::LoadOBJ(const string &fp) 
{
	vector<Material> materials;
	vector<vector<double>> vertices;
	vector<Face> faces;
	vector<vector<double>> textureCoords;

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> tobjmaterials;
	std::string err;
	std::string mtlBasePath = fp.substr(0, fp.find_last_of('/') + 1);

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &tobjmaterials, &err, fp.c_str(), mtlBasePath.c_str());

	if (!err.empty()) { // `err` may contain warning message.
	  std::cerr << err << std::endl;
	}

	if (!ret) {
	  exit(1);
	}

	int v;
	// build vertices
	assert (attrib.vertices.size() % VERTICES_PER_FACE == 0);
	for (v = 0; v < attrib.vertices.size(); v += VERTICES_PER_FACE) {
		vector<double> vert = {attrib.vertices[v], attrib.vertices[v+1], attrib.vertices[v+2]};
		vertices.push_back(vert);
	}

	// build texture coords
	int textureCoordsPerFace = 2;
	if (attrib.texcoords.size() % textureCoordsPerFace != 0) {
		int textureCoordsPerFace = 3;
	}
	for (v = 0; v < attrib.texcoords.size(); v += textureCoordsPerFace) {
		vector<double> texcoord = {attrib.texcoords[v], attrib.texcoords[v+1]};
		textureCoords.push_back(texcoord);
	}

	// build materials
	for (int m = 0; m < tobjmaterials.size(); m++) {
		Material material = Material();
		material.name = tobjmaterials[m].name;
		material.ambient.Set(tobjmaterials[m].ambient[0], tobjmaterials[m].ambient[1], tobjmaterials[m].ambient[2]);
		material.diffuse.Set(tobjmaterials[m].diffuse[0], tobjmaterials[m].diffuse[1], tobjmaterials[m].diffuse[2]);
		material.ambientMap = tobjmaterials[m].ambient_texname;
		material.diffuseMap = tobjmaterials[m].diffuse_texname;
		material.alphaMap = tobjmaterials[m].alpha_texname;
		material.basePath = mtlBasePath;
		materials.push_back(material);
	}

	for (size_t s = 0; s < shapes.size(); s++) {
	  // Loop over faces(polygon)
	  size_t index_offset = 0;
	  for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
	    Face face = Face();
	    tinyobj::index_t idxvA = shapes[s].mesh.indices[index_offset];
	    tinyobj::index_t idxvB = shapes[s].mesh.indices[index_offset + 1];
	    tinyobj::index_t idxvC = shapes[s].mesh.indices[index_offset + 2];
      	face.vA = idxvA.vertex_index;
      	face.vB = idxvB.vertex_index;
      	face.vC = idxvC.vertex_index;
		if (!textureCoords.empty()) {
			face.vtA = idxvA.texcoord_index;
			face.vtB = idxvB.texcoord_index;
			face.vtC = idxvC.texcoord_index;
		}
		face.materialIndex = shapes[s].mesh.material_ids[f];
		faces.push_back(face);
	    index_offset += VERTICES_PER_FACE;
	  }
	}
	return Mesh(vertices, faces, textureCoords, materials);
}

Mesh Load::LoadMesh(const string &fp) 
{
	if (fp.find(".obj") != string::npos) {
		return LoadOBJ(fp);
	}
		
  vector<Material> materials;
  vector<vector<double>> vertices;
  vector<Face> faces;
  vector<vector<double>> textureCoords;

return Mesh(vertices, faces, textureCoords, materials);

}

pair<vector<vector<double>>, vector<vector<int>>> Load::LoadPLT(const std::string &fp)
{
	vector<vector<double>> vs;
	vector<vector<int>> fs;
	FILE * file = fopen(fp.c_str(),"r");
	if(file == NULL) {
		Log::Error("{} does not exist!", fp);
		return make_pair(vs, fs);
	}
	
	char line[LINEMAX];
	double v[2];
	int f[3];
	
	while (fgets(line, LINEMAX, file) != NULL) {
		if (sscanf(line, "%d %d %d\n", &f[0], &f[1], &f[2]) == 3) {
			vector<int> face {f[0] - 1, f[1] - 1, f[2] - 1};
			fs.push_back(face);	
		} else if (sscanf(line, "%lf %lf\n", &v[0], &v[1]) == 2) {
			vector<double> vertex {v[0], v[1]};
			vs.push_back(vertex);
		} 
	}
	return make_pair(vs, fs);
}
