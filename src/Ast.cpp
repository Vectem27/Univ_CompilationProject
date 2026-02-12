#include "Ast.h"

bool BinaryOperatorNode::Validate(INodeValidator& validator) const 
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

int BinaryOperatorNode::GenerateCode(std::ostream& os) const
{
    std::string opStr;

    switch (op) 
    {
    case BinaryOperation::ADD:
        opStr = "+";
        break;
    case BinaryOperation::SUBSTRACT:
        opStr = "-";
        break;
    case BinaryOperation::MULTIPLY:
        opStr = "*";
        break;
    case BinaryOperation::DIVIDE:
        opStr = "/";
        break;
    case BinaryOperation::MODULO:
        opStr = "%";
        break;
    default:
        throw std::runtime_error("Unknown operator");
    }

    os << "(";
    leftExpr->GenerateCode(os);
    os << opStr;
    rightExpr->GenerateCode(os);
    os << ")";
    return 0;
}