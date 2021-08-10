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

void DGGrid::checkBound(DGGridPoint& point){
	for(int i = 0; i < 3; ++i){
		if(point.postion[i] < bounds[i].first) {
			bounds[i].first = point.postion[i];
		} else if (point.postion[i] > bounds[i].second) {
			bounds[i].second = point.postion[i];
		}
	}
	return;
}

void DGGrid::addPoint(DGGridPoint& point){
	checkBound(point);
	grid_points.emplace_back(point);
}

void DGGrid::transForm(tTrans& itrans){
	trans = itrans * trans;
	for(auto draw_point: draw_points){
		draw_point = itrans * draw_point;
	}
	return;
}

void DGGrid::initGraph() {
	initPoints();
	initDrawPoints();
}

void DGGrid::initPoints(){
	for(auto grid_point : grid_points) {
		points.emplace_back(
				grid_point.postion[0] * controler.scale,
				grid_point.postion[1] * controler.scale,
				grid_point.postion[2] * controler.scale);
	}
}

void DGGrid::initDrawPoints(){
	if (!trans.isApprox(tTrans::Identity())) {
		for (auto point: points) {
			draw_points.emplace_back(trans * point);
		}
	} else {
		for (auto point: points) {
			draw_points.emplace_back(trans * point);
		}	
	}
}

int DGGrid::size(){
	return grid_points.size();
}
};
