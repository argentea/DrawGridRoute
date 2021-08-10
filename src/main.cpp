#include <blend2d.h>
#include <array>
#include <type_traits>
#include <utility>
#include "Eigen/Dense"

using namespace Eigen;

int main(int argc, char* argv[]) {

	BLImage img(1000, 1000, BL_FORMAT_PRGB32);

	// Attach a rendering context into `img`.
	BLContext ctx(img);

	// Clear the image.
	ctx.setCompOp(BL_COMP_OP_SRC_COPY);
	ctx.fillAll();
	double scale = 100;
	double postep = 1*scale;
	double inclinedstep = 0.5*scale;
	std::array<std::pair<double, double> , 6> dir = 
	{
		std::make_pair(inclinedstep, -inclinedstep),
		std::make_pair(-inclinedstep, inclinedstep),
		std::make_pair(-postep, 0),
		std::make_pair(postep, 0),
		std::make_pair(0, -postep),
		std::make_pair(0, postep)
	};

	BLPath path;
	BLPoint startPoint(200, 200);
	BLPoint tmpPoint(startPoint);

	path.moveTo(startPoint);
	tmpPoint.reset(tmpPoint.x + dir[0].first, tmpPoint.y + dir[0].second);
	path.lineTo(500, 200);
	path.lineTo(500, 500);
	path.lineTo(200, 500);
	path.lineTo(200, 200);
	for (int i = 1; i < 1; i++){
		tmpPoint.reset(tmpPoint.x + dir[i%dir.size()].first, tmpPoint.y + dir[i%dir.size()].second);
	}
	BLCircle cir(300, 300, 200);
	BLMatrix2D mat(0.866, 0, -0.25, 0.866, 0.866/2, -0.5);
//	path.addCircle(cir, mat);

	BLPath good;
//	good.addPath(path);
	good.addPath(path, mat);

//	ctx.skew(0.5,-0.5);
	ctx.setCompOp(BL_COMP_OP_SRC_COPY);
	ctx.setStrokeStyle(BLRgba32(0xFFFF8000));
	ctx.setStrokeWidth(2);
	ctx.strokePath(good);

	ctx.end();

	// Let's use some built-in codecs provided by Blend2D.
	BLImageCodec codec;
	codec.findByName("BMP");
	img.writeToFile("bl-getting-started-1.bmp", codec);

	return 0;
}
