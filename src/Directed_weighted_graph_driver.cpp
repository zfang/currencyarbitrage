#include "Directed_weighted_graph.h"
#include "Bellman_Ford_algorithms.h"

#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>
#include <stdlib.h>

using namespace std;

vector<string> tokenize(string str) {
    // use stream iterators to copy the stream to the vector as
    // whitespace separated strings
   stringstream strstr(str);
   return vector<string> (istream_iterator<string> (strstr), istream_iterator<string>());
}

void populate_graph( Directed_weighted_graph<string> & graph ) {
   string line;
   while (!cin.eof()) {
      getline(cin, line);
      //cout << line << endl;
      vector<string> tokens = tokenize(line);
      if (tokens.size() < 3)
         continue;
      double weight = -log(atof(tokens[2].c_str()));
      graph.add_vertex(tokens[0]);
      graph.add_vertex(tokens[1]);
      graph.add_edge(tokens[0], tokens[1], weight);
      graph.add_edge(tokens[1], tokens[0], -weight);
   }
}

int main() {
   Directed_weighted_graph<string> graph;
   populate_graph(graph);
   /*
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
   */
   //cout << graph << endl;
   string src = "USD";
   Bellman_Ford_algorithms<string> algo (graph);
   for (auto &d : algo.find_shortest_paths_and_negative_cycles(src)) {
      for (auto &s : d)
         cout << s << " ";
      cout << endl;
   }
   //cout << algo << endl;
   return 0;
}
