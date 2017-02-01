#pragma once

#include "Mesh.h"
#include <string>       

	using namespace std;
	class Load
	{
		public:
			Load();
			static Mesh LoadMesh(const std::string &fp);
			static pair<vector<vector<double>>, vector<vector<int>>> LoadPLT(const std::string &fp);
		private:
			static Mesh LoadOBJ(const std::string &fp);
	};
