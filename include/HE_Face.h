#pragma once


#include "HE_Vertex.h"
#include "HE_Edge.h"
#include "TextureCoord.h"
#include <memory>

	using namespace std;
	class HE_Edge;
	class TextureCoord;
	class HE_Face
	{

			
		public:
			HE_Face();
			HE_Face(long unsigned int index, HE_Vertex*  vA, HE_Vertex* vB, HE_Vertex*  vC);

			// void Assign_Edge(const shared_ptr<HE_Edge>& edge);
			long unsigned int uniqueID=0; //Index of this Face, this is one way to Compare a Face to a Triangle.
			//References 
			//HE_Vertex* vA = NULL;
			//HE_Vertex* vB = NULL;
			//HE_Vertex* vC = NULL;
			int materialIndex;

			HE_Vertex* vA();
			HE_Vertex* vB();
			HE_Vertex* vC();
			TextureCoord* vtA();
			TextureCoord* vtB();
			TextureCoord* vtC();
		
			HE_Edge* edge = NULL;

			string ToString();

			HE_Vertex Normal();
			void Flip();
            bool IsBoundary(); 
			bool operator==(const HE_Face& f)
			{
				return edge == f.edge;
			}
	};
