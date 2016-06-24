//#include "CalcGrammar.h"
//
//template<typename ItType, typename NodeType>
//CalcGrammar<ItType,NodeType>::CalcGrammar():
//base_type(expr)
//{
//	using	boost::spirit::qi::_1;
//	using	boost::spirit::qi::_val;
//	using	boost::spirit::qi::double_;
//	using	boost::spirit::qi::eps;
//	using	boost::spirit::qi::alpha;
//	using	boost::spirit::qi::alnum;
//
//	expr	= term[_val = _1]	>>	*(	('+' >> term)[_val += _1] | ('-' >> term)[_val -= _1]	);
//
//	term	= factor[_val = _1]	>>	*(	('*' >> factor)[_val *= _1] | ('/' >> factor)[_val /= _1]	);
//
//	factor	= double_[_val = _1] | '('	>>	expr[_val = _1]	>>	')' | vari[_val = _1];
//
//	vari	= identifier[_val = _1]	>>	('('	>> (expr % ',')[boost::phoenix::bind(&NodeType::HandleArgList, _val, _1)]
//	>>	')' | eps[boost::phoenix::bind(&NodeType::HandleIdentifier, _val)]);
//
//	identifier = alpha	>>	*alnum;
//}