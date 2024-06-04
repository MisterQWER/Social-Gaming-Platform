
#include "Common.h"


void NodeOutputDecorator::printChildren(const ts::Node * node_ptr) {
    ts::Cursor node_cursor = node_ptr->getCursor();
    bool has_children = node_cursor.gotoFirstChild();
    while(has_children) {
        ts::Node node = node_cursor.getCurrentNode();
        std::cout << "--- CHILD ---- \n";
        NodeOutputDecorator::printNodeInfo(&node);
        has_children = node_cursor.gotoNextSibling();
    }
}

std::string NodeOutputDecorator::getNodeString(const ts::Node * node_ptr, const std::string& str_buffer) {
    ts::Extent range = node_ptr->getByteRange();
    return str_buffer.substr(range.start, range.end - range.start);
}

void NodeOutputDecorator::printNodeInfo(const ts::Node * node_ptr) {
    std::cout << "Type = " << node_ptr->getType() << '\n';
    std::cout << "Number of children = " << node_ptr->getNumChildren() << '\n';
    std::cout << "Number of named children = " << node_ptr->getNumNamedChildren() << '\n';
    std::cout << "Symbol = " << node_ptr->getSymbol() << '\n'; 
}


std::vector<int> NumberRange::getRange(const ts::Node * node_ptr, const std::string& str_buffer) {
    std::vector<int> nums;
    ts::Cursor cursor = node_ptr->getCursor();
    cursor.gotoFirstChild();
    do {
        ts::Node current_node = cursor.getCurrentNode();
        if(current_node.isNull() == false && current_node.getType() == "number") {
            int num = extract_int(&current_node, str_buffer);
            nums.push_back(num);
        }
    } while (cursor.gotoNextSibling());

    return nums;
}

int extract_int(const ts::Node * node, const std::string& source_code) {
    std::string str = NodeOutputDecorator::getNodeString(node, source_code);
    return std::stoi(str);
}

bool extract_boolean(const ts::Node * node, const std::string& source_code) {
    std::string str = NodeOutputDecorator::getNodeString(node, source_code);
    std::string str_lower = str;
    std::transform(str_lower.begin(), str_lower.end(), str_lower.begin(), [](unsigned char c) {
        return tolower(c);
    });
    if(str_lower == "false") {
        return false;
    }
    else if(str_lower == "true") {
        return true;
    }
    // Return false on inparsible str_val (might need to do assert)
        return false;
}

std::string extract_string(const ts::Node * node, const std::string& source_code) {
    return NodeOutputDecorator::getNodeString(node, source_code);
}

//Expression extraction methods

ExpressionType 
getExpressionType(const ts::Node& node){
	//only for named expressions
	std::string_view type = node.getType();
	if(type == "identifier"){
		return ExpressionType::identifier;
	}
	else if(type == "boolean"){
		return ExpressionType::boolean;
	}
	else if(type == "number"){
		return ExpressionType::number;
	}
	else if(type == "quoted_string"){
		return ExpressionType::quotedString;
	}
	else if(type == "builtin"){
		return ExpressionType::builtin;
	}
	else if(type == "expression"){
		return ExpressionType::expression;
	}
	else{
		return ExpressionType::unknown;
	}
}

std::vector<std::pair<std::string_view, ExpressionType>> 
extractArgumentList(const std::string_view& sourceCode, ts::Node node){
	//expects the node passed in to be of an argument_list type
	assert(node.getType() == "argument_list");
	std::vector<std::pair<std::string_view, ExpressionType>> args;
	args.emplace_back("(", ExpressionType::beginArg);
	uint32_t numOfChildren = node.getNumNamedChildren();
	if(numOfChildren == 0){
		args.emplace_back(")", ExpressionType::endArg);
		return args;
	}
	else{
		assert(numOfChildren == 1);
		ts::Node expressionList = node.getNamedChild(0);
		for(uint32_t i = 0; i< expressionList.getNumNamedChildren();i++){
			ts::Node child = expressionList.getNamedChild(i);
			std::vector<std::pair<std::string_view, ExpressionType>> resolvedExpression = extractExpression(sourceCode, child);
			for(auto a: resolvedExpression){
				args.emplace_back(a);
			}
		}
		args.emplace_back(")", ExpressionType::endArg);
		return args;
	}
}

std::vector<std::pair<std::string_view, ExpressionType>> 
extractExpression(const std::string_view& sourceCode, ts::Node node){
	std::vector<std::pair<std::string_view, ExpressionType>> args;

	ts::Cursor cursor = node.getCursor();
	assert(cursor.gotoFirstChild());


	ts::Node child = cursor.getCurrentNode();
	//loop through all children to process arugments with syntax
	while(!child.isNull()){
		if(child.isNamed()){
			
			ExpressionType type = getExpressionType(child);
			if(type == ExpressionType::expression){
				//recursive call here
				std::vector<std::pair<std::string_view, ExpressionType>> expressionVals = extractExpression(sourceCode, child);
				for(auto a: expressionVals){
					args.emplace_back(a);
				}
			}
			else if(child.getType() == "argument_list") {
				//argument list special case
				std::vector<std::pair<std::string_view, ExpressionType>> argumentList = extractArgumentList(sourceCode, child);
				for(auto a: argumentList){
					args.emplace_back(a);
				}
			}
			else{
				//if it reaches here, this means that its either a number, quoted_string, builtin function name or an identifier)
				std::string_view expression = child.getSourceRange(sourceCode);
				args.emplace_back(expression, type);
			}
		}
		else{
			// special case for syntax
			std::string_view expression = child.getSourceRange(sourceCode);
			if(expression != "."){
				args.emplace_back(expression, ExpressionType::syntax);
			}
		}
		child = child.getNextSibling();
	}
	return args;
}

