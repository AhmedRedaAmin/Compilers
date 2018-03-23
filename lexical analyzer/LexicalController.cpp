//
// Created by ahmed on 16/03/18.
//

#include "LexicalController.h"

#include <string>
#include <vector>
#include <iostream>

using namespace std;

void Lexical_controller::run_(const string grammar_rule_file, const string src_program_file)
{
    NFAGenerator generator;
    //for ()
//    {
        generator.generate_grammar("id=(a|b)*.a.b.b");

//    }
    NFA machine = generator.generate_machine();

    // Contains NFA-states
    vector<State> states = (*machine.get_states());

//    vector<State> tmp_state;
//    tmp_state.push_back(states[0]);
//    tmp_state.push_back(states[6]);
//
//
//    DFANode dfa_node(tmp_state, false, false, false, 0);

    DFATransformer transformer;
    transformer.set_nfa_graph(states);
    transformer.transform();
    //DFANode res = transformer.normal_transition(&dfa_node, 'a');
    vector<DFANode> tmp = *transformer.get_dfa_nodes();
    for (DFANode x : tmp)
    {
        cout << "New Dfa node id: " << x.id << "  consists of states: ";
        for (State y : x.dfa_state)
            cout << y.get_state_number() << " ";

        cout << endl;
    }
//    cout << "States: ";
//    for (State curr : res.dfa_state)
//    {
//        cout << curr.get_state_number() << " " << "acceptance: " << curr.is_acceptance_state() << endl;
//    }
//    cout << "\nacceptance state: " << res.acceptance_state << endl;


//    for (int i = 0; i < states.size(); i++) {
//        vector<pair <State, char>> transitions = *states[i].get_transitions();
//        for (int j = 0; j < transitions.size(); j++) {
//            cout << states[i].get_state_number() << " " << transitions[j].first.get_state_number()
//                 << " " << transitions[j].second << endl;
//        }
//    }
}