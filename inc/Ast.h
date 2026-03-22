#pragma once
#include <string>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <vector>


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
    virtual int GenerateCode(std::ostream& os) const = 0;
};

enum class ExprTypeBase
{
    INT,
    FLOAT,
    STRING,
    BOOL,
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

    bool IsNumber() const
    {
        return typeBase == ExprTypeBase::INT || typeBase == ExprTypeBase::FLOAT;
    }

    bool IsString() const
    {
        return typeBase == ExprTypeBase::STRING;
    }

    bool IsUserDefined() const
    {
        return typeBase == ExprTypeBase::USER_DEFINED;
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

class SymbolTable
{
    std::vector<std::unordered_map<std::string, ExprType>> scopes;

    SymbolTable()
    {
        scopes.emplace_back();
    }

public:
    SymbolTable(const SymbolTable&) = delete;
    SymbolTable& operator=(const SymbolTable&) = delete;

    static SymbolTable& GetInstance()
    {
        static SymbolTable instance;
        return instance;
    }

    void EnterScope()
    {
        scopes.emplace_back();
    }

    void ExitScope()
    {
        if (scopes.size() <= 1)
            throw std::runtime_error("Attempted to pop global scope.");

        scopes.pop_back();
    }

    bool DeclareType(const std::string& name, const ExprType& type)
    {
        auto& currentScope = scopes.back();
        if (currentScope.find(name) != currentScope.end())
            return false;

        currentScope.insert_or_assign(name, type);
        return true;
    }

    bool TryGetType(const std::string& name, ExprType& outType) const
    {
        for (auto scope = scopes.rbegin(); scope != scopes.rend(); ++scope)
        {
            auto entry = scope->find(name);
            if (entry == scope->end())
                continue;

            outType = entry->second;
            return true;
        }

        return false;
    }

    void Clear()
    {
        scopes.clear();
        scopes.emplace_back();
    }
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
    virtual int GenerateCode(std::ostream& os) const override { os << value; return 0;}
};

struct Float : public ConstExprNode
{
    double value;
public:
    Float(double value) : value(value) {}

    virtual ExprType GetType() const override { return ExprType(ExprTypeBase::FLOAT); }

    std::string Eval() const override { return std::to_string(value); }
    virtual bool Validate(INodeValidator& validator) const override { return true; }
    virtual int GenerateCode(std::ostream& os) const override { os << value; return 0; }
};

struct String : public ConstExprNode
{
    std::string value;
public:
    String(std::string value) : value(value) {}

    virtual ExprType GetType() const override { return ExprType(ExprTypeBase::STRING); }

    std::string Eval() const override { return value; }
    virtual bool Validate(INodeValidator& validator) const override { return true; }
    virtual int GenerateCode(std::ostream& os) const override { os << '"' << value << '"'; return 0; }
};

struct Bool : public ConstExprNode
{
    bool value;
public:
    Bool(bool value) : value(value) {}

    virtual ExprType GetType() const override { return ExprType(ExprTypeBase::BOOL); }

    std::string Eval() const override { return value ? "true" : "false"; }
    virtual bool Validate(INodeValidator& validator) const override { return true; }
    virtual int GenerateCode(std::ostream& os) const override { os << (value ? "true" : "false"); return 0; }
};

enum class BinaryOperation
{
    ADD,
    SUBSTRACT,
    MULTIPLY,
    DIVIDE,
    MODULO
};

enum class ComparisonOperation
{
    EQ,
    NEQ,
    LT,
    LTE,
    GT,
    GTE
};

struct ComparisonNode : ExprNode
{
    ComparisonOperation op;
    std::shared_ptr<ExprNode> leftExpr, rightExpr;
public:
    ComparisonNode(ComparisonOperation op, std::shared_ptr<ExprNode> leftExpr, std::shared_ptr<ExprNode> rightExpr)
        : op(op), leftExpr(leftExpr), rightExpr(rightExpr)
    {}

    virtual bool Validate(INodeValidator& validator) const override;

    virtual ExprType GetType() const override
    {
        return ExprType(ExprTypeBase::BOOL);
    }

    virtual int GenerateCode(std::ostream& os) const override;
};

struct BinaryOperatorNode : ExprNode
{
    BinaryOperation op; 
    std::shared_ptr<ExprNode> leftExpr, rightExpr;
public:
    BinaryOperatorNode(BinaryOperation op, std::shared_ptr<ExprNode> leftExpr, std::shared_ptr<ExprNode> rightExpr)
        : op(op), leftExpr(leftExpr), rightExpr(rightExpr)
    {}

    virtual bool Validate(INodeValidator& validator) const override;

    virtual ExprType GetType() const override 
    {
        return rightExpr->GetType();
    }

    virtual int GenerateCode(std::ostream& os) const override;
};

struct VarAccessorNode : ExprNode
{
    std::string varName;
public:
    VarAccessorNode(std::string varName)
        : varName(varName)
    {}

    virtual bool Validate(INodeValidator& validator) const override
    {
        ExprType symbolType = ExprType(ExprTypeBase::INT);
        if (SymbolTable::GetInstance().TryGetType(varName, symbolType))
            return true;

        validator.Send(ENodeValidationMessageType::Error, "Unknown variable: " + varName);
        return false;
    }

    virtual ExprType GetType() const override 
    {
        ExprType symbolType = ExprType(ExprTypeBase::INT);
        if (SymbolTable::GetInstance().TryGetType(varName, symbolType))
            return symbolType;

        return ExprType(ExprTypeBase::INT); // TODO: Report unknown symbols explicitly
    }

    virtual int GenerateCode(std::ostream& os) const override { os << varName; return 0; }
};

struct AssignationNode : ExprNode
{
    std::shared_ptr<ExprNode> target, value;
public:
    AssignationNode(std::shared_ptr<ExprNode> target, std::shared_ptr<ExprNode> value)
        : target(target), value(value)
    {}

    virtual bool Validate(INodeValidator& validator) const override
    {
        if (!target->Validate(validator) || !value->Validate(validator))
            return false;

        if (target->GetType().IsNumber() && value->GetType().IsNumber())
            return true;

        if(target->GetType() == value->GetType())
            return true;

        validator.Send(ENodeValidationMessageType::Error, "Trying to assign value of different types.");
        return false;
    }

    virtual ExprType GetType() const override 
    {
        return target->GetType();
    }

    virtual int GenerateCode(std::ostream& os) const override
    {
        //os << "(";
        target->GenerateCode(os);
        os << "=";
        value->GenerateCode(os);
        //os << ")";
        return 0;
    }
};

struct VarDeclaration : AstNodeBase
{
    ExprType type;
    std::string varName;
public:
    VarDeclaration(ExprType type, std::string varName)
        : type(type), varName(varName)
    {}

    virtual bool Validate(INodeValidator& validator) const override
    {
        if (!SymbolTable::GetInstance().DeclareType(varName, type))
        {
            validator.Send(ENodeValidationMessageType::Error, "Variable already declared in this scope: " + varName);
            return false;
        }

        return true;
    }

    virtual ExprType GetType() const 
    {
        return type;
    }

    virtual int GenerateCode(std::ostream& os) const override
    {
        std::string typeName;
        switch (type.typeBase) 
        {
        case ExprTypeBase::INT:
            typeName = "int";
            break;
        case ExprTypeBase::FLOAT:
            typeName = "float";
            break;
        case ExprTypeBase::STRING:
            typeName = "std::string";
            break;
        case ExprTypeBase::BOOL:
            typeName = "bool";
            break;
        case ExprTypeBase::USER_DEFINED:
            typeName = type.userDefinedSymbol;
            break;
        default:
            throw std::runtime_error("Unknown var type");
        }

        os << typeName << " " << varName;
        return 0;
    }
};

struct ScopeBlockNode : AstNodeBase
{
    std::vector<std::shared_ptr<AstNodeBase>> statements;
public:
    ScopeBlockNode(std::vector<std::shared_ptr<AstNodeBase>> statements)
        : statements(statements)
    {}

    virtual bool Validate(INodeValidator& validator) const override
    {
        SymbolTable::GetInstance().EnterScope();

        for (const auto& statement : statements)
        {
            if (statement->Validate(validator))
                continue;

            SymbolTable::GetInstance().ExitScope();
            return false;
        }

        SymbolTable::GetInstance().ExitScope();
        return true;
    }

    virtual int GenerateCode(std::ostream& os) const override
    {
        os << "{" << std::endl;
        for (const auto& statement : statements)
        {
            os << "    ";
            statement->GenerateCode(os);
            os << ";" << std::endl;
        }
        os << "}";

        return 0;
    }
};

struct WhileNode : AstNodeBase
{
    std::shared_ptr<ExprNode> condition;
    std::shared_ptr<AstNodeBase> body;
public:
    WhileNode(std::shared_ptr<ExprNode> condition, std::shared_ptr<AstNodeBase> body)
        : condition(condition), body(body)
    {}

    virtual bool Validate(INodeValidator& validator) const override;
    virtual int GenerateCode(std::ostream& os) const override;
};

struct DoWhileNode : AstNodeBase
{
    std::shared_ptr<ExprNode> condition;
    std::shared_ptr<AstNodeBase> body;
public:
    DoWhileNode(std::shared_ptr<ExprNode> condition, std::shared_ptr<AstNodeBase> body)
        : condition(condition), body(body)
    {}

    virtual bool Validate(INodeValidator& validator) const override;
    virtual int GenerateCode(std::ostream& os) const override;
};

struct IfNode : AstNodeBase
{
    std::shared_ptr<ExprNode> condition;
    std::shared_ptr<AstNodeBase> thenBody;
    std::shared_ptr<AstNodeBase> elseBody; // nullptr if no else
public:
    IfNode(std::shared_ptr<ExprNode> condition,
           std::shared_ptr<AstNodeBase> thenBody,
           std::shared_ptr<AstNodeBase> elseBody)
        : condition(condition), thenBody(thenBody), elseBody(elseBody)
    {}

    virtual bool Validate(INodeValidator& validator) const override;
    virtual int GenerateCode(std::ostream& os) const override;
};

struct PrintFunctionNode : AstNodeBase
{
    std::vector<std::shared_ptr<ExprNode>> expressions;
public:
    PrintFunctionNode(std::vector<std::shared_ptr<ExprNode>> expressions)
        : expressions(expressions)
    {}

    virtual bool Validate(INodeValidator& validator) const override
    {
        for (const auto& expr : expressions)
        {
            if (expr->Validate(validator))
                continue;

            return false;
        }

        return true;
    }

    virtual int GenerateCode(std::ostream& os) const override
    {
        os << "std::cout";
        for(const auto& expr : expressions)
        {
            os << " << ";
            expr->GenerateCode(os);
        }
        os << " << std::endl";
        return 0;
    }
};