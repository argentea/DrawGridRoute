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
	if (argc < 2){
		cerr << "need file name" << endl;
		return 1;
	}
	auto start = chrono::steady_clock::now();
	chrono::duration<double> time_use;
	size_t length;
	auto f = map_file(argv[1], length);
	auto l = f + length;

	int numVertex = 0;
	vector<array<int, 3>> grid_data;
	vector<vector<int>> conn_data;
	vector<int> conn_attri;
	vector<list<int>> sol_data;
	sol_data.push_back({471, 470, 469, 468, 1180, 1181, 1182,
			1183, 1184, 2005, 2004, 2003, 2002, 2001, 2000,
			1999, 1998, 1997, 1996, 1995, 1994, 1990, 1982,
			1981, 1980, 1979, 1978, 1977, 1976, 1975, 1974,
			1973, 1972, 1971, 793, 794, 209});
	vector<vector<int>> highlight_vertex;
	highlight_vertex.push_back({208, 225, 242, 209, 226, 243,
			260, 210, 227, 244});
	highlight_vertex.push_back({447, 448, 449, 450, 451, 452,
			453, 454, 455, 456, 471, 472, 473, 474, 475, 476,
			477, 478, 479, 480, 354, 331, 355, 356, 357, 358,
			359, 360, 379, 332, 380, 333, 381, 334, 382, 335,
			383, 407, 431, 336, 384, 408, 432});
	float nodeCost;
	int nodeuId, layerIdu, layerIdv, trackId, crossId, dir;

	const char format1[] = "n%d: %f, layer:%d, track:%d, cross:%d, dir:%d\n";
	const char format2[] = "%d: %f , layer:%d, track:%d\n";

	float connCost;
	int nodevId;

	numVertex = 0;
	for (;(f = static_cast<const char*>(memchr(f, 'n', l-f))) && f!=l ; ++f) {
		sscanf(f,format1,
				&nodeuId, &nodeCost, &layerIdu, &trackId, &crossId, &dir);
		if(dir) {
			grid_data.push_back({layerIdu, trackId, crossId});
		}else {
			grid_data.push_back({layerIdu, crossId, trackId});
		}

		for(int i = 0; i < 6; i++){
			f = static_cast<const char*>(memchr(f, '\n', l-f));
			f++;
			sscanf(f, format2, &nodevId, &connCost, &layerIdv, &trackId);
			conn_data.push_back({nodeuId, nodevId});
			if (layerIdu == layerIdv)
				conn_attri.push_back(layerIdv);
			else { 
				conn_attri.push_back(-1);
			}
		}
		numVertex++;

	}

	auto data_init_end = chrono::steady_clock::now();
	chrono::duration<double> time_data_read = data_init_end - start;
	cout << "main construct drawer\n";


	DG::DGDrawer drawer(grid_data, conn_data, sol_data, conn_attri, highlight_vertex);
	auto drawer_init = chrono::steady_clock::now();
	chrono::duration<double> time_draw_init = drawer_init - data_init_end;
	cout << "main construct drawer end\n";


	drawer.initGraph();
	auto graph_init_end = chrono::steady_clock::now();
	chrono::duration<double> time_graph_init = graph_init_end - drawer_init;

	drawer.drawGraph();
	auto draw_graph_end = chrono::steady_clock::now();
	chrono::duration<double> time_draw_graph = draw_graph_end - graph_init_end;

	
	drawer.printBMP();
	auto print_graph_end = chrono::steady_clock::now();
	chrono::duration<double> time_print_graph = print_graph_end - draw_graph_end;


	chrono::duration<double> time_total = print_graph_end - start;
	cout << endl << "**************time use******************\n";
	cout << "total time: " << time_total.count() << "s, \n";
	cout << "reading data time: " << time_data_read.count() << "s, " << time_data_read.count()/time_total.count()*100 << "%\n";
	cout << "drawer construct time: " << time_draw_init.count() << "s, " << time_draw_init.count()/time_total.count()*100 << "%\n";
	cout << "graph init time: " << time_graph_init.count() << "s, " << time_graph_init.count()/time_total.count()*100 << "%\n";
	cout << "draw graph time: " << time_draw_graph.count() << "s, " << time_draw_graph.count()/time_total.count()*100 << "%\n";
	cout << "print graph time: " << time_print_graph.count() << "s, " << time_print_graph.count()/time_total.count()*100 << "%\n";
	return 0;
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

