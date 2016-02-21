#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "first.h"

int idkdd(char str[]);

//ROOT OF TREE    
struct trieNodeTag *root; //Root

int words = 0;

//Trie Node Tag
typedef struct trieNodeTag {
    char letter; //its actual letter
    struct trieNodeTag* chil[26]; //all its children
    char isLeafNode; //if it is a leaf node
    int occur; //amount of occurances
    int prefix; //prefixes
} trieNodeT;

//Constructs a new node using malloc
struct trieNodeTag* create_trieNodeTag(char letter){
    struct trieNodeTag* node;
    
    node = (struct trieNodeTag *)malloc(1* sizeof(struct trieNodeTag));//malloc allocates space. 1 node * size of the node
    node->letter = tolower(letter); //way to access fields
    node->isLeafNode = 'f';
    node->occur = 0;
    node->prefix = 0;
    
    //sets all the children to null
    int i = 0;
    for(;i<26;i++){node->chil[i] = NULL;}
    return node;
}


//Recursively Deallocates the tree
//Does this by going down each level. When it reaches a node (which is in the end of the tree) it deletes it.
//Then the program goes to the next child and does the same thing
void deAllocateHelper(struct trieNodeTag *ptr){
    
    //case 1. root is null
    if(ptr == NULL){
        return;
    }
    
    //L R TRAVERSAL
    int i;
    for(i = 0; i<26; i++){
        deAllocateHelper(ptr->chil[i]);
    }
    
    
    //VISIT
    
    free(ptr);
    return;
}

//Deallocates the tree.
int deAllocate(){
    deAllocateHelper(root);
    return 0;
}


//--------------------------------------------------------------------
//gets the index (int) of an alphabet letter
int indexGetter(char l){
    int index = tolower(l) - 'a'; //convert to lower case
    return index;
}


//--------------------------------------------------------------------/

//Reads the dictionary file, and creates a trie for it
void readDict (FILE *dicF){
    
    //INITILIZES THE ROOT TO HAVE 0 OR NULL
    root= create_trieNodeTag('a');
    root->letter = '\0';
    
    char letter;
    
    int numberOfStuff = fscanf(dicF,"%c",&letter);
    
    struct trieNodeTag *ptr = root;
    
    for(;numberOfStuff!=EOF; numberOfStuff = fscanf(dicF,"%c",&letter)){
        
        if(isalpha(letter)){ //insert
            
            if(ptr->chil[indexGetter(letter)]!=NULL){ //do not insert if its already there
                ptr = ptr->chil[indexGetter(letter)];
                continue;
            }
            
            struct trieNodeTag *newnode = create_trieNodeTag(letter);
            
            ptr->chil[indexGetter(letter)] = newnode;
            
            ptr = newnode;
        
        }else{ //Word is over. Set ptr back to front and get ready to repeat
            
            if(ptr != root){
                ptr->isLeafNode = 't';
                words++;
            } //last letter is leaf node
            ptr = root;
        }
    }
    fclose(dicF);
    return;
}



//--------------------------------------

/*
leafNodeDater takes the data file. From this data file it creates strings by adding and clearing out of an array.  Once the method creates a string, it sends the method idkdd the string name. Here is where the magic happens. The idkdd method searches for the string, and updates the prefix/super word and occurrence count. 
*/

void leafNodeDater (char* dataFile){
    
    FILE *dataF = fopen(dataFile, "r");
    
    char letter;
    
    
    char word[100];
    int j =0;
    for(;j<100;j++){
        word[j] = '\0';
    }
    
    int i = 0;
    int start = 0; //1 when you begin a word
    
    int numberOfStuff = fscanf(dataF,"%c",&letter);
    
    while (numberOfStuff != EOF){

        if(isalpha(letter)){
              
            word[i] = letter;
            i++;
            start = 1;
            
        }else{//not a letter
            
            if(start == 1){
            
                j = i;
                for(;j<100;j++){
                    word[j]= '\0';
                }
                
                idkdd(word);
                
                start = 0;
                i = 0;
                
                j = 0;
              
            }
        }
        numberOfStuff = fscanf(dataF, "%c", &letter);
    }
    
    fclose(dataF);
}



int idkdd(char str[]){ //takes word
    
    struct trieNodeTag *ptr = root;
    
    long i = 0;
    
    for(;i < strlen(str);i++){
        
        if(ptr == NULL){continue;}
        
        if(ptr->isLeafNode =='t'){
        
            long elementsLeftInString = strlen(str) - (i);
            
            if(elementsLeftInString != 0){
         
                /*
                printf(" character end is %c", ptr->letter);
                
                ptr->occur = ptr->occur +1;
                break;
                 */
                ptr->prefix = ptr->prefix + 1;
            }
            
        
        }
         ptr = ptr->chil[indexGetter(str[i])];
    
    }
    
    if(ptr!=NULL){
        if(ptr->isLeafNode == 't'){
            ptr->occur = ptr->occur +1;
        }
    }
    
    
    

    return 0;
}




//------------------------------------




//PRINT FUNCTION
/*
prints out all the unique words.  printResult accomplishes this by using preorder traversal. 
*/

void printHelper(struct trieNodeTag *ptr, char word[100], FILE *output, int index, int write){
    if(ptr == NULL){
        return;
    }
    
    
    //VISIT
    
    if(ptr!=root){
    word[index] = ptr->letter;
    }

    if(ptr->isLeafNode == 't'){
        
        int i = 0;
        for(i = index+1; i < 100; i++){word[i] = '\0';}
        
            fprintf(output,"%s %d %d\n", word, ptr->occur, ptr->prefix);
      
        
    }
    
    
    //A-Z TRAVERSAL
    int i;
    for(i = 0; i<26; i++){
        printHelper(ptr->chil[i], word, output, (index+1), write);
    }
    
    
  
}

void printResult(char* outputName){
    
    FILE *output = fopen(outputName, "w");
    
    char word[100];
    
    printHelper(root, word , output, -1, 0); //go through all
    
    fclose(output);
}



















//-----------------------------------
int main(int argc, char ** argv) {
    
    //INPUT THE MAIN FILE
    if(argc != 2){//too much command lines
        printf("invalid input\n");
        return 0;
    }
    
    char* fileName = argv[1];
    
    FILE* fp = fopen(fileName, "r");
    
    if(fp==NULL){ //can not find
        printf("invalid input\n");
        return 0;
    }
    
    //INPUT THE DICTIONARY AND DATA FILE
    char *dictionary = (char *)malloc(20 * sizeof(char));
    
    char *data = (char *)malloc(20 * sizeof(char));
    
    //TRAVERSE THROUGH MAIN FILE . TAKE THE STRING NAMES OF DICTIONARY AND DATA
    
    int numberOfStuff = fscanf(fp,"%s %s", dictionary, data);
    
    int i = 1;
    
    for(;numberOfStuff!=EOF;numberOfStuff = fscanf(fp,"%s %s", dictionary, data)){
        
        
        FILE *dicF = fopen(dictionary, "r");
        
        //SEG FAULT
        readDict(dicF); //Reads dictionary file and puts it in a trie
        
        leafNodeDater(data); //indexes each node of the tree to have prefix and occurances
        
        //Create An OutPut File
        
        char outputName[11]; //output is 6 chars, int can be 0-999, null at end
        sprintf(outputName,"out%d.txt", i);
        
        printResult(outputName);
      
   
        
        deAllocate();
        root = NULL;
        i++;
        
    }
    
    free(dictionary);
    free(data);
    
    fclose(fp);
    
    return 0;
}