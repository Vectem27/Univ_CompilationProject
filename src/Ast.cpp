#include "Ast.h"

bool ComparisonNode::Validate(INodeValidator& validator) const
{
    if (!leftExpr->Validate(validator) || !rightExpr->Validate(validator))
        return false;

    if (op == ComparisonOperation::EQ || op == ComparisonOperation::NEQ)
    {
        if (leftExpr->GetType() == rightExpr->GetType())
            return true;
        if (leftExpr->GetType().IsNumber() && rightExpr->GetType().IsNumber())
            return true;
    }
    else
    {
        if (leftExpr->GetType().IsNumber() && rightExpr->GetType().IsNumber())
            return true;
    }

    validator.Send(ENodeValidationMessageType::Error, "Incompatible types in comparison.");
    return false;
}

int ComparisonNode::GenerateCode(std::ostream& os) const
{
    std::string opStr;
    switch (op)
    {
    case ComparisonOperation::EQ:  opStr = "=="; break;
    case ComparisonOperation::NEQ: opStr = "!="; break;
    case ComparisonOperation::LT:  opStr = "<";  break;
    case ComparisonOperation::LTE: opStr = "<="; break;
    case ComparisonOperation::GT:  opStr = ">";  break;
    case ComparisonOperation::GTE: opStr = ">="; break;
    default:
        throw std::runtime_error("Unknown comparison operator");
    }

    os << "(";
    leftExpr->GenerateCode(os);
    os << opStr;
    rightExpr->GenerateCode(os);
    os << ")";
    return 0;
}

bool BinaryOperatorNode::Validate(INodeValidator& validator) const 
{ 
    if (!leftExpr->Validate(validator) || !rightExpr->Validate(validator))
        return false;

    if (leftExpr->GetType().IsNumber() && rightExpr->GetType().IsNumber())
        return true;

    if (leftExpr->GetType().IsString() || rightExpr->GetType().IsString())
        validator.Send(ENodeValidationMessageType::Error, " do operations with strings.");

    if (leftExpr->GetType().IsUserDefined() || rightExpr->GetType().IsUserDefined())
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

bool WhileNode::Validate(INodeValidator& validator) const
{
    if (!condition->Validate(validator))
        return false;

    if (condition->GetType() != ExprTypeBase::BOOL)
    {
        validator.Send(ENodeValidationMessageType::Error, "While condition must be of type bool.");
        return false;
    }

    return body->Validate(validator);
}

int WhileNode::GenerateCode(std::ostream& os) const
{
    os << "while (";
    condition->GenerateCode(os);
    os << ") ";
    bool isBlock = dynamic_cast<ScopeBlockNode*>(body.get()) != nullptr;
    body->GenerateCode(os);
    if (!isBlock) os << ";";
    return 0;
}

bool DoWhileNode::Validate(INodeValidator& validator) const
{
    if (!condition->Validate(validator))
        return false;

    if (condition->GetType() != ExprTypeBase::BOOL)
    {
        validator.Send(ENodeValidationMessageType::Error, "Do-while condition must be of type bool.");
        return false;
    }

    return body->Validate(validator);
}

int DoWhileNode::GenerateCode(std::ostream& os) const
{
    os << "do ";
    bool isBlock = dynamic_cast<ScopeBlockNode*>(body.get()) != nullptr;
    body->GenerateCode(os);
    if (!isBlock) os << ";";
    os << " while (";
    condition->GenerateCode(os);
    os << ")";
    return 0;
}

bool IfNode::Validate(INodeValidator& validator) const
{
    if (!condition->Validate(validator))
        return false;

    if (condition->GetType() != ExprTypeBase::BOOL)
    {
        validator.Send(ENodeValidationMessageType::Error, "If condition must be of type bool.");
        return false;
    }

    if (!thenBody->Validate(validator))
        return false;

    if (elseBody && !elseBody->Validate(validator))
        return false;

    return true;
}

int IfNode::GenerateCode(std::ostream& os) const
{
    os << "if (";
    condition->GenerateCode(os);
    os << ") ";
    bool thenIsBlock = dynamic_cast<ScopeBlockNode*>(thenBody.get()) != nullptr;
    thenBody->GenerateCode(os);
    if (!thenIsBlock) os << ";";

    if (elseBody)
    {
        os << std::endl << "    else ";
        bool elseIsBlock = dynamic_cast<ScopeBlockNode*>(elseBody.get()) != nullptr;
        elseBody->GenerateCode(os);
        if (!elseIsBlock) os << ";";
    }
    return 0;
}