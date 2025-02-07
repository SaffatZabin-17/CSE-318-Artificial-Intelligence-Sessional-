#include<bits/stdc++.h>
using namespace std;

// Global variables
map<string, vector<string>> allAttributes = {{"buying", {"vhigh", "high", "med", "low"}}, {"maint", {"vhigh", "high", "med", "low"}}, {"doors", {"2", "3", "4", "5more"}}, {"persons", {"2", "4", "more"}}, {"lug_boot", {"small", "med", "big"}}, {"safety", {"low", "med", "high"}}};

// class to store the example data points
class carExample {  
    public:
        string buying;          // vhigh, high, med, low
        string maint;           // vhigh, high, med, low
        string doors;           // 2, 3, 4, 5more
        string persons;         // 2, 4, more
        string lugBoot;         // small, med, big
        string safety;          // low, med, high
        string classValue;      // unacc, acc, good, vgood

        carExample(string buying, string maint, string doors, string persons, string lugBoot, string safety, string classValue) {
            this->buying = buying;
            this->maint = maint;
            this->doors = doors;
            this->persons = persons;
            this->lugBoot = lugBoot;
            this->safety = safety;
            this->classValue = classValue;
        }
};

// class to store the nodes of the decision tree
class node{
    public:
        string attribute;   // buying, maint, doors, persons, lug_boot, safety - which one to split on
        string splitOn;     // vhigh, high, med, low - split on this value of the attribute
        string classValue;  // unacc, acc, good, vgood - classValue to be assigned to the leaf node
        vector<node*> children; // children of the node - 4 for buying, maint; 3 for doors; 2 for persons,      lug_boot, safety
        
        node(string attribute, string splitOn, string classValue) {
            this->attribute = attribute;
            this->splitOn = splitOn;
            this->classValue = classValue;
        }

        node() {
            this->attribute = "";
            this->splitOn = "";
            this->classValue = "";
        }
};


// Function signatures
double entropy(vector<carExample> examples);
double informationGain(vector<carExample> examples, string attribute);
string findBestAttribute(vector<carExample> examples, vector<string> attributes);
bool sameClassValue(vector<carExample> examples);
string pluralityClassValue(vector<carExample> examples);
node* decisionTreeLearning(vector<carExample> examples, vector<string> attributes, vector<carExample> parentExamples);
void printDecisionTree(node* root, int level);
double testAccuracy(node* root, vector<carExample> testExamples);
double standardDeviation(double accuracy[21], double meanAccuracy);


// Function to find the entropy in a given example set
double entropy(vector<carExample> examples) {

    // Map to store the count of each classValue in the examples
    map<string, int> classValueCount;

    // Counting the number of examples with each classValue
    for (carExample example : examples) {
        classValueCount[example.classValue]++;
    }

    // Calculating the entropy
    double entropy = 0.0;
    for (auto it = classValueCount.begin(); it != classValueCount.end(); it++) {
        double probability = (double) it->second / examples.size();
        entropy += -probability * log2(probability);
    }

    return entropy;
}

// Function to find the information gain in a given example set
double informationGain(vector<carExample> examples, string attribute) {

    // map to store the list of examples of each individual value of the attribute
    map<string, vector<carExample>> attributeValueExamples;

    // Counting the number of examples with each value of the selected attribute
    for (carExample example : examples) {
        if (attribute == "buying") {
            attributeValueExamples[example.buying].push_back(example);
        } 
		else if (attribute == "maint") {
            attributeValueExamples[example.maint].push_back(example);
        } 
		else if (attribute == "doors") {
            attributeValueExamples[example.doors].push_back(example);
        } 
		else if (attribute == "persons") {
            attributeValueExamples[example.persons].push_back(example);
        } 
		else if (attribute == "lug_boot") {
            attributeValueExamples[example.lugBoot].push_back(example);
        } 
		else if (attribute == "safety") {
            attributeValueExamples[example.safety].push_back(example);
        }
    }

    // Calculating the information gain
    double informationGain = entropy(examples);
    for (auto it = attributeValueExamples.begin(); it != attributeValueExamples.end(); it++) {
        double probability = (double) it->second.size() / examples.size();
        informationGain -= probability * entropy(it->second);
    }
    return informationGain;
}

