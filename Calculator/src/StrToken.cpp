#ifndef NO_STR_TOKEN_FUNC
#include "ASTCalculator.h"

#include <regex>
#include <algorithm>
#include <boost/assert.hpp>
#include <boost/range.hpp>
#include <boost/range/adaptor/reversed.hpp>

SStrItem::SStrItem(std::size_t offset, const std::wstring& str):
Str_(str)
{
	Offset_ = offset;
	OldLength_ = Str_.length();

	static std::wregex rg(L"([^_]+)");

	std::wsregex_iterator itorBeg(str.begin(), str.end(), rg);
	for ( auto& cur : boost::make_iterator_range(itorBeg, std::wsregex_iterator()) )
	{
		Token_.push_back(cur.str(1));
	}
}


StrTokens::StrTokens() {}

ItemVector& StrTokens::GetBlocks()
{
	return Blocks_;
}

std::wstring	StrTokens::GetNewString()
{
	for ( auto& cur : boost::adaptors::reverse(Blocks_) )
	{
		SaveStr_.replace(cur.Offset_, cur.OldLength_, cur.Str_);
	}

	SaveStr_.erase(std::remove_if(SaveStr_.begin(), SaveStr_.end(), [](std::wstring::reference& curVal){ return curVal == L'[' || curVal == L']';}), SaveStr_.end());
	return SaveStr_;
}


void			StrTokens::StrToken(std::wstring str)
{
	SaveStr_ = str;
	static std::wregex rg(L"\\[(.+?)\\]");

	std::wsregex_iterator itBeg(SaveStr_.begin(), SaveStr_.end(), rg);
	for ( auto& cur : boost::make_iterator_range(itBeg, std::wsregex_iterator()) )
	{
		SStrItem newItem(cur[1].first-SaveStr_.begin(), cur.str(1));
		Blocks_.push_back(newItem);
	}
}


const ColonTokenResult	StrTokens::GetColonToken() const
{
	return ColonToken_;
}


void			StrTokens::StrTokenColon(std::wstring str)
{
	static	std::wregex rg(L"([^:]+)");

	std::wsregex_iterator itBeg(str.begin(), str.end(), rg);
	for ( auto& cur : boost::make_iterator_range(itBeg, std::wsregex_iterator()) )
	{
		ColonToken_.push_back(cur.str(1));
	}
}

#endif
