#pragma once

#include "HE_Face.h"
#include "HE_Vertex.h"
#include "Material.h"
#include "TextureCoord.h"
#include "Mesh.h"
#include "Log.h"
#include <string>       
#include <vector>       
#include <tuple>
	using namespace std;
	class Export
	{
		public:
			Export();
			template<typename... Args>
			static void ExportMesh(const string &fp, const Mesh &mesh)			
			{
					
				if (fp.find(".obj") != string::npos) {
					Export::WriteOBJ(fp, mesh);  
				} else {
					Log::Error("Non-obj export not supported");
				}

			}

			static void WriteBDM(const string &fp, const vector<vector<Vector2>> &contours);
		private:
			static void WriteOBJ(const string &fp, const Mesh &m); 
			static void WriteMTL(const string &fp, const vector<Material> &ms);
	};
