#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cassert>
#include "graph.h"
#include "city.h"
#include "person.h"
using namespace std;

Graph::Graph() {
    evader = NULL;

}

//adding a new city to the graph
bool Graph::addCity(const std::string& city_name) {

    //added in alphabetically
    if (cities.size() == 0) {
        City* city = new City(city_name);
        cities.push_back(city);
        return true;
    }

    for (vector<City*>::iterator itr = cities.begin(); itr != cities.end(); itr++) {
        //check if its already in the list
        if(city_name == (*itr)->getName()) {
            return false;
        }
        if(city_name < (*itr)->getName()) {
            City* city = new City(city_name);
            cities.insert(itr,city);
            return true;
        }
    }
    //if havent found it yet. add it to the end of the vector
    City* city = new City(city_name);
    cities.push_back(city);
    return true;
}
//removal of a city from the graph
bool Graph::removeCity(const std::string& city_name) {
    //finding the right city
    vector<City*>::iterator c = getCityWithName(city_name);
    //check if found
    if (!(*c))
        return false;

    else {
        //check to see if you need to remove players from the game too
        if (evader) {
            if (evader->getLocation()->getName() == city_name) {
                cout << "Evader " << evader->getName() << " removed from the game" << endl;
                delete evader;
                evader = NULL;
            }
        }
        vector<Person*>::iterator a = pursuers.begin();
        while(a != pursuers.end()) {
            if ((*a)->getLocation()->getName() == city_name) {
                cout << "Pursuer " << (*a)->getName() << " removed from the game" << endl;
                delete *a;
                *a = NULL;
                a = pursuers.erase(a);
            }
            else {
                a++;
            }
        }
        //deleting the city and reallocating memory
        (*c)->removeCityFromItsNeighbors();
        delete *c;
        cities.erase(c);
        return true;
    }



}

//connecting 2 cities together
bool Graph::addLink(const std::string& city_name1, const std::string& city_name2) {
    assert(city_name1 != city_name2);
    vector<City*>::iterator city1 =	getCityWithName(city_name1);
    vector<City*>::iterator city2 = getCityWithName(city_name2);
    //check to make sure they both exist
    if(city1 != cities.end() && city2 != cities.end()) {
        if ((*city1 != NULL) && (*city2 != NULL)) {
            if (!((*city1)->isNeighbor(*city2))) {
                (*city1)->addNeighbor(*city2);
                (*city2)->addNeighbor(*city1);
                return true;
            }
        }
    }
    return false;
}

//removal of a link between two cities.
bool Graph::removeLink(const std::string& city_name1, const std::string& city_name2) {
    assert(city_name1 != city_name2);
    vector<City*>::iterator city1 = getCityWithName(city_name1);
    vector<City*>::iterator city2 = getCityWithName(city_name2);
    //making sure they both exist and are in fact neighbors
    if ((*city1) && (*city2) && (*city1)->isNeighbor(*city2)) {
        (*city1)->removeNeighbor(*city2);
        (*city2)->removeNeighbor(*city1);
        return true;
    }

    return false;


}

//Creating and putting a pursuer on the map
bool Graph::placePursuer(const std::string& person_name, const std::string& city_name) {
    for (vector<Person*>::iterator itr = pursuers.begin(); itr != pursuers.end(); itr++) {
        //check if that pursuer has already been created
        if ((*itr)->getName() == person_name)
            return false;

    }
    if(*getCityWithName(city_name)) {
        Person* person = new Person(person_name, *getCityWithName(city_name));
        //new?
        pursuers.push_back(person);
        return true;
    }
    return false;
}

//same as placePursuer except now the evader instead.
bool Graph::placeEvader(const std::string& person_name, const std::string& city_name) {
    if (evader) {
        return false;
    }
    if(*getCityWithName(city_name)) {
        Person* person = new Person(person_name, *getCityWithName(city_name));
        evader = person;
        return true;
    }
    return false;
}



// OUTPUT
std::ostream& operator<<(std::ostream &ostr, const Graph &city_graph) {
//outputs cities and their connections and then players and their locations
    cout << endl;
    for (vector<City*>::const_iterator citylist = city_graph.cities.begin(); citylist != city_graph.cities.end(); citylist++) {
        cout << "Neighbors for city " << (*citylist)->getName() << ": ";
        for(vector<City*>::const_iterator temp = (*citylist)->getNeighbors().begin(); temp != (*citylist)->getNeighbors().end(); temp++) {
            cout << " " << (*temp)->getName();
        }
        cout << endl;

    }
    if (city_graph.evader != NULL) {
        cout << "Evader " <<  city_graph.evader->getName() <<" is in " << city_graph.evader->getLocation()->getName() << endl;
    }
    vector<Person*>::const_iterator a = city_graph.pursuers.begin();
    while(a != city_graph.pursuers.end()) {
        cout << "Pursuer " << (*a)->getName() << " is in " << (*a)->getLocation()->getName() << endl;
        a++;
    }
    cout << endl;

}



// helper function
//grabs a city name and returns an iterator to that city if it exists.
//if not return a NULL iterator
vector<City*>::iterator Graph::getCityWithName(const std::string& name) {
    for (vector<City*>::iterator itr = cities.begin(); itr != cities.end(); itr++) {
        if((*itr)->getName() == name) {
            return itr;
        }
    }
    return cities.end();


}


//Destructer
Graph::~Graph() {
    //gets rid of evador
    delete evader;
    //go through persuers and reallocate memory
    vector<Person*>::iterator a = pursuers.begin();
    vector<Person*>::iterator temp;
    while(a != pursuers.end()) {
        delete *a;
        *a = NULL;
        temp = pursuers.erase(a);

        a = temp;
    }
//go through cities and reallocate memory
    vector<City*>::iterator b = cities.begin();
    vector<City*>::iterator temp2;
    while(b != cities.end()) {
        (*b)->removeCityFromItsNeighbors();
        delete *b;
        temp2 = cities.erase(b);

        b = temp2;
    }



}

