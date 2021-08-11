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
	array<int, 3> position;
	string name;

	DGGridPoint(int id, int i, int j, int k, string name = "")
		:id(id), position({i, j ,k}), name(name) {}
	DGGridPoint(int id, array<int, 3> &pos, string name = "")
		:id(id), name(name) {
			copy(pos.begin(), pos.end(), position.begin());
	}
	DGGridPoint(array<int, 3> &pos, string name = "")
		:name(name) {
			copy(pos.begin(), pos.end(), position.begin());
	}
	void reset(int iid, int ii, int ij, int ik){
		id = iid;
		position[0] = ii;
		position[1] = ij;
		position[2] = ik;
	}
	void reset(int iid, array<int, 3> pos){
		id = iid;
		copy(pos.begin(), pos.end(), position.begin());
	}
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
	void initPoints();
	void initDrawPoints();
	//grid_points init at when call addPoint or at when class construct

public:
	DGGrid(int size);
	DGGrid(vector<array<int, 3>> &raw_data);
	int size();
	void addPoint(DGGridPoint& point);
	void addPoint(array<int, 3> &raw_data);
	void transForm(tTrans& trans);
	//Must be after grid_points init
	void initGraph();
};
};

