#include <iostream>
#include <string>
#include <vector>

//your AVL tree implementation here

class AVL {
private:
    struct TreeNode{
        std::string name;
        int id;
        int height=0;
        int bal=0;
        TreeNode *left;
        TreeNode *right;
        TreeNode(int num, std::string stud) : id(num), name(stud), left(nullptr), right(nullptr) {}
    };
    TreeNode* root = nullptr;
    int getHeight(TreeNode* helpRoot);
    AVL::TreeNode* left(TreeNode* helpRoot);
    //AVL::TreeNode* leftRight(TreeNode* helpRoot);
    AVL::TreeNode* right(TreeNode* helpRoot);
    //AVL::TreeNode* rightLeft(TreeNode* helpRoot);
    AVL::TreeNode* helperInsert(TreeNode* helpRoot, int key, std::string name);
    AVL::TreeNode* helperRemove(TreeNode* helpRoot, int key);
    void helperInOrder(TreeNode* helpRoot, std::vector<std::string> &names);
    void helperPreOrder(TreeNode* helpRoot, std::vector<std::string> &names);
    void helperPostOrder(TreeNode* helpRoot, std::vector<std::string> &names);
    AVL::TreeNode* helperSearchID(TreeNode* root, int key);
    AVL::TreeNode* helperSearchName(TreeNode* root, std::string stud);
    void helperRemoveInOrder(TreeNode* root, int n, int &count);
    int helperLevelCount(TreeNode* root);


public:
    void insert(int stud, std::string name);
    void remove(int stud);
    void inOrder();
    void preOrder();
    void postOrder();
    void searchID(int key);
    void searchName(std::string name);
    int printLevelCount();
    void removeInOrder(int n);

};
int AVL::getHeight(TreeNode *helpRoot) {
    if (helpRoot == nullptr)
        return 0;
    return helpRoot->height;
}

AVL::TreeNode* AVL::left(TreeNode* temp){
    TreeNode* newPar = temp->right;
    if (newPar != NULL) {
        TreeNode *grand = newPar->left;
        newPar->left = temp;
        temp->right = grand;
    }

    temp->height = std::max(getHeight(temp->left), getHeight(temp->right))+1;
    if (newPar != NULL)
        newPar->height = std::max(getHeight(newPar->left), getHeight(newPar->right))+1;

    return newPar;
}

AVL::TreeNode* AVL::right(TreeNode* temp){
    TreeNode* newPar = temp->left;
    if (newPar != NULL) {
        TreeNode *grand = newPar->right;
        newPar->right = temp;
        temp->left = grand;
    }


    temp->height = std::max(getHeight(temp->left), getHeight(temp->right))+1;
    if (newPar != NULL)
        newPar->height = std::max(getHeight(newPar->left), getHeight(newPar->right))+1;

    return newPar;
}

//based off of lecture slides
AVL::TreeNode* AVL::helperInsert(TreeNode* helpRoot, int key, std::string name) {
    if (helpRoot == nullptr) {
        helpRoot = new TreeNode(key, name);
        std::cout << "successful\n";
        //return helpRoot;
    }
    else if (key==helpRoot->id){
        std::cout << "unsuccessful, ID must be unique\n";
        return helpRoot;
    }
    else if ( key < helpRoot->id) {
        helpRoot->left = helperInsert(helpRoot->left, key, name);
        //return helpRoot->left;
    }
    else {
        helpRoot->right = helperInsert(helpRoot->right, key, name);
        //helpRoot->right;
    }

    //check balance
    helpRoot->height = std::max(getHeight(helpRoot->left), getHeight(helpRoot->right))+1;
    //if (helpRoot->left != nullptr && helpRoot->right != nullptr)
    helpRoot->bal = (getHeight(helpRoot->left) - getHeight(helpRoot->right));


    //do rotations if needed
    if(helpRoot->bal > 1){
        //left side heavy
        if (helpRoot->left != nullptr) {
            if (key < helpRoot->left->id)
                //right rotation
                return right(helpRoot);
            else {
                //left-right rotation
                helpRoot->left = left(helpRoot->left);
                return right(helpRoot);
            }
        }
    }
    else if (helpRoot->bal < -1){
        //right side heavy
        if (helpRoot->right != nullptr) {
            if (key > helpRoot->right->id)
                //left rotation
                return left(helpRoot);
            else {
                //right-left rotation
                helpRoot->right = right(helpRoot->right);
                return left(helpRoot);
            }
        }
    }

    return helpRoot;
}

