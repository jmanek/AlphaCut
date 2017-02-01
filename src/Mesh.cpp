#include "Mesh.h"
#include "Log.h"
#include <iostream>
#include <iterator>
#include <map>
#include <unordered_set>

using namespace std;

Mesh::Mesh() {}

Mesh::Mesh(vector<vector<double>> &vertices, vector<Face> &faces, vector<vector<double>> &textureCoords, vector<Material> &materials) 
{
	this->materials = materials;
	int v;
	//Create our half-edge vertices
	for (v = 0; v < vertices.size(); v++) {
		Allocate_Vertex(vertices[v][0], vertices[v][1], vertices[v][2]);
	}
	for (v = 0; v < textureCoords.size(); v++) {
		Allocate_TextureCoord(textureCoords[v][0], textureCoords[v][1]);
	}


	for (int t=0;t< faces.size();++t) {
		//Create the half-edge face
		// HE_Face* face = (Allocate_Face(triangle.index,triangle.vA, triangle.vB, triangle.vC));
		HE_Vertex* vA = vertexReferences[faces[t].vA].get();
		HE_Vertex* vB = vertexReferences[faces[t].vB].get();
		HE_Vertex* vC = vertexReferences[faces[t].vC].get();

		// Skip degenerate faces
		if (HE_Vertex::Distance(*vA, *vB) < 0.1e-10 ||
			HE_Vertex::Distance(*vA, *vC) < 0.1e-10 ||
			HE_Vertex::Distance(*vB, *vC) < 0.1e-10) {
			continue;
		}
		if (!textureCoords.empty()) {
			TextureCoord* vtA = textureCoordReferences[faces[t].vtA].get();
			TextureCoord* vtB = textureCoordReferences[faces[t].vtB].get();
			TextureCoord* vtC = textureCoordReferences[faces[t].vtC].get();
			CreateFace(vA, vB, vC, vtA, vtB, vtC, faces[t].materialIndex);	
		} else {
			CreateFace(vA, vB, vC); 
		}

	}
	BuildEdgeConnectivity();
}


void Mesh::BuildEdgeConnectivity()
{
		//Iterate over the Reference List as this contains all our information anyways.
	for (int e = 0; e< edgeReferences.size(); e++) {
		HE_Edge* currentEdge = edgeReferences[e].get();
		//Grab the list of Faces using vA and vB
		vector<HE_Face*> touchingFaces;

		//Insert vA touching faces
		//We only need to add the faces from a single vertice, an opposite edge's face will be in both facess, 
		//so to prevent duplicates we can just search one.
		touchingFaces.insert(touchingFaces.end(), currentEdge->vA->faces.begin(), currentEdge->vA->faces.end());
		
		for (int f = 0; f < touchingFaces.size(); f++) {
			HE_Face* face = touchingFaces[f];
			if (face == currentEdge->face) {
				continue;
			}

			HE_Edge* oppositeEdge = face->edge;

			// Start =0, Next=1, Next =2 => 3 Edges
			for(int i = 0; i < 3; i++) {
				if( 
					(oppositeEdge->vA == currentEdge->vA ||
	 				oppositeEdge->vA == currentEdge->vB) &&
	 				(oppositeEdge->vB == currentEdge->vA ||
	 				oppositeEdge->vB == currentEdge->vB)
				  )
				{
					//By only Adding opposites to Current we avoid having to check if it's in the list when we get to the 
					//opposite edge as it's opposite edge list will be empty.
					if (find(currentEdge->oppositeEdges.begin(), 
						currentEdge->oppositeEdges.end(), oppositeEdge) == currentEdge->oppositeEdges.end()) {
						currentEdge->oppositeEdges.push_back(oppositeEdge);
					}

				}
				//Go to next cyclic edge in this face
				oppositeEdge = oppositeEdge->nextEdge;
			}
		}
	}
}

