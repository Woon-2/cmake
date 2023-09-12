#include <iostream>
#include <vector>
#include <memory>

#include "../inc/onehot.h"

template <class Enum, class Integer = int>
constexpr decltype(auto) etoi(Enum e)
{
    return static_cast<Integer>(e);
}

class BehaviorTree
{
public:
    enum class Result
    {
        ONEHOT_ENCODE(
            success, failure, running,
            invalid, error
        )
    };
    
    static bool isResultUnfinished(Result res);

    class Sequence;
    class Select;

private:
    class INode_;
    class Composite_;
    
public:
    using Action = INode_;
};

bool BehaviorTree::isResultUnfinished(Result val)
{
    return etoi(val) & (etoi(Result::running) |
        etoi(Result::invalid) | etoi(Result::error));
}


class BehaviorTree::INode_
{
public:
    virtual Result run() = 0;
    virtual ~INode_() = 0 {}
};

class BehaviorTree::Composite_
    : public INode_
{
protected:
    decltype(auto) begin()
    {
        return children.begin();
    }
    
    decltype(auto) begin() const
    {
        return children.begin();
    }
    
    decltype(auto) end()
    {
        return children.end();
    }
    
    decltype(auto) end() const
    {
        return children.end();
    }

private:
    template <class T>
    using Ptr = std::shared_ptr<T>;
    
    template <class T>
    using Cont = std::vector<T>;
    
    Cont<Ptr<INode_>> children;
};

class BehaviorTree::Sequence
    final : public Composite_
{
public:
    Result run() override
    {
        for (auto child : *this)
        {
            auto res = child->run();
            
            if (BehaviorTree::isResultUnfinished(
                    res))
            {
                return res;
            }
            
            if (Result::failure == res)
            {
                return Result::failure;
            }
        }
        
        return Result::success;
    }
};

class BehaviorTree::Select
    final : public Composite_
{
public:
    Result run() override
    {
        for (auto child : *this)
        {
            auto res = child->run();
            
            if (BehaviorTree::isResultUnfinished(
                    res))
            {
                return res;
            }
            
            if (Result::success == res)
            {
                return Result::success;
            }
        }
        
        return Result::failure;
    }
};

int main()
{
    //__genOneHotEncodeMacros(64, IntegerSuffix{"ull"});
    
    std::cout << etoi(BehaviorTree::Result::success) << '\n';
    std::cout << etoi(BehaviorTree::Result::failure) << '\n';
    std::cout << etoi(BehaviorTree::Result::running) << '\n';
    std::cout << etoi(BehaviorTree::Result::invalid) << '\n';
    std::cout << etoi(BehaviorTree::Result::error) << '\n';
    
    std::cout << __ONEHOT_NUMARGS(1,2,3,4,5) << '\n';

    std::cin.get();
}