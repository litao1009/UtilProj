#include <boost/config/warning_disable.hpp>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;

using qi::double_;
using ascii::space;
using phoenix::ref;

typedef	std::string::iterator	ItorType;

class	SFunc
{
public:

	SFunc()
	{
		Success_ = false;
		Res_ = 0;
	}

	bool	Success_;
	double	Res_;
	std::string	FuncName_;
	std::vector<double>	Args_;
};
BOOST_FUSION_ADAPT_STRUCT(
	SFunc,
	(double, Res_)
	(std::string, FuncName_)
	(std::vector<double>, Args_)
	)

class	TestGrammer : public qi::grammar<ItorType, double()>
{
public:

	TestGrammer():base_type(Exp_)
	{
		Group_ = qi::char_('(') >> Exp_[qi::_val=qi::_1] >> qi::char_(')');
		Factor_ = qi::double_[qi::_val=qi::_1] | Group_[qi::_val=qi::_1] | Func_[qi::_val=phoenix::at_c<0>(qi::_1)];
		Term_ = Factor_[qi::_val=qi::_1] >> *( (qi::char_('*') >> Factor_[qi::_val*=qi::_1]) | (qi::char_('/') >> Factor_[qi::_val/=qi::_1]) );
		Exp_ = Term_[qi::_val=qi::_1] >> *( (qi::char_('+') >> Term_[qi::_val+=qi::_1]) | (qi::char_('-') >> Term_[qi::_val-=qi::_1]) );

		Func_ = 
			qi::eps[phoenix::at_c<0>(qi::_val)=0]
		>> +qi::alpha[phoenix::at_c<1>(qi::_val)+=qi::_1]
		>> qi::char_('(')
			>> (Exp_[phoenix::push_back(phoenix::at_c<2>(qi::_val), qi::_1)] % qi::char_(','))
			>> qi::char_(')') 
			>> qi::eps[boost::phoenix::bind(&TestGrammer::PostProc, qi::_val)];
	}

	static void PostProc(SFunc& val)
	{
		for (auto& cur : val.Args_ )
		{
			val.Res_ += cur;
		}
	}

private:

	qi::rule<ItorType, double()>	Group_,Term_,Factor_,Exp_;
	qi::rule<ItorType, SFunc()>		Func_;
};

class	TestGrammer2 : public qi::grammar<ItorType, double()>
{
public:

	TestGrammer2():base_type(Res_)
	{
		Res_ = Func_[qi::_val=phoenix::at_c<0>(qi::_1)];

		Func_ = 
			qi::eps[phoenix::at_c<0>(qi::_val)=0]
		>> +qi::alpha[phoenix::at_c<1>(qi::_val)+=qi::_1]
		>> qi::char_('(')
		>> (qi::double_[phoenix::push_back(phoenix::at_c<2>(qi::_val), qi::_1)] % qi::char_(','))
		>> qi::char_(')') 
		>> qi::eps[boost::phoenix::bind(&TestGrammer2::Print, qi::_val)];
	}

	static void Print(SFunc& val)
	{
		for (auto& cur : val.Args_ )
		{
			val.Res_ += cur;
		}
	}

private:

	qi::rule<ItorType, SFunc()>		Func_;
	qi::rule<ItorType, double()>	Res_;

};

int	main()
{
	{
		std::string testStr = "asdf(1,2,3)";
		double val;
		TestGrammer2 g;
		auto ret = qi::phrase_parse(testStr.begin(), testStr.end(), g, ascii::space_type(), val);
	}

	std::string testStr = "1+2*(3-4)";
	double val;
	auto ret = qi::phrase_parse(testStr.begin(), testStr.end(), TestGrammer(), ascii::space_type(), val);
}