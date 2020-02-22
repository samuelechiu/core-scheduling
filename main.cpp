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
int nfreecores;
int curr_time;

struct sched {
    int arrivalTime;
    vector<string> events;
    int pid;
    bool isInteract;
    bool isComplete;
    sched(int a, vector<string> e, int p, bool i, bool c) {
        arrivalTime = a;
        events = e;
        pid = p;
        isInteract = i;
        isComplete = c;
    }
    int getArrival() {
        return arrivalTime;
    }
    vector<string> getEvents() {
        return events;
    }
    int getPID() {
        return pid;
    }
    bool getInteractive() {
        return isInteract;
    }
    bool getIsComplete() {
        return isComplete;
    }
    void setInteractiveTrue() {
        isInteract = true;
    }
    void setInteractiveFalse() {
        isInteract = false;
    }
    void setCompleteTrue() {
        isComplete = true;
    }
    void setCompleteFalse() {
        isComplete = false;
    }
};

queue<sched> niq;
queue<sched> iq;
queue<sched> ssd;
bool isSSDFree = true;

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
    //string temp = "";
    int startTime = 0;
    int pid;
    vector<string> stringVec;
    startTime = getTime(eventList[1]);
    for (int i = 2; i < eventList.size(); i++) {
        if (eventList[i].find("PID") != string::npos) {
            pid = getTime(eventList[i]);
        }
        else if (eventList[i].find("START") == string::npos) {
            stringVec.push_back(eventList[i]);
        }
        else {
            sched temp(startTime, stringVec, pid, false, false);
            pq.push(temp);
            startTime = getTime(eventList[i]);
            stringVec.clear();
        }
    }
    sched temp(startTime, stringVec, pid, false, false);
    pq.push(temp);
    stringVec.clear();
}

void arrival(int time, sched p) {
    p.setInteractiveTrue();
    
}

void core_request(int how_long, sched p) {
    if (nfreecores > 0) {
        nfreecores--;
        //schedule complete at curr_time + how_long for process seqno
        int completion_time = curr_time + how_long;
        vector<string> strtemp = p.getEvents();
        strtemp.erase(strtemp.begin());
        sched temp(completion_time, strtemp, p.getPID(), p.getInteractive(), true);
        pq.push(temp);
    }
    else {
        if (p.getInteractive() == true) {
            //queue proc_id in iq
            iq.push(p);
        }
        else {
            //queue proc_id in niq
            niq.push(p);
        }
    }
}

void core_release(sched p) {
    if (iq.empty()) {
        sched iqtemp = p;
        int completion_time = curr_time + iqtemp.getArrival();
        sched temp(completion_time, iqtemp.getEvents(), iqtemp.getPID(), iqtemp.getInteractive(), iqtemp.getIsComplete());
        pq.push(temp);
    }
    else if (niq.empty()) {
        sched niqtemp = p;
        int completion_time = curr_time + niqtemp.getArrival();
        sched temp(completion_time, niqtemp.getEvents(), niqtemp.getPID(), niqtemp.getInteractive(), niqtemp.getIsComplete());
        pq.push(temp);
    }
    else {
        nfreecores++;
    }
    //process next process request
}

void ssd_request(int how_long, sched p) {
    if (isSSDFree == true) {
        isSSDFree = false;
        int completion_time = curr_time + how_long;
        sched temp(completion_time, p.getEvents(), p.getPID(), p.getInteractive(), p.getIsComplete());
        pq.push(temp);
    }
    else {
        ssd.push(p);
    }
}

void ssd_release(sched p) {
    p.setInteractiveFalse();
    if (!(ssd.empty())) {
        // pop first req in ssd q sched @ curr_tiem + howlong;
    }
    else {
        isSSDFree = true;
    }
    //process next process request
}

void user_request(int how_long, sched p) {
    int completion_time = curr_time + how_long;
    sched temp(completion_time, p.getEvents(), p.getPID(), p.getInteractive(), p.getIsComplete());
}

void user_release(sched p) {
    p.setInteractiveTrue();
    //process next process request
}

int main() {
    string str;
    int time;
    int numCores;
    nfreecores = numCores;
    cin >> str;
        cin >> numCores;
    int count = 0;
    while(getline(cin, str)) {
        eventList.push_back(str);
        count++;
    }
    
    createPQ();
    
    //print pq
    /*while (! pq.empty()) {
        sched t2 = pq.top();
        svprint(t2.events);
        cout << endl;
        pq.pop();
    }*/
    
    //vector<string> stringVec;
    /*for (int i = 1; i < eventList.size(); i++) {
        if (!(eventList[i].find("START") != string::npos)) {
            stringVec.push_back(eventList[i]);
        }
        else {break;}
    }
    svprint(stringVec);*/
    
    //iterate over pq
    while (!pq.empty()) {
        //set temp variable to top of the priority queue
        sched pqtemp = pq.top();
        curr_time = pqtemp.getArrival();
        vector<string> currEvent = pqtemp.getEvents();
        if (currEvent.size() == 1) {cout << "currevent size is 1" << endl;}
        else if (currEvent[0].find("CORE") != string::npos) {
            if (pqtemp.getIsComplete() == false) {
                core_request(getTime(currEvent[0]), pqtemp);
                cout << "core req called" << endl;
            }
            else {
                core_release(pqtemp);
                cout << "core release called" << endl;
            }
        }
        else if (currEvent[0].find("SSD") != string::npos) {
            if (pqtemp.getIsComplete() == false) {
                ssd_request(getTime(currEvent[0]), pqtemp);
                cout << "ssd req called" << endl;
            }
            else {
                ssd_release(pqtemp);
                cout << "ssd release called" << endl;
            }
        }
        else if (currEvent[0].find("TTY") != string::npos) {
            if(pqtemp.getIsComplete() == false) {
                user_request(getTime(currEvent[0]), pqtemp);
                cout << "TTY called " << endl;
            }
            else {
                user_release(pqtemp);
                cout << "tty release called" << endl;
            }
        }
    }
    return 0;
}