//Get Faces
vector<HE_Face*> Mesh::GetFaces() const
{
	vector<HE_Face*> faces;
	for(int i=0;i<faceReferences.size();i++)
	{
		HE_Face* face = faceReferences[i].get();
		faces.push_back(face);
	}

	return faces;
}

//Get Vertices
vector<HE_Vertex*> Mesh::GetVertices() const
{
	vector<HE_Vertex*> vertices;
	for (const auto &v : vertexReferences) { vertices.push_back(v.get()); }
	return vertices;
}

vector<TextureCoord*> Mesh::GetTextureCoords() const
{
	vector<TextureCoord*> textureCoords;
	for (const auto &tc : textureCoordReferences) { textureCoords.push_back(tc.get()); }
	return textureCoords;
}

//Pass in the Parameters for the Face you wish to create on the Heap
HE_Face* Mesh::Allocate_Face(int index, HE_Vertex a, HE_Vertex b, HE_Vertex c)
{
	//Grab 3 References (Or Allocate up to 3 new ones if none exist for each vertex)
	HE_Vertex* vA = Allocate_Vertex(a.x, a.y, a.z);
	HE_Vertex* vB = Allocate_Vertex(b.x, b.y, b.z);
	HE_Vertex* vC = Allocate_Vertex(c.x, c.y, c.z);

	HE_Face* face = NULL;
	string key = HE_Face(index,vA,vB,vC).ToString();
	//If it Doesn't Exist
  	if ( insertedFaces.find (key) == insertedFaces.end() )
  	{
		unique_ptr<HE_Face> smartface = unique_ptr<HE_Face>(new HE_Face(index,vA,vB,vC)); //Create Smart Pointer
		face = smartface.get(); //Return the Reference
		Store_Face(smartface); //Store it in our list of smart pointers

  	}
  	else
  	{
		face = insertedFaces.at(key);
	}

	if(face!=NULL) //If face isn't null (which it shouldn't be at this point)
	{
		vA->AssignFace(face);
		vB->AssignFace(face);
		vC->AssignFace(face);
	}
	return face;
}

HE_Face* Mesh::Allocate_Face()
{
	unique_ptr<HE_Face> facePtr = unique_ptr<HE_Face>(new HE_Face()); //Create Smart Pointer
	HE_Face* face = facePtr.get();
	Store_Face(facePtr); //Store it in our list of smart pointers
	return face;
}


HE_Edge* Mesh::Allocate_Edge(HE_Face* face, HE_Vertex* vA, HE_Vertex* vB) 
{
	unique_ptr<HE_Edge> edgePtr = unique_ptr<HE_Edge>(new HE_Edge(face, vA, vB)); //Create Smart Pointer
	HE_Edge* edge = edgePtr.get();
	Store_Edge(edgePtr); //Store it in our list of smart pointers
	return edge; //Return the Reference
}

HE_Edge* Mesh::Allocate_Edge(HE_Face* face, HE_Vertex* vA, HE_Vertex* vB, TextureCoord* vtA, TextureCoord* vtB) 
{
	unique_ptr<HE_Edge> edgePtr = unique_ptr<HE_Edge>(new HE_Edge(face, vA, vB, vtA, vtB)); //Create Smart Pointer
	HE_Edge* edge = edgePtr.get();
	Store_Edge(edgePtr); //Store it in our list of smart pointers
	return edge; //Return the Reference
}

//Pass in the Parameters for the Edge you wish to create on the Heap
HE_Vertex* Mesh::Allocate_Vertex(double x, double y, double z)
{
	unique_ptr<HE_Vertex> vertexPtr = unique_ptr<HE_Vertex>(new HE_Vertex(x,y,z)); //Create Smart Pointer
	HE_Vertex* vertex = vertexPtr.get();
	Store_Vertex(vertexPtr); //Store it in our list of smart pointers
	return vertex; //Return the Reference
}

