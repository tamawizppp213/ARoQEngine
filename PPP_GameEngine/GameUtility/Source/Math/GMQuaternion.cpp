#include "GameUtility/Include/Math/GMTransform.hpp"

using namespace gm;

// d‚ß
Quaternion gm::GetAffineTransformRotation(const gm::Matrix3& matrix)
{
	Scalar trace = matrix.GetElement(0, 0) + matrix.GetElement(1, 1) + matrix.GetElement(2, 2);

	Vector4 temp;
	if (trace > 0.0f)
	{
		float s = Sqrt(trace + 1.0f);
		temp.SetW(s * 0.5f);
		s = 0.5f / s;

		temp.SetX((matrix.GetElement(2, 1) - matrix.GetElement(1, 2)) * s);
		temp.SetY((matrix.GetElement(0, 2) - matrix.GetElement(2, 0)) * s);
		temp.SetZ((matrix.GetElement(1, 0) - matrix.GetElement(0, 1)) * s);
	}
	else
	{
		int i = (float)matrix.GetElement(0, 0) < matrix.GetElement(1, 1) ? ((float)matrix.GetElement(1, 1) < matrix.GetElement(2, 2) ? 2 : 1) : ((float)matrix.GetElement(0, 0) < matrix.GetElement(2, 2) ? 2 : 0);
		int j = (i + 1) % 3;
		int k = (i + 2) % 3;

		Scalar s = Sqrt(matrix.GetElement(i, i) - matrix.GetElement(j, j) - matrix.GetElement(k, k) + Scalar(1.0f));
		temp.SetElement(i, s * Scalar(0.5f));
		s = Scalar(0.5f) / s;

		temp.SetW((matrix.GetElement(k, j) - matrix.GetElement(j, k)) * s);
		temp.SetElement(j, (matrix.GetElement(j, i) + matrix.GetElement(i, j)) * s);
		temp.SetElement(k, (matrix.GetElement(k, i) + matrix.GetElement(i, k)) * s);
	}

	return Quaternion(temp);
}