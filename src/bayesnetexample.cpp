// Uses C++11 constructs, requires OpenGM
// Compile using
// g++ -g -std=c++0x -I <location to opengm include> ./bayesnetexample.cpp
// 
// @author: Vikas Dhiman (dhiman@umich.edu)

#include <vector>

#include <opengm/graphicalmodel/graphicalmodel.hxx>
#include <opengm/operations/multiplier.hxx>
#include <opengm/functions/explicit_function.hxx>
#include <opengm/graphicalmodel/space/discretespace.hxx>
#include <opengm/inference/messagepassing/messagepassing.hxx>
#include <opengm/operations/adder.hxx>

int main() { 
  // Value Type: Codomain of the factors
  typedef double ValueType;
  // OperationType: The factorization operator
  typedef opengm::Multiplier OperationType;
  // /home/pgmstudent/co/opengm/include/opengm/operations/multiplier.hxx

  // FunctionTypeList: The type of each factor
  typedef opengm::ExplicitFunction<ValueType> FunctionType;


  // SpaceType: The index and label type of each variable. 
  //
  // IndexType: The type for number of index variables 
  typedef size_t IndexType;
  // LabelType:  The type for label values
  typedef size_t LabelType;
  typedef opengm::DiscreteSpace<IndexType, IndexType> SpaceType;
  SpaceType space;
  space.addVariable(2); // X0
  space.addVariable(2); // X1
  space.addVariable(3); // X2
  space.addVariable(2); // X3
  space.addVariable(2); // X4


  std::vector<IndexType> vars4phi0 {0}; // c++0x
  // Size of value space of each random variable i.e. X0
  std::vector<size_t> shape0 {2}; // c++0x
  opengm::ExplicitFunction<ValueType> phi0(shape0.begin(), shape0.end(), 0.0);
  phi0(0) = 0.6; phi0(1) = 0.4; // Rvalue assignment


  // Size of value space of each random variable i.e. X1
  std::vector<IndexType> vars4phi1 {1};
  std::vector<size_t> shape1 {2};
  opengm::ExplicitFunction<ValueType> phi1(shape1.begin(), shape1.end(), 0.0);
  phi1(0) = 0.7; phi1(1) = 0.3;

  std::vector<IndexType> vars4phi2 {0, 1, 2};
  // Size of value space of each random variable i.e. X0, X1, X2
  std::vector<size_t> shape2 {2, 2, 3};
  opengm::ExplicitFunction<ValueType> phi2(shape2.begin(), shape2.end(), 0.0);
  phi2(0, 0, 0) = 0.3;  phi2(0, 0, 1) = 0.4;  phi2(0, 0, 2) = 0.3;
  phi2(1, 0, 0) = 0.05; phi2(1, 0, 1) = 0.25; phi2(1, 0, 2) = 0.7;
  phi2(0, 1, 0) = 0.9;  phi2(0, 1, 1) = 0.08; phi2(0, 1, 2) = 0.02;
  phi2(1, 1, 0) = 0.5;  phi2(1, 1, 1) = 0.3;  phi2(1, 1, 2) = 0.2;

  std::vector<IndexType> vars4phi3 {1, 3};
  // Size of value space of each random variable i.e. X1, X3
  std::vector<size_t> shape3 {2, 2};
  opengm::ExplicitFunction<ValueType> phi3(shape3.begin(), shape3.end(), 0.0);
  phi3(0, 0) = 0.95; phi3(0, 1) = 0.05;
  phi3(1, 0) = 0.20; phi3(1, 1) = 0.80;

  std::vector<IndexType> vars4phi4 {2, 4};
  // Size of value space of each random variable i.e. X2, X4
  std::vector<size_t> shape4 {3, 2};
  opengm::ExplicitFunction<ValueType> phi4(shape4.begin(), shape4.end(), 0.0);
  phi4(0, 0) = 0.1;  phi4(0, 1) = 0.9;
  phi4(1, 0) = 0.4;  phi4(1, 1) = 0.6;
  phi4(2, 0) = 0.99; phi4(2, 1) = 0.01;

  typedef opengm::GraphicalModel<
          ValueType,
          OperationType,
          FunctionType,
          SpaceType
            > Model;
  Model gm(space);
  Model::FunctionIdentifier fid0 = gm.addFunction(phi0);
  Model::FunctionIdentifier fid1 = gm.addFunction(phi1);
  Model::FunctionIdentifier fid2 = gm.addFunction(phi2);
  Model::FunctionIdentifier fid3 = gm.addFunction(phi3);
  Model::FunctionIdentifier fid4 = gm.addFunction(phi4);

  // Finally adding the edges
  gm.addFactor(fid0, vars4phi0.begin(), vars4phi0.end());
  gm.addFactor(fid1, vars4phi1.begin(), vars4phi1.end());
  gm.addFactor(fid2, vars4phi2.begin(), vars4phi2.end());
  gm.addFactor(fid3, vars4phi3.begin(), vars4phi3.end());
  gm.addFactor(fid4, vars4phi4.begin(), vars4phi4.end());

  // Playing around with the graph object
  std::cout << " +++ Properties of created graph +++ " << std::endl;
  std::cout << "numberOfVariables = " << gm.numberOfVariables() << std::endl;
  std::cout << "numberOfFactors = " << gm.numberOfFactors() << std::endl;
  std::cout << "isAcyclic = " << gm.isAcyclic() << std::endl;
  std::cout << "maxFactorOrder = " << gm.maxFactorOrder() << std::endl;

  // Evaluating the total probability for a given set of variable values
  std::vector<LabelType> labelings {0, 0, 0, 0, 0};
  std::cout << "P(0, 0, 0, 0, 0) = " << gm.evaluate(labelings.begin()) << std::endl;

  // Set up inference method
  typedef opengm::Integrator QueryOperator; // for computing marginals
  // typedef opengm::Maximizer QueryOperator; // for computing MAP queries
  typedef opengm::BeliefPropagationUpdateRules<Model, QueryOperator>
    UpdateRules;
  typedef opengm::MessagePassing<Model, QueryOperator, UpdateRules,
          opengm::MaxDistance> BeliefPropagation;
  const size_t maxNumberOfIterations = 100;
  const double convergenceBound = 1e-7;
  const double damping = 0.0;
  BeliefPropagation::Parameter parameter(maxNumberOfIterations,
      convergenceBound, damping);
  parameter.useNormalization_ = true;

  // Inference on graphical model
  BeliefPropagation bp(gm, parameter);
  bp.infer();

  // Find most likely labeling
  std::vector<LabelType> labeling(gm.numberOfVariables());
  bp.arg(labeling);
  std::cout << "Labeling : ";
  for(IndexType var=0; var<gm.numberOfVariables(); ++var)
    std::cout << labeling[var] << ", ";
  std::cout << std::endl;

  // Accessing marginal probabilities
  Model::IndependentFactorType marg;
  for(IndexType var=0; var<gm.numberOfVariables(); ++var)
  {
    std::cout<< "Variable x_" << var 
      << " has the following marginal distribution P(x_"<<var<<") :";
    bp.marginal(var,marg);
    for(LabelType i=0; i<gm.numberOfLabels(var); ++i)
      std::cout <<marg(&i) << " ";
    std::cout<<std::endl;
  }   
}