HE_Vertex* Mesh::Allocate_Vertex()
{
	unique_ptr<HE_Vertex> vertexPtr = unique_ptr<HE_Vertex>(new HE_Vertex()); //Create Smart Pointer
	HE_Vertex* vertex = vertexPtr.get();
	Store_Vertex(vertexPtr); //Store it in our list of smart pointers
	return vertex; //Return the Reference
}

TextureCoord* Mesh::Allocate_TextureCoord(double x, double y)
{
	unique_ptr<TextureCoord> textureCoordPtr = unique_ptr<TextureCoord>(new TextureCoord(x, y)); //Create Smart Pointer
	TextureCoord* textureCoord = textureCoordPtr.get();
	Store_TextureCoord(textureCoordPtr); //Store it in our list of smart pointers
	return textureCoord; //Return the Reference
}

TextureCoord* Mesh::Allocate_TextureCoord()
{
	unique_ptr<TextureCoord> textureCoordPtr = unique_ptr<TextureCoord>(new TextureCoord()); //Create Smart Pointer
	TextureCoord* textureCoord = textureCoordPtr.get();
	Store_TextureCoord(textureCoordPtr); //Store it in our list of smart pointers
	return textureCoord; //Return the Reference
}


//Add the Allocated Pointers to the list for easy cleanup later.
void Mesh::Store_Face( unique_ptr<HE_Face> &faceReference)
{ 	
	faceReferences.push_back(move(faceReference));	

}

void Mesh::Store_Edge( unique_ptr<HE_Edge> &edgeReference)
{ 	edgeReferences.push_back(move(edgeReference));		

}


void Mesh::Store_Vertex( unique_ptr<HE_Vertex> &vertexReference)
{ 	
	vertexReferences.push_back(move(vertexReference));
}

void Mesh::Store_TextureCoord( unique_ptr<TextureCoord> &textureCoordReference)
{
	textureCoordReferences.push_back(move(textureCoordReference));
}


void Mesh::DeleteVertex(HE_Vertex* v)
{
	for (int u = 0; u < vertexReferences.size(); u++) {
		if (vertexReferences[u].get() == v) {
			// vertexReferences[u].reset();
			 vertexReferences.erase(vertexReferences.begin() + u);			
		}
	}
}
void Mesh::DeleteTextureCoord(TextureCoord* tc)
{
	for (int u = 0; u < textureCoordReferences.size(); u++) {
		if (textureCoordReferences[u].get() == tc) {
			 textureCoordReferences.erase(textureCoordReferences.begin() + u);
		}			
	}
}

void Mesh::DeleteEdge(HE_Edge* e)
{
	for (int i = 0; i < e->oppositeEdges.size(); i++) {
		e->oppositeEdges[i]->oppositeEdges.erase(find(e->oppositeEdges[i]->oppositeEdges.begin(), e->oppositeEdges[i]->oppositeEdges.end(), e));
	}
	for (int u = 0; u < edgeReferences.size(); u++) {
		if (edgeReferences[u].get() == e) {
			// edgeReferences[u].reset();
			 edgeReferences.erase(edgeReferences.begin() + u);
		}
	}
}

void Mesh::DeleteFace(HE_Face* f)
{
	HE_Edge* e1 = f->edge;
	HE_Vertex* vA = e1->vA;
	TextureCoord* vtA = e1->vtA;
	HE_Edge* e2 = f->edge->nextEdge;
	HE_Vertex* vB = e2->vA;
	TextureCoord* vtB = e2->vtA;
	HE_Edge* e3 = f->edge->prevEdge;
	HE_Vertex* vC = e3->vA;
	TextureCoord* vtC = e3->vtA;
	DeleteEdge(e1);
	DeleteEdge(e2);
	DeleteEdge(e3);
	vA->faces.erase(find(vA->faces.begin(), vA->faces.end(), f));
	if (vA->faces.size() == 0) { DeleteVertex(vA); }
	vB->faces.erase(find(vB->faces.begin(), vB->faces.end(), f));
	if (vB->faces.size() == 0) { DeleteVertex(vB); }
	vC->faces.erase(find(vC->faces.begin(), vC->faces.end(), f));
	if (vC->faces.size() == 0) { DeleteVertex(vC); }
	vtA->faces.erase(find(vtA->faces.begin(), vtA->faces.end(), f));
	if (vtA->faces.size() == 0) { DeleteTextureCoord(vtA); }
	vtB->faces.erase(find(vtB->faces.begin(), vtB->faces.end(), f));
	if (vtB->faces.size() == 0) { DeleteTextureCoord(vtB); }
	vtC->faces.erase(find(vtC->faces.begin(), vtC->faces.end(), f));
	if (vtC->faces.size() == 0) { DeleteTextureCoord(vtC); }

	for (int u = 0; u < faceReferences.size(); u++) {
		if (faceReferences[u].get() == f) {
			// faceReferences[u].reset();
			faceReferences.erase(faceReferences.begin() + u);
		}
	}
}

