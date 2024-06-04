#ifndef BASERULE_H
#define BASERULE_H

#include <string_view>
#include <optional>
#include <iostream>
#include <variant>

#include "Common.h"
#include "Action.hpp"
#include "BaseEnvironment.hpp"
#include "GameInstance.hpp"

using StringExpressionTypePair = std::pair<std::string_view, ExpressionType>;

// Forward Declaration
class BaseRule;

// Defining a Visitor
class RuleVisitor;

// Forward Declaring the other Rules for RuleVisitor
class Assignment;
class Discard;
class Extend;
class For;
class InputChoice;
class Match;
class Message;
class ParallelFor;
class Scores;
class RuleContainer;

/*
    A Visitor for Rules

    - Takes a copy of the given rule, DOES NOT allow modificiation of the original 
    value
*/
class RuleVisitor {
    public:
        ~RuleVisitor() = default;
        virtual void visit(Assignment assignment_rule) = 0;
        virtual void visit(Discard discard_rule) = 0;
        virtual void visit(Extend extend_rule) = 0;
        virtual void visit(For for_rule_rule) = 0;
        virtual void visit(InputChoice input_choice_rule) = 0;
        virtual void visit(Match match_rule) = 0;
        virtual void visit(Message message_rule) = 0;
        virtual void visit(ParallelFor parallel_for_rule) = 0;
        virtual void visit(Scores scores_rule) = 0;
};

class BaseRule {
    public:
        virtual uintptr_t GetType() = 0;
        virtual void accept(RuleVisitor& visitor) const = 0;
        virtual std::optional<std::vector<std::shared_ptr<BaseRule>>> GetSubRules() const = 0;
        virtual void addEventsTo(GameInstance& game) = 0;
        virtual ~BaseRule() = default;                                 // Make base class polymorphic
};

#endif
