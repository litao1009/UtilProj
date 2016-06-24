#pragma once
#include <string>
#include <vector>

// AST���Ľ�㶨��
class  SASTnode
{
public:

	typedef	std::string					StringType;
	typedef	std::vector<SASTnode>		VectorType;

	enum ENodeType
	{
		ENT_UNKNOWN = 0,
		ENT_DOUBLE,			// �ն�int���
		ENT_IDENTIFIER,		// �ն˱������
		ENT_FUNC,			// ������� 
		ENT_BINARY_OP,		// ��Ԫ������
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

	// �����Ԫ���㣬�����Զ�Ԫ�����Ϊ������
	SASTnode	HandleBinaryOpt(const SASTnode& r, char op);

	// ������ڣ�name�Ǳ�����������������
	void		HandleIdentifier();

	// ������ڣ�����������
	void		HandleArgList(const VectorType& n);

	// ִ������
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