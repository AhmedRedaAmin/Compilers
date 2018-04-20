//
// Created by ahmed on 24/03/18.
//

#ifndef LEXICAL_ANALYZER_TOKENIZER_H
#define LEXICAL_ANALYZER_TOKENIZER_H

#include "DFANode.h"
#include "State.h"
#include <vector>
#include <stack>
#include "DFATransformer.h"
#include "Helper.h"

class Tokenizer
{
    private:
        std::vector< std::vector< std::pair<DFANode, string> > > dfa_graph;
        std::string character_pool;
        std::vector<std::string> tokens;
        stack<DFANode> stk_node;
        DFANode *starting_state;
        DFANode *current_state;
        DFATransformer tr;
    public:
        Tokenizer(DFANode st_state, DFATransformer dfa_trans, std::vector< std::vector< std::pair<DFANode, string> > > dfa);
        void tokenize(string input_line);
        void set_starting_state(DFANode *st_state);
       // void set_dfa_graph(std::vector< std::vector< std::pair<DFANode, string> > > *dfa);
};

#endif //LEXICAL_ANALYZER_TOKENIZER_H