// Function to find the attribute to split on
string findBestAttribute(vector<carExample> examples, vector<string> attributes) {
    double maxInformationGain = -1.0;
    string attributeToSplitOn = "";

    for (string attribute : attributes) {
        double informationGainOfAttribute = informationGain(examples, attribute);
        if (informationGainOfAttribute > maxInformationGain) {
            maxInformationGain = informationGainOfAttribute;
            attributeToSplitOn = attribute;
        }
    }

    return attributeToSplitOn;
}

// Function to check if all examples have the same classValue
bool sameClassValue(vector<carExample> examples) {
    string classValue = examples[0].classValue;
    for (carExample example : examples) {
        if (example.classValue != classValue) {
            return false;
        }
    }
    return true;
}

// Function to find the majority classValue in a set of examples
string pluralityClassValue(vector<carExample> examples) {
    map<string, int> classValueCount;
    for (carExample example : examples) {
        classValueCount[example.classValue]++;
    }

    int maxCount = -1;
    string majorityClassValue = "";
    for (auto it = classValueCount.begin(); it != classValueCount.end(); it++) {
        if (it->second > maxCount) {
            maxCount = it->second;
            majorityClassValue = it->first;
        }
    }

    return majorityClassValue;
}

// Function to build the decision tree
node* decisionTreeLearning(vector<carExample> examples, vector<string> attributes, vector<carExample> parentExamples) {
    if (examples.size() == 0) {
        return new node("", "", pluralityClassValue(parentExamples));
    } 
	else if (sameClassValue(examples)) {
        return new node("", "", examples[0].classValue);
    } 
	else if (attributes.size() == 0) {
        return new node("", "", pluralityClassValue(examples));
    } 
	else {
        string attributeToSplitOn = findBestAttribute(examples, attributes);
        node* root = new node(attributeToSplitOn, "", "");
        vector<string> newAttributes = attributes;
        newAttributes.erase(remove(newAttributes.begin(), newAttributes.end(), attributeToSplitOn), newAttributes.end());
        // remove(newAttributes.begin(), newAttributes.end(), attributeToSplitOn);

        map<string, vector<carExample>> attributeValueExamples;
        for(int i = 0; i < allAttributes[attributeToSplitOn].size(); i++) { 
        // allAttributes[attributeToSplitOn] is a vector of all possible values of the attributeToSplitOn
            attributeValueExamples[allAttributes[attributeToSplitOn][i]] = {};
        }
        for (carExample example : examples) {
            if (attributeToSplitOn == "buying") {
                attributeValueExamples[example.buying].push_back(example);
            } 
            else if (attributeToSplitOn == "maint") {
                attributeValueExamples[example.maint].push_back(example);
            } 
            else if (attributeToSplitOn == "doors") {
                attributeValueExamples[example.doors].push_back(example);
            } 
            else if (attributeToSplitOn == "persons") {
                attributeValueExamples[example.persons].push_back(example);
            } 
            else if (attributeToSplitOn == "lug_boot") {
                attributeValueExamples[example.lugBoot].push_back(example);
            } 
            else if (attributeToSplitOn == "safety") {
                attributeValueExamples[example.safety].push_back(example);
            }
        }
        for (auto it = attributeValueExamples.begin(); it != attributeValueExamples.end(); it++) {
            node* child = decisionTreeLearning(it->second, newAttributes, examples);
            child->splitOn = it->first;
            root->children.push_back(child);
        }
        return root;
    }
}

// Function to print the decision tree
void printDecisionTree(node* root, int level) {
    if (root == NULL) {
        return;
    }
    for (int i = 0; i < level; i++) {
        cout << "| ";
    }
    if (root->attribute == "") {
        cout << root->classValue << endl;
    } else {
        cout << root->attribute << " = " << root->splitOn << endl;
        for (node* child : root->children) {
            printDecisionTree(child, level + 1);
        }
    }
}

