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
const string st_delim = " ;.,/!\"@#$%^&*()~_-+=�'|{}[]<>:'";
/*

 Here "word" considered to be arbitrary sequence of latin characters(lowercase after transform) or digits.
 Any other char's considered as delimeters and to be ignored
 
*/

vector<string> parceLine(string &s, string delim = "") {
               transform(s.begin(), s.end(), s.begin(), ::tolower);
               vector<string> resList;
               resList.push_back("");
               int idx;
               for (int i = 0; i < s.size(); i ++) {
                   bool dl = !(isdigit(s[i]) || (s[i] >= 'a' && s[i] <= 'z'));
                   // delim.find(s[i]) != string::npos);
                   if (dl) {
                      if (resList.back().size() > 0)
                         resList.push_back("");
                   }
                   else {
                        resList.back() += s[i];
                   }
               }
               if (resList.back().size() == 0)
                  resList.pop_back();
               return resList;
}

map<string, int> wordToId;       
inline void process(string &word) {
       static int idNumber = 0;
       
       if (wordToId.find(word) == wordToId.end()) {
          wordToId[word] = ++idNumber;
       }
}

inline bool printToFile() {
       static map<string,int> :: iterator it = wordToId.begin();
       if (it == wordToId.end())
          return false;
       cout << it -> first << " " << it -> second << endl;
       it ++;
       return true;
}

int main() {
    ios_base::sync_with_stdio(0);
    freopen("word_to_id.txt","w",stdout);
    
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
        
        string s;
        while (getline(F,s)) {
              vector<string> res = parceLine(s, st_delim);
              for (int i = 0; i < res.size(); i ++)
                  process(res[i]);
        }
        F.close();
    }
    closedir(mydir);
    
    while (printToFile());
    
    return 0;   
}
