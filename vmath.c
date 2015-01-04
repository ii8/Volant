
#include "vmath.h"

void mlt4_ms(mat4 m, float s)
{
	int _i, _j;
	for(_i=0; _i<4; _i++)
	{
		for(_j=0; _j<4; _j++)
		{
			m[_i][_j] *= s;
		}
	}
}

void mlt4_mv(vec4 v, mat4 m)
{
	vec4 _result;
	int _i, _j;
	for(_i=0; _i<4; _i++)
	{
		_result[_i]=0;
		for(_j=0; _j<4; _j++)
		{
			_result[_i] += m[_i][_j] * v[_j];
		}
	}
	memcpy(v, _result, sizeof(_result));
}

void mlt4_mm(mat4 m, mat4 n)
{
	mat4 _result;
	int _i, _j, _k;
	for(_i=0; _i<4; _i++)
	{
		for(_j=0; _j<4; _j++)
		{
			_result[_i][_j] = 0.0;
			for(_k=0; _k<4; _k++)
			{
				_result[_i][_j] += m[_i][_k] * n[_k][_j];
			}
		}
	}
	memcpy(m, _result, sizeof(_result));
}

void add(vec3 v, vec3 u)
{
	v[0] += u[0];
	v[1] += u[1];
	v[2] += u[2];
}

void sub(vec3 v, vec3 u)
{
	v[0] -= u[0];
	v[1] -= u[1];
	v[2] -= u[2];
}

void normalize(vec3 v)
{
	double len = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	v[0] /= len;
	v[1] /= len;
	v[2] /= len;
}

double dot(vec3 v, vec3 u)
{
	return v[0]*u[0]+v[1]*u[1]+v[2]*u[2];
}

void cross(vec3 result, vec3 v, vec3 u)
{
	result[0] = v[1] * u[2] - v[2] * u[1];
	result[1] = v[2] * u[0] - v[0] * u[2];
	result[2] = v[0] * u[1] - v[1] * u[0];
}


void normal(vec3 normal, vec3 v, vec3 u, vec3 w)
{
	sub(u, v);
	sub(w, v);
	cross(normal, u, w);
}

void identity(mat4 m)
{
	mat4 tmp = IDMAT4;
	memcpy(m, tmp, sizeof(tmp));
}

void translate(mat4 matrix, double x, double y, double z)
{
	//mat4 tmp = IDMAT4;
	matrix[3][0] += x;
	matrix[3][1] += y;
	matrix[3][2] += z;
	//memcpy(matrix, tmp, sizeof(tmp));
}

void scale(mat4 m, double x, double y, double z)
{
	mat4 tmp = IDMAT4;
	tmp[0][0] = x;
	tmp[1][1] = y;
	tmp[2][2] = z;
	mlt4_mm(m, tmp);
}

