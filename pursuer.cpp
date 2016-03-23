#include <cassert>
#include <iostream>
#include <vector>
#include "MersenneTwister.h"
#include "person.h"
#include "city.h"
#include "graph.h"
using namespace std;

//finds the move that would ultimately allow the shortest time to the  student. This is done recursivly
City* closestPath(City* p, const Graph& city_graph, int &min, int length, vector<City*> previous) {
    if (length + 1 > min && min != -1) {
        return NULL;
    }
    for (vector<City*>::iterator i = previous.begin(); i != previous.end(); i++) {
        if (*i == p) {
            return NULL;
        }
    }
    if (p == (city_graph.getEvader())->getLocation()) {
        if (length + 1 < min || min == -1) {
            min = length + 1;
        }
        return p;
    }


    previous.push_back(p);
    vector<City*> neib = (p)->getNeighbors();
    vector<City*>::iterator a = neib.begin();

    City* best;

    while(a != neib.end()) {

        City* temp = closestPath(*a,city_graph,min,length + 1,previous);

        if (temp) {
            best = temp;
        }
        a++;
    }
    if (length == -1) {
        return best;
    }
    if (best) {
        return p;
    }
    else {
        return NULL;
    }

}

City* MY_PURSUER_STRATEGY(const Person& p, const Graph& city_graph) {
    int temp = -1;
    vector<City*> passed;
    return closestPath(p.getLocation(), city_graph, temp, -1, passed);
}