void AVL::insert(int stud, std::string name){
    //O(n) since it is log(n) * n since i am updating height and bf as i add nodes which are n complexity operations
    this->root=helperInsert(this->root, stud, name);
}

AVL::TreeNode* AVL::helperRemove(TreeNode* helpRoot, int key){
    if(helpRoot==NULL) {
        std::cout << "unsuccessful\n";
        return root;
    }

    //item is less than local root
    if(key < helpRoot->id) {
        if (helpRoot->left != NULL) {
            helpRoot->left = helperRemove(helpRoot->left, key);
        }
    }
        //item is more than local root
    else if (key > helpRoot->id) {
        if (helpRoot->right != NULL) {
            helpRoot->right = helperRemove(helpRoot->right, key);
        }
    }
        //item is local root
    else if (key == helpRoot->id){
        //no children
        if (helpRoot->left == NULL && helpRoot->right == NULL){
            std::cout << "successful\n";
            delete(helpRoot);
            return NULL;
        }
            //one child
        else if (helpRoot->left && !helpRoot ->right){
            TreeNode* temp = helpRoot->left;
            delete(helpRoot);
            std::cout << "successful\n";
            return temp;
        }
        else if (helpRoot->right && !helpRoot ->left){
            TreeNode* temp = helpRoot->right;
            delete(helpRoot);
            std::cout << "successful\n";
            return temp;
        }
            //two children
        else{
            TreeNode* temp = helpRoot->right;
            while(temp->left != NULL){
                temp=temp->left;
            }
            helpRoot->id = temp->id;
            helpRoot->name = temp->name;
            helpRoot->right = helperRemove(helpRoot->right, helpRoot->id);
            //std::cout << "successful\n";
        }
    }
    else
        std::cout << "unsuccessful";

    //std::cout << "successful";
    return helpRoot;
}
void AVL::remove(int stud) {
    //O(log(n))since it follows down one branch at a time reducing the nodes by half with each recursion
    this->root=helperRemove(this->root, stud);
}

AVL::TreeNode* AVL::helperSearchID(TreeNode* helpRoot, int key){
    if (helpRoot != nullptr) {
        //if id is found on tree, print name
        if (key == helpRoot->id)
            std::cout << helpRoot->name << "\n";
        else if (key < helpRoot->id)
            helpRoot->left = helperSearchID(helpRoot->left, key);
        else if (key > helpRoot->id)
            helpRoot->right = helperSearchID(helpRoot->right, key);
            //otherwise, print unsuccessful
        else
            std::cout << "unsuccessful\n";
    }
    else
        std::cout << "unsuccessful\n";

    return helpRoot;
}
void AVL::searchID(int key){
    //O(log(n)) since it follows down one branch each time
    helperSearchID(this->root, key);
}
AVL::TreeNode* AVL::helperSearchName(TreeNode* helpRoot, std::string stud){
    //if name doesnt exist, print unsuccessful
    if (helpRoot != nullptr) {
        //if student name is found, print id num
        // if multiple of same name, print id's in order of a pre-transversal on new lines no spaces
        if(helpRoot->name == stud) {
            std::string id = std::to_string(helpRoot->id);
            if (id.length() < 8){
                for(int i = 0; i < 8-id.length(); i++)
                    id.insert(0, "0");
            }
            std::cout << id << "\n";
        }
        else if (helpRoot->left || helpRoot->right) {
            helperSearchName(helpRoot->left, stud);
            helperSearchName(helpRoot->right, stud);
        }
    }
    else {
        std::cout << "unsuccessful\n";
    }
}
void AVL::searchName(std::string name){
    //O(n) time complexity since it follows a pre-order traversal and goes through each node within the tree
    helperSearchName(this->root, name);
}

