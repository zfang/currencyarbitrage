#include "common.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <future>
#include <stdlib.h>
#include <getopt.h>
#include <json/json.h>
#include <linux/limits.h>

using namespace std;

void populateGraph(Directed_weighted_graph<string> & graph, double flotationCostInPercentage) {
   string line;
   vector<future<string> > futures;
   while (!cin.eof()) {
      getline(cin, line);
      futures.push_back(async(launch::async, downloadJSON, line));
   }

   Json::Reader reader;
   for (auto &f : futures) {
      Json::Value root;
      reader.parse(f.get(), root, false);
      for (auto &rate : root["query"]["results"]["rate"])
      {
         string from = rate["id"].asString().substr(0, 3);
         string to = rate["id"].asString().substr(3);
#ifdef SET_ASK_RATE
         setAskRate(graph, from, to, atof(rate["Ask"].asCString()), flotationCostInPercentage);
#endif
         setBidRate(graph, from, to, atof(rate["Bid"].asCString()), flotationCostInPercentage);
      }
   }
}

template<typename Type>
void printRates(Directed_weighted_graph<Type>& graph, Cycle<Type>& cycle) {
   string from = cycle.front();
   for (auto &to : cycle) {
      if (to == from) continue;
      cout << "\t" << from << " " << to << " " << exp(-graph.weight(from, to)) << endl;
      from = to;
   }

   cout << "\t" << from << " " << cycle.front() << " " << exp(-graph.weight(from, cycle.front())) << endl;
}

int main(int argc, char* args[]) {
   // Get the currencies json file
   char current_path[PATH_MAX];
   realpath(args[0], current_path);
   string path(current_path);
   path = path.substr(0, path.rfind('/')+1);
   ifstream currencies_file(path + "../src/currencies.all.json");
   Json::Value currencies_root;
   Json::Reader().parse(currencies_file, currencies_root, false);

   double flotationCostInPercentage = 0;
   int c;
   while ((c = getopt (argc, args, "f:")) != -1)
      switch (c) {
         case 'f':
            flotationCostInPercentage = atof(optarg);
            break;
      }

   Directed_weighted_graph<string> graph;
   populateGraph(graph, flotationCostInPercentage);
   vector<Cycle<string> > cycles;
   Bellman_Ford_algorithms<string>().find_shortest_paths_and_negative_cycles(graph, cycles);
   for (auto &cycle : cycles) {
      double rate = getRate(graph, cycle);
      if (rate < 1)
         cycle.reverse();
      cycle.adjust_by_source("USD");
      rate = getRate(graph, cycle);
      cout << "Rates:" << endl;
      printRates(graph, cycle);
      cout << "Names:" << endl;
      for (auto &to : cycle)
         cout << "\t" << to << ": " << currencies_root[to];

      cout << "Sequence:" << endl;
      cout << "\t" << cycle << cycle.front() << ": " <<  rate << endl << endl;
   }
   return 0;
}
