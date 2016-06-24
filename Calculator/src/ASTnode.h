#pragma once
#include <string>
#include <vector>

// AST树的结点定义
class  SASTnode
{
public:

	typedef	std::string					StringType;
	typedef	std::vector<SASTnode>		VectorType;

	enum ENodeType
	{
		ENT_UNKNOWN = 0,
		ENT_DOUBLE,			// 终端int结点
		ENT_IDENTIFIER,		// 终端变量结点
		ENT_FUNC,			// 函数结点 
		ENT_BINARY_OP,		// 二元运算结点
	};

	SASTnode(double i);

	SASTnode();

	SASTnode(const SASTnode& n);

	SASTnode(const StringType& s);

public:

	SASTnode operator+=(const SASTnode& r);

	SASTnode operator-=(const SASTnode& r);

	SASTnode operator*=(const SASTnode& r);

	SASTnode operator/=(const SASTnode& r);

public:

	// 处理二元运算，生成以二元运算符为根的树
	SASTnode	HandleBinaryOpt(const SASTnode& r, char op);

	// 变量结节，name是变量名，变量无类型
	void		HandleIdentifier();

	// 函数结节，处理函数参数
	void		HandleArgList(const VectorType& n);

	// 执行运算
	double		DoCalc();

	bool		IsSuccessed() const;

private:

	VectorType	mChildren;
	ENodeType	mNodeType;

	StringType	mName;
	double		mValueNr;
	char		mOpt;

	bool		mIsSuccess;
};