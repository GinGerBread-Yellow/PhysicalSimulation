#ifndef CLOTHSYSTEM_H
#define CLOTHSYSTEM_H

#include <iostream>
#include <vecmath.h>
#include <vector>
#include <GLUT/glut.h>


#include "particleSystem.h"

class ClothSystem: public ParticleSystem
{
///ADD MORE FUNCTION AND FIELDS HERE
public:

	static const float CLOTH_GRAVITY;
	static const float CLOTH_DRAG;
	static const float CLOTH_GRID_LEN;
	static const float SPRING_STRUCT_K;
	static const float SPRING_SHEER_K;
	static const float SPRING_FLEX_K;

	ClothSystem();
	vector<Vector3f> evalF(vector<Vector3f> state);
	
	void draw();

	class SpringEdge {
	public:
		SpringEdge(int _v1, int _v2, float _restLength, float _k):
			p0(_v1), p1(_v2), restLength(_restLength), k(_k) {}
		int p0;
		int p1;
		float restLength;
		float k;
	};

private:

	inline int indexOf(int i, int j) {
		return (i*ncols + j)<<1;
	}

	vector< SpringEdge > springs;	

	int nrows;
	int ncols;

};


#endif
