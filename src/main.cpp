#include "Mesh.h"
#include "Load.h"
#include "Export.h"
#include "AlphaCut.h"
#include "Log.h"



int main (int argc, char *argv[])
{
    Log::ShowDebug();
	if (argc < 2) {
		Log::Info("Incorrect usage:");
		Log::Info("alphacut [filePath]");
		exit(EXIT_FAILURE);
	}
	string fp(argv[1]);
    Log::Debug("Loading mesh {0}", fp);
    Mesh m = Load::LoadMesh(fp);
    Log::Debug("Running AlphaCut");
    AlphaCut::Process(m);
    m.PrintStatistics();
    Log::Debug("Exporting to: {0}", fp.substr(0, fp.find_last_of('.')) + "_cut" + fp.substr(fp.find_last_of('.'), fp.size()));
    Export::ExportMesh(fp.substr(0, fp.find_last_of('.')) + "_cut" + fp.substr(fp.find_last_of('.'), fp.size()), m);
  	exit(0);
}