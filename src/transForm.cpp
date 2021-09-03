#include "transForm.h"

namespace DG {


DGControler controler(100);

//tTrans DGGrid::defaultTrans = tTrans({{0.7331748,0.3557667,0.5795556},{0.3557667,0.5256447,-0.7727404},{-0.5795556,0.7727404,0.2588195}});
//tTrans DGGrid::defaultTrans = tTrans::Identity();
tTrans DGGrid::defaultTrans(AngleAxisd(-0.16*M_PI, Vector3d::UnitZ()) *AngleAxisd(0.08*M_PI, Vector3d::UnitX()) * AngleAxisd(0.08*M_PI, Vector3d::UnitY()));
DGGrid::DGGrid(int size) {
    cout << "init by size\n";
    grid_points.reserve(size);
    points.reserve(size);
    draw_points.reserve(size);
    trans = defaultTrans;
    return;
}

DGGrid::DGGrid(vector<array<int, 3>> &raw_data) {
    cout << "init by vector<array<int,3>>\n";
    int size = raw_data.size();
    grid_points.assign(raw_data.begin(), raw_data.end());
    points.reserve(size);
    draw_points.reserve(size);
    trans = defaultTrans;

    return;
}


void DGGrid::addPoint(DGGridPoint& point) {
    grid_points.emplace_back(point);
}

void DGGrid::addPoint(array<int, 3> &raw_data) {
    int id = grid_points.size();
    grid_points.emplace_back(id, raw_data);
}


void DGGrid::transForm(tTrans& itrans) {
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

void DGGrid::initPoints() {
    cout << "init point\n";
    vector<int> tmppoint(3);
    for(auto grid_point : grid_points) {
        for(int i = 0; i < 3; i++) {
            tmppoint[i] = grid_point.position[i];
        }
        tPosd pos(controler.scale *trans* tPosd(tmppoint[1], tmppoint[0], tmppoint[2]));
        for (int i = 0; i < 3; i++) {
            if (pos[i] < lower_bound[i]) {
                lower_bound[i] = pos[i];
            }
            if (pos[i] > upper_bound[i]) {
                upper_bound[i] = pos[i];
            }
        }
        points.emplace_back(pos);
    }
    return;
}

void DGGrid::initDrawPoints() {
    cout << "init draw point\n";
    for (auto point: points) {
        draw_points.emplace_back((point-lower_bound).block(0, 0, 2, 1));
    }
}

int DGGrid::size() {
    return grid_points.size();
}
};
