#include "TimeStepper.hpp"

///TODO: implement Explicit Euler time integrator here
void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize)
{

  std::vector<Vector3f> currentState = particleSystem->getState();
  std::vector<Vector3f> f0 = particleSystem->evalF(currentState);

  int n = f0.size();
  std::vector<Vector3f> nextState;
  nextState.reserve(n);
  for (int i = 0; i < f0.size(); i++) {
    nextState.push_back(currentState[i] + stepSize * f0[i]);
  }

  particleSystem->setState(nextState);
}

///TODO: implement Trapzoidal rule here
void Trapzoidal::takeStep(ParticleSystem* particleSystem, float stepSize)
{
  std::vector<Vector3f> currentState = particleSystem->getState();
  std::vector<Vector3f> f0 = particleSystem->evalF(currentState);
  std::vector<Vector3f> nextState;
  

  int n = f0.size();
  nextState.reserve(n);
  for (int i = 0; i < n; i++) {
    nextState.push_back(currentState[i] + stepSize * f0[i]);
  }

  std::vector<Vector3f> f1 = particleSystem->evalF(nextState);
  std::vector<Vector3f> ret;

  ret.reserve(n);
  for (int i = 0; i < n; i++) {
    ret.push_back(currentState[i] + stepSize * (f0[i] + f1[i]) / 2);
  }

  particleSystem->setState(ret);
}
