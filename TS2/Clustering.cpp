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

const int N = 10000;
const int bits = 64;
const int bitDiff = 15;

struct record {
       string url;
       int len;
       bitset<bits> simhash;
       record(){};
       record(int _len):len(_len), url("") {
                  simhash = 0;
       }
       record(int _len, string _url):len(_len),url(_url){
                  simhash = 0;
       }
       bool operator < (record R) const {
            if (this -> len != R.len)
               return this -> len < R.len;
            return this -> url < R.url;
       }
} list[N + 10];

map<int, vector<string> > cluster;
set<record> keeper, ranger;

int main() {
    srand(31415);
    ios_base::sync_with_stdio(0);
    freopen("simHashes.txt","r",stdin);
    freopen("clusters15.txt","w",stdout);
    
    string url;
    for (int i = 0; i < N; i ++) {
        cin >> url;
        list[i].url = url.substr(0, url.size() - 1);
        cin >> list[i].len;
        for (int j = 0; j < bits; j ++) {
            int bt;
            cin >> bt;
            list[i].simhash[j] = bt;
        }
        keeper.insert(list[i]);
    }
    
    int it = 0;
    while (!keeper.empty()) {
          int val = 300 + rand() % 600;
          set<record> :: iterator basic= keeper.lower_bound(record(val));
          if (basic == keeper.end())
             basic = keeper.begin();
          
          int groupSize = 1;
          it ++;
          val = basic -> len;
          cluster[it].push_back(basic -> url);
          bitset<bits> current = basic -> simhash;
          keeper.erase(basic);
          
          set<record> :: iterator leftIT = keeper.lower_bound(record((int)(val * 0.8)));
          set<record> :: iterator rightIT = keeper.lower_bound(record((int)(val * 1.2) + 2));
          while (leftIT != rightIT) {
                bool halfReplica = (current ^ (leftIT -> simhash)).count() <= bitDiff;
                if (!halfReplica) {
                   leftIT ++;
                   continue;
                }
                groupSize ++;
                cluster[it].push_back(leftIT -> url);
                set<record>::iterator copied = leftIT;
                leftIT ++;
                keeper.erase(copied);
          }
          cerr << it << " " << groupSize << endl;
          assert(groupSize == cluster[it].size());
          ranger.insert(record(-groupSize, cluster[it][0]));
    }
    
    for (int i = 1; i <= 10; i ++) {
        cout << i << ") Size = " << -(ranger.begin() -> len) << ": " << ranger.begin() -> url << endl;
        ranger.erase(ranger.begin());
    }
    for (int i = 1; i <= it; i ++) {
        cout << "Cluster " << i << " size = " << cluster[i].size() << ":";
        for (int j = 0; j < cluster[i].size(); j ++) {
            cout << " " << cluster[i][j];
        }
        cout << endl;
    }
    
    return 0;   
}
