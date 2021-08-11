#include "transForm.h"

namespace DG{

DGControler controler(1);

DGGrid::DGGrid(int size){
	grid_points.reserve(size);
	points.reserve(size);
	draw_points.reserve(size);
	bounds.fill(make_pair(0,0));
	//TODO Check this
	trans = tTrans::Identity();
	return;
}

DGGrid::DGGrid(vector<array<int, 3>> &raw_data){
	int size = raw_data.size();
	grid_points.assign(raw_data.begin(), raw_data.end());
	points.reserve(size);
	draw_points.reserve(size);
	bounds.fill(make_pair(0,0));
	//TODO Check this
	trans = tTrans::Identity();
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
	for(auto grid_point : grid_points) {
		points.emplace_back(grid_point.position[0],
				grid_point.position[1],
				grid_point.position[2]);
	}
	if (!trans.isApprox(tTrans::Identity())) {
		for(auto point : points) {
			point = trans * point;
		}
	}
}

void DGGrid::initDrawPoints(){
	for (auto point: points) {
		draw_points.emplace_back(point.block(0, 0, 2, 1));
	}	
}

int DGGrid::size(){
	return grid_points.size();
}
};
