// @author: Vikas Dhiman (dhiman@umich.edu)

#include <vector>

#include <opengm/graphicalmodel/graphicalmodel.hxx>
#include <opengm/operations/multiplier.hxx>
#include <opengm/functions/explicit function.hxx>
#include <opengm/graphicalmodel/space/discretespace.hxx>

int main() { 
  // Step 1: Value Type: Codomain of the factors
  typedef double MyValueType;
  // Step 2: OperationType: The factorization operator
  typedef opengm::Multiplier MyOperationType;
  // /home/pgmstudent/co/opengm/include/opengm/operations/multiplier.hxx

  // Step 3: FunctionTypeList: The type of each factor
  typedef opengm::ExplicitFunction<MyValueType> MyFunctionType;


  // Step 4: SpaceType: The index and label type of each variable. 
  //
  // Stepm 4a : IndexType: The type for number of index variables 
  typedef size_t MyIndexType;
  // Stepm 4b : LabelType:  The type for label values
  typedef size_t MyLabelType;
  typedef opengm::DiscreteSpace<MyIndexType, MyIndexType> MySpaceType;
  MySpaceType space;
  space.addVariable(2); // X0
  space.addVariable(2); // X1
  space.addVariable(3); // X2
  space.addVariable(2); // X3
  space.addVariable(2); // X4


  // Size of value space of each random variable i.e. X0
  MyIndexType var0[] = {0};
  size t shape0[] = {2};
  opengm::ExplicitFunction<double> phi0(shape0, shape0 + 1, 0.0);
  phi0(0) = 0.6; phi0(1) = 0.4; // Rvalue assignment


  // Size of value space of each random variable i.e. X1
  MyIndexType var1[] = {1};
  size t shape1[] = {2};
  opengm::ExplicitFunction<double> phi1(shape1, shape1 + 1, 0.0);
  phi1(0) = 0.7; phi1(1) = 0.3;

  // Size of value space of each random variable i.e. X2, X0, X1
  MyIndexType var2[] = {2, 0, 1};
  size t shape2[] = {3, 2, 2};
  opengm::ExplicitFunction<double> phi2(shape, shape + 3, 0.0);
  phi2(0, 0, 0) = 0.3;  phi2(1, 0, 0) = 0.4;  phi2(2, 0, 0) = 0.3;
  phi2(0, 0, 1) = 0.05; phi2(1, 0, 1) = 0.25; phi2(2, 0, 1) = 0.7;
  phi2(0, 1, 0) = 0.9;  phi2(1, 1, 0) = 0.08; phi2(2, 1, 0) = 0.02;
  phi2(0, 1, 1) = 0.5;  phi2(1, 1, 1) = 0.3;  phi2(2, 1, 1) = 0.2;

  // Size of value space of each random variable i.e. X1, X3
  MyIndexType var3[] = {1, 3};
  size t shape3[] = {2, 2};
  opengm::ExplicitFunction<double> phi3(shape3, shape3 + 2, 0.0);
  phi3(0, 0) = 0.95; phi3(0, 1) = 0.05;
  phi3(1, 0) = 0.20; phi3(1, 1) = 0.80;

  // Size of value space of each random variable i.e. X2, X4
  MyIndexType var4[] = {2, 4};
  size t shape4[] = {2, 2};
  opengm::ExplicitFunction<double> phi4(shape4, shape4 + 2, 0.0);
  phi4(0, 0) = 0.1;  phi4(0, 1) = 0.9;
  phi4(1, 0) = 0.4;  phi4(1, 1) = 0.6;
  phi4(2, 0) = 0.99; phi4(2, 1) = 0.01;

  typedef opengm::GraphicalModel<MyValueType, MyOperationType, opengm::ExplicitFunction<MyValueType>, opengm::DiscreteSpace<> > GMType;
  GMType gm(space);
  typedef GMType::FunctionIndentifier GMFunctionIndentifier;
  GMFunctionIndentifier fid0 = gm.addFunction(phi0);
  GMFunctionIndentifier fid1 = gm.addFunction(phi1);
  GMFunctionIndentifier fid2 = gm.addFunction(phi2);
  GMFunctionIndentifier fid3 = gm.addFunction(phi3);
  GMFunctionIndentifier fid4 = gm.addFunction(phi4);

  // Finally adding the edges
  gm.addFactor(fid0, var0, var0 + 1);
  gm.addFactor(fid1, var1, var1 + 1);
  gm.addFactor(fid2, var2, var2 + 3);
  gm.addFactor(fid3, var3, var3 + 2);
  gm.addFactor(fid4, var4, var4 + 2);

  std::vector<MyLabelType> labelings(5);
  gm.evaluate(labelings);
}
