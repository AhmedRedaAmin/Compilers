//
// Created by ahmed on 16/03/18.
//

#include "DFATransformer.h"
#include "Helper.h"
#include <stack>
#include <set>
#include <algorithm>
#include <iostream>
#include <regex>

using namespace std;

#define NODES_SZ 1000
#define DFA_NODES 300
#define EPSILON "$"


DFATransformer::DFATransformer(DFANode s_state)
    : starting_dfa_state(s_state)
{
    functional_id = 0;
    //vector<State> x;

   // starting_dfa_state = DFANode(x, false, false, false, -1);
}

DFANode* DFATransformer::get_dfa_node(int id)
{
    return id_to_node[id];
}

void DFATransformer::add_dfa_node(DFANode *node, int id)
{
    id_to_node[id] = node;
}

void DFATransformer::transform()
{
    dfa_graph.resize(DFA_NODES);
    vector<State> starting_nfa_state;
    starting_nfa_state.push_back(nfa_graph[0]);
    DFANode starting_nfa_node(starting_nfa_state, false, false, false, 0, "");
    DFANode starting_dfa_node = normal_transition(starting_nfa_node, EPSILON);
    starting_dfa_node.id = functional_id++;
    starting_dfa_state = starting_dfa_node;
    dfa_nodes.push_back(starting_dfa_node);
    vector<string> inputs = NFAGenerator::get_symbols();


    while (exist_unmarked_state(&dfa_nodes))
    {
        DFANode *current_dfa_node = get_unmarked_node(&dfa_nodes);
        DFANode sss = *current_dfa_node;
        current_dfa_node->marked = true;
        int node_id = current_dfa_node->id;
        for (string x : inputs)
        {
            DFANode result_node_without_eps = normal_transition(sss, x);
            DFANode dfa_state = normal_transition(result_node_without_eps, EPSILON);
            EdgeLabel e(x);
            if (!already_inserted_dfa_node(&dfa_state))
            {
                dfa_state.marked = false;
                dfa_state.id = functional_id++;
                dfa_nodes.push_back(dfa_state);
            }
            if (x == EPSILON) continue;
            dfa_graph[node_id].push_back({dfa_state, e});
        }
        set<string> upper_case_chars;
        set<string> lower_case_chars;
        for (auto transition : dfa_graph[node_id])
        {
            if (transition.second.get_input().length() == 1 && isalpha(transition.second.get_input()[0]))
            {
                if (isupper(transition.second.get_input()[0]))
                {
                    string tmp = "";
                    tmp.push_back(transition.second.get_input()[0]);
                    upper_case_chars.insert(tmp);
                }
                else
                {
                    string tmp = "";
                    tmp.push_back(transition.second.get_input()[0]);
                    lower_case_chars.insert(tmp);
                }

            }
        }
        for (int i = 0; i < dfa_graph[node_id].size(); i++)
        {
            string current_input = dfa_graph[node_id][i].second.get_input();
            Helper h;
            string expanded_string = h.normalize_classes(current_input);
            regex b("(.)-(.)");
            if(regex_match(current_input, b))
            {
                if (isalpha(current_input[0]))
                {
                    if (isupper(current_input[0]))
                    {
                        EdgeLabel e(dfa_graph[node_id][i].second.get_input());
                        for (auto it = upper_case_chars.begin(); it != upper_case_chars.end(); ++it)
                        {
                            string expanded_string = h.normalize_classes(current_input);
                            bool inside_region = expanded_string.find(*it) !=  string::npos;
                            if (inside_region)
                                e.discard_char(*it);
                        }
                        dfa_graph[node_id][i].second = e;
                    }
                    else
                    {
                        EdgeLabel e(dfa_graph[node_id][i].second.get_input());
                        for (auto it = lower_case_chars.begin(); it != lower_case_chars.end(); ++it)
                        {
                            string expanded_string = h.normalize_classes(current_input);
                            bool inside_region = expanded_string.find(*it) !=  string::npos;
                            if (inside_region)
                                e.discard_char(*it);
                        }
                        dfa_graph[node_id][i].second = e;
                    }
                }
            }
        }
    }
}

DFANode DFATransformer::normal_transition(DFANode dfa_state, string input)
{
    vector<State> dfa_trans;
    stack<State> stk_states;
    string acc_state_name = "";
    bool res_acceptance_state = false;
    for (State curr : dfa_state.dfa_state)
    {
        stk_states.push(curr);
        if (input == EPSILON) {
            dfa_trans.push_back(curr);
        }
        res_acceptance_state |= curr.is_acceptance_state();
    }
    while (!stk_states.empty())
    {
        State top = stk_states.top();
        stk_states.pop();
        for (pair<State, string> trans : *top.get_transitions())
        {
            // Here goes the check of character inside a region.
            bool inside_region = false;
            Helper h;
            regex b("(.)-(.)");
            if(regex_match(trans.second, b)) {
                string expanded_string = h.normalize_classes(trans.second);
                inside_region = expanded_string.find(input) !=  string::npos;
            }
            if (trans.second == input || inside_region)
            {
                if (!already_inserted(&dfa_trans, trans.first))
                {
                    res_acceptance_state |= trans.first.is_acceptance_state();
                    dfa_trans.push_back(nfa_graph[trans.first.get_state_number()]);
                    stk_states.push(nfa_graph[trans.first.get_state_number()]);
                }
            }
        }
    }
    for (auto curr : dfa_trans)
    {
        if (curr.is_acceptance_state())
        {
            acc_state_name = curr.get_acceptance_state_name();
            break;
        }
    }
    DFANode new_dfa_node(dfa_trans, res_acceptance_state, false, false, -1, acc_state_name);
    return new_dfa_node;
}

bool DFATransformer::already_inserted(vector<State> *vec, State s) {
    for (State x : *vec)
    {
        if (x.get_state_number() == s.get_state_number())
            return true;
    }
    return false;
}

void DFATransformer::set_nfa_graph(std::vector<State> nfa)
{
    this->nfa_graph = nfa;
}

bool DFATransformer::exist_unmarked_state(std::vector<DFANode> *dfa_combined_nodes)
{
    for (DFANode x : *dfa_combined_nodes)
    {
        if (!x.marked)
            return true;
    }
    return false;
}

DFANode *DFATransformer::get_unmarked_node(std::vector<DFANode> *dfa_combined_nodes)
{
    for (int i = 0; i < (*dfa_combined_nodes).size(); ++i)
    {
        if (!(*dfa_combined_nodes)[i].marked)
            return &((*dfa_combined_nodes)[i]);
    }
}

bool DFATransformer::already_inserted_dfa_node(DFANode *dfa_node)
{
    set<int> st2;
    for (State y : dfa_node->dfa_state)
    {
        st2.insert(y.get_state_number());
    }
    for (DFANode x : dfa_nodes)
    {
        set<int> st1;
        for (State y : x.dfa_state)
        {
            st1.insert(y.get_state_number());
        }
        if (st1 == st2)
        {
            dfa_node->id = x.id;
            return true;
        }
    }
    return false;
}

std::vector<DFANode> *DFATransformer::get_dfa_nodes() {
    return &dfa_nodes;
}

vector< vector< pair<DFANode, EdgeLabel> > > *DFATransformer::get_dfa_graph()
{
    return &dfa_graph;
}

int DFATransformer::get_dfa_graph_size() {
    return functional_id;
}

DFANode *DFATransformer::get_starting_dfa_state() {
    return &this->starting_dfa_state;
};

