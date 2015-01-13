// @author: Vikas Dhiman (dhiman@umich.edu)

#include <vector>

#include <opengm/graphicalmodel/graphicalmodel.hxx>
#include <opengm/operations/multiplier.hxx>
#include <opengm/functions/explicit_function.hxx>
#include <opengm/graphicalmodel/space/discretespace.hxx>
#include <opengm/inference/messagepassing/messagepassing.hxx>
#include <opengm/operations/adder.hxx>

int main() { 
  // Step 1: Value Type: Codomain of the factors
  typedef double ValueType;
  // Step 2: OperationType: The factorization operator
  typedef opengm::Multiplier OperationType;
  // /home/pgmstudent/co/opengm/include/opengm/operations/multiplier.hxx

  // Step 3: FunctionTypeList: The type of each factor
  typedef opengm::ExplicitFunction<ValueType> FunctionType;


  // Step 4: SpaceType: The index and label type of each variable. 
  //
  // Stepm 4a : IndexType: The type for number of index variables 
  typedef size_t IndexType;
  // Stepm 4b : LabelType:  The type for label values
  typedef size_t LabelType;
  typedef opengm::DiscreteSpace<IndexType, IndexType> SpaceType;
  SpaceType space;
  space.addVariable(2); // X0
  space.addVariable(2); // X1
  space.addVariable(3); // X2
  space.addVariable(2); // X3
  space.addVariable(2); // X4


  // Size of value space of each random variable i.e. X0
  IndexType var0[] = {0};
  size_t shape0[] = {2};
  opengm::ExplicitFunction<double> phi0(shape0, shape0 + 1, 0.0);
  phi0(0) = 0.6; phi0(1) = 0.4; // Rvalue assignment


  // Size of value space of each random variable i.e. X1
  IndexType var1[] = {1};
  size_t shape1[] = {2};
  opengm::ExplicitFunction<double> phi1(shape1, shape1 + 1, 0.0);
  phi1(0) = 0.7; phi1(1) = 0.3;

  // Size of value space of each random variable i.e. X0, X1, X2
  IndexType var2[] = {0, 1, 2};
  size_t shape2[] = {2, 2, 3};
  opengm::ExplicitFunction<double> phi2(shape2, shape2 + 3, 0.0);
  phi2(0, 0, 0) = 0.3;  phi2(0, 0, 1) = 0.4;  phi2(0, 0, 2) = 0.3;
  phi2(1, 0, 0) = 0.05; phi2(1, 0, 1) = 0.25; phi2(1, 0, 2) = 0.7;
  phi2(0, 1, 0) = 0.9;  phi2(0, 1, 1) = 0.08; phi2(0, 1, 2) = 0.02;
  phi2(1, 1, 0) = 0.5;  phi2(1, 1, 1) = 0.3;  phi2(1, 1, 2) = 0.2;

  // Size of value space of each random variable i.e. X1, X3
  IndexType var3[] = {1, 3};
  size_t shape3[] = {2, 2};
  opengm::ExplicitFunction<double> phi3(shape3, shape3 + 2, 0.0);
  phi3(0, 0) = 0.95; phi3(0, 1) = 0.05;
  phi3(1, 0) = 0.20; phi3(1, 1) = 0.80;

  // Size of value space of each random variable i.e. X2, X4
  IndexType var4[] = {2, 4};
  size_t shape4[] = {3, 2};
  opengm::ExplicitFunction<double> phi4(shape4, shape4 + 2, 0.0);
  phi4(0, 0) = 0.1;  phi4(0, 1) = 0.9;
  phi4(1, 0) = 0.4;  phi4(1, 1) = 0.6;
  phi4(2, 0) = 0.99; phi4(2, 1) = 0.01;

  typedef opengm::GraphicalModel<ValueType, OperationType, opengm::ExplicitFunction<ValueType>, opengm::DiscreteSpace<> > Model;
  Model gm(space);
  Model::FunctionIdentifier fid0 = gm.addFunction(phi0);
  Model::FunctionIdentifier fid1 = gm.addFunction(phi1);
  Model::FunctionIdentifier fid2 = gm.addFunction(phi2);
  Model::FunctionIdentifier fid3 = gm.addFunction(phi3);
  Model::FunctionIdentifier fid4 = gm.addFunction(phi4);

  // Finally adding the edges
  gm.addFactor(fid0, var0, var0 + 1);
  gm.addFactor(fid1, var1, var1 + 1);
  gm.addFactor(fid2, var2, var2 + 3);
  gm.addFactor(fid3, var3, var3 + 2);
  gm.addFactor(fid4, var4, var4 + 2);

  // Set up inference method
  typedef opengm::BeliefPropagationUpdateRules<Model, opengm::Integrator> UpdateRules;
  typedef opengm::MessagePassing<Model, opengm::Integrator, UpdateRules, opengm::MaxDistance> BeliefPropagation;
  const size_t maxNumberOfIterations = 100;
  const double convergenceBound = 1e-7;
  const double damping = 0.0;
  BeliefPropagation::Parameter parameter(maxNumberOfIterations, convergenceBound, damping);
  parameter.useNormalization_ = true;
  BeliefPropagation bp(gm, parameter);
  // optimize (approximately)
 // BeliefPropagation::VerboseVisitorType visitor;
  bp.infer();
  // obtain the (approximate) argmax 14
  // vector<size_t> labeling(numberOfVariables);
  // bp.arg(labeling);

  Model::IndependentFactorType marg;
  for(IndexType var=0; var<gm.numberOfVariables(); ++var)
  {
    std::cout<< "Variable x_" << var << " has the following marginal distribution P(x_"<<var<<") :";
    bp.marginal(var,marg);
    for(LabelType i=0; i<gm.numberOfLabels(var); ++i)
      std::cout <<marg(&i) << " ";
    std::cout<<std::endl;
  }   
}
