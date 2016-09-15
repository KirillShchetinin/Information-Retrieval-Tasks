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
const int filesNumber = 15000;

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

inline int strToInt(string s) {
    int x = 0;
    for (int i = 0; i < s.size(); i ++) {
        assert(isdigit(s[i]));
        x = x * 10 + (s[i] - '0');
    }
    return x;
}

vector< bitset<filesNumber> > titleForWord(1);
vector< map<int, int>  > titleForWordCnt(1);

map<string, int> wordToId;
vector<string> idToTitle(filesNumber);

int wordCOUNT[filesNumber];
double rank[filesNumber];

inline int getWordCnt(int idWord, int idDoc) {
       if (titleForWordCnt[idWord].count(idDoc) == 0)
          return 0;
       return titleForWordCnt[idWord][idDoc];
}

double TFIDF(int idWord, int idDoc) {
       return 1.0 * getWordCnt(idWord,idDoc) / wordCOUNT[idDoc];
}
bool cmp(int x,int y) {
     return rank[x] > rank[y];
}
int main() {
    ios_base::sync_with_stdio(0);
    string s;
    
    ifstream F,G,H;
    F.open("word_to_id.txt",ios::in);
    
    while (getline(F,s)) {
          vector<string> parts = parceLine(s);
          assert(parts.size() == 2);
          wordToId[parts[0]] = strToInt(parts[1]);
    }
    F.close();
    
    G.open("id_to_title.txt",ios::in);
    while (getline(G,s)) {
          int p = s.find(' ');
          idToTitle[strToInt(s.substr(0,p))] = s.substr(p + 1);
    }
    G.close();
    
    H.open("title_for_word.txt",ios::in);
    while (getline(H,s)) {
          vector<string> parts = parceLine(s);
          int wordId = strToInt(parts[0]);
          int old = titleForWord.size();
          if (wordId >= old) {
             titleForWord.resize(wordId + 1);
             titleForWordCnt.resize(wordId + 1);
          }
          
          titleForWord[wordId] = 0;
          for (int i = 1; i < parts.size(); i += 2) {
              titleForWord[wordId][strToInt(parts[i])] = 1;
              titleForWordCnt[wordId][strToInt(parts[i])] = strToInt(parts[i + 1]);
              
              // calculate for each document total number of words
              wordCOUNT[strToInt(parts[i])] += strToInt(parts[i + 1]);
          }
          
    }
    
    while (1) {
          cout << "Print words for query(per space):\n";
          getline(cin, s);
          vector<string> parts = parceLine(s);
          bitset<filesNumber> unionn(0);
          
          for (int i = 0; i < parts.size(); i ++) {
              if (wordToId.count(parts[i]) == 0)
                 continue;
              int idWord = wordToId[parts[i]];
              unionn |= titleForWord[idWord];
          }
          
          for (int i = 0; i < filesNumber; i ++) {
              rank[i] = 0;
              // for each word sum TF-IFP i-th doument per this word
          }
          
          for (int i = 0; i < parts.size(); i ++) {
              if (wordToId.count(parts[i]) == 0)
                 continue;
              int idWord = wordToId[parts[i]];
              for (int j = 1; j < filesNumber; j ++) {
                  if (titleForWord[idWord][j] == 0) continue;
                  rank[j] += TFIDF(idWord, j);
                  //rank[j] += 1.0 * titleForWordCnt[idWord][j] / wordCOUNT[j];
              }
          }
          
          /*
            rank[document] - some measurement function in regards of input data
            
          */
          vector<int> pSort;
          for (int i = 1; i < filesNumber; i ++)
              if (unionn[i])
                 pSort.push_back(i);
          
          sort(pSort.begin(), pSort.end(), cmp);
          
          assert(unionn.count() == pSort.size());
          
          cout << "Total number of files = " << unionn.count() << endl;
          for (int i = 0; i < min(10,(int)pSort.size()); i ++) {
              if (i == 9)
                 cout << "10";
              else
                  cout << (char)(i + 1 + '0');
              cout << ") " << idToTitle[pSort[i]] << endl << rank[pSort[i]];
              cout << "(";
              for (int j = 0; j < parts.size(); j ++) {
                  if (wordToId.count(parts[j]) == 0)
                     continue;
                  int idWord = wordToId[parts[j]];
                  if (!titleForWord[idWord][pSort[i]])
                     continue;
                  cout << parts[j] << ":" << TFIDF(idWord, pSort[i]) << ",";
              }
              cout << ")\n";
          }
          
          cout << endl;
    }
    
    return 0;   
}
