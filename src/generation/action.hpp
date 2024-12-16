#pragma once

enum class Action {
    Operation,
    Element,
    Input,
    Output,
    Index,
    Goto,
    FalseGoto,
    FunctionCall,
    PushArray,
    Array,
    SetScope
};