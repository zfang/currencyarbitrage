#include "common.hpp"
#include <cstring>
#include <cmath>
#include <curl/curl.h>
#include <sstream>
#include <iterator>

void setAskRate(Directed_weighted_graph<string> & graph, const string &from, const string &to, double ask, double flotationCostInPercentage) {
   if (ask) {
      graph.add_vertex(to);
      graph.add_vertex(from);
      graph.add_edge(to, from, log(ask * (1 + flotationCostInPercentage/100)));
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

static size_t writer(char *ptr, size_t size, size_t nmemb, string *userdata) {
    if (userdata)  {
        userdata->append(ptr, size * nmemb);
        return size * nmemb;
    }

    return 0;
}

string downloadJSON(const string& URL)
{
   CURL *curl;
   CURLcode res;
   struct curl_slist *headers = nullptr;
   std::ostringstream oss;
   curl_slist_append(headers, "Accept: application/json");
   curl_slist_append( headers, "Content-Type: application/json");
   curl_slist_append( headers, "charsets: utf-8");
   curl = curl_easy_init();
   string data;

   if (curl)
   {
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
      curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
      curl_easy_setopt(curl, CURLOPT_HTTPGET,1);
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&data);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
      res = curl_easy_perform(curl);
   }

   curl_easy_cleanup(curl);

   return data;
}
