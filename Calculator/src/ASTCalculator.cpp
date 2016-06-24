#include "ASTCalculator.h"

#include "ASTnode.h"
#include "CalcGrammar.h"

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <regex>


typedef	CalcGrammar<SASTnode::StringType, SASTnode>	ASTCalculator;

#ifndef THREAD_SAFETY
ASTCalculator	cal;
#endif


double	Calculator(const std::string& str, bool& res)
{
	using namespace boost::spirit::qi;

	std::string strTrim = str;
	strTrim.erase(std::remove_copy_if(str.begin(), str.end(), strTrim.begin(), [](char ch){ return ch == ' '; }), strTrim.end());
	boost::to_lower(strTrim);
	
	std::regex rgHead("^(-\\(.+)");		//-30
	std::regex rgMid("\\((-[^\\(]*)");	//(-30)
	strTrim = std::regex_replace(strTrim, rgHead, "0$1");
	strTrim = std::regex_replace(strTrim, rgMid, "(0$1");
	
#ifdef	THREAD_SAFETY
	ASTCalculator	cal;
#endif
	
	SASTnode astNode;
	phrase_parse(strTrim.begin(), strTrim.end(), cal, ascii::space_type(), astNode);
	
	double dResult = astNode.DoCalc();
	res = astNode.IsSuccessed();
	return dResult;
}


double	Calculator(const std::wstring& str, bool& res)
{
	std::string strA;
	std::transform(str.begin(), str.end(), std::back_inserter(strA), [](wchar_t ch){ return char(ch); });

	return Calculator(strA, res);
}


double	Calculator(const char* cstr, bool& res)
{
	std::string str(cstr);
	return Calculator(str, res);
}


double	Calculator(const wchar_t* cwstr, bool& res)
{
	std::wstring str(cwstr);

	return Calculator(str, res);
}