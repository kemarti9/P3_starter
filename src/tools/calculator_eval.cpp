// src/tools/calculator_eval.cpp — YOU IMPLEMENT (spec §4.6).
//
// calc_evaluate: evaluate a well-formed token list with two stacks
// (one for numbers, one for operators and '('). See spec §4.6 and the
// worked trace in Appendix C. std::stack or std::vector are both fine.

#include "tools/calculator.h"
#include <vector>

namespace {
    int precedence(CalcToken::Kind k) {
        if (k==CalcToken::Kind::Times ||
            k==CalcToken::Kind::Divide) return 2;
        if (k==CalcToken::Kind::Plus ||
            k==CalcToken::Kind::Minus) return 1;
        return 0;
    }
bool is_operator(CalcToken::Kind k) {
    return precedence(k) > 0;
}
bool apply_top(std::vector<double>& values,
                std::vector<CalcToken::Kind>& ops,
                std::string* error) {
    if (ops.empty() || values.size() < 2) {
        *error = "malformed expression";
        return false;
    }

    CalcToken::Kind op = ops.back();
    ops.pop_back();
    double right = values.back();
    values.pop_back();
    double left = values.back();
    values.pop_back();
    double result = 0.0;
    if (op == CalcToken::Kind::Plus) {
        result = left + right;
    } else if (op == CalcToken::Kind::Minus) {
        result = left - right;
    } else if (op == CalcToken::Kind::Times) {
        result = left * right;
    } else if (op == CalcToken::Kind::Divide) {
        if (right == 0.0) {
            *error = "division by zero";
            return false;
        } 
        result = left/right;
    } else {
        *error = "malformed expression";
        return false;
    }
    values.push_back(result);
    return true;
}
}

bool calc_evaluate(const std::vector<CalcToken>& tokens,
        double* out, std::string* error) {
    if (tokens.empty()) {
        *error = "empty expression";
        return false;
    }

    std::vector<double> values;
    std::vector<CalcToken::Kind> ops;

    for (const CalcToken& tok : tokens) {
        if (tok.kind == CalcToken::Kind::Number) {
            values.push_back(tok.value);
        } else if (tok.kind == CalcToken::Kind::LParen) {
            ops.push_back(tok.kind);
        } else if (tok.kind == CalcToken::Kind::RParen) {
            while (!ops.empty() && ops.back() != CalcToken::Kind::LParen) {
            if (!apply_top(values, ops, error))
                return false;
            }
        if (ops.empty()) {
            *error = "malformed expression" ;
            return false;
        }
        ops.pop_back(); 
    } else if (is_operator(tok.kind)) {
        while (!ops.empty() && is_operator(ops.back()) &&
        precedence(ops.back()) >= precedence(tok.kind)) {
            if (!apply_top(values, ops, error)) return false;
        }
        ops.push_back(tok.kind);
    }
}

while (!ops.empty()) {
    if (ops.back() == CalcToken::Kind::LParen) {
        *error = "malformed expression";
        return false;
    }
    if (!apply_top(values, ops, error)) return false;
}
    if (values.size() != 1) {
        *error="malformed expression";
        return false;
    }
    *out = values.back();
    return true;
}
