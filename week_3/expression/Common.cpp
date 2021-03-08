#include "Common.h"

using namespace std;


class Add : public Expression
{
public:
	Add(ExpressionPtr left, ExpressionPtr right)
		: left_(move(left)), right_(move(right))
	{
	}

	string ToString() const override
	{
		return "(" + left_->ToString() + ")" + "+" +
			"(" + right_->ToString() + ")";
	}

	int Evaluate() const override
	{
		return left_->Evaluate() + right_->Evaluate();
	}

private:
	ExpressionPtr left_;
	ExpressionPtr right_;
};

class Mul : public Expression
{
public:
	Mul(ExpressionPtr left, ExpressionPtr right)
		: left_(move(left)), right_(move(right))
	{
	}

	string ToString() const override
	{
		return "(" + left_->ToString() + ")" + "*" +
			"(" + right_->ToString() + ")";
	}

	int Evaluate() const override
	{
		return left_->Evaluate() * right_->Evaluate();
	}

private:
	ExpressionPtr left_;
	ExpressionPtr right_;
};

class Val : public Expression
{
public:
	Val(int val) : value(val)
	{
	}

	string ToString() const override
	{
		return to_string(value);
	}

	int Evaluate() const override
	{
		return value;
	}

private:
	int value;
};


ExpressionPtr Value(int value)
{
	return make_unique<Val>(value);
}

ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right)
{
	return make_unique<Add>(move(left), move(right));
}

ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right)
{
	return make_unique<Mul>(move(left), move(right));
}