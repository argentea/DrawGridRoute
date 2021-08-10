#include <blend2d.h>
#include <transForm.h>
#include <vector>
#include <list>

namespace DG{

class DGSolution {
public:
	int id;
	BLRgba32 color;
	list<int> solutions;

};

class DGConnections{
	vector<vector<int>> conns;
	vector<BLRgba32> color_set;
};

class DGDrawer {

private:
	//geo
	DGGrid& grid;
	DGConnections& conns;
	vector<DGSolution> solus;

	//picture item
	BLImage img;
	BLContext ctx;
	vector<BLPath> psolus;
	vector<BLPath> pconns;

	//color item

};


};
