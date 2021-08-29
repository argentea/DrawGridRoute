#include "Drawer.h"
#include <iostream>

namespace DG{

void DGDrawer::initGraph(){
	//init draw point
	int point_num = grid.size();
	dpoints.reserve(point_num);
	for (int i = 0; i < point_num; ++i) {
		dpoints.emplace_back(grid.draw_points[i][0],grid.draw_points[i][1]);
	}

	//init draw conn;
	int conns_num = conns.lines_num();
	dconns.resize(conns_num);
	for (int i = 0; i < conns_num; ++i) {
		dconns[i].moveTo(dpoints[*conns.data[i].begin()]);
		for (auto conn_point_index: conns.data[i]) {
			if (conn_point_index == -1) {
				continue;
			}
			dconns[i].lineTo(dpoints[conn_point_index]);
		}
	}

	//init draw solu
	int sols_num = solus.size();
	dsolus.resize(sols_num);
	for (int i = 0; i < sols_num; ++i) {
		dsolus[i].moveTo(dpoints[*solus[i].sol.begin()]);
		for (auto sol_point_index: solus[i].sol) {
			dsolus[i].lineTo(dpoints[sol_point_index]);
		}
	}

	//init image and context
	if (img.create(1000, 1000, BL_FORMAT_PRGB32) != BL_SUCCESS) {
		cerr << "create image fail, please check image size or format\n";
	}
	ctx.begin(img);
	if (!ctx){
		cerr << "bind img to context fail\n";
		exit(1);
	}
	ctx.setCompOp(BL_COMP_OP_SRC_COPY);
	ctx.fillAll();
}

//Do not use it
DGConnections::DGConnections(vector<array<int, 6>>& raw_conns){
	int layerNum = 0;
	int trackNum = 0;
	int viaNum = 0;
	int lineNum = 0;
	int vertexNum = raw_conns.size();
	vector<vector<bool>> accessFlag(vertexNum, vector<bool>(6, false));
	for (int vertexId = 0; vertexId < vertexNum; ++vertexNum){
		int current = vertexId;
		for (int dimId = 0; dimId < 3; ++dimId) {
			//three dim, every have 2 directions,
			//forward, backward, up, down, left, right
			int direction = dimId*2;

			if (accessFlag[current][direction]) {
				continue;
			}
			if (dimId == 0||dimId == 2) {
				data_attribute[lineNum].layerIdx = layerNum;
				data_attribute[lineNum].trackIdx = trackNum;
				data_attribute[lineNum].viaIdx = -1;
				trackNum++;
			}
			else if (dimId == 1) {
				data_attribute[lineNum].layerIdx = -1;
				data_attribute[lineNum].trackIdx = -1;
				data_attribute[lineNum].viaIdx = viaNum;
				viaNum++;
			}
			else {
				cerr << "unexpected dimId\n";
			}
			accessFlag[current][direction] = true;
			int tmp = current;
			data[lineNum].push_back(tmp);
			while(raw_conns[tmp][direction] != -1){
				tmp = raw_conns[tmp][direction];
				data[lineNum].push_back(tmp);
				accessFlag[tmp][direction] = true;
			}

			direction = dimId*2 + 1;
			accessFlag[current][direction] = true;
			tmp = current;
			while(raw_conns[tmp][direction] != -1){
				tmp = raw_conns[tmp][direction];
				data[lineNum].push_front(tmp);
				accessFlag[tmp][direction] = true;
			}

		}
	}
}

void DGDrawer::drawGraph(){

	//set color
	ctx.setStrokeStyle(BLRgba32(0xFFFF8000));
	ctx.setStrokeWidth(2);
	//draw grid
	for(auto path: dconns){
		ctx.strokePath(path);
	}
	for(auto path: dsolus){
		ctx.strokePath(path);
	}
	ctx.end();
}

void DGDrawer::printBMP(){
	codec.findByName("BMP");
	img.writeToFile("good.bmp", codec);
}
};