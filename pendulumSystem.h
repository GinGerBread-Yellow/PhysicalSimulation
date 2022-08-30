#ifndef PENDULUMSYSTEM_H
#define PENDULUMSYSTEM_H

#include <vecmath.h>
#include <vector>
#include <GLUT/glut.h>

#include "particleSystem.h"

// #define PENDULUM_GRAVITY 3
// #define PENDULUM_DRAG 0.2
// #define PENDULUM_PARTICLE_MASS 0.2
// #define PENDULUM_SPRING_K 8.0
// #define PENDULUM_SPRING_LEN 1.0


class PendulumSystem: public ParticleSystem
{
public:

	static const float PENDULUM_GRAVITY;
	static const float PENDULUM_DRAG;
	static const float PENDULUM_PARTICLE_MASS;
	static const float PENDULUM_SPRING_K;
	static const float PENDULUM_SPRING_LEN;
	
	PendulumSystem(int numParticles);
	
	vector<Vector3f> evalF(vector<Vector3f> state);
	
	void draw();

	class SpringEdge {
	public:
		SpringEdge(int _vertice, int _restLength, float _k):
			nei(_vertice), restLength(_restLength), k(_k) {}
		int nei;
		float restLength;
		float k;
	};

private:
	// new variables
	vector< vector< SpringEdge > > adjacentSprings;	
};

#endif
