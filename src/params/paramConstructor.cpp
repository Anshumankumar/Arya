#include <params/paramConstructor.hpp>

template<> void ParamConstructor::postrun<Params>(std::string key)
{
    paramStack.push(&paramStack.top()->get<Params>(key));
}


