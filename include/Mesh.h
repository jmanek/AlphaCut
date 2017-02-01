#pragma once

#include "Face.h"
#include "HE_Face.h"
#include "HE_Edge.h"
#include "Material.h"
#include "TextureCoord.h"
#include "HE_Vertex.h"
#include <vector>
#include <memory>
#include <unordered_map>
	using namespace std;
	class FaceCluster;
	class Mesh 
	{
		public:
			Mesh();
			Mesh(vector<vector<double>> &vertices, vector<Face> &faces, vector<vector<double>> &textureCoords, vector<Material> &materials);
			void BuildEdgeConnectivity();
			//Whenever a Member wishes to Allocate a Half-Edge, or Half-Vertex we store it
			//here so we can Clean up properly and avoide Memleaks
			void Store_Edge( unique_ptr<HE_Edge> &edgeReference);
			void Store_Vertex( unique_ptr<HE_Vertex> &vertexReference);
			void Store_Face( unique_ptr<HE_Face> &faceReference);
			void Store_TextureCoord( unique_ptr<TextureCoord> &textureCoordReference);
			void DeleteFace(HE_Face* f);
			void DeleteEdge(HE_Edge* e);
			void DeleteVertex(HE_Vertex* v);
			void DeleteTextureCoord(TextureCoord* tc);
			vector<HE_Face*> GetFaces() const;
			vector<HE_Vertex*> GetVertices() const;
			vector<TextureCoord*> GetTextureCoords() const;
			//Pass in the Parameters for the Edge you wish to create on the Heap
			HE_Edge* Allocate_Edge(HE_Face *face, HE_Vertex* vA, HE_Vertex* vB);
			HE_Edge* Allocate_Edge(HE_Face *face, HE_Vertex* vA, HE_Vertex* vB, TextureCoord* vtA, TextureCoord* vtB);
			//Pass in the Parameters for the Edge you wish to create on the Heap
			HE_Vertex* Allocate_Vertex(double x, double y, double z);
			HE_Vertex* Allocate_Vertex();
			//Pass in the Parameters for the Face you wish to create on the Heap
			HE_Face* Allocate_Face(int index, HE_Vertex a, HE_Vertex b, HE_Vertex c);
			HE_Face* Allocate_Face();
			TextureCoord* Allocate_TextureCoord(double u, double v);
			TextureCoord* Allocate_TextureCoord();
			HE_Face* CreateFace(HE_Vertex* vA, HE_Vertex* vB, HE_Vertex* vC);
			HE_Face* CreateFace(HE_Vertex* vA, HE_Vertex* vB, HE_Vertex* vC, 
								TextureCoord* vtA, TextureCoord* vtB, TextureCoord* vtC, int materialIndex);
			HE_Face* GetMostXPositiveVertex();
			// Creates an offset surface from the given face cluster
			// void Offset(FaceCluster& fc, double offsetAmount);
			// Returns a new vertex offset by the magnitude
			HE_Vertex* OffsetVertex(HE_Vertex* v, double magnitude);
			void PrintStatistics();
            void RemoveDuplicateVertices(const vector<HE_Vertex*> &vertices = vector<HE_Vertex*>());
            void SimplifyFaces(vector<HE_Face*> faces);
			vector<Material> materials;
			vector<HE_Face*> AddGeometry(const vector<vector<double>> &vs, const vector<Face> &faces, const vector<vector<double>> &tcs);

			//Call this to Ensure the tree has been properly disposed of.
			void Cleanup();
		private:

			//Lists keeping track of References to Clear (Smart Pointers)
			vector<unique_ptr<HE_Face>>   faceReferences;			
			vector<unique_ptr<HE_Edge>>   edgeReferences;
			vector<unique_ptr<HE_Vertex>> vertexReferences;
			vector<unique_ptr<TextureCoord>> textureCoordReferences;

			//Hash Tables used to Determine whether Faces/Vertices/Edges were already inserted
			//Key:  [Face.INDEX]  -> "0"
			unordered_map<string, HE_Face*> insertedFaces;
			//Key:  [x,y,z]  -> "3.531,324234,123123"			
			unordered_map<string, HE_Vertex*> insertedVertices;
			//Key:  [vA, vB] -> "(3.4123123,23423423,3423423)(3.4123123,23423423,3423423) "
			unordered_map<string, HE_Edge*> insertedEdges;
	};
