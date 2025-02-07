#include <bits/stdc++.h>
using namespace std;

int** graph;
int vertex_count;
set<int> initial_vertex_set;

int local_search_iteration_count = 0;

int local_search_val_randomized = 0, local_search_val_greedy = 0, local_search_val_semi_greedy = 0;

int local_search_val = 0;

class Edge{
    int from, to, weight;
public:
    Edge(int from, int to, int weight){
        this->from = from;
        this->to = to;
        this->weight = weight;
    }
    int get_from(){
        return from;
    }
    int get_to(){
        return to;
    }
    int get_weight(){
        return weight;
    }
};

void init_graph(int vertices){
    graph = new int*[vertices+1];
    for(int i=0;i<=vertices;i++){
        graph[i] = new int[vertices+1];
    }
    for(int i=0;i<=vertices;i++){
        for(int j=0;j<=vertices;j++){
            graph[i][j] = INT_MIN;
        }
    }

    for(int i=1;i<vertex_count;i++){
        initial_vertex_set.insert(i);
    }
}

int get_minimum_weight(){
    int min_weight = INT_MAX;

    for(int i=1;i<vertex_count;i++){
        for(int j=1;j<vertex_count;j++){
            if(graph[i][j] < min_weight && graph[i][j] != INT_MIN){
                min_weight = graph[i][j];
            }
        }
    }
    return min_weight;
}

int get_maximum_weight(){
    int max_weight = INT_MIN;

    for(int i=1;i<vertex_count;i++){
        for(int j=1;j<vertex_count;j++){
            if(graph[i][j] > max_weight){
                max_weight = graph[i][j];
            }
        }
    }
    return max_weight;
}

double generate_random(){
    srand(time(0));
    return (double)rand() / RAND_MAX;
}

bool match_vertex_list(set<int> vertex_list){
    if(vertex_list.size() == initial_vertex_set.size()){
        return true;
    }
    return false;
}

int sigma_x(int vertex, set<int> vertex_set){
    int sigma_x_value = 0;
    set<int>::iterator it;
    for(it = vertex_set.begin(); it != vertex_set.end(); it++){
        if(graph[vertex][*it] != INT_MIN){
            sigma_x_value += graph[vertex][*it];
        }
        else{
            sigma_x_value += 0;
        }
    }
    return sigma_x_value;
}

int sigma_y(int vertex, set<int> vertex_set){
    int sigma_y_value = 0;
    set<int>::iterator it;
    for(it = vertex_set.begin(); it != vertex_set.end(); it++){
        if(graph[vertex][*it] != INT_MIN){
            sigma_y_value += graph[vertex][*it];
        }
        else{
            sigma_y_value += 0;
        }
    }
    return sigma_y_value;
}

void output_vertex_set(set<int> vertex_set){
    set<int>::iterator it;
    for(it = vertex_set.begin(); it != vertex_set.end(); it++){
        cout << *it << " ";
    }
    cout << endl;
}

void output_vertex_map(map<int, int> map){
    auto it = map.begin();
    for(it = map.begin(); it != map.end(); it++){
        cout << "The key of the map is " << it->first << " and the value of the map is " << it->second << endl;
    }
}

int calculate_cut_weight(set<int> vertex_set_1, set<int> vertex_set_2){
    int cut_weight = 0;
    set<int>::iterator it;
    for(it = vertex_set_1.begin(); it != vertex_set_1.end(); it++){
        set<int>::iterator it2;
        for(it2 = vertex_set_2.begin(); it2 != vertex_set_2.end(); it2++){
            if(graph[*it][*it2] != INT_MIN){
                cut_weight += graph[*it][*it2];
            }
        }
    }
    return cut_weight;
}

set<int> randomized_MaxCut(){
    set<int>::iterator it;
    set<int> vertex_set_1;
    set<int> vertex_set_2;
    
    for(it = initial_vertex_set.begin(); it != initial_vertex_set.end(); it++){
        int random_number = random() % 2;
        if(random_number == 0){
            vertex_set_1.insert(*it);
        }
        else{
            vertex_set_2.insert(*it);
        }
    }
    return vertex_set_1;
}


