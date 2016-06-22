#pragma once

#include <functional>
#include <string>
#include <map>

template<typename BaseT>
class	IReflection
{
protected:

	typedef	std::function<BaseT*()>	ClassGen;

public:

	static	BaseT*	Create(const std::string& className)
	{
		auto& genMap_ = _GetGenMap();

		auto itor = genMap_.find("class " + className);
		if ( itor != genMap_.end() )
		{
			return itor->second();
		}

		return nullptr;
	}

	static	void Register(const std::string& className, const ClassGen& genFtr)
	{
		auto& genMap_ = _GetGenMap();

		genMap_.emplace(className, genFtr);
	}

private:

	typedef	std::map<std::string, ClassGen>	GenMapType;
	static	GenMapType&	_GetGenMap()
	{
		static	GenMapType genMap;
		return genMap;
	}
};

template<typename DerivedT, typename BaseT>
class	IReflectionDerived
{
public:

	static BaseT*	Factory()
	{
		return new DerivedT();
	}

protected:

	class	Context
	{
	public:

		Context()
		{
			DerivedT::Register(typeid(DerivedT).name(), std::bind(DerivedT::Factory));
		}
	};

	static	Context	sContext;
};

#define IMPLEMNET_REFLECTION(DerivedT, BaseT)		\
	DerivedT::Context	IReflectionDerived<DerivedT, BaseT>::sContext;