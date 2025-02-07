#include <bits/stdc++.h>
using namespace std;

int** correct_matrix, n;

int hamming_distance(int** matrix){
    int hamming_distance = 0;;
    for(int i=0;i<n;i++){
        for(int j =0;j<n;j++){
            if(matrix[i][j] != 0 && matrix[i][j] != correct_matrix[i][j]){
                hamming_distance++;
            }
        }
    }
    return hamming_distance;
}

int manhattan_distance(int** matrix){
    int manhattan_distance = 0;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(matrix[i][j] != 0 && matrix[i][j] != correct_matrix[i][j]){
                if(matrix[i][j] % n == 0){
                    manhattan_distance += (abs((i+1)-(matrix[i][j]/n)) + abs((j+1)-n));
                }
                else{
                    manhattan_distance += (abs((i+1)-((matrix[i][j]/n)+1)) + abs((j+1)-(matrix[i][j]%n)));
                }
            }
        }
    }
    return manhattan_distance;
}

int inversion_count(int** matrix){
    int* array = new int[n*n-1];
    int k =0;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(matrix[i][j] != 0){
                array[k] = matrix[i][j];
                k++;
            }
        }
    }
    int inversion_count = 0;
    for(int i=0;i<n*n-1;i++){
        for(int j=i+1;j<n*n-1;j++){
            if(array[i] > array[j]){
                inversion_count++;
            }
        }
    }
    return inversion_count;
}

int getBlankRow(int** matrix){
    int blank_row = 0;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(matrix[i][j] == 0){
                blank_row = i;
                break;
            }
        }
    }
    return blank_row;
}

int getBlankColumn(int** matrix){
    int blank_column = 0;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(matrix[i][j] == 0){
                blank_column = j;
                break;
            }
        }
    }
    return blank_column;
}

bool isSolvable(int** matrix){
    if(n%2 != 0){
        if(inversion_count(matrix)%2 == 0){
            return true;
        }
    }
    else{
        if((inversion_count(matrix) + abs(getBlankRow(matrix)-4)) % 2 != 0){
            return true;
        }
    }
    return false;
}

bool isMatching(int** matrix1, int** matrix2){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(matrix1[i][j] != matrix2[i][j]){
                return false;
            }
        }
    }
    return true;
}

void copyMatrix(int** source, int** destination){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            destination[i][j] = source[i][j];
        }
    }
}

bool isMatrixZero(int** matrix){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(matrix[i][j] != 0){
                return false;
            }
        }
    }
    return true;
}

int** upperRowMove(int** matrix){
    int** temp_matrix = new int*[n];
    for(int i=0;i<n;i++){
        temp_matrix[i] = new int[n];
    }

    int blank_row = getBlankRow(matrix);
    int blank_column = getBlankColumn(matrix);

    if(blank_row - 1 >= 0){
        copyMatrix(matrix, temp_matrix);
        
        temp_matrix[blank_row][blank_column] = matrix[blank_row-1][blank_column];

        temp_matrix[blank_row-1][blank_column] = 0;
    }

    return temp_matrix;
}

int** lowerRowMove(int** matrix){
    int** temp_matrix = new int*[n];
    for(int i=0;i<n;i++){
        temp_matrix[i] = new int[n];
    }

    int blank_row = getBlankRow(matrix);
    int blank_column = getBlankColumn(matrix);

    if(blank_row + 1 <= n-1){
        copyMatrix(matrix, temp_matrix);
        temp_matrix[blank_row][blank_column] = matrix[blank_row+1][blank_column];
        temp_matrix[blank_row+1][blank_column] = 0;
    }

    return temp_matrix;
}

int** leftColumnMove(int** matrix){
    int** temp_matrix = new int*[n];
    for(int i=0;i<n;i++){
        temp_matrix[i] = new int[n];
    }

    int blank_row = getBlankRow(matrix);
    int blank_column = getBlankColumn(matrix);

    if(blank_column-1 >= 0){
        copyMatrix(matrix, temp_matrix);
        temp_matrix[blank_row][blank_column] = matrix[blank_row][blank_column-1];
        temp_matrix[blank_row][blank_column-1] = 0;
    }
    return temp_matrix;
}

int** rightColumnMove(int** matrix){
    int** temp_matrix = new int*[n];
    for(int i=0;i<n;i++){
        temp_matrix[i] = new int[n];
    }

    int blank_row = getBlankRow(matrix);
    int blank_column = getBlankColumn(matrix);

    if(blank_column + 1 <= n-1){
        copyMatrix(matrix, temp_matrix);
        temp_matrix[blank_row][blank_column] = matrix[blank_row][blank_column + 1];
        temp_matrix[blank_row][blank_column + 1] = 0;
    }
    return temp_matrix;
}

