#pragma once
#include <string>
#include <iostream>
#include <memory>
#include <stdexcept>

enum class ENodeValidationMessageType
{
    Critical,
    Error,
    Warning,
    Info,
    Debug,
    Trace
};

class INodeValidator
{
public:
    virtual void Send(ENodeValidationMessageType messageType, std::string message) = 0; 
};

struct IValidable
{
public:
    virtual bool Validate(INodeValidator& validator) const = 0;
};

struct AstNodeBase : public IValidable
{
public:
    virtual ~AstNodeBase() = default;
};

enum class ExprTypeBase
{
    INT,
    FLOAT,
    STRING,
    USER_DEFINED
};

struct ExprType
{
    ExprTypeBase typeBase;
    std::string userDefinedSymbol;
public:
    ExprType(ExprTypeBase typeBase) 
        : typeBase(typeBase)
    {
        if (typeBase == ExprTypeBase::USER_DEFINED)
            throw std::invalid_argument("Constructing expression type as 'user defined' without the user defined value.");
    }

    ExprType(std::string userDefinedSymbol) 
        : typeBase(ExprTypeBase::USER_DEFINED), userDefinedSymbol(userDefinedSymbol) 
    {
        if (userDefinedSymbol.empty())
            throw std::invalid_argument("Constructing expression type as 'user defined' with an empty user defined value.");
    }

    bool operator==(ExprType o) const 
    { 
        return typeBase == o.typeBase && (typeBase == ExprTypeBase::USER_DEFINED ? userDefinedSymbol == o.userDefinedSymbol : true); 
    }

    bool operator==(ExprTypeBase typeBase) const 
    { 
        return typeBase == this->typeBase; 
    }

    bool operator!=(ExprType o) const { return !(*this == o); }
    bool operator!=(ExprTypeBase typeBase) const { return !(*this == typeBase); }
};

struct ExprNode : public AstNodeBase
{
public:
    virtual ~ExprNode() = default;
    virtual ExprType GetType() const = 0;
};

struct ConstExprNode : public ExprNode
{
public:
    virtual ~ConstExprNode() = default;

private:
    virtual std::string Eval() const = 0;
};

struct Integer : public ConstExprNode
{
    int value;
public:
    Integer(int value) : value(value) {}

    virtual ExprType GetType() const override { return ExprType(ExprTypeBase::INT); }

    std::string Eval() const override { return std::to_string(value); }
    virtual bool Validate(INodeValidator& validator) const override { return true; }
};

enum class BinaryOperation
{
    ADD,
    SUBSTRACT,
    MULTIPLY,
    DIVIDE,
    MODULO
};

struct BinaryOperatorNode : ExprNode
{
    BinaryOperation op; 
    std::shared_ptr<ExprNode> leftExpr, rightExpr;
public:
    BinaryOperatorNode(BinaryOperation op, std::shared_ptr<ExprNode> leftExpr, std::shared_ptr<ExprNode> rightExpr)
        : op(op), leftExpr(leftExpr), rightExpr(rightExpr)
    {}

    virtual bool Validate(INodeValidator& validator) const override 
    { 
        if (leftExpr->GetType() != ExprTypeBase::STRING && leftExpr->GetType() != ExprTypeBase::USER_DEFINED &&
            rightExpr->GetType() != ExprTypeBase::STRING && rightExpr->GetType() != ExprTypeBase::USER_DEFINED)
            return true;

        if (leftExpr->GetType() != ExprTypeBase::STRING || rightExpr->GetType() != ExprTypeBase::STRING)
            validator.Send(ENodeValidationMessageType::Error, " do operations with strings.");

        if (leftExpr->GetType() != ExprTypeBase::USER_DEFINED || rightExpr->GetType() != ExprTypeBase::USER_DEFINED)
            validator.Send(ENodeValidationMessageType::Error, "Trying to do operations with user defined types.");

        return false;
    }

    virtual ExprType GetType() const override 
    {
        return rightExpr->GetType();
    }

};