void AVL::helperInOrder(TreeNode* helpRoot, std::vector<std::string> &names){
    if (helpRoot != nullptr){
        helperInOrder(helpRoot->left, names);
        names.push_back(helpRoot->name);
        names.push_back(", ");
        helperInOrder(helpRoot->right, names);
    }
}
void AVL::inOrder(){
    //O(n) time complexity since it prints out each node within the tree
    std::vector<std::string> names;
    helperInOrder(this->root, names);
    for(int i = 0; i < names.size()-1; i++){
        std::cout << names.at(i);
    }
    std::cout << "\n";
}
void AVL::helperPreOrder(TreeNode* helpRoot, std::vector<std::string> &names){
    if (helpRoot != nullptr) {
        names.push_back(helpRoot->name);
        names.push_back(", ");
        helperPreOrder(helpRoot->left, names);
        helperPreOrder(helpRoot->right, names);
    }
}
void AVL::preOrder(){
    //O(n) time complexity since it prints out each node within the tree
    std::vector<std::string> names;
    helperPreOrder(this->root, names);
    for(int i = 0; i < names.size()-1; i++){
        std::cout << names.at(i);
    }
    std::cout << "\n";
}
void AVL::helperPostOrder(TreeNode* helpRoot, std::vector<std::string> &names){
    if (helpRoot != nullptr) {
        helperPostOrder(helpRoot->left, names);
        helperPostOrder(helpRoot->right, names);
        names.push_back(helpRoot->name);
        names.push_back(", ");
    }
}
void AVL::postOrder(){
    //O(n) time complexity since it prints out each node within the tree
    std::vector<std::string> names;
    helperPostOrder(this->root, names);
    for(int i = 0; i < names.size()-1; i++){
        std::cout << names.at(i);
    }
    std::cout << "\n";
}

int AVL::helperLevelCount(TreeNode* root){
    if(root == NULL)
        return 0;
    return root->height;
}
int AVL:: printLevelCount(){
    //O(1) time complexity since it is just returning the height value stored within the node struct
    int x = helperLevelCount(this->root);
    std::cout << x << "\n";
}
void AVL::helperRemoveInOrder(TreeNode* helpRoot, int n, int &count){
    //if nth node is successfully removed, print successful
    //int count = 0;

    if (helpRoot == NULL){
        return;
    }

    if (count <= n) {
        helperRemoveInOrder(helpRoot->left, n, count);


        if (count == n) {
            remove(helpRoot->id);
        }
        else if (count > n){
            std::cout << "unsuccessful\n";
            return;
        }
        count++;

        helperRemoveInOrder(helpRoot->right, n, count);
    }
}
void AVL::removeInOrder(int n){
    //O(n) time complexity since it traverses through the whole tree in worst case
    //checks each node by in-order traversal making it O(n)
    int count = 0;
    helperRemoveInOrder(this->root, n, count);
}
int main() {
    //your code to invoke the respective commands
    AVL myavl;

    std::string input;
    std::string command;
    std::string curName;
    std::string curID;
    int id;
    int n;
    int numCommands;

    std::cin >> numCommands;

    while (numCommands > 0) {

        std::cin >> command;

        if (command == "insert") {
            std::getline(std::cin, input) ;

            curName = input.substr(1, input.find_first_of('"'));
            curID = input.substr(input.find_last_of('"')+2, input.length());

            std::cout << curName;

            if (curID.length() != 8)
                std::cout << "ID must be 8 digits long";
            if (!stoi(curID))
                std::cout << "ID must be all digits";
            else
                id = stoi(curID);

            curName = curName.substr(1, curName.size() - 2);
            for(int i = 0; i < curName.length(); i++){
                if ((curName.at(i) >= 'a' && curName.at(i) <= 'z') || (curName.at(i) >= 'A' && curName.at(i) <= 'Z') || curName.at(i) == ' ')
                    continue;
                else{
                    std::cout << "names must consist of only letters or spaces";
                    break;
                }
            }
            myavl.insert(id, curName);
        }
        if (command == "remove") {
            std::cin >> curID;
            id = stoi(curID);
            myavl.remove(id);
        }
        if (command == "printInorder")
            myavl.inOrder();
        if(command == "printPreorder")
            myavl.preOrder();
        if (command == "printPostorder")
            myavl.postOrder();
        if (command == "removeInorder"){
            std::cin >> n;
            myavl.removeInOrder(n);
        }
        if (command == "search"){
            std::getline(std::cin, input) ;
            if (input.find('"')== 1) {
                curName = input.substr(2, input.size() - 3);
                myavl.searchName(curName);
            }
            else{
                curID = input.substr(1, input.size()-1);
                id = stoi(curID);
                myavl.searchID(id);
            }
        }
        if (command == "printLevelCount")
            myavl.printLevelCount();

        numCommands--;
    }
}