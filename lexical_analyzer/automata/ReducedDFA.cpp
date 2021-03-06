////
//// Created by ahmed on 16/03/18.
////
//
//#include <algorithm>
//#include <iostream>
//#include "ReducedDFA.h"
//#include "Utilities.h"
//
//
//
//Minimize_DFA::Minimize_DFA(std::vector<std::vector<std::pair<DFANode, EdgeLabel>>> dfa_graph,
//                           DFANode start_node) : start_node(start_node) {
//    Graph = dfa_graph;
//    set_up_bool_matrix();
//}
//
//void Minimize_DFA::set_up_bool_matrix()
//{
//
//
//    for(int x = 0 ; x <= Graph.size() ; x ++){
//        DFANode received_node  =  x == start_node.id ?  start_node : *get_dfa_node(Graph, x ) ;
//        State_Nodes.push_back(x);
//    }
//    ////////Brute Test
//
//
//
//
//
//
//
//    int rows = static_cast<int>(Graph.size());
//    int cols = static_cast<int>(Graph.size());
//    for(int x = 0 ; x < rows ; x++){
//        std::vector<bool> temp ;
//        temp.resize(cols,false);
//        Cells_to_be_marked.push_back(temp);
//    }
//    std::cout <<Cells_to_be_marked.size() << std::endl;
//    std::cout <<Cells_to_be_marked[0].size() << std::endl;
//}
//
//
//void Minimize_DFA::Myhill_Nerode()
//{
//    int previous_marked_count ;
//    do{
//        previous_marked_count = marked_count;
//        marked_count =  Myhill_Nerode_Iteration();
//    }while(marked_count != previous_marked_count );
//    determine_Final_Unmatched_States();
//}
//
//
//int Minimize_DFA::Myhill_Nerode_Iteration()
//{
//    if(marked_count == 0){
//        for (int i = 0 ; i < Graph.size() ; i++){
//            for(int j = 0 ; j < i ; j++){
//                if(Cells_to_be_marked[i][j] == false &&
//                   (int)State_Nodes[i].acceptance_state + (int)State_Nodes[j].acceptance_state == 1){
//                    Cells_to_be_marked[i][j] = true;
//                    marked_count++;
//                }
//            }
//        }
//    }
//    else{
//        for (int i = 0 ; i < Graph.size() ; i++){
//            for(int j = 0 ; j < i ; j++){
//                if(Cells_to_be_marked[i][j] == false &&
//                   is_one_a_terminal_state(i, j)){
//                    Cells_to_be_marked[i][j] = true;
//                    marked_count++;
//                }
//            }
//        }
//    }
//    return marked_count;
//}
//
//
//bool Minimize_DFA::is_one_a_terminal_state(int i, int j)
//{
//    std::vector<char> Common_Transitions;
//    std::vector<char> Transitions_i;
//    std::vector<char> Transitions_j;
//
//
//    for(int x = 0 ; x < Graph[i].size() ; x++){
//        char in1 = Graph[i][x].second;
//        Transitions_i.push_back(in1);
//    }
//    for(int y = 0 ; y < Graph[j].size() ; y++){
//        char in2 = Graph[j][y].second;
//        Transitions_j.push_back(in2);
//    }
//
//
//    std::sort(Transitions_i.begin(), Transitions_i.end());
//    std::sort(Transitions_j.begin(), Transitions_j.end());
//
//    std::set_intersection(Transitions_i.begin(), Transitions_i.end(),
//                          Transitions_j.begin(), Transitions_j.end(),
//                          std::back_inserter(Common_Transitions));
//
//    std::cout << "Transitions_i" << Transitions_i.size() << std::endl;
//    std::cout << "Transitions_j" << Transitions_j.size() << std::endl;
//    for(char x : Common_Transitions){
//        DFANode temp1 = *get_next_node(Graph,i , x);
//        DFANode temp2 = *get_next_node(Graph,j , x);
//        if( Cells_to_be_marked[temp1.id][temp2.id] == true ||
//            Cells_to_be_marked[temp2.id][temp1.id] == true ){
//
//            return true;
//        }
//    }
//    return false;
//}
//
//
//void Minimize_DFA::determine_Final_Unmatched_States()
//{
//    for (int i = 0 ; i < Graph.size() ; i++){
//        for(int j = 0 ; j < i ; j++){
//            if(Cells_to_be_marked[i][j] == false){
//                std::pair<int,int> temporary ;
//                temporary.first = i;
//                temporary.second = j;
//                Unmatched_States.push_back(temporary);
//            }
//        }
//    }
//}
//
//std::vector<std::vector<std::pair<DFANode,char>>> Minimize_DFA::Calculate_Minimum_DFA()
//{
//    Myhill_Nerode();
//    return merge_final_states();
//
//}
//
//std::vector<std::vector<std::pair<DFANode,char>>> Minimize_DFA::merge_final_states()
//{
//    //times previously merged is a second criterion that we employ to choose the dominant parent after
//    // the criterion of which one is a final state  .
//    std::vector<int> parent_Vector ;
//    std::vector<int> times_previously_merged ;
//    parent_Vector.resize(State_Nodes.size());
//    times_previously_merged.resize(State_Nodes.size());
//    Reduced_Graph.resize(Graph.size());
//
//    for(int x = 0 ; x < State_Nodes.size() ; x++ ){
//        parent_Vector[x] = x;
//        times_previously_merged[x] = 0;
//    }
//
//    for(int h : parent_Vector){
//        std::cout <<"Parent " <<h << std::endl;
//    }
//
//
//    for(int x = 0 ; x < Unmatched_States.size() ; x++){
//        int first = Unmatched_States[x].first;
//        int second = Unmatched_States[x].second;
//        if(!mergeNodes(Graph,first,second,&parent_Vector,&times_previously_merged)){
//            printf("Already have the same parent !");
//        }
//    }
//
//
//    for(int h : parent_Vector){
//        std::cout <<"Parent " <<h << std::endl;
//    }
//
//
//    for(int x = 0 ; x < Graph.size(); x++){
//        std::vector<std::pair<DFANode,char>> temporary = Graph[x];
//        Reduced_Graph[parent_Vector[x]].insert(Reduced_Graph[parent_Vector[x]].end()
//                , temporary.begin(), temporary.end());
//    }
//
//    //re-assigns the starting node after merge is completed
//
//    for(int x = 0 ; x < Reduced_Graph.size() ; x++){
//        for(int y = 0 ; y < Reduced_Graph[x].size() ; y++){
//            int parent_id_current = parent_Vector[Reduced_Graph[x][y].first.id];
//            DFANode temp = parent_id_current == start_node.id ?
//                           start_node :
//                           *get_dfa_node(Graph,parent_id_current);
//            if(Reduced_Graph[x][y].first.id == start_node.id)
//                start_node = temp;
//            Reduced_Graph[x][y].first = temp;
//        }
//    }
//
//
//    trim_redundant_states();
//
//
//
//    printf("Test Finished!");
//
//}
//
//
//
//void Minimize_DFA::trim_redundant_states()
//{
//    for(int x = 0 ; x < Reduced_Graph.size() ; x++){
//        while(trim_redundant_states_single_node(x) != 0);
//    }
//}
//
//int Minimize_DFA::trim_redundant_states_single_node(int index)
//{
//    int duplicates_found;
//    std::vector<char> set_duplicates;
//    std::vector<char> set_all_possible_inputs;
//    for(int x = 0 ; x < Reduced_Graph[index].size() ; x++){
//        if(std::find(set_all_possible_inputs.begin(),
//                     set_all_possible_inputs.end(),
//                     Reduced_Graph[index][x].second) != set_all_possible_inputs.end()) {
//
//            set_duplicates.push_back(Reduced_Graph[index][x].second);
//
//        } else {
//
//            set_all_possible_inputs.push_back(Reduced_Graph[index][x].second);
//        }
//    }
//
//    duplicates_found = set_duplicates.size();
//
//    for(char x : set_duplicates){
//        for(int m = 0 ; m < Reduced_Graph[index].size() ; m++){
//            if(Reduced_Graph[index][m].second == x){
//                Reduced_Graph[index].erase(Reduced_Graph[index].begin()+ m );
//                break;
//            }
//        }
//    }
//
//    return duplicates_found;
//
//}