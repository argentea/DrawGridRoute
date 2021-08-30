#include "Drawer.h"
#include "transForm.h"
#include "chrono"

using namespace Eigen;

#include <algorithm>
#include <iostream>
#include <cstring>

// for mmap:
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

const char* map_file(const char* fname, size_t& length);

int main(int argc, char* argv[]){
	auto start = chrono::steady_clock::now();
	chrono::duration<double> time_use;
	size_t length;
	auto f = map_file("../log", length);
	auto l = f + length;

	int numVertex = 0;
	vector<array<int, 3>> grid_data;
	vector<vector<int>> conn_data;
	vector<list<int>> sol_data;
	float nodeCost;
	int nodeuId, layerId, trackId, crossId, dir;

	const char format1[] = "n%d: %f, layer:%d, track:%d, cross:%d, dir:%d\n";
	const char format2[] = "%d: %f , layer:%d, track:%d\n";

	float connCost;
	int nodevId;

	numVertex = 0;
	for (;(f = static_cast<const char*>(memchr(f, 'n', l-f))) && f!=l ; ++f) {
		sscanf(f,format1,
				&nodeuId, &nodeCost, &layerId, &trackId, &crossId, &dir);
		if(dir) {
			grid_data.push_back({layerId, trackId, crossId});
		}else {
			grid_data.push_back({layerId, crossId, trackId});
		}
//		printf(format1, nodeuId, nodeCost, layerId, trackId, crossId, dir);

		for(int i = 0; i < 6; i++){
			f = static_cast<const char*>(memchr(f, '\n', l-f));
			f++;
			sscanf(f, format2, &nodevId, &connCost, &layerId, &trackId);
			//printf(format2, nodevId, connCost, layerId, trackId);
			conn_data.push_back({nodeuId, nodevId});
		}
		numVertex++;

	}

	auto data_init_end = chrono::steady_clock::now();
	chrono::duration<double> time_data_read = data_init_end - start;
	cout << "main construct drawer\n";


	DG::DGDrawer drawer(grid_data, conn_data, sol_data);
	auto drawer_init = chrono::steady_clock::now();
	chrono::duration<double> time_draw_init = drawer_init - data_init_end;
	cout << "main construct drawer end\n";


	drawer.initGraph();
	auto graph_init_end = chrono::steady_clock::now();
	chrono::duration<double> time_graph_init = graph_init_end - drawer_init;

	drawer.drawGraph();
	auto draw_graph_end = chrono::steady_clock::now();
	chrono::duration<double> time_draw_graph = draw_graph_end - graph_init_end;

	chrono::duration<double> time_total = draw_graph_end - start;

	cout << endl << "**************time use******************\n";
	cout << "total time: " << time_total.count() << "s, \n";
	cout << "reading data time: " << time_data_read.count() << "s, " << time_data_read.count()/time_total.count()*100 << "%\n";
	cout << "drawer construct time: " << time_draw_init.count() << "s, " << time_draw_init.count()/time_total.count()*100 << "%\n";
	cout << "graph init time: " << time_graph_init.count() << "s, " << time_graph_init.count()/time_total.count()*100 << "%\n";
	cout << "draw graph time: " << time_draw_graph.count() << "s, " << time_draw_graph.count()/time_total.count()*100 << "%\n";


//	std::cout << "numVertex ="  << grid_data.size() << "\n";
}

void handle_error(const char* msg) {
    perror(msg); 
    exit(255);
}

const char* map_file(const char* fname, size_t& length)
{
    int fd = open(fname, O_RDONLY);
    if (fd == -1)
        handle_error("open");

    // obtain file size
    struct stat sb;
    if (fstat(fd, &sb) == -1)
        handle_error("fstat");

    length = sb.st_size;

    const char* addr = static_cast<const char*>(mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0u));
    if (addr == MAP_FAILED)
        handle_error("mmap");

    // TODO close fd at some point in time, call munmap(...)
    return addr;
}

