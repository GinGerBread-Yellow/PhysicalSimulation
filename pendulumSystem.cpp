
#include "pendulumSystem.h"

const float PendulumSystem::PENDULUM_GRAVITY = 4.9f;
const float PendulumSystem::PENDULUM_DRAG = 0.01f;
const float PendulumSystem::PENDULUM_PARTICLE_MASS = 0.3f;
const float PendulumSystem::PENDULUM_SPRING_K = 16.f;
const float PendulumSystem::PENDULUM_SPRING_LEN = 1.f;

PendulumSystem::PendulumSystem(int numParticles):ParticleSystem(numParticles)
{
	m_numParticles = numParticles;
	
	// fill in code for initializing the state based on the number of particles
	m_vVecState.reserve(m_numParticles*2); // x, v

	for (int i = 0; i < m_numParticles; i++) {
		
		// for this system, we care about the position and the velocity
		Vector3f dir(3.f, -4.f, 0.f);
		dir.normalize();
		m_vVecState.push_back(i*dir);
		m_vVecState.push_back(Vector3f(0.f));

	}

	// link spring
	adjacentSprings.resize(m_numParticles);
	for (int i = 0; i < m_numParticles - 1; i++) {
		adjacentSprings[i].push_back(SpringEdge(i+1, PENDULUM_SPRING_LEN, PENDULUM_SPRING_K));
		adjacentSprings[i+1].push_back(SpringEdge(i, PENDULUM_SPRING_LEN, PENDULUM_SPRING_K));
	}
}


// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> PendulumSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;
	// f.resize(state.size(), Vector3f(0));

	// YOUR CODE HERE
	
	f.reserve(state.size());
	for (int i = 0; i < m_numParticles; i++) {
		int xid = i<<1, vid = xid+1;

		Vector3f force(0.f);
		
		// drag
		force += -PENDULUM_DRAG * state[vid];
		// spring
		for (int adj = 0; adj < adjacentSprings[i].size(); adj++) {
			SpringEdge &edge = adjacentSprings[i][adj];
			Vector3f diff = state[(edge.nei)<<1] - state[xid];
			force += edge.k * (diff.abs() - edge.restLength) * (diff.normalized());
		}

		Vector3f acc = force / PENDULUM_PARTICLE_MASS + Vector3f(0.f, -PENDULUM_GRAVITY, 0.f);

		f.push_back(state[vid]); 	// dx <- v
		f.push_back(acc); 				// dv <- a
	}
	// first point constraint
	f[0] = Vector3f(0.f);
	f[1] = Vector3f(0.f);
	
	return f;
}

// render the system (ie draw the particles)
void PendulumSystem::draw()
{
	// assert(m_numParticles*2 == m_vVecState.size());
	for (int i = 0; i < (m_numParticles<<1); i+=2) {
		Vector3f &pos = m_vVecState[i];//  position of particle i. YOUR CODE HERE

		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(0.075f,10.0f,10.0f);
		glPopMatrix();
	}
}
