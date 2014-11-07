#include "common.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <getopt.h>
#include <json/json.h>
#include <linux/limits.h>

using namespace std;

void populateGraph(Directed_weighted_graph<string> & graph, double flotationCostInPercentage) {
   string line;
   while (!cin.eof()) {
      getline(cin, line);
      //cout << line << endl;
      vector<string> tokens;
      tokenize(line, tokens);
      if (tokens.size() < 4) continue;

#ifdef ASK
      setAskRate(graph, tokens[0], tokens[1], atof(tokens[2].c_str()), flotationCostInPercentage);
#endif

      setBidRate(graph, tokens[0], tokens[1], atof(tokens[3].c_str()), flotationCostInPercentage);
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
   Json::Reader().parse(currencies_file, currencies_root);

   double flotationCostInPercentage = 0;
   int c;
   while ((c = getopt (argc, args, "f:")) != -1)
      switch (c) {
         case 'f':
            flotationCostInPercentage = atof(optarg);
            break;
      }

   Directed_weighted_graph<string> graph;
#ifdef TEST_BASE_CASE
   string currencies[] = {"EUR", "USD", "GBP"};
   graph.add_vertex(currencies[0]);
   graph.add_vertex(currencies[1]);
   graph.add_vertex(currencies[2]);
   graph.add_edge(currencies[0], currencies[1], -log(1.1837));
   graph.add_edge(currencies[1], currencies[0], -log(1/1.1837));
   graph.add_edge(currencies[0], currencies[2], -log(0.7231));
   graph.add_edge(currencies[2], currencies[0], -log(1/0.7231));
   graph.add_edge(currencies[2], currencies[1], -log(1.6388));
   graph.add_edge(currencies[1], currencies[2], -log(1/1.6388));
   cout << graph << endl;
#else
   populateGraph(graph, flotationCostInPercentage);
#endif
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
