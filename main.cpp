//
//  main.cpp
//  Samuel_Chiu
//
//  Created by Sam on 2/19/20.
//  Copyright © 2020 Samuel Chiu. All rights reserved.
//

#include <iostream>
#include <string>
#include <queue>
#include <vector>

using namespace std;

vector<string> eventList;

struct sched {
    int arrivalTime;
    vector<string> events;
    sched(int a, vector<string> e) {
        arrivalTime = a;
        events = e;
    }
};

//orders with lowest time priority
class lessTime {
public:
    bool operator()(sched& t1, sched& t2) {
        return t1.arrivalTime > t2.arrivalTime;
    }
};

//prints string vectors
void svprint (const vector<string>& v){
    for (int i=0; i<v.size();i++){
        cout << v[i] << endl;
    }
}

int getTime(string event) {
    string time = "";
    for (int i = 0; i < event.length(); i++) {
        if (isdigit(event[i])) {
            time += event[i];
        }
    }
    return stoi(time);
}

priority_queue<sched, vector<sched>, lessTime> pq;
void createPQ() {
    string temp = "";
    int startTime = 0;
    vector<string> stringVec;
    for (int i = 1; i < eventList.size(); i++) {
        if (eventList[i].find("START") != string::npos) {
            stringVec.push_back(eventList[i]);
        }
        else {
            int startTime = getTime(stringVec[0]);
            sched temp(startTime, stringVec);
            pq.push(temp);
            stringVec.clear();
        }
    }
}


int main() {
    string str;
    int time;
    int numCores;
    cin >> str;
        cin >> numCores;
    int count = 0;
    while(getline(cin, str)) {
        eventList.push_back(str);
        count++;
    }
    
    
    createPQ();
    
    while (! pq.empty()) {
        sched t2 = pq.top();
        svprint(t2.events);
        cout << endl;
        pq.pop();
    }
    
    
    //vector<string> stringVec;
    /*for (int i = 1; i < eventList.size(); i++) {
        if (!(eventList[i].find("START") != string::npos)) {
            stringVec.push_back(eventList[i]);
        }
        else {break;}
    }
    svprint(stringVec);*/
    
    //test pq
    /*vector<string> sv;
    sv.push_back("1test1");
    sv.push_back("1test2");
    sv.push_back("1test3");
    sv.push_back("1test4");
    sched test(4, sv);
    sv.clear();
    pq.push(test);
    sv.push_back("2test1");
    sv.push_back("2test2");
    sv.push_back("2test3");
    sv.push_back("2test4");
    sched test2(1, sv);
    pq.push(test2);
    sv.clear();
    sv.push_back("3test1");
    sv.push_back("3test2");
    sv.push_back("3test3");
    sv.push_back("3test4");
    sched test3(3, sv);
    pq.push(test3);
    
    while (! pq.empty()) {
        sched t2 = pq.top();
        svprint(t2.events);
        cout << endl;
        pq.pop();
    }*/
    
    //svprint(eventList);
    return 0;
}
