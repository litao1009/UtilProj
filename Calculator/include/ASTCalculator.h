#pragma once

#include <string>
#include <vector>



//************************************
// Method:    Calculator
// FullName:  Calculator
// Access:    public 
// Returns:   double	----------------返回值
// Parameter: const std::string & str---输入字符串
// Parameter: bool & res----------------是否成功计算
//************************************
double	Calculator(const std::string& str, bool& res);

double	Calculator(const std::wstring& str, bool& res);

double	Calculator(const char* cstr, bool& res);

double	Calculator(const wchar_t* cwstr, bool& res);



#ifndef NO_STR_TOKEN_FUNC

class	SStrItem
{
public:
	SStrItem(std::size_t offset, const std::wstring& str);

	std::size_t					Offset_;
	std::size_t					OldLength_;
	std::wstring				Str_;
	std::vector<std::wstring>	Token_;
};

typedef	std::vector<SStrItem>		ItemVector;
typedef	std::vector<std::wstring>	ColonTokenResult;

class	StrTokens
{
public:

	StrTokens();

public:

	ItemVector&		GetBlocks();

	std::wstring	GetNewString();

	void			StrToken(std::wstring str);

	void			StrTokenColon(std::wstring str);

	const ColonTokenResult	GetColonToken()	const;

private:

	ItemVector		Blocks_;
	std::wstring	SaveStr_;

	ColonTokenResult	ColonToken_;
};
#endif
