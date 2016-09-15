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
const int bits = 64;
const int Nsize = 37;
unsigned long long powerBasis[] = {31, 57, 197, 213};
vector< vector<int> > basis(bits, vector<int> (Nsize, 0));

inline unsigned long long getWordHash(string word, unsigned long long powBasis) {
       unsigned long long res = 0,
                          power = 1;
       for (int i = 0; i < word.size(); i ++) {
           res = res + (word[i] - 'a' + 1)*1ull*power;
           power = power * powBasis;
       }
       return res;
}
inline vector<int> getSimHash(vector<string> &words) {
       vector<int> temp(Nsize, 0);
       vector<int> simhash(bits, 0);
       for (int i = 0; i < words.size(); i ++) {
           for (int j = 0; j < 3; j ++) {
               int idx = getWordHash(words[i], powerBasis[j]) % Nsize;
               int add = getWordHash(words[i], powerBasis[j + 1]) % 11 - 5;
               temp[idx] += add;  
           }
       }
       for (int i = 0; i < bits; i ++) {
           long long Q = 0;
           for (int j = 0; j < Nsize; j ++)
               Q += temp[j] * 1ull * basis[i][j];
           simhash[i] = (Q > 0);
       }
       return simhash;
}
inline void printSimHash(string url, int fileSize, vector<int> simhash) {
       cout << url << ": " << fileSize;
       for (int i = 0; i < bits; i ++)
           cout << " " << simhash[i];
       cout << endl;
}
int main() {
    ios_base::sync_with_stdio(0);
    freopen("simHashes.txt","w",stdout);
    
    processUrl();
    for (int i = 0; i < bits; i ++) {
        for (int j = 0; j < Nsize; j ++)
            basis[i][j] = rand() % 201 - 100;
    }
    
    
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
              vector<int> simhash = getSimHash(res);
              printSimHash(idxToUrl[entry->d_name][it], res.size(), simhash);
        }
        assert(it == 0);
        
        F.close();
    }
    closedir(mydir);

    return 0;   
}
