#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <cstdio>
#include <memory>
#include <string>
#include <algorithm>
#include <string>
#include <cpp-tree-sitter.h>
#include <vector>
#include <assert.h>

#pragma once

using namespace std;

extern "C" {
    TSLanguage* tree_sitter_socialgaming();
}

class NodeOutputDecorator {
    public:
        static void printChildren(const ts::Node * node_ptr);
        static std::string getNodeString(const ts::Node * node_ptr, const std::string& str_buffer);
        static void printNodeInfo(const ts::Node * node_ptr);

    private:
};


class NumberRange {
    public:
        static std::vector<int> getRange(const ts::Node * _node_ptr, const std::string& str_buffer);
    
    private:
};

/*
    For extracting data types from leaf nodes. The 
    idea is to only extract the values of these nodes.

    Expected values include:
    - numbers
    - booleans
    - strings
    - (possibly) enum values
*/
enum class DataType {
    Int,
    Boolean,
    String,
};


// Extracting values from leafs
int extract_int(const ts::Node * node, const std::string& source_code);
bool extract_boolean(const ts::Node * node, const std::string& source_code);
std::string extract_string(const ts::Node * node, const std::string& source_code);

// Enum for identifying what data type is in the string extracted in the pair

enum ExpressionType{
	identifier,
	number,
	quotedString,
	builtin,
	expression,
	boolean,
	syntax,
	beginArg,
	endArg,
	unknown
};

// Functions to extract expression nodes
ExpressionType getExpressionType(const ts::Node& node);

std::vector<std::pair<std::string_view, ExpressionType>> extractExpression(const std::string_view& sourceCode, ts::Node node);

std::vector<std::pair<std::string_view, ExpressionType>> extractArgumentList(const std::string_view& sourceCode, ts::Node node);

#endif