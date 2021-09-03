#pragma once
#include <blend2d.h>
#include <transForm.h>
#include <vector>
#include <list>
#include <iterator>

namespace DG{
typedef list<int> DGSolution;
class DGDrawer;

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
DGLineAtrri(int layer_attri){
	if (layer_attri != -1) {
		layerIdx = layer_attri;
	}else {
		viaIdx = 0;
	}
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
	DGConnections(vector<vector<int>>& raw_conns, vector<int>& raw_attri_layer){
		int lineNum = raw_conns.size();
		data.resize(lineNum);
		data_attribute.reserve(lineNum);
		conns_size = 0;
		for (int i = 0; i < lineNum; ++i) {
			copy(raw_conns[i].begin(), raw_conns[i].end(), back_inserter(data[i]));
			data_attribute.emplace_back(raw_attri_layer[i]);
			conns_size += data[i].size();
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
	int x_size;
	int y_size;

	//picture item
	bool ready_to_draw = false;
	BLImage img;
	BLContext ctx;
	vector<BLPoint> dpoints;
	vector<BLPath> dconns;
	vector<int> dconns_color;
	vector<BLPath> dsolus;
	vector<vector<int>> pinset;
	vector<vector<BLCircle>> dpins;

	//color item
	static vector<BLRgba32> conns_color_set;
	static vector<BLRgba32> solus_color_set;

	//image codec
	BLImageCodec codec;

	static void connsColorInit(){
		vector<long> conns_color({0xA52A2AFF, 0xCD853FFF});
		for (auto col: conns_color){
			conns_color_set.emplace_back(col);
		}
	}
	static void solusColorInit(){
		vector<long> solus_color({0xF0F8FFFF, 0x8A2BE2FF, 0x5F9EA0FF, 0x98F5FFFF,
			0x8EE5EEFF, 0x7AC5CDFF, 0x53868BFF, 0x6495EDFF,
			0x00008BFF, 0x008B8BFF, 0x483D8BFF, 0x00CED1FF,
			0x00BFFFFF, 0x00BFFFFF, 0x00B2EEFF, 0x009ACDFF,
			0x00688BFF, 0x1E90FFFF, 0x1E90FFFF, 0x1C86EEFF});
		for (auto col: solus_color){
			solus_color_set.emplace_back(col);
		}
	}

public:
	DGDrawer(vector<array<int, 3>> &raw_grid,
			vector<vector<int>> &raw_conns, 
			vector<list<int>> &raw_sols,
			vector<int> &raw_conns_attrib,
			vector<vector<int>> &highlight_vertex):grid(raw_grid), conns(raw_conns, raw_conns_attrib), pinset(highlight_vertex){
		//grid, conns, solus init
		cout << "init DGDrawer\n";
		if (conns_color_set.empty()){
			connsColorInit();
		}
		if (solus_color_set.empty()){
			solusColorInit();
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
			auto tmp = solu.begin();
			while(tmp != solu.end()){
				cout << *tmp << "-> ";
				tmp++;
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
