#pragma once

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

// 采用spirit的解析器
template<typename StringType, typename NodeType>
struct CalcGrammar : boost::spirit::qi::grammar<typename StringType::iterator, boost::spirit::ascii::space_type, NodeType()>
{
	typedef	typename StringType::iterator ItType;
	CalcGrammar();

	boost::spirit::qi::rule<ItType, boost::spirit::ascii::space_type, NodeType()> expr, term, factor, vari, func;
	boost::spirit::qi::rule<ItType, boost::spirit::ascii::space_type, std::string()> identifier;
};


template<typename ItType, typename NodeType>
CalcGrammar<ItType,NodeType>::CalcGrammar():
base_type(expr)
{
	using	boost::spirit::qi::_1;
	using	boost::spirit::qi::_val;
	using	boost::spirit::qi::double_;
	using	boost::spirit::qi::eps;
	using	boost::spirit::qi::alpha;
	using	boost::spirit::qi::alnum;

	expr	= term[_val = _1]	>>	*(	('+' >> term)[_val += _1] | ('-' >> term)[_val -= _1]	);

	term	= factor[_val = _1]	>>	*(	('*' >> factor)[_val *= _1] | ('/' >> factor)[_val /= _1]	);

	factor	= double_[_val = _1] | '('	>>	expr[_val = _1]	>>	')' | vari[_val = _1];

	vari	= identifier[_val = _1]	>>	('('	>> (expr % ',')[boost::phoenix::bind(&NodeType::HandleArgList, _val, _1)]
	>>	')' | eps[boost::phoenix::bind(&NodeType::HandleIdentifier, _val)]);

	identifier = alpha	>>	*alnum;
}