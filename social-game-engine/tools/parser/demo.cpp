#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <assert.h>

#include <cpp-tree-sitter.h>
#include <Rules.h>

using namespace std;

extern "C" {
TSLanguage* tree_sitter_socialgaming();
}


//helper functions
void setArrayFromString(const std::string& str, int input[2]) {
  bool isSecondNum = false;
  for(char c: str)  {
    if (c == ',') isSecondNum = true;
    
    if (c <= '9' && c >= '0') {
      int intVal = std::stoi(std::string(1, c));
      if (!isSecondNum) input[0] = (10 * input[0]) + intVal;
      else input[1] = (10 * input[1]) + intVal;
    }
  }
}
//

// returning a string containing the tree from the root node
std::string getString(const ts::Node& node, std::string str_buffer) {
  ts::Extent range = node.getByteRange();
  return str_buffer.substr(range.start, range.end - range.start);
}

// TODO: make the tree traversal generic using some algorithm (BFS or DFS)
std::string getGameName(const ts::Tree& tree, std::string& str_buffer) {
  ts::Node node = tree.getRootNode();
  if(node.getChildByFieldName("configuration").isNull()) {
    cerr << "ERROR, no configuration node detected in the tree" << endl;
  
  } else {
    node = node.getChildByFieldName("configuration").getChildByFieldName("name");
    return getString(node, str_buffer);
  }
}

void getPlayerRangeAsArray(const ts::Tree& tree, int result[2], std::string& str_buffer) {
  ts::Node node = tree.getRootNode();
  if(node.getChildByFieldName("configuration").isNull()) 
    cerr << "ERROR, no configuration node detected in the tree" << endl;
  
  else {
    node = node.getChildByFieldName("configuration").getChildByFieldName("player_range");
    std::string rangeString = getString(node, str_buffer); // "(2,4)"
    setArrayFromString(rangeString, result);
  }
}

//helper print function
void printSubtreePreorder(const std::string& sourceCode, ts::Node node, std::string indent) {

  for (uint32_t i = 0; i < node.getNumNamedChildren(); i++) {
    ts::Node child = node.getNamedChild(i);
	//print child type
	std::cout << indent << "Type: " << child.getType() << std::endl;
	ts::Extent range = child.getByteRange();
	std::string expression = sourceCode.substr(range.start, range.end - range.start);
	//print actual string found in this section
	std::cout << indent << "expression: \t" << expression << std::endl;
	//pint divider
	std::cout << indent << "==============================" << std::endl;
	printSubtreePreorder(sourceCode, child, indent + "  ");
  }
}


//helper print function
void printArgs(std::vector<std::pair<std::string_view, ExpressionType>> args){
	std::cout << "[";
	for(auto s: args){
		std::cout << "(\'" << s.first << "\'|" << s.second << "),";
	}
	std::cout << "]\n";
}

/*
  This will be refactored to Game Manager object later
*/
void printRulesPreorder(const std::string_view& sourceCode, ts::Node node, std::string indent) {
  // Get each child's type and value under Rules

  for (uint32_t i = 0; i < node.getNumNamedChildren(); i++) {
    ts::Node child = node.getNamedChild(i);
	//Calls extract expression on every expression subtree it finds, but doesn't recurse into it
	if(child.getType() == "expression") {
		std::cout << child.getType() << std::endl;
		std::string_view expression = child.getSourceRange(sourceCode);
		std::cout << expression << std::endl;
		vector<std::pair<std::string_view, ExpressionType>> args = extractExpression(sourceCode, child);
		printArgs(args);
		std::cout << "==============================" << std::endl;
	}
	else{
    	printRulesPreorder(sourceCode, child, indent + "  ");
	}
  }
}

void executeGameRules(const std::vector<std::unique_ptr<BaseRule>>& rules) {
  for (const auto& rule : rules) {
	//Execute now takes a game instance as an argument.
    //rule->execute();
  }
}

int main() {
  std::cout << "hello\n";
  // Create a language and parser.
  ts::Language language = tree_sitter_socialgaming();
  ts::Parser parser{language};

  // Open file containing RPS configurations
  std::ifstream ifs("rock-paper-scissors.game");
  std::stringstream buffer;
  buffer << ifs.rdbuf();
  std::string sourcecode = buffer.str();
  ifs.close();

  // Parse the provided string into a syntax tree.
  ts::Tree tree = parser.parseString(sourcecode);

  // Get the root node of the syntax tree. 
  ts::Node root = tree.getRootNode();
  ts::Node config_node = root.getChildByFieldName("configuration");
  // printRulesPreorder(sourcecode, config_node, "");

  std::cout << "Number of named children from the root: " << root.getNumNamedChildren() << std::endl;
  for (uint32_t i = 0; i < root.getNumChildren(); i++) {
    ts::Node child = root.getNamedChild(i);
    if (child.getType() == "rules") {
      //std::cout << child.getType() << ":" << std::endl;
      //printRulesPreorder(sourcecode, child, "");
      //printSubtreePreorder(sourcecode, child, "");
      Rules game_rules{child, sourcecode};
      const std::vector<std::unique_ptr<BaseRule>>& rules = game_rules.getGameRules();
      std::cout << "Rules: " << rules.size() << std::endl;
      // executeGameRules(rules);
    }
  }

  // Print the syntax tree as an S-expression.
  auto treestring = root.getSExpr();
  //printf("Syntax tree: %s\n", treestring.get());

  return 0;
}
