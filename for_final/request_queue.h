#pragma once

#include <queue>
#include "search_server.h"

class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server);
    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
        QueryResult query_result = {raw_query, server_.FindTopDocuments(raw_query, document_predicate)};
        if(query_result.result.empty())
        {
            count_no_result_requests_++;
            requests_.push_back(query_result);
            DeleteFirst();
        }
        else
        {
            requests_.push_back(query_result);
            DeleteFirst();
        }
        return query_result.result;
    }
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);
    std::vector<Document> AddFindRequest(const std::string& raw_query);
    int GetNoResultRequests() const;
private:
    void DeleteFirst();
    struct QueryResult
    {
        std::string req;
        std::vector<Document> result;
    };
    std::deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;
    int count_no_result_requests_ = 0;
    SearchServer server_;
};
