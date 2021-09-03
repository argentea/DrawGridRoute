#pragma once
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <string>
#include <vector>
#include <array>
#include "iostream"

using namespace Eigen;
using namespace std;
namespace DG {
typedef Matrix<double, 3, 3> tTrans;
typedef Matrix<double, 3, 1> tPosd;
typedef Matrix<double, 2, 1> tDrawPosd;
class DGGrid;
class DGControler {
private:
public:
    double scale;
    DGControler() {
        scale = 20;
    }
    DGControler(double iscale):scale(iscale) {
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
        :id(id), position({i, j,k}), name(name) {}
    DGGridPoint(int id, array<int, 3> &pos, string name = "")
        :id(id), name(name) {
        copy(pos.begin(), pos.end(), position.begin());
    }
    DGGridPoint(array<int, 3> &pos, string name = "")
        :name(name) {
        copy(pos.begin(), pos.end(), position.begin());
    }
    void reset(int iid, int ii, int ij, int ik) {
        id = iid;
        position[0] = ii;
        position[1] = ij;
        position[2] = ik;
    }
    void reset(int iid, array<int, 3> pos) {
        id = iid;
        copy(pos.begin(), pos.end(), position.begin());
    }
};

class DGGrid {
private:
    //raw data, layer, x,y
    vector<DGGridPoint> grid_points;
    vector<tPosd> points;
    tPosd lower_bound;
    tPosd upper_bound;
    vector<tDrawPosd> draw_points;
    tTrans trans;
    static tTrans defaultTrans;

    //helper
    void initPoints();
    void initDrawPoints();
    //grid_points init at when call addPoint or at when class construct

public:
    DGGrid() {
        cout << "init by vector<array<int,3>>\n";
        trans = defaultTrans;
    }
    DGGrid(int size);
    DGGrid(vector<array<int, 3>> &raw_data);
    int size();
    int picSizex() {
        return upper_bound[0] - lower_bound[0];
    }
    int picSizey() {
        return upper_bound[1] - lower_bound[1];
    }
    void addPoint(DGGridPoint& point);
    void addPoint(array<int, 3> &raw_data);
    void transForm(tTrans& trans);
    //Must be after grid_points init
    void initGraph();

    void printGPoints() {
        cout << "printGPoint\n";
        int index = 0;
        for (auto pos: grid_points) {
            printf("r %d:x: %d, y: %d, z: %d\n", index, pos.position[0], pos.position[1], pos.position[2]);
            index++;
        }
        cout << "printGPoint end\n";
    }
    void printPoints() {
        cout << "printPoint\n";
        int index = 0;
        for (auto dp: points) {
            printf("d %d:x: %f, y: %f, z: %f\n ", index, dp[0], dp[1], dp[2]);
            index++;
        }
        cout << "printPoint end\n";
    }

    void printDpoints() {
        cout << "printDPoint\n";
        int index = 0;
        for (auto dp: draw_points) {
            printf("d %d:x: %f, y: %f\n ", index, dp[0], dp[1]);
            index++;
        }
        cout << "printDPoint end\n";
    }

    friend class DGDrawer;
};
};
