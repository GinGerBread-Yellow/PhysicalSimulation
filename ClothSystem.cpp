#include "ClothSystem.h"

const float ClothSystem::CLOTH_GRAVITY = 2.f;
const float ClothSystem::CLOTH_DRAG = 0.25f;
const float ClothSystem::CLOTH_GRID_LEN = 0.25f; // fixed
const float ClothSystem::SPRING_STRUCT_K = 128.f;
const float ClothSystem::SPRING_SHEER_K = 128.f;
const float ClothSystem::SPRING_FLEX_K = 128.f;


//TODO: Initialize here
// ClothSystem::ClothSystem() 
ClothSystem::ClothSystem():ParticleSystem(225),nrows(15), ncols(15)
{
	// init 3*3
	m_vVecState.reserve(m_numParticles*2); // x, v
	Vector3f orig = Vector3f(0.f, 0.f, 0.f);
	Vector3f dirU = Vector3f(0.f, 0.f, CLOTH_GRID_LEN);
	// Vector3f dirU = Vector3f(0.f, -CLOTH_GRID_LEN, 0.f);
	Vector3f dirV = Vector3f(CLOTH_GRID_LEN, 0.f, 0.f);
	for (int i = 0; i < nrows; i++) {
		for (int j = 0; j < ncols; j++) {
			m_vVecState.push_back(orig + (i*dirU + j*dirV));
			m_vVecState.push_back(Vector3f(0.f));
		}
	}

	// link spring
	springs.reserve(m_numParticles*6);

	// 1. structural
	for (int i = 0; i < nrows; i++) {
		for (int j = 0; j < ncols-1; j++) {
			// horizontal
			int p0 = indexOf(i,j);
			int p1 = indexOf(i,j+1);
			springs.push_back(SpringEdge(p0, p1, CLOTH_GRID_LEN, SPRING_STRUCT_K));
		}
	}
	for (int i = 0; i < nrows-1; i++) {
		for (int j = 0; j < ncols; j++) {
			// vertical
			int p0 = indexOf(i,j);
			int p1 = indexOf(i+1,j);
			springs.push_back(SpringEdge(p0, p1, CLOTH_GRID_LEN, SPRING_STRUCT_K));
		}
	}

	// sheer edges
	for (int i = 0; i < nrows-1; i++) {
		for (int j = 0; j < ncols-1; j++) {
			// sheer
			float len = Vector3f(CLOTH_GRID_LEN, CLOTH_GRID_LEN, 0.f).abs();
			springs.push_back(SpringEdge(indexOf(i,j), indexOf(i+1,j+1), len, SPRING_SHEER_K));
			springs.push_back(SpringEdge(indexOf(i,j+1), indexOf(i+1,j), len, SPRING_SHEER_K));
		}
		
	}
	
	// flex: 
	for (int i = 0; i < nrows; i++) {
		for (int j = 0; j < ncols - 2; j++) {
			float len = 2 * CLOTH_GRID_LEN;
			springs.push_back(SpringEdge(indexOf(i,j), indexOf(i,j+2), len, SPRING_FLEX_K));
		}
	}
	for (int i = 0; i < nrows - 2; i++) {
		for (int j = 0; j < ncols; j++) {
			float len = 2 * CLOTH_GRID_LEN;
			springs.push_back(SpringEdge(indexOf(i,j), indexOf(i+2,j), len, SPRING_FLEX_K));
		}
	}
}


// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> ClothSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;
	// f.resize(state.size(), Vector3f(0));
	// return f;
	// YOUR CODE HERE
	
	f.reserve(state.size());
	for (int i = 0; i < m_numParticles; i++) {
		int xid = i<<1, vid = xid+1;

		// drag + gravity
		Vector3f force = Vector3f(0.f, -CLOTH_GRAVITY, 0.f) - CLOTH_DRAG * state[vid];

		f.push_back(state[vid]); 	// dx <- v
		f.push_back(force); 				// dv <- a
	}

	// springs
	for (int i = 0, n = springs.size(); i < n; i++) {

		SpringEdge &edge = springs[i];
		Vector3f diff = state[edge.p1] - state[edge.p0];
		float length = diff.abs();
		Vector3f acc_0 = edge.k * ( length - edge.restLength) * (diff / length);
		f[edge.p0+1] += acc_0; // p0.v
		f[edge.p1+1] -= acc_0; // p1.v
	}

	if(flags & CLOTH_WIND_ON) {

		for (int i = 0; i < m_numParticles; i++) {
			float rnd = (float)rand()/RAND_MAX;
			f[2*i+1] += rnd * Vector3f(0.f, 0.f, 4.f);
		}
	}

	// first point constraint
	int i1 = indexOf(0, 0), i2 = indexOf(0, ncols-1);

	// f[i1] = Vector3f(0.f);
	f[i1+1] = Vector3f(0.f);

	// f[i2] = Vector3f(0.f);
	f[i2+1] = Vector3f(0.f);

	// cerr << "spring force\n";
	// for (int i = 0; i < 2*m_numParticles; i+=2) {
	// 	cerr << "vec" << i << '=' 
	// 		<< f[i][0] << ' ' << f[i][1] << ' '<< f[i][2] << '\n';
	// 	cerr << "force" << i << '=' 
	// 		<< f[i+1][0] << ' ' << f[i+1][1] << ' '<< f[i+1][2] << '\n';
	// }
	
	return f;
}

///TODO: render the system (ie draw the particles)
void ClothSystem::draw()
{
	/**
	 * draw different graph 
	*/

	if(flags & CLOTH_JOINT_ON) {
		drawJoints();
	}

	if(flags & CLOTH_WIREFRAME_ON) {
		drawWireframes();
	} else {
		drawMeshes();
	}
 
	
}

void ClothSystem::drawWireframes() {
	glLineWidth(2.0f);
	glColor4f(0.5f, 0.5f, 0.5f, 1);
	glBegin(GL_LINES);
	for (int i = 0; i < springs.size(); i++) {
		if(springs[i].restLength != CLOTH_GRID_LEN) continue;
		Vector3f p0 = m_vVecState[springs[i].p0], p1 = m_vVecState[springs[i].p1];
		glVertex3f(p0.x(), p0.y(), p0.z());
		glVertex3f(p1.x(), p1.y(), p1.z());
	}
	
	glEnd();
}

void ClothSystem::drawJoints() {
	// check for node
	for (int i = 0; i < (m_numParticles<<1); i+=2) {
		Vector3f &pos = m_vVecState[i];//  position of particle i. YOUR CODE HERE

		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(0.075f,10.0f,10.0f);
		glPopMatrix();
	}
}

void ClothSystem::drawMeshes() {
	// write mesh
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < nrows - 1; ++i) {
		for (int j = 0; j < ncols - 1; j++) {
			// (i,j+1) > (i,j) > (i+1, j)
			int ii = indexOf(i, j), ij = indexOf(i, j+1), 
					ji = indexOf(i+1, j), jj = indexOf(i+1,j+1);

			Vector3f v1 = m_vVecState[ii] - m_vVecState[ij];
			Vector3f v2 = m_vVecState[ji] - m_vVecState[ij];
			Vector3f v3 = m_vVecState[jj] - m_vVecState[ij];

			Vector3f norm1 = Vector3f::cross(v1, v2);
			Vector3f norm2 = Vector3f::cross(v2, v3);
			glColor4f(1, 1, 1, 1);
			glNormal3d(norm1[0], norm1[1], norm1[2]);
			glVertex3d(m_vVecState[ij][0], m_vVecState[ij][1], m_vVecState[ij][2]);
			glVertex3d(m_vVecState[ii][0], m_vVecState[ii][1], m_vVecState[ii][2]);
			glVertex3d(m_vVecState[ji][0], m_vVecState[ji][1], m_vVecState[ji][2]);

			// (i, j+1) > (i+1, j) > (i+1, j+1)
			glNormal3d(norm2[0], norm2[1], norm2[2]);
			glVertex3d(m_vVecState[ij][0], m_vVecState[ij][1], m_vVecState[ij][2]);
			glVertex3d(m_vVecState[ji][0], m_vVecState[ji][1], m_vVecState[ji][2]);
			glVertex3d(m_vVecState[jj][0], m_vVecState[jj][1], m_vVecState[jj][2]);
			
		}
	}
	glEnd();
}
