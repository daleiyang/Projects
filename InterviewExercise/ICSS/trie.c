#include<cstdio>
#include<cstring>
#include<iostream>
#include<vector>
#include<map>
using namespace std;

//Hold one line in the pricing list. The pattern will be converted into a path in the Trie.
struct Price{
    int operatorId;
    float price;
};

struct Trie{
    //Prefix only contains 0~9. Need to change here if need bigger vocabulary.
    const int sigma_size = 10;    

    //Nodes collection. Each node is a vector<int> with the size = sigma_size.
    vector<vector<int>> nodes;          
    //Holding price index list for each node (pattern).
    vector<vector<int>> prefixs;        
    //Holding all prices for all operators.
    vector<Price> prices;                

    //Init Trie. Call it first for each test case.
    void Clear() {
        //Init root node.
        nodes.clear();
        vector<int> root(sigma_size);   
        nodes.push_back(root);

        //Holding price list for root node.
        prefixs.clear();
        vector<int> price_list;         
        prefixs.push_back(price_list);    

        //Clean all prices; 
        prices.clear();                 
    }

    //From char "1" -> int 1.
    int CharToInt(char c) {
        return c-'0';                   
    }

    //Build Trie
    void Insert(const char* prefix, int price_index){
        int index = 0;                  //Start from the root node.
        int prefix_size = strlen(prefix);
        for(int i = 0; i < prefix_size; i++){
            int number = CharToInt(prefix[i]);
            if(nodes[index][number] == 0){

                //It's a new path in Trie, create a new node.
                vector<int> new_node(sigma_size);
                nodes.push_back(new_node);
                
                //Link new node to it's parent at specific position.
                nodes[index][number] = nodes.size()-1;

                //Holding prefixs for this new node. It has same index with the corresponding node's index.
                vector<int> price_list; 
                prefixs.push_back(price_list);
            }
            index = nodes[index][number];       //move to the next node.
        }
        prefixs[index].push_back(price_index);  //Add price_index to prefixs list.
    }

    //Find the longest match for each operator.
    void LongestMatch(const char* phoneNumber, map<int, float>& match){
        int len = strlen(phoneNumber);
        int index = 0;                          //Start from the root node.

        for(int i = 0; i < len; i++){
            int number = CharToInt(phoneNumber[i]);
            if(nodes[index][number] == 0) return;    //No more pattern found, return from there.

            index = nodes[index][number];       //move to the next node.
            if(prefixs[index].size() != 0){  
                //Check each price in this node and keep the longest one for each operator.
                for(int j = 0; j < prefixs[index].size(); j++){
                    int priceId = prefixs[index][j];
                    int operatorId = prices[priceId].operatorId;
                    float price = prices[priceId].price;
                    match[operatorId] = price;
                }   
            }
        }
    }

    //Print internal structure for debuging
    void Debug(){
        printf("\nPrice list:\n");
        for(int i = 0; i < prices.size(); i++){
            printf("PriceId = %d, Operator Id = %d, Price = %f\n", i, prices[i].operatorId, prices[i].price);
        }

        printf("\nNodes in the Trie:\n");
        for(int i = 0; i < nodes.size(); i++){
            printf("Node Id = %d, content = ", i);
            for(int j = 0; j < nodes[i].size(); j++){
                printf(" %d ", nodes[i][j]);
            }
            printf("\n");
        }

        printf("\nPrefix nodes in the Trie:\n");
        for(int i = 0; i < prefixs.size(); i++){
            printf("Node Id = %d, pricesId list = ", i);
            for(int j = 0; j < prefixs[i].size(); j++){
                printf(" %d ", prefixs[i][j]);
            }
            printf("\n");
        }
    }
};

int main(){

//redirect input, output
#ifdef LOCAL
	freopen("trie.in", "r", stdin);
	freopen("trie.out", "w", stdout);
#endif

    Trie trie;  //Create a prefix tree.

    int kase, cnt = 1;   //How many test case we have.
    scanf("%d", &kase);

    while(kase--){
        printf("Test Case %d\n\n", cnt++);
        //Init trie
        trie.Clear();     
    
        //For reading input only
        int lines;
        char pattern[1024];
        float price;     
        int operatorId = 1; 
        
        //lines means how many price one operator has.
        //lines == 0 means test case is finished, no more operator data need to read.
        while(scanf("%d", &lines) == 1 && lines){
            printf("Reading Operator %d's price list\n", operatorId);
            for(int i = 0; i < lines; i++){
                scanf("%s%f", pattern, &price);
                Price p;
                p.operatorId = operatorId;
                p.price = price;
                trie.prices.push_back(p);
                int priceId = trie.prices.size()-1;
                trie.Insert(pattern, priceId);  //Build trie
            }
            operatorId++; //read price list for next operator.
        }

        trie.Debug(); //For debuging only

        //For one phone nunmber, get the longest match for each operator and price.
        char phone_number[1024];
        scanf("%s", &phone_number);
        map<int, float> match;
        trie.LongestMatch(phone_number, match);

        map<int, float>::iterator it;
        //For debuging 
        printf("\n");
        printf("Phone number is = %s\n", phone_number);
        for(it = match.begin(); it != match.end(); it++){
            printf("Operator ID = %d, Price = %f\n", it->first, it->second);
        }

        //Find the operator with lowest price
        int lowestId;
        float lowestPrice = 1e10;
        for(it = match.begin(); it != match.end(); it++){
            if(it->second < lowestPrice){
                lowestId = it->first;
                lowestPrice = it->second;
            }
        }
        printf("Lowest price: operator id = %d, price = %f\n\n", lowestId, lowestPrice);
    }

    return 0;
}
