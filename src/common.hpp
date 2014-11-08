#ifndef COMMON_HPP
#define COMMON_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include "Directed_weighted_graph.hpp"
#include "Bellman_Ford_algorithms.hpp"

using namespace std;

void setAskRate(Directed_weighted_graph<string>&, const string&, const string&, double, double);

void setBidRate(Directed_weighted_graph<string>&, const string&, const string&, double, double);

void tokenize(const string &str, vector<string>& tokens);

double getRate(const Directed_weighted_graph<string>&, const Cycle<string>&);

string downloadJSON(const string&);

#endif
