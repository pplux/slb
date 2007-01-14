#include <SLB/Instance.hpp>
#include <SLB/ClassInfo.hpp>
#include <SLB/Manager.hpp>

namespace SLB {

	InstanceBase::InstanceBase(Type t, const std::type_info &type) : _flags(t), _class(0)
	{
		_class = Manager::getInstance().getClass( type );
	}

	InstanceBase::~InstanceBase() {}

	InstanceFactory::~InstanceFactory() {}
}