void inverse(mat4 m)
{
	float sub_factor00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
	float sub_factor01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	float sub_factor02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	float sub_factor03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	float sub_factor04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	float sub_factor05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
	float sub_factor06 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
	float sub_factor07 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
	float sub_factor08 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
	float sub_factor09 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
	float sub_factor10 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
	float sub_factor11 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
	float sub_factor12 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
	float sub_factor13 = m[1][2] * m[2][3] - m[2][2] * m[1][3];
	float sub_factor14 = m[1][1] * m[2][3] - m[2][1] * m[1][3];
	float sub_factor15 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
	float sub_factor16 = m[1][0] * m[2][3] - m[2][0] * m[1][3];
	float sub_factor17 = m[1][0] * m[2][2] - m[2][0] * m[1][2];
	float sub_factor18 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

	mat4 inverse;
	inverse[0][0] = + (m[1][1] * sub_factor00 - m[1][2] * sub_factor01 + m[1][3] * sub_factor02);
	inverse[0][1] = - (m[1][0] * sub_factor00 - m[1][2] * sub_factor03 + m[1][3] * sub_factor04);
	inverse[0][2] = + (m[1][0] * sub_factor01 - m[1][1] * sub_factor03 + m[1][3] * sub_factor05);
	inverse[0][3] = - (m[1][0] * sub_factor02 - m[1][1] * sub_factor04 + m[1][2] * sub_factor05);

	inverse[1][0] = - (m[0][1] * sub_factor00 - m[0][2] * sub_factor01 + m[0][3] * sub_factor02);
	inverse[1][1] = + (m[0][0] * sub_factor00 - m[0][2] * sub_factor03 + m[0][3] * sub_factor04);
	inverse[1][2] = - (m[0][0] * sub_factor01 - m[0][1] * sub_factor03 + m[0][3] * sub_factor05);
	inverse[1][3] = + (m[0][0] * sub_factor02 - m[0][1] * sub_factor04 + m[0][2] * sub_factor05);

	inverse[2][0] = + (m[0][1] * sub_factor06 - m[0][2] * sub_factor07 + m[0][3] * sub_factor08);
	inverse[2][1] = - (m[0][0] * sub_factor06 - m[0][2] * sub_factor09 + m[0][3] * sub_factor10);
	inverse[2][2] = + (m[0][0] * sub_factor11 - m[0][1] * sub_factor09 + m[0][3] * sub_factor12);
	inverse[2][3] = - (m[0][0] * sub_factor08 - m[0][1] * sub_factor10 + m[0][2] * sub_factor12);

	inverse[3][0] = - (m[0][1] * sub_factor13 - m[0][2] * sub_factor14 + m[0][3] * sub_factor15);
	inverse[3][1] = + (m[0][0] * sub_factor13 - m[0][2] * sub_factor16 + m[0][3] * sub_factor17);
	inverse[3][2] = - (m[0][0] * sub_factor14 - m[0][1] * sub_factor16 + m[0][3] * sub_factor18);
	inverse[3][3] = + (m[0][0] * sub_factor15 - m[0][1] * sub_factor17 + m[0][2] * sub_factor18);

	float determinant = 1/
		( m[0][0] * inverse[0][0]
		+ m[0][1] * inverse[0][1]
		+ m[0][2] * inverse[0][2]
		+ m[0][3] * inverse[0][3]);


	mlt4_ms(inverse, determinant);

	memcpy(m, inverse, sizeof(inverse));

}

void rotatex(mat4 matrix, double a)
{
	mat4 tmp = {{1, 0, 0, 0},
				{0, cos(a), -sin(a), 0},
				{0, sin(a), cos(a), 0},
				{0, 0, 0, 1}};
	mlt4_mm(matrix, tmp);
}

void rotatey(mat4 matrix, double a)
{
	mat4 tmp = {{cos(a), 0, sin(a), 0},
				{0, 1, 0, 0},
				{-sin(a), 0, cos(a), 0},
				{0, 0, 0, 1}};
	mlt4_mm(matrix, tmp);
}

void rotatez(mat4 matrix, double a)
{
	mat4 tmp = {{cos(a), -sin(a), 0, 0},
				{sin(a), cos(a), 0, 0},
				{0, 0, 1, 0},
				{0, 0, 0, 1}};
	mlt4_mm(matrix, tmp);
}

void rotate(mat4 matrix, vec3 axis, double angle)
{
	normalize(axis);
	vec3 b = {1,1,1};
	sub(b, axis);
	vec3 c;
	cross(c, axis, b);
	cross(b, c, axis);

	mat4 tmp = {{b[0], b[1], b[2], 0},
				{c[0], c[1], c[2], 0},
				{axis[0], axis[1], axis[2], 0},
				{0, 0, 0, 1}};

	mat4 inv = {{b[0], b[1], b[2], 0},
				{c[0], c[1], c[2], 0},
				{axis[0], axis[1], axis[2], 0},
				{0, 0, 0, 1}};

	mat4 rot;
	//mat4 result;


	inverse(inv);
	rotatez(rot, angle);



	mlt4_mm(matrix, inv);
	mlt4_mm(matrix, rot);
	mlt4_mm(matrix, tmp);


	//mlt4_mm(result, tmp, rotmat);

	//memcpy(matrix, result, sizeof(tmp));
}


void perspective(mat4 matrix, float fov_angle, float aspect, float znear, float zfar)
{
	float tan_half_fov = tan(fov_angle / 2.0f);

	matrix[0][0] = 1 / (aspect * tan_half_fov);
	matrix[1][1] = 1 / tan_half_fov;
	matrix[2][2] = -(zfar + znear) / (zfar - znear);
	matrix[2][3] = -1;
	matrix[3][2] = -(2 * zfar * znear) / (zfar - znear);
}
