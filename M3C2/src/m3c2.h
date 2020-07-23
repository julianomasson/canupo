#pragma once
#include <vector>

namespace M3C2
{
	//Used to define the reconstruction options
	struct Options
	{
		std::vector<double> scales;
		double cylinder_base = -1;
		double cylinder_length = -1;
		std::string result_filename;
	};

	struct PointXYZ
	{
		PointXYZ() {};
		PointXYZ(double x, double y, double z) : x(x), y(y), z(z) {};
		double x;
		double y;
		double z;
	};

	struct Cloud
	{
		std::vector<PointXYZ> points;
	};

	class M3C2
	{
	public:
		M3C2(void) {};
		bool compute(const Cloud& p1, const Cloud& p1Reduced,
			const Cloud& p2, const Cloud& p2Reduced,
			const Cloud& cores, const Cloud& extpts, const Options& options);
	};
}