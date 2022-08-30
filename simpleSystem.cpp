
#include "simpleSystem.h"

using namespace std;

SimpleSystem::SimpleSystem(): ParticleSystem(1)
{
	assert(m_numParticles == 1);
	m_vVecState.reserve(1);
	m_vVecState.push_back(Vector3f(1.f, 0.f, 0.f));
}

// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> SimpleSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;

	// YOUR CODE HERE
	int n = state.size();
	f.reserve(n);

	for (int i = 0; i < n; i++) {
		// x, y, z => -y, x, 0
		f.push_back( Vector3f(-state[i].y(), state[i].x(), 0.f) );
	}

	return f;
}

// render the system (ie draw the particles)
void SimpleSystem::draw()
{
		Vector3f pos ;//YOUR PARTICLE POSITION
		glPushMatrix();
		for (int i = 0, n = m_vVecState.size(); i < n; i++){
			glTranslatef(m_vVecState[i][0], m_vVecState[i][1], m_vVecState[i][2] );
			glutSolidSphere(0.075f,10.0f,10.0f);
		}
		glPopMatrix();
		return;
		  
}
