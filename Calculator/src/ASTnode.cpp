#pragma once
#include "ASTnode.h"
//#include <boost/assert.hpp>


const double  K	= (0.01745329251994329576923690768489);


SASTnode::SASTnode(double Nr):
mValueNr(Nr),
mNodeType(ENT_DOUBLE),
mIsSuccess(true)
{

}


SASTnode::SASTnode():
mValueNr(0),
mNodeType(ENT_UNKNOWN),
mIsSuccess(true)
{

}


SASTnode::SASTnode(const SASTnode& rhs):
mChildren(rhs.mChildren),
mNodeType(rhs.mNodeType),
mName(rhs.mName),
mValueNr(rhs.mValueNr),
mOpt(rhs.mOpt),
mIsSuccess(true)
{

}


SASTnode::SASTnode(const StringType& str):
mName(str),
mNodeType(ENT_UNKNOWN),
mIsSuccess(true)
{

}


SASTnode	SASTnode::operator+=(const SASTnode& r)
{
	return HandleBinaryOpt(r, '+');
}
SASTnode	SASTnode::operator-=(const SASTnode& r)
{
	return HandleBinaryOpt(r, '-');
}
SASTnode	SASTnode::operator*=(const SASTnode& r)
{
	return HandleBinaryOpt(r, '*');
}
SASTnode	SASTnode::operator/=(const SASTnode& r)
{
	return HandleBinaryOpt(r, '/');
}


SASTnode	SASTnode::HandleBinaryOpt(const SASTnode& r, char op)
{
	SASTnode father;
	father.mOpt = op;
	father.mNodeType = ENT_BINARY_OP;
	father.mChildren.push_back(*this);
	father.mChildren.push_back(r);
	*this = father;
	return father;
}


void		SASTnode::HandleIdentifier()
{
	mNodeType = ENT_IDENTIFIER;
}


void		SASTnode::HandleArgList(const VectorType& n)
{
	mChildren = n;
	mNodeType = ENT_FUNC;
}


double		SASTnode::DoCalc()
{
	double result = 0;

	std::vector<double> children_result;
	int chSize = static_cast<int>(mChildren.size());
	for (int i = 0; i < chSize; ++i)
	{
		children_result.push_back(mChildren[i].DoCalc());
		mIsSuccess &= mChildren[i].mIsSuccess;
	}

	switch (mNodeType)
	{
	case ENT_DOUBLE:
		{
			result = mValueNr;
		}
		break;
	case ENT_IDENTIFIER:
		{
			mIsSuccess = false;
		}
		break;
	case ENT_FUNC:
		{
			int chSize = children_result.size();

			if (mName == "sin" && 1==chSize )
			{
				result = std::sin(K*children_result[0]);
			}
			else if ( mName == "cos" && 1==chSize )
			{
				result = std::cos(K*children_result[0]);
			}
			else if ( mName == "tan" && 1==chSize )
			{
				result = std::tan(K*children_result[0]);
			}
			else if ( mName == "atan" && 1==chSize )
			{
				result = std::atan(K*children_result[0]);
			}
			else if ( mName == "asin" && 1==chSize )
			{
				result = std::asin(K*children_result[0]);
			}
			else if ( mName == "acos" && 1==chSize )
			{
				result = std::acos(K*children_result[0]);
			}
			else if ( mName == "abs" && 1==chSize )
			{
				result = std::abs(children_result[0]);
			}
			else if ( mName == "pow" && 2==chSize )
			{
				result = std::pow(children_result[0], children_result[1]);
			}
			else
			{
				mIsSuccess = false;
				//BOOST_ASSERT(0);
			}
		}
		break;
	case ENT_BINARY_OP:
		{
			switch (mOpt)
			{
			case '+':
				{
					result = children_result[0] + children_result[1];
				}	
				break;
			case '-':
				{
					result = children_result[0] - children_result[1];
				}
				break;
			case '*':
				{
					result = children_result[0] * children_result[1];
				}
				break;
			case '/':
				{
					result = children_result[0] / children_result[1];
				}	
				break;
			default:
				{
					mIsSuccess = false;
					//BOOST_ASSERT(0);
				}
				break;
			}
		}
		break;
	default:
		{
			mIsSuccess = false;
			//BOOST_ASSERT(0);
		}
		break;
	}

	return result;
}


bool		SASTnode::IsSuccessed() const
{
	return mIsSuccess;
}