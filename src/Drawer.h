#pragma once
#include <blend2d.h>
#include <transForm.h>
#include <vector>
#include <list>
#include <iterator>

namespace DG{
class DGDrawer;

class DGSolution {
public:
	int id;
	BLRgba32 color;
	list<int> sol;
	DGSolution(list<int> &raw_sol , int raw_id = 0){
		copy(raw_sol.begin(), raw_sol.end(), sol.begin());
		id = raw_id;
	}

};

class DGLineAtrri {
public:
	int layerIdx = -1;
	int trackIdx = -1;
	int viaIdx = -1;
DGLineAtrri(int layerId, int trackId, int viaId)
	:layerIdx(layerId), trackIdx(trackId), viaIdx(viaId){
}
DGLineAtrri(vector<int> attri)
	:layerIdx(attri[0]), trackIdx(attri[1]), viaIdx(attri[2]){
}

};

class DGConnections{
public:
	//To draw attention to, data is not a adjacency list.
	//data is a vector of grid line. The index of data is the line
	//id. The points in data[i] are connected sequentially.
	vector<list<int>> data;
	vector<DGLineAtrri> data_attribute;
	int conns_size;
	int lines_num(){
		return data.size();
	}
	int total_lines_size(){
		return conns_size;
	}
	DGConnections(){
		conns_size = 0;
	}
	DGConnections(vector<vector<int>>& raw_conns){
		int lineNum = raw_conns.size();
		data.resize(lineNum);
		for (int i = 0; i < lineNum; ++i) {
			copy(raw_conns[i].begin(), raw_conns[i].end(), back_inserter(data[i]));
		}
		conns_size = 0;
		for(auto conn: data){
			conns_size += conn.size();
		}
	}
	//suggest to use that
	DGConnections(vector<vector<int>>& raw_conns, vector<vector<int>> raw_attri){
		int lineNum = raw_conns.size();
		data.resize(lineNum);
		data_attribute.reserve(lineNum);
		for (int i = 0; i < lineNum; ++i) {
			copy(raw_conns[i].begin(), raw_conns[i].end(), back_inserter(data[i]));
			data_attribute.emplace_back(raw_attri[i]);
		}
		conns_size = 0;
		for(auto conn: data){
			conns_size += conn.size();
		}
	}

	//Do not use it
	DGConnections(vector<array<int, 6>>& raw_conns);

};

class DGDrawer {
private:
	//geo
	DGGrid grid;
	DGConnections conns;
	vector<DGSolution> solus;

	//picture item
	bool ready_to_draw = false;
	BLImage img;
	BLContext ctx;
	vector<BLPoint> dpoints;
	vector<BLPath> dconns;
	vector<BLPath> dsolus;

	//color item
	static vector<BLRgba32> conns_color_set;
	static vector<BLRgba32> solus_color_set;

	//image codec
	BLImageCodec codec;

	static void connsColorInit(){
		vector<int> conns_color({0xF0F8FF, 0x8A2BE2, 0x5F9EA0, 0x98F5FF,
			0x8EE5EE, 0x7AC5CD, 0x53868B, 0x6495ED,
			0x00008B, 0x008B8B, 0x483D8B, 0x00CED1,
			0x00BFFF, 0x00BFFF, 0x00B2EE, 0x009ACD,
			0x00688B, 0x1E90FF, 0x1E90FF, 0x1C86EE,});
		for (auto col: conns_color){
			conns_color_set.emplace_back(col);
		}
	}

public:
	DGDrawer(vector<array<int, 3>> &raw_grid, vector<vector<int>> &raw_conns, vector<list<int>> & raw_sols):grid(raw_grid), conns(raw_conns){
		//grid, conns, solus init
		cout << "init DGDrawer\n";
		if (conns_color_set.empty()){
			connsColorInit();
		}
		grid.initGraph();
		solus.reserve(raw_sols.size());
		for(auto raw_sol: raw_sols){
			solus.emplace_back(raw_sol);
		}
	}
	void initGraph();
	void drawGraph();
	void printBMP();
	void printGrid(){
		//grid.printPoint();
		grid.printPoints();
	}
	void printSolus(){
		cout << "print Solus\n";
		for (auto solu: solus) {
			auto tmp = solu.sol.begin();
			while(tmp != solu.sol.end()){
				cout << *tmp << "-> ";
			}
			cout << endl;
		}
		cout << "print Solus end\n";
	}
	void printConns(){
		cout << "print Conns\n";
		for (auto conn: conns.data) {
			auto tmp = conn.begin();
			cout << *tmp;
			tmp++;
			while(tmp != conn.end()){
				cout << "-> " << *tmp;
				tmp++;
			}
			cout << endl;
		}
		cout << "print Conns end\n";
	}
	void printPoints(){
		cout << "print Points\n";
		for (auto point: dpoints){
			cout << point.x << " " << point.y << endl;
		}
		cout << "print Points end\n";
	}
};

};
