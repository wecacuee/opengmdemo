// From opengm 2.0.2 beta manual  (Section 2.4.1)
// author: Bjoern Andres et al
#include <opengm/graphicalmodel/graphicalmodel.hxx>
#include <opengm/operations/adder.hxx>
#include <opengm/graphicalmodel/space/simplediscretespace.hxx>
#include <opengm/functions/explicit function.hxx>

typedef opengm::SimpleDiscreteSpace<> Space;
typedef opengm::GraphicalModel<double, opengm::Adder, opengm::ExplicitFunction<double>, Space> Model;
int main() {
  Space space(10, 4);
  Model gm(space);
  size t shape[] = {4, 4};
  opengm::ExplicitFunction<double> f(shape, shape + 2, 1.0);
  f(0, 0) = 0.2;
  f(1, 1) = 0.2;
  f(2, 2) = 0.2;
  f(3, 3) = 0.2;
  Model::FunctionIdentifier fid = gm.addFunction(f);
  return 0;
}
