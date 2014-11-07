#include "common.hpp"

void setAskRate(Directed_weighted_graph<string> & graph, const string &from, const string &to, double ask, double flotationCostInPercentage) {
   if (ask) {
      graph.add_vertex(to);
      graph.add_vertex(from);
      graph.add_edge(to, from, log(ask * (1 - flotationCostInPercentage/100)));
   }
}

void setBidRate(Directed_weighted_graph<string> & graph, const string &from, const string &to, double bid, double flotationCostInPercentage) {
   if (bid) {
      graph.add_vertex(from);
      graph.add_vertex(to);
      graph.add_edge(from, to, -log(bid * (1 - flotationCostInPercentage/100)));
   }
}

void tokenize(const string &str, vector<string> &tokens) {
   // use stream iterators to copy the stream to the vector as
   // whitespace separated strings
   stringstream strstr(str);
   tokens = vector<string> (istream_iterator<string> (strstr), istream_iterator<string>());
}

double getRate(const Directed_weighted_graph<string>& graph, const Cycle<string>& cycle) {
   string from = cycle.front();
   double rate = 1;
   for (auto &to : cycle) {
      if (to == from) continue;
      rate *= exp(-graph.weight(from, to));
      from = to;
   }

   return rate * exp(-graph.weight(from, cycle.front()));
}
