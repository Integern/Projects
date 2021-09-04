/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

/*

    Joel Broad 2020

    added hitbox capability to speed up raytracing; ignores rays that dont hit the hitbox
	added barycentric capability; averages normals from vertexes of a triangle to get a smooth surface

*/

#pragma once

#include "vertex.h"
#include "transform.h"
#include "object.h"
#include "box.h"
#include "extents.h"

typedef int TriangleIndex[3];

class PolyMesh:public Object{
public:
	int vertex_count;
	int triangle_count;
        Vertex *vertex;
	Vector *face_normal;
	Vector *vertex_normal;
	TriangleIndex *triangle;

	Box *hitbox; // used to reduce complexity of raytracing; check if ray hits the hitbox first
	Extents *extents; //what extents all vertexes are stored in
	int hitbox_misses; //tracks how many times a ray doesnt hit our hitbox; so saves us computation

	void do_construct(char *file, Transform *transform);
	float test_edge(Vector &normal, Vertex &p, Vertex &v1, Vertex &v0);
	Vertex barycentric(Vertex point, Vertex a, Vertex b, Vertex c); //gives coefficients on how to weight normals from the vertex a,b,c to give interpolated normal of vertex point
	void triangle_intersection(Ray ray, Hit &hit, int which_triangle);
	void intersection(Ray ray, Hit &hit);
	void compute_face_normal(int which_triangle, Vector &normal);
	void compute_vertex_normals(void);
	bool rayTriangleIntersect(const Ray& ray, const Vector &v0, const Vector &v1, const Vector &v2, float &t);
        PolyMesh(char *file);
	PolyMesh(char *file, Transform *transform);
	~PolyMesh(){}
};
