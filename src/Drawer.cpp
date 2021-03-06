#include "Drawer.h"
#include <iostream>

namespace DG {

vector<BLRgba32>  DGDrawer::conns_color_set;
vector<BLRgba32>  DGDrawer::solus_color_set;


void DGDrawer::initGraph() {
    //init draw point
    int point_num = grid.size();
    dpoints.reserve(point_num);
    for (int i = 0; i < point_num; ++i) {
        dpoints.emplace_back(grid.draw_points[i][0],grid.draw_points[i][1]);
    }

    //init draw conn;
    int conns_num = conns.lines_num();
    dconns.resize(conns_num);
    dconns_color.resize(conns_num);
    for (int i = 0; i < conns_num; ++i) {
        dconns_color[i] = conns.data_attribute[i].layerIdx;
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
        auto cur = solus[i].begin();
        dsolus[i].moveTo(dpoints[*cur]);
        while(cur != solus[i].end()) {
            dsolus[i].lineTo(dpoints[*cur]);
            cur++;
        }
    }

    //init pins
    int pin_num = pinset.size();
    dpins.resize(pin_num);
    for (int i = 0; i < pin_num; ++i) {
        dpins[i].reserve(pinset[i].size());
        for (auto point: pinset[i]) {
            dpins[i].emplace_back(BLCircle(dpoints[point].x, dpoints[point].y, 5));
        }
    }

    //init image and context
    if (img.create(grid.picSizex(), grid.picSizey(), BL_FORMAT_PRGB32) != BL_SUCCESS) {
    }
    ctx.begin(img);
    if (!ctx) {
        cerr << "bind img to context fail\n";
		cerr << "img size:" << grid.picSizex() << "x" << grid.picSizey() << endl;
        exit(1);
    }
    ctx.setCompOp(BL_COMP_OP_SRC_COPY);
    ctx.fillAll();
}

//Do not use it
DGConnections::DGConnections(vector<array<int, 6>>& raw_conns) {
    int layerNum = 0;
    int trackNum = 0;
    int viaNum = 0;
    int lineNum = 0;
    int vertexNum = raw_conns.size();
    vector<vector<bool>> accessFlag(vertexNum, vector<bool>(6, false));
    for (int vertexId = 0; vertexId < vertexNum; ++vertexNum) {
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
            while(raw_conns[tmp][direction] != -1) {
                tmp = raw_conns[tmp][direction];
                data[lineNum].push_back(tmp);
                accessFlag[tmp][direction] = true;
            }

            direction = dimId*2 + 1;
            accessFlag[current][direction] = true;
            tmp = current;
            while(raw_conns[tmp][direction] != -1) {
                tmp = raw_conns[tmp][direction];
                data[lineNum].push_front(tmp);
                accessFlag[tmp][direction] = true;
            }

        }
    }
}

void DGDrawer::drawGraph() {

    //set width
    ctx.setStrokeWidth(2);
    ctx.setFillStyle(BLRgba32(0xFFFFFFFF));
    //draw grid
    int conns_size = dconns.size();
    int color_set_size = conns_color_set.size();
    for (int i = 0; i < conns_size; ++i) {
        int colorid = dconns_color[i];
        if(colorid == -1) {
            ctx.setStrokeStyle(BLRgba32(0xFFFF8000));
            ctx.strokePath(dconns[i]);
            continue;
        }
        ctx.setStrokeStyle(conns_color_set[colorid%color_set_size]);
        ctx.strokePath(dconns[i]);
    }

    //draw solus
    ctx.setStrokeWidth(4);
    int solus_size = dsolus.size();
    for(int i = 0; i < solus_size; i++) {
        ctx.setStrokeStyle(solus_color_set[i%color_set_size]);
        ctx.strokePath(dsolus[i]);
    }


    //draw pins
    int pin_num = pinset.size();
    for(int i = 0; i < pin_num; i++) {
        for (auto dp: dpins[i]) {
            ctx.fillCircle(dp);
        }
    }
    ctx.end();
}

void DGDrawer::printBMP() {
    codec.findByName("BMP");
    if (img.writeToFile((fileName+".bmp").c_str(), codec) != BL_SUCCESS) {
        cerr << "write fail" << endl;
    }
}
};
