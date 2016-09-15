#include<iostream>
#include<fstream>
#include<stdio.h>
#include<stack>
#include<queue>
#include<set>
#include<iomanip>
#include<complex>
#include<vector>
#include<map>
#include<algorithm>
#include<cmath>
#include<string>
#include<bitset>
#include<memory.h>
#include<cassert>
#include<ctime>
#include<dirent.h>

using namespace std;
const string st_delim = " ;.,/!\"@#$%^&*()~_-+=¹'|{}[]<>:'";
/*

 Here "word" considered to be arbitrary sequence of latin characters(lowercase after transform) or digits.
 Any other char's considered as delimeters and to be ignored
 
*/

map<string, int> titleToId;       
inline void process(string word) {
       static int idNumber = 0;
       
       if (titleToId.find(word) == titleToId.end()) {
          titleToId[word] = ++idNumber;
       }
}

inline bool printToFile() {
       static map<string,int> :: iterator it = titleToId.begin();
       if (it == titleToId.end())
          return false;
       cout << it -> second << " " << it -> first << endl;
       it ++;
       return true;
}

int main() {
    ios_base::sync_with_stdio(0);
    freopen("id_to_title.txt","w",stdout);
    
    DIR *mydir = opendir("D:\\docs");
    if(mydir == NULL) {
        perror("opendir");
        return -1;
    }
    struct dirent *entry;
    while(entry = readdir(mydir)) {
        
        ifstream F;
        
        char file[200] = "D:\\docs\\";
        strcat(file, entry-> d_name);
        
        F.open(file, ios::in);
        if (!F) {
           continue;
        }
        cerr << entry->d_name << endl;
        
        process(entry->d_name);
        F.close();
    }
    closedir(mydir);
    
    while (printToFile());
    
    return 0;   
}