HE_Face* Mesh::CreateFace(HE_Vertex* vA, HE_Vertex* vB, HE_Vertex* vC) 
{
		HE_Face* face = Allocate_Face();
		vA->AssignFace(face);
		vB->AssignFace(face);
		vC->AssignFace(face);
		HE_Edge* edge1 = Allocate_Edge(face, vA, vB);
		HE_Edge* edge2 = Allocate_Edge(face, vB, vC);		 
		HE_Edge* edge3 = Allocate_Edge(face, vC, vA);

		// //Assign the Next Edges for Each
		edge1->nextEdge = edge2;
		edge2->nextEdge = edge3;
		edge3->nextEdge = edge1;

		// //Assign the Prev Edges for Each
		edge1->prevEdge = edge3;
		edge2->prevEdge = edge1;
		edge3->prevEdge = edge2;	

		face->edge = edge1;

		return face;
}

HE_Face* Mesh::CreateFace(HE_Vertex* vA, HE_Vertex* vB, HE_Vertex* vC, TextureCoord* vtA, TextureCoord* vtB, TextureCoord* vtC, int materialIndex) 
{
		HE_Face* face = Allocate_Face();

		vA->faces.push_back(face);
		vB->faces.push_back(face);
		vC->faces.push_back(face);
		vtA->faces.push_back(face);
		vtB->faces.push_back(face);
		vtC->faces.push_back(face);
		HE_Edge* edge1 = Allocate_Edge(face, vA, vB, vtA, vtB);
		HE_Edge* edge2 = Allocate_Edge(face, vB, vC, vtB, vtC);		 
		HE_Edge* edge3 = Allocate_Edge(face, vC, vA, vtC, vtA);

		// //Assign the Next Edges for Each
		edge1->nextEdge = edge2;
		edge2->nextEdge = edge3;
		edge3->nextEdge = edge1;

		// //Assign the Prev Edges for Each
		edge1->prevEdge = edge3;
		edge2->prevEdge = edge1;
		edge3->prevEdge = edge2;	

		face->edge = edge1;
		face->materialIndex = materialIndex;

		return face;
}

vector<HE_Face*> Mesh::AddGeometry(const vector<vector<double>> &vs, const vector<Face> &faces, const vector<vector<double>> &tcs)
{
	vector<HE_Vertex*> newVs;
	vector<TextureCoord*> newTcs;
    vector<HE_Face*> newFaces;
	for (const auto &v : vs) {
		newVs.push_back(Allocate_Vertex(v[0], v[1], v[2]));
	}
	for (const auto &tc : tcs) {
		newTcs.push_back(Allocate_TextureCoord(tc[0], tc[1]));
	}
	for (const auto &f : faces) {
		newFaces.push_back(CreateFace(newVs[f.vA], newVs[f.vB], newVs[f.vC], newTcs[f.vtA], newTcs[f.vtB], newTcs[f.vtC], f.materialIndex));
	}	   
    return newFaces;
}
// Will use the edge collapse algorithm to try and simplify the set of faces
void Mesh::SimplifyFaces(vector<HE_Face*> faces)
{
    unordered_set<HE_Face*> deletedFaces;
    for (auto &f : faces) {
        if (deletedFaces.find(f) != deletedFaces.end()) { continue; }
        if (f->IsBoundary()) { continue; }
        // Choose an edge to collapse
        // For simplicity we are assuming the edge is 2-manifold
        HE_Edge* e = f->edge;        
        vector<double> mp = e->GetMidpoint();   
        // This new vertex is the midpoint of the edge we are collapsing
        HE_Vertex* newVert = Allocate_Vertex(mp[0], mp[1], mp[2]);
        HE_Vertex* vA = e->vA;
        HE_Vertex* vB = e->vB;
        // Reassign all faces and edges to the new vertex
        for (auto &face : vA->faces) {
            HE_Edge* vertEdge = face->edge;
            for (int i = 0; i < 3; i++) {
               if (vertEdge->vA == vA) {
                    vertEdge->vA = newVert;
                    break;
               } else if (vertEdge->vB == vA) {
                   vertEdge->vB = newVert;
                   break;
               }
               vertEdge = vertEdge->nextEdge;
            }
            newVert->faces.push_back(face);
        }
        for (auto &face : vB->faces) {
            HE_Edge* vertEdge = face->edge;
            for (int i = 0; i < 3; i++) {
               if (vertEdge->vA == vB) {
                    vertEdge->vA = newVert;
                    break;
               } else if (vertEdge->vB == vA) {
                   vertEdge->vB = newVert;
                   break;
               }
               vertEdge = vertEdge->nextEdge;
            }
            newVert->faces.push_back(face);
        }
        deletedFaces.insert(f);
        deletedFaces.insert(e->oppositeEdges[0]->face);
        DeleteFace(f);
        DeleteFace(e->oppositeEdges[0]->face);
    }
}

void Mesh::RemoveDuplicateVertices(const vector<HE_Vertex*> &vertices)
{
    vector<HE_Vertex*> vs;
    unordered_set<int> deletedIndices;
    if (vertices.size() == 0) {
        vs = GetVertices();
    } else {
        vs = vertices;
    }
    sort(vs.begin(), vs.end(), [](HE_Vertex* a, HE_Vertex* b) { return a->x < b->x; }); 
    int i = 0;
    while (i < vs.size()) {
        HE_Vertex* v = vs[i];
        vector<HE_Vertex*> duplicateVs;
        int idx = -1;
        while (idx + 1 < vs.size()) {
            idx++;
            HE_Vertex* nV = vs[idx];
            if (idx == i || deletedIndices.find(idx) != deletedIndices.end()) { continue; }
            if (HE_Vertex::Distance(*v, *nV) < 0.1e-10) {
                duplicateVs.push_back(nV);
                deletedIndices.insert(idx);
            }
        }
        for (const auto &duplicateV : duplicateVs) {
            for (const auto &f : duplicateV->faces) {
                v->faces.push_back(f);
                HE_Edge* e = f->edge;
                for (int u = 0; u < 3; u++) {
                    if (e->vA == duplicateV) {
                        e->vA = v;
                    } else if (e->vB == duplicateV) {
                        e->vB = v;
                    }
                    e = e->nextEdge;
                }
                DeleteVertex(duplicateV);
            }
        }
        i++;
    }

}
HE_Face* Mesh::GetMostXPositiveVertex()
{
	vector<HE_Vertex*> vertices = GetVertices();
	HE_Vertex* mostX = NULL;
	for (int v = 0; v < vertices.size(); v++) {
		if (mostX == NULL || vertices[v]->x > mostX->x) {
			mostX = vertices[v];
		}
	}
	return mostX->faces[0];
}

HE_Vertex* Mesh::OffsetVertex(HE_Vertex* v, double magnitude)
{
	HE_Vertex* offsetV = Allocate_Vertex();
	for (int i = 0; i < v->faces.size(); i++) {
		*offsetV = *offsetV + v->faces[i]->Normal();
	}
	*offsetV = *v + ((*offsetV)/float(v->faces.size()) * magnitude);
	return offsetV;
}


