#pragma once

#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream
#include <limits>
#include <cmath>
#include <vector>
#include "TextureCoord.h"
#include "HE_Vertex.h"
#include "HE_Face.h"

#include <memory>

	using namespace std;
	class TextureCoord;
	class HE_Edge  
	{
		private:


		public:
			HE_Edge(HE_Face* face, HE_Vertex *vA, HE_Vertex* vB);
			HE_Edge(HE_Face* face, HE_Vertex *vA, HE_Vertex* vB, TextureCoord* vtA, TextureCoord* vtB);

			bool IsSingular();
			bool IsBoundary();
			bool OrientationIsConsistent(HE_Edge* e);

			HE_Edge* GetContinuation();
			// bool IsDegenerate();

			//Face this is Connected to
			HE_Face* face = NULL;

			//The ID of Edges to be incremented each time a new EDGE is created
			static long unsigned int lastID;

			//Book Keeping
			long unsigned int uniqueID=0;

            vector<double> GetMidpoint();

			HE_Vertex*  vA;
			TextureCoord* vtA;
			HE_Vertex*  vB;
			TextureCoord* vtB;

			//List of Edges linked to this Edge
			std::vector<HE_Edge*> oppositeEdges;

			//References 
			HE_Edge* nextEdge = NULL;
			HE_Edge* prevEdge = NULL;
			string ToString();

			bool operator==(const HE_Edge& e)
			{
				return vA == e.vA && vB == e.vB; 
			}
	};
