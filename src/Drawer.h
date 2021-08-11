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
	vector<BLPoint> ppoints;
	vector<BLPath> psolus;
	vector<BLPath> pconns;

	//color item
	vector<BLRgba32> conns_color_set;
	vector<BLRgba32> solus_color_set;

};


};
