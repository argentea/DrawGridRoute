#include <Eigen/Dense>
#include <string>
#include <vector>
#include <array>

using namespace Eigen;
using namespace std;
namespace DG{
typedef Matrix<double, 3, 3> tTrans;
typedef Matrix<double, 3, 1> tPosd;
typedef Matrix<double, 2, 1> tDrawPosd;

class DGControler {
private:
public:
	double scale;
	
	DGControler(double iscale = 1):scale(iscale){
		return;
	}
};

extern  DGControler controler;

class DGGridPoint {
private:
public:
	int id;
	array<int, 3> postion;
	string name;

	DGGridPoint(int id, int i, int j, int k, string name = "")
		:id(id), postion({i, j ,k}), name(name) {}
};

class DGGrid {
private:
	vector<DGGridPoint> grid_points;
	vector<tPosd> points;
	vector<tDrawPosd> draw_points;
	array<pair<int, int>, 6> bounds;
	tTrans trans;

	//helper
	void checkBound(DGGridPoint& point);
	void initDrawPoints();
	void initPoints();

public:
	DGGrid(int size);
	int size();
	void addPoint(DGGridPoint& point);
	void transForm(tTrans& trans);
	//Must be after addPoint
	void initGraph();
};
};

