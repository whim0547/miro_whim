#include "Triangle.h"
#include "TriangleMesh.h"
#include "Ray.h"
#include <algorithm>


Triangle::Triangle(TriangleMesh * m, unsigned int i) :
	m_mesh(m), m_index(i)
{
}


Triangle::~Triangle()
{
}


void
Triangle::renderGL()
{
	TriangleMesh::TupleI3 ti3 = m_mesh->vIndices()[m_index];
	const Vector3 & v0 = m_mesh->vertices()[ti3.x]; //vertex a of triangle
	const Vector3 & v1 = m_mesh->vertices()[ti3.y]; //vertex b of triangle
	const Vector3 & v2 = m_mesh->vertices()[ti3.z]; //vertex c of triangle

	glBegin(GL_TRIANGLES);
		glVertex3f(v0.x, v0.y, v0.z);
		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);
	glEnd();
}


// use Cramer's Rule
bool
Triangle::intersect(HitInfo& result, const Ray& r, float tMin, float tMax)
{
	TriangleMesh::TupleI3 ti3 = m_mesh->vIndices()[m_index];
	const Vector3 & v0 = m_mesh->vertices()[ti3.x]; //vertex a of triangle
	const Vector3 & v1 = m_mesh->vertices()[ti3.y]; //vertex b of triangle
	const Vector3 & v2 = m_mesh->vertices()[ti3.z]; //vertex c of triangle
	const Vector3 A[3] = {v0 - v1, v0 - v2, r.d};
	const Vector3 b = v0 - r.o;
	//Crmer's Rule
	const float detA = dot(cross(A[0], A[1]), A[2]);
	float beta = dot(cross(b, A[1]), A[2]) / detA;
	float gamma = dot(cross(A[0], b), A[2]) / detA;
	float t = dot(cross(A[0], A[1]), b) / detA;
	float alpha = 1.0f - beta - gamma;

	if(alpha <= 0.0f || alpha >= 1.0f ||
		beta <= 0.0f || beta >= 1.0f ||
		gamma <= 0.0f || gamma >= 1.0f ||
		t <= tMin || t >= tMax)
	{
		// not intersect or invalid t
		return false;
	}
	//else = intersect
	result.t = t;
	result.P = r.o + result.t * r.d;
	ti3 = m_mesh->nIndices()[m_index];
	Vector3* normals = m_mesh->normals();
	result.N = alpha * normals[ti3.x] + beta * normals[ti3.y] + gamma * normals[ti3.z];
	result.N.normalize();
	result.material = this->m_material;
	return true;
}

BoundingVolume
Triangle::calcBoundingVolume()
{
	TriangleMesh::TupleI3 ti3 = m_mesh->vIndices()[m_index];
	const Vector3 & v0 = m_mesh->vertices()[ti3.x]; //vertex a of triangle
	const Vector3 & v1 = m_mesh->vertices()[ti3.y]; //vertex b of triangle
	const Vector3 & v2 = m_mesh->vertices()[ti3.z]; //vertex c of triangle
	float min_x = std::min(v0.x, std::min(v1.x, v2.x));
	float min_y = std::min(v0.y, std::min(v1.y, v2.y));
	float min_z = std::min(v0.z, std::min(v1.z, v2.z));
	float max_x = std::max(v0.x, std::max(v1.x, v2.x));
	float max_y = std::max(v0.y, std::max(v1.y, v2.y));
	float max_z = std::max(v0.z, std::max(v1.z, v2.z));
	return BoundingVolume(Vector3(min_x, min_y, min_z), Vector3(max_x, max_y, max_z));
}

Vector3
Triangle::getGeometriNormal(Vector3 point) {// not need parameter "point" but this function is overrided.
	point = 0.0f; //for avoid warning
	TriangleMesh::TupleI3 ti3 = m_mesh->vIndices()[m_index];
	const Vector3 & v0 = m_mesh->vertices()[ti3.x]; //vertex a of triangle
	const Vector3 & v1 = m_mesh->vertices()[ti3.y]; //vertex b of triangle
	const Vector3 & v2 = m_mesh->vertices()[ti3.z]; //vertex c of triangle
	return (cross(v1 - v0, v2 - v0)).normalize();
}