set<int> greedy_semi_greedy_maxcut(double parameter){
    int minimum_weight = get_minimum_weight();
    int maximum_weight = get_maximum_weight();

    int threshold = minimum_weight + parameter * (maximum_weight - minimum_weight);

    vector<Edge> edge_candidate_list;
    for(int i=1;i<vertex_count;i++){
        for(int j=1;j<vertex_count;j++){
            if(graph[i][j] >= threshold){
                edge_candidate_list.push_back(Edge(i, j, graph[i][j]));
            }
        }
    }

    set<int> vertex_set_1;
    set<int> vertex_set_2;

    shuffle(edge_candidate_list.begin(), edge_candidate_list.end(), default_random_engine(time(0)));

    int index = random() % edge_candidate_list.size();
    Edge edge = edge_candidate_list[index];
       
    vertex_set_1.insert(edge.get_from());
    vertex_set_2.insert(edge.get_to());

    set<int> included_vertex_set;

    set<int> excluded_vertex_set = initial_vertex_set;

    included_vertex_set.insert(edge.get_from());
    included_vertex_set.insert(edge.get_to());
    excluded_vertex_set.erase(edge.get_from());
    excluded_vertex_set.erase(edge.get_to());

    while(!match_vertex_list(included_vertex_set)){
        map<int, int> sigma_x_value;
        map<int, int> sigma_y_value;

        set<int>::iterator it;
        for(it = excluded_vertex_set.begin(); it != excluded_vertex_set.end(); it++){
            sigma_x_value.insert(pair<int, int>(*it, sigma_x(*it, vertex_set_2)));
            sigma_y_value.insert(pair<int, int>(*it, sigma_y(*it, vertex_set_1)));
        }

        int minimum, maximum;

        minimum = min(min_element(sigma_x_value.begin(), sigma_x_value.end())->second, min_element(sigma_y_value.begin(), sigma_y_value.end())->second);
        maximum = max(max_element(sigma_x_value.begin(), sigma_x_value.end())->second, max_element(sigma_y_value.begin(), sigma_y_value.end())->second);

        int threshold = minimum + parameter * (maximum - minimum);

        vector<int> vertex_candidate_list;

        for(it = excluded_vertex_set.begin(); it != excluded_vertex_set.end(); it++){
            if(max(sigma_x_value[*it] , sigma_y_value[*it]) >= threshold){
                vertex_candidate_list.push_back(*it);
            }
        }

        shuffle(vertex_candidate_list.begin(), vertex_candidate_list.end(), default_random_engine(time(0)));

        int index = random() % vertex_candidate_list.size();

        int vertex = vertex_candidate_list[index];

        included_vertex_set.insert(vertex);
        excluded_vertex_set.erase(vertex);

        if(sigma_x_value[vertex] > sigma_y_value[vertex]){
            vertex_set_1.insert(vertex);
        }
        else{
            vertex_set_2.insert(vertex);
        }
    }

    return vertex_set_1;
}

set<int> localSearch(set<int> vertex_set_1, set<int> vertex_set_2){
    bool change = true;

    while(change){
        change = false;
        set<int>::iterator it;
        for(it = initial_vertex_set.begin(); it != initial_vertex_set.end(); it++){
            if(vertex_set_1.find(*it) != vertex_set_1.end() && (sigma_x(*it, vertex_set_1) - sigma_y(*it, vertex_set_2)) > 0){
                vertex_set_1.erase(*it);
                vertex_set_2.insert(*it);
                change = true;
                local_search_iteration_count++;
            }
            else{
                if(vertex_set_2.find(*it) != vertex_set_2.end() && (sigma_y(*it, vertex_set_2) - sigma_x(*it, vertex_set_1)) > 0){
                    vertex_set_2.erase(*it);
                    vertex_set_1.insert(*it);
                    change = true;
                    local_search_iteration_count++;
                }
            }
        }   
    }
    return vertex_set_1;
}

set<int> calculate_complement_set(set<int> vertex_set){
    set<int> complement_set;
    set<int>::iterator it;
    for(it = initial_vertex_set.begin(); it != initial_vertex_set.end(); it++){
        if(vertex_set.find(*it) == vertex_set.end()){
            complement_set.insert(*it);
        }
    }
    return complement_set;
}