// Function to test the accuracy of the decision tree
double testAccuracy(node* root, vector<carExample> testExamples) {
    int correct = 0;
    for (carExample example : testExamples) {
        node* temp = root;
        while (temp->attribute != "") {
            if (temp->attribute == "buying") {
                for (node* child : temp->children) {
                    if (child->splitOn == example.buying) {
                        temp = child;
                        break;
                    }
                }
            } 
			else if (temp->attribute == "maint") {
                for (node* child : temp->children) {
                    if (child->splitOn == example.maint) {
                        temp = child;
                        break;
                    }
                }
            } 
			else if (temp->attribute == "doors") {
                for (node* child : temp->children) {
                    if (child->splitOn == example.doors) {
                        temp = child;
                        break;
                    }
                }
            } 
			else if (temp->attribute == "persons") {
                for (node* child : temp->children) {
                    if (child->splitOn == example.persons) {
                        temp = child;
                        break;
                    }
                }
            } 
			else if (temp->attribute == "lug_boot") {
                for (node* child : temp->children) {
                    if (child->splitOn == example.lugBoot) {
                        temp = child;
                        break;
                    }
                }
            } 
			else if (temp->attribute == "safety") {
                for (node* child : temp->children) {
                    if (child->splitOn == example.safety) {
                        temp = child;
                        break;
                    }
                }
            } 
			else {
                cout << "Error: Invalid attribute." << endl;
                return -1.0;
            }
        }
        if (temp->classValue == example.classValue) {
            correct++;
        }
    }
    return (double) correct / testExamples.size();
}

// Function to calculate the standard deviation of the accuracy of the decision tree
double standardDeviation(double accuracy[21], double meanAccuracy) {
    double sum = 0.0;
    for(int i = 1; i <= 20; i++) {
        sum += pow(accuracy[i] - meanAccuracy, 2);
    }
    return sqrt(sum/20);
}


// Main function
int main() {
    ifstream inputFile("./car evaluation dataset/car.data"); // has 1728 data points

    if (!inputFile.is_open()) {
        cerr << "Error: Could not open the input file." << endl;
        return 1;
    }

    vector<vector<string>> allInputs; // Vector to store all input lines.

    string line;

    while (getline(inputFile, line)) {
        istringstream iss(line);
        string token;
        vector<string> values;
        int count = 0;

        while (getline(iss, token, ',') && count < 7) {
            values.push_back(token);
            count++;
        }

        if (count != 7) {
            cerr << "Error: Each line in the file should contain exactly 7 values separated by commas." << endl;
            return 1;
        }

        allInputs.push_back(values);
    }

    inputFile.close();

    double accuracy[21]; // accuracy of the decision tree for each of the 20 tests
    double meanAccuracy = 0.0; // mean accuracy of the decision tree
    double stdDeviation = 0.0; // standard deviation of the accuracy of the decision tree

	node* root;

    for(int test = 1; test <= 20; test++) {
        // using built-in random generator to shuffle the vector allInputs
        random_shuffle(allInputs.begin(), allInputs.end());

        vector<vector<string>> trainInputs (allInputs.begin(), allInputs.begin() + allInputs.size()*8/10);
        vector<vector<string>> testInputs (allInputs.begin() + allInputs.size()*8/10, allInputs.end());
        vector<carExample> trainExamples;
        vector<carExample> testExamples;
        vector<string> attributes = {"buying", "maint", "doors", "persons", "lug_boot", "safety"};

        for(int i = 0; i < trainInputs.size(); i++) {
            trainExamples.push_back(carExample(trainInputs[i][0], trainInputs[i][1], trainInputs[i][2], trainInputs[i][3], trainInputs[i][4], trainInputs[i][5], trainInputs[i][6]));
        }
        for(int i = 0; i < testInputs.size(); i++) {
            testExamples.push_back(carExample(testInputs[i][0], testInputs[i][1], testInputs[i][2], testInputs[i][3], testInputs[i][4], testInputs[i][5], testInputs[i][6]));
        }

        root = decisionTreeLearning(trainExamples, attributes, trainExamples);

        accuracy[test] = testAccuracy(root, testExamples);
        cout << "Test: " << test << " --> Accuracy: " << accuracy[test] << endl;
        meanAccuracy += accuracy[test];
    }

	printDecisionTree(root, 0);
    meanAccuracy /= 20;
    stdDeviation = standardDeviation(accuracy, meanAccuracy);

    cout << endl << endl << "Mean accuracy: " << meanAccuracy << endl;
    cout << "Standard deviation: " << stdDeviation << endl;

    return 0;
}