void outputMatrix(int** matrix){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

class Node{
    int** matrix;
    int distance = 0;
    int blank_row, blank_column;
    Node* parent;

public:
    int hamming_score = 0, manhattan_score = 0;

    Node(int** matrix){
        this->matrix = matrix;
    }

    Node(const Node& node){
        this->matrix = node.matrix;
        this->distance = node.distance;
        this->parent = node.parent;
    }

    Node(int** matrix, int distance){
        this->matrix = matrix;
        this->distance = distance;
    }

    int** getMatrix(){
        return this->matrix;
    }

    int getHammingDistance(){
        return hamming_distance(this->matrix);
    }

    int getManhattanDistance(){
        return manhattan_distance(this->matrix);
    }

    int getDistance(){
        return this->distance;
    }

    void setHammingScore(){
        this->hamming_score = distance + getHammingDistance();
    }

    int getHammingScore(){
        return this->hamming_score; 
    }

    void setManhattanScore(){
        this->manhattan_score = distance + getManhattanDistance();
    }

    int getManhattanScore(){
        return this->manhattan_score;
    }

    void setDistance(int distance){
        this->distance = distance;
    }

    void setParent(Node* parent){
        this->parent = parent;
    }

    Node* getParent(){
        return this->parent;
    }
};

struct compareHammingScore{
    
    bool operator()(Node* n1, Node* n2) {
        if(n1->hamming_score > n2->hamming_score){
            return true;
        }
        return false;
    }
};

struct compareManhattanScore{

    bool operator()(Node* n1, Node* n2){
        if(n1->manhattan_score > n2->manhattan_score){
            return true;
        }
        return false;
    }
};



int main(){
    cin >> n;
    int** matrix;
    matrix = new int*[n];
    correct_matrix = new int*[n];
    for(int i=0;i<n;i++){
        matrix[i] = new int[n];
        correct_matrix[i] = new int[n];
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cin >> matrix[i][j];
        }
    }
    int k = 1;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(i == n-1 && j == n-1){
                correct_matrix[i][j] = 0;
            }
            else{
                correct_matrix[i][j] = k;
                k++;
            }
        }
    }

    Node initial_node(matrix, 0);

    int blank_row = getBlankRow(matrix);
    int blank_column = getBlankColumn(matrix);

    priority_queue<Node*, vector<Node*>, compareHammingScore> pq;

    initial_node.setHammingScore();
    initial_node.setManhattanScore();
    initial_node.setParent(nullptr);

    pq.push(&initial_node);

    vector<int**> correct_matrices;

    int count = 0;

    Node* final_node;

    if(!isSolvable(matrix)){
        cout << "Unsolvable Puzzle" << endl;
    }

    else{
        set<int**> matrix_set;
        //matrix_set.insert(matrix);
        Node* temp_node;
        int** temp_matrix = new int*[n];
        for(int i=0;i<n;i++){
            temp_matrix[i] = new int[n];
        }
        int k = 1;
        while(!pq.empty()){
            temp_node = pq.top();
            pq.pop();
            count++;
            correct_matrices.push_back(temp_node->getMatrix());
            if(temp_node->getHammingDistance() == 0 || temp_node->getManhattanDistance() == 0){
                final_node = temp_node;
                break;
            }
            temp_matrix = temp_node->getMatrix();
            vector<int**> matrices;
            int** matrix1;
            int** matrix2; 
            int** matrix3;
            int** matrix4;
            matrix1 = new int*[n];
            matrix2 = new int*[n];
            matrix3 = new int*[n];
            matrix4 = new int*[n];
            for(int i=0;i<n;i++){
                matrix1[i] = new int[n];
                matrix2[i] = new int[n];
                matrix3[i] = new int[n];
                matrix4[i] = new int[n];
            }
            matrix1 = upperRowMove(temp_matrix);
            matrix2 = lowerRowMove(temp_matrix);
            matrix3 = leftColumnMove(temp_matrix);
            matrix4 = rightColumnMove(temp_matrix);
            
            if(!isMatrixZero(matrix1)){
                matrices.push_back(matrix1);
            }
            if(!isMatrixZero(matrix2)){
                matrices.push_back(matrix2);
            }
            if(!isMatrixZero(matrix3)){
                matrices.push_back(matrix3);
            }
            if(!isMatrixZero(matrix4)){
                matrices.push_back(matrix4);
            }
            
            for(int i=0;i<matrices.size();i++){
                auto it = matrix_set.find(matrices[i]);
                if(it == matrix_set.end()){
                    if(temp_node->getParent() != nullptr){
                        if(!isMatching(temp_node->getParent()->getMatrix(), matrices[i])){
                            Node* inserted_node = new Node(matrices[i], temp_node->getDistance() + 1);
                            inserted_node->setParent(temp_node);
                            inserted_node->setHammingScore();
                            inserted_node->setManhattanScore();
                            pq.push(inserted_node);
                            //matrix_set.insert(matrices[i]);
                        }   
                    }
                    else{
                        Node* inserted_node = new Node(matrices[i], temp_node->getDistance() + 1);
                        inserted_node->setParent(temp_node);
                        inserted_node->setHammingScore();
                        inserted_node->setManhattanScore();
                        pq.push(inserted_node);
                        //matrix_set.insert(matrices[i]);
                    }
                }
            }
            matrix_set.insert(temp_matrix);
            matrices.clear();
        }
        stack<int**> stack;
        int moves = 0;
        while(final_node->getParent() != nullptr){
            moves++;
            stack.push(final_node->getMatrix());
            final_node = final_node->getParent();
        }
        cout << stack.size() << endl;
        cout << "Minimum number of moves = " << moves << endl;
        cout << endl;
        while(!stack.empty()){
            outputMatrix(stack.top());
            stack.pop();
        }
    }
}
