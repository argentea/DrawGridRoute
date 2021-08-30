#include "transForm.h"

namespace DG{


DGControler controler(50);

tTrans DGGrid::defaultTrans = tTrans({{0.7331748,0.3557667,0.5795556},{0.3557667,0.5256447,-0.7727404},{-0.5795556,0.7727404,0.2588195}});
DGGrid::DGGrid(int size){
	cout << "init by size\n";
	grid_points.reserve(size);
	points.reserve(size);
	draw_points.reserve(size);
	bounds.fill(make_pair(0,0));
	//TODO Check this
	trans = defaultTrans;
	return;
}

DGGrid::DGGrid(vector<array<int, 3>> &raw_data){
	cout << "init by vector<array<int,3>>\n";
	int size = raw_data.size();
	grid_points.assign(raw_data.begin(), raw_data.end());
	points.reserve(size);
	draw_points.reserve(size);
	bounds.fill(make_pair(0,0));
	//TODO Check this
	trans = defaultTrans;
	return;
}

void DGGrid::checkBound(DGGridPoint& point){
	for(int i = 0; i < 3; ++i){
		if(point.position[i] < bounds[i].first) {
			bounds[i].first = point.position[i];
		} else if (point.position[i] > bounds[i].second) {
			bounds[i].second = point.position[i];
		}
	}
	return;
}

void DGGrid::addPoint(DGGridPoint& point){
	checkBound(point);
	grid_points.emplace_back(point);
}

void DGGrid::addPoint(array<int, 3> &raw_data){
	int id = grid_points.size();
	grid_points.emplace_back(id, raw_data);
}


void DGGrid::transForm(tTrans& itrans){
	trans = itrans * trans;
	int size = points.size();
	for (int i = 0; i < size; i++) {
		points[i] = itrans * points[i];
		draw_points[i] = points[i].block(0, 0, 2, 1);
	}
	return;
}

void DGGrid::initGraph() {
	initPoints();
	initDrawPoints();
}

void DGGrid::initPoints(){
	cout << "init point\n";
	vector<int> tmppoint(3);
	vector<int> offset(3);
	for(int i = 0; i < 3; i++){
		offset[i] = (bounds[i].first + bounds[i].second)/2;
	}
	for(auto grid_point : grid_points) {
		for(int i = 0; i < 3; i++){
			tmppoint[i] = grid_point.position[i] - offset[i];
		}
		points.emplace_back(controler.scale *trans* tPosd(tmppoint[0], tmppoint[1], tmppoint[2]));
	}
}

void DGGrid::initDrawPoints(){
	cout << "init draw point\n";
	for (auto point: points) {
		draw_points.emplace_back(point.block(0, 0, 2, 1));
	}	
}

int DGGrid::size(){
	return grid_points.size();
}
};
