#include "request_queue.h"

RequestQueue::RequestQueue(const SearchServer& search_server) : server_(search_server)
{
}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentStatus status) {
    QueryResult query_result = {raw_query, server_.FindTopDocuments(raw_query, status)};
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

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query) {
    QueryResult query_result = {raw_query, server_.FindTopDocuments(raw_query)};
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

int RequestQueue::GetNoResultRequests() const
{
    return count_no_result_requests_;
}

void RequestQueue::DeleteFirst()
{
    if(requests_.size() >= min_in_day_)
    {
        if(requests_.front().result.empty())
        {
            count_no_result_requests_--;
        }
        requests_.pop_front();
    }
}