int generate_randomized_maxcut_result(int iteration){

    int best_value = 0;

    set<int> resultSet;
    set<int> complementSet;
    for(int i=0;i<iteration;i++){
        cout << "Randomized maxcut iteration: " << i << endl;
        resultSet = randomized_MaxCut();
        complementSet = calculate_complement_set(resultSet);
        int cut_weight = calculate_cut_weight(resultSet, complementSet);

        resultSet = localSearch(resultSet, complementSet);
        complementSet = calculate_complement_set(resultSet);
        int cut_weight_local_search = calculate_cut_weight(resultSet, complementSet);

        local_search_val += cut_weight_local_search;

        best_value += cut_weight;   
    }
    return best_value/iteration;
}

int generate_local_search_result(int iteration){
    int result = local_search_val/(3*iteration);
    return result;
}

int GRASP(int iteration){
    int best_value = INT_MIN;

    set<int> returnSet;

    for(int i=0;i<iteration;i++){
        cout << "GRASP iteration: " << i << endl;
        returnSet = greedy_semi_greedy_maxcut(generate_random());

        set<int> complementSet = calculate_complement_set(returnSet);

        returnSet = localSearch(returnSet, complementSet);

        complementSet = calculate_complement_set(returnSet);

        int cut_weight = calculate_cut_weight(returnSet, complementSet);

        if(cut_weight > best_value){
            best_value = cut_weight;
        }
    }

    return best_value;
}

int generate_randomized_semi_greedy_maxcut_result(int iteration, double parameter){

    int best_value = 0;

    set<int> resultSet;
    set<int> complementSet;
    for(int i=0;i<iteration;i++){
        if(parameter == 1){
            cout << "Randomized greedy maxcut iteration: " << i << endl;
        }
        else{
            cout << "Randomized semi-greedy maxcut iteration: " << i << " with parameter: " << parameter << endl;
        }
        cout << "Randomized semi-greedy maxcut iteration: " << i << " with parameter: " << parameter << endl;
        resultSet = greedy_semi_greedy_maxcut(parameter);
        complementSet = calculate_complement_set(resultSet);
        int cut_weight = calculate_cut_weight(resultSet, complementSet);

        resultSet = localSearch(resultSet, complementSet);
        complementSet = calculate_complement_set(resultSet);
        int cut_weight_local_search = calculate_cut_weight(resultSet, complementSet);

        local_search_val += cut_weight_local_search;

        best_value += cut_weight;   
    }
    return best_value/iteration;
}


string removeExtension(const std::string& filename) {
    size_t lastDotPos = filename.find_last_of(".");
    if (lastDotPos != std::string::npos) {
        return filename.substr(0, lastDotPos);
    }
    return filename;
}

string extractFilename(const std::string& filepath) {
    size_t lastSlashPos = filepath.find_last_of("/");
    size_t lastDotPos = filepath.find_last_of(".");
    
    if (lastSlashPos != std::string::npos && lastDotPos != std::string::npos) {
        return filepath.substr(lastSlashPos + 1, lastDotPos - lastSlashPos - 1);
    }
    
    return filepath;
}


int main(int argc, char** argv){

    ofstream file("Assignment_output.csv", ios::out | ios::app);

    fstream input;
    input.open(argv[1], ios::in);
    int vertices, edges;
    input >> vertices >> edges;
    vertex_count = vertices+1;
    init_graph(vertices);
    for(int i=0;i<edges;i++){
        int u, v, weight;
        input >> u >> v >> weight;
        graph[u][v] = weight;
        graph[v][u] = weight;
    }

    int iteration = 1;

    vector<int> values;

    values.push_back(generate_randomized_maxcut_result(iteration));
    values.push_back(generate_randomized_semi_greedy_maxcut_result(iteration, 1.0));
    values.push_back(generate_randomized_semi_greedy_maxcut_result(iteration, generate_random()));
    values.push_back(generate_local_search_result(iteration));

    int average_iteration = local_search_iteration_count/(3*iteration);

    values.push_back(GRASP(iteration));

    file << extractFilename(argv[1]) 
         << "," 
         << vertices 
         << "," 
         << edges 
         << "," 
         << values[0] 
         << "," 
         << values[1] 
         << "," 
         << values[2] 
         << "," 
         << average_iteration 
         << "," 
         << values[3] 
         << "," 
         << iteration 
         << "," 
         << values[4]
         << "," 
         << *max_element(values.begin(), values.end())
         << endl;
}