// From opengm 2.0.2 beta manual  (Section 6.2)
// author: Bjoern Andres et al
#include ”opengm/opengm.hxx”
#include ”opengm/functions/function registration.hxx”
#include ”opengm/functions/function properties base.hxx”

template<class T, class I = size t, class L = size t>
class ModuloFunction
: public FunctionBase<ModuloFunction<T, I, L>, T, size t, size t>
{
  public:
    typedef T ValueType;
    typedef L LabelType;
    typedef I IndexType;

    ModuloFunction(const IndexType shape)
      : shape (shape); {}
    LabelType shape(const size t j) const
    { OPENGM ASSERT(j == 0); return shape ; }
    size t size() const
    { return shape ; }
    size t dimension() const
    { return 1; }
    template<class ITERATOR> ValueType operator()(ITERATOR it) const
    { OPENGM ASSERT(∗it < shape ); return ∗it % 2; }
    bool operator==(const ModuloFunction& other) const
    { return other.shape == shape ; }
    IndexType numberOfParameters() const
    { return 1; }
    IndexType parameter(const size t j) const
    { OPENGM ASSERT(j == 0); return shape ; }
    IndexType& parameter(const size t j);
    { OPENGM ASSERT(j == 0); return shape ; }

  private:
    IndexType shape ;
    friend class FunctionSerialization<ModuloFunction<T, I, L> > ;
};

// Every custom function needs to be registered with a unique ID between 0 and 16000 (IDs
// above 16000 are reserved for the OpenGM standard). Registration works as follows:
//
template <class T, class I, class L>
struct FunctionRegistration<ModuloFunction<T, I, L> > {
  enum ID { Id = 0; };
};

// One last step is necessary to add the custom class of functions to the OpenGM file format.
// This works by specializing the class template FunctionSerialization:
template<class T, class I, class L>
class FunctionSerialization<ModuloFunction<T, I, L> > {
  public:
    typedef typename ModuloFunction<T, I, L>::ValueType ValueType;
    static size t indexSequenceSize(const ModuloFunction<T, I, L>&)
    { return 1; }
    static size t valueSequenceSize(const ModuloFunction<T, I, L>&)
    { return 0; }
    template<class INDEX ITERATOR, class VALUE ITERATOR>
      static void serialize(const ModuloFunction<T, I, L>& f,
          INDEX ITERATOR iit, VALUE ITERATOR vit)
      { ∗iit = f.size(); }
    template<class INDEX ITERATOR, class VALUE ITERATOR>
      static void deserialize(INDEX ITERATOR iit, VALUE ITERATOR vit,
          ModuloFunction<T, I, L>& f)
      { f.shape = ∗iit; }
};
