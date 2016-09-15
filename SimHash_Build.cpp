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

vector<string> parceLine(string &s) {
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

map<string, vector<string> > idxToUrl;
const int bits = 64;

inline void processUrl() {
       freopen("URLS.txt","r",stdin);
       string s;
       while (cin >> s) {
             string url = s;
             getline(cin, s);
             s = s.substr(1);
             
             while (s.find(';') != string::npos) {
                   idxToUrl[url].push_back(s.substr(0, s.find(';')));
                   s = s.substr(s.find(';') + 1);
             }
             assert(s == "");
       }
}
inline void printSimHash(string url, int fileSize, vector<int> simhash) {
       cout << url << ": " << fileSize;
       for (int i = 0; i < bits; i ++)
           cout << " " << simhash[i];
       cout << endl;
}
inline unsigned long long getWordHash(string word) {
       unsigned long long res = 0,
                          power = 1;
       for (int i = 0; i < word.size(); i ++) {
           res = res + (word[i] - 'a' + 1)*1ull*power;
           power = power * 197ull;
       }
       return res;
}

int main() {
    ios_base::sync_with_stdio(0);
    freopen("simHashes.txt","w",stdout);
    
    processUrl();
    
    
    
    DIR *mydir = opendir("D:\\docs");
    if(mydir == NULL) {
        perror("opendir");
        return -1;
    }
    struct dirent *entry;
    int idx = 0;
    while(entry = readdir(mydir)) {
        ifstream F;
        char file[200] = "D:\\docs\\";
        strcat(file, entry-> d_name);
        F.open(file, ios::in);
        if (!F) {
           continue;
        }
        idx ++;
        cerr << entry->d_name << endl;
        
        string s;
        getline(F, s);
        
        int it = 25;
        while (s.rfind("Jump to navigation") != string::npos) {
              it --;
              string fileText = s.substr(s.rfind("Jump to navigation"));
              s = s.substr(0, s.rfind("Jump to navigation"));
              if (s.rfind("Jump to navigation") == string::npos) {
                 fileText = s + fileText;
                 s = "";
              }
              
              // fileText - string containing all the words from one page of simple Wiki.
              
              vector<string> res = parceLine(fileText);
              vector<int> simhash(bits,0);
              for (int i = 0; i < res.size(); i ++) {
                  unsigned long long hash = getWordHash(res[i]);
                  for (int j = 0; j < bits; j ++) {
                      simhash[j] += ((hash & 1)?1:-1);
                      hash >>= 1;
                  }
              }
              for (int j = 0; j < bits; j ++) {
                  simhash[j] = ((simhash[j] >= 0)?1:0);
              }
              printSimHash(idxToUrl[entry->d_name][it], res.size(), simhash);
        }
        assert(it == 0);
        
        F.close();
    }
    closedir(mydir);

    return 0;   
}
