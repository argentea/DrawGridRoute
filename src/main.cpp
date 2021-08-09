#include <blend2d.h>

int main(int argc, char* argv[]) {

	BLImage img(480, 480, BL_FORMAT_PRGB32);

	// Attach a rendering context into `img`.
	BLContext ctx(img);

	// Clear the image.
	ctx.setCompOp(BL_COMP_OP_SRC_COPY);
	ctx.fillAll();

	BLMatrix2D matrix;
	matrix.resetToSkewing(0.5, 0.5);

	BLPath path;
	BLPoint point[20];
	double movev[2] = {1, -1};
	double movep[2] = {1, 2};
	point[0] = BLPoint(20, 20);
	path.moveTo(point[0]);
	for (int i = 0; i < 20; i++){
		point[i] = BLPoint(point[i-1].x+movep[i%2], point[i-1].y+movev[i%2]);
		path.moveTo(point[i]);
	}

	ctx.setCompOp(BL_COMP_OP_SRC_COPY);
	ctx.setFillStyle(BLRgba32(0xFFFFFFFF));
	ctx.fillPath(path);
	ctx.end();

	// Let's use some built-in codecs provided by Blend2D.
	BLImageCodec codec;
	codec.findByName("BMP");
	img.writeToFile("bl-getting-started-1.bmp", codec);

	return 0;
}