// void Mesh::Offset(FaceCluster& fc, double offsetAmount)
// {
	// map<string, vector<HE_Vertex*>> vertexOffsets;
	// for (int u = 0; u < fc.faces.size(); u++) {
	// 	HE_Edge* e = fc.faces[u]->edge;
	// 	HE_Vertex* v = e->vA;
	// 	for (int i = 0; i < 3; i++) {
	// 		if (vertexOffsets.find(v->ToString()) == vertexOffsets.end()) {
	// 			vector<HE_Vertex*> vOffset;
	// 			vOffset.push_back(OffsetVertex(v, offsetAmount/2.0));
	// 			vOffset.push_back(OffsetVertex(v, -offsetAmount/2.0));
	// 			vertexOffsets.insert(pair<string, vector<HE_Vertex*>>(v->ToString(), vOffset));
	// 		}
	// 		e = e->nextEdge;
	// 		v = e->vA;
	// 	}
	// }

	// for (int i = 0; i < fc.faces.size(); i++) {
	// 	HE_Face* f = fc.faces[i];
	// 	vector<HE_Vertex*> vAOffset = vertexOffsets[f->edge->vA->ToString()];
	// 	vector<HE_Vertex*> vBOffset = vertexOffsets[f->edge->vB->ToString()];
	// 	vector<HE_Vertex*> vCOffset = vertexOffsets[f->edge->nextEdge->vB->ToString()];

	// 	CreateFace(vAOffset[0], vBOffset[0], vCOffset[0]);
	// 	CreateFace(vAOffset[1], vBOffset[1], vCOffset[1]);

	// 	if (f->edge->IsBoundary() || fc.IsClusterEdge(f->edge)) {
	// 		CreateFace(vAOffset[1], vBOffset[1], vBOffset[0]);
	// 		CreateFace(vAOffset[1], vBOffset[0], vAOffset[0]);
	// 	}
	// 	if (f->edge->nextEdge->IsBoundary() || fc.IsClusterEdge(f->edge->nextEdge)) {
	// 		CreateFace(vBOffset[1], vCOffset[1], vCOffset[0]);
	// 		CreateFace(vBOffset[1], vCOffset[0], vBOffset[0]);
	// 	}
	// 	if (f->edge->prevEdge->IsBoundary() || fc.IsClusterEdge(f->edge->prevEdge)) {
	// 		CreateFace(vCOffset[1], vAOffset[1], vAOffset[0]);
	// 		CreateFace(vCOffset[1], vAOffset[0], vCOffset[0]);
	// 	}
	// }
	// for (int u = 0; u < fc.faces.size(); u++) {
	// 	DeleteFace(fc.faces[u]);
	// }
	// BuildEdgeConnectivity();
// }

void Mesh::PrintStatistics() 
{
	cout << faceReferences.size() << " Faces" << endl;
	cout << vertexReferences.size() << " Vertices" << endl;
	cout << count_if(edgeReferences.begin(), edgeReferences.end(), [](unique_ptr<HE_Edge> &edge) { return edge->IsSingular();})
		 << " singular edges" << endl;
	cout << count_if(edgeReferences.begin(), edgeReferences.end(), [](unique_ptr<HE_Edge> &edge) { return edge->IsBoundary();})
		 << " boundary edges" << endl;
	cout << materials.size() << " Materials" << endl;
}

void Mesh::Cleanup()
{
	for (auto &tc : textureCoordReferences) { tc.reset(); }
	for (auto &v : vertexReferences) { v.reset(); }
	for (auto &e : edgeReferences) { e.reset(); }
	for (auto &f : faceReferences) { f.reset(); }
	// vector<HE_Face*> faces = GetFaces();
	// for (int i = 0; i < faces.size(); i++)
	// {
	// 	DeleteFace(faces[i]);
	// }
}
