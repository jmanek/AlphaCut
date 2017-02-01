#pragma once

	class Face 
	{
		public:
			Face();
			Face(unsigned int vA, unsigned int vB, unsigned int vC, unsigned int vtA, unsigned int vtB, unsigned int vtC, unsigned int materialIndex);
			Face(unsigned int vA, unsigned int vB, unsigned int vC);
			unsigned int vA;
			unsigned int vB;
			unsigned int vC;
			unsigned int vtA;
			unsigned int vtB;
			unsigned int vtC;
			int materialIndex;
	};
