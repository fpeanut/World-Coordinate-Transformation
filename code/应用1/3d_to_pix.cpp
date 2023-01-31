#include"3d_to_pix.h"
//camera坐标转屏幕3d坐标
cv::Point3f screenxyz(cv::Point3f cameraxyz, float translation[1][3], float rotation[3][3])
{
	cv::Point3f c;
	c.x = rotation[0][0] * cameraxyz.x + rotation[0][1] * cameraxyz.y + rotation[0][2] * cameraxyz.z + translation[0][0];
	c.y = rotation[1][0] * cameraxyz.x + rotation[1][1] * cameraxyz.y + rotation[1][2] * cameraxyz.z + translation[0][1];
	c.z = rotation[2][0] * cameraxyz.x + rotation[2][1] * cameraxyz.y + rotation[2][2] * cameraxyz.z + translation[0][2];

	return c;
}
//屏幕坐标转像素坐标
cv::Point2f picxy(cv::Point3f screenxyz, float px_width, float px_heigh, float width, float heigh) {
	cv::Point2f a;
	a.x = screenxyz.x*(px_width / width) + px_width / 2;
	a.y = screenxyz.y*(px_heigh / heigh) + px_heigh / 2;
	return a;
}