#include "Directed_weighted_graph.hpp"
#include "Bellman_Ford_algorithms.hpp"

#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>
#include <stdlib.h>
#include <json/json.h>
#include <linux/limits.h>

using namespace std;

void tokenize(string str, vector<string>& tokens) {
   // use stream iterators to copy the stream to the vector as
   // whitespace separated strings
   stringstream strstr(str);
   tokens = vector<string> (istream_iterator<string> (strstr), istream_iterator<string>());
}

void populate_graph(Directed_weighted_graph<string> & graph ) {
   string line;
   while (!cin.eof()) {
      getline(cin, line);
      //cout << line << endl;
      vector<string> tokens;
      tokenize(line, tokens);
      if (tokens.size() < 4) continue;

      double ask = atof(tokens[2].c_str());
      if (ask) {
         graph.add_vertex(tokens[1]);
         graph.add_vertex(tokens[0]);
         graph.add_edge(tokens[1], tokens[0], log(ask));
      }

      double bid = atof(tokens[3].c_str());
      if (bid) {
         graph.add_vertex(tokens[0]);
         graph.add_vertex(tokens[1]);
         graph.add_edge(tokens[0], tokens[1], -log(bid));
      }
   }
}

template<typename Type>
double get_rate(Directed_weighted_graph<Type>& graph, Cycle<Type>& cycle) {
   string from = cycle.front();
   double rate = 1;
   for (auto &to : cycle) {
      if (to == from) continue;
      rate *= exp(-graph.weight(from, to));
      from = to;
   }

   return rate * exp(-graph.weight(from, cycle.front()));
}

template<typename Type>
void print_rates(Directed_weighted_graph<Type>& graph, Cycle<Type>& cycle) {
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
   ifstream currencies_file(path + "../src/currencies.json");
   Json::Value currencies_root;
   Json::Reader().parse(currencies_file, currencies_root);

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
   populate_graph(graph);
#endif
   vector<Cycle<string> > cycles;
   Bellman_Ford_algorithms<string>().find_shortest_paths_and_negative_cycles(graph, cycles);
   for (auto &cycle : cycles) {
      double rate = get_rate(graph, cycle);
      if (rate < 1)
         cycle.reverse();
      cycle.adjust_by_source("USD");
      rate = get_rate(graph, cycle);
      cout << "Rates:" << endl;
      print_rates(graph, cycle);
      cout << "Names:" << endl;
      for (auto &to : cycle)
         cout << "\t" << to << ": " << currencies_root[to];

      cout << "Sequence:" << endl;
      cout << "\t" << cycle << cycle.front() << ": " <<  rate << endl;
   }
   return 0;
}
