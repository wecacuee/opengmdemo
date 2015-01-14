#include <opengm/inference/messagepassing.hxx>
#include <opengm/operations/maximizer.hxx>
// set up the optimizer (loopy belief propagation)
typedef BpUpdateRules<Model, Maximizer> UpdateRules;
typedef MessagePassing<Model, Maximizer, UpdateRules, MaxDistance> BeliefPropagation
const size t maxNumberOfIterations = 100;
const double convergenceBound = 1e−7;
const double damping = 0.0;
BeliefPropagation::Parameter parameter(maxNumberOfIterations, convergenceBound, damping); 9
  BeliefPropagation bp(gm, parameter);
// optimize (approximately)
BeliefPropagation::VerboseVisitorType visitor;
bp.infer(visitor);
// obtain the (approximate) argmax 14
vector<size t> labeling(numberOfVariables);
bp.arg(labeling);
