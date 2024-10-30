#pragma once

#include "document.h"
#include "request_queue.h"

template <typename Iterator>
class IteratorRange {
public:
    IteratorRange() = default;
    IteratorRange(const Iterator& iterator_begin,const Iterator& iterator_end)
    {
        iterator_begin_ = iterator_begin;
        iterator_end_ = iterator_end;
    }
    auto begin() const {
        return iterator_begin_;
    }
    auto end() const {
        return iterator_end_;
    }

private:
    Iterator iterator_begin_;
    Iterator iterator_end_;
};
template <typename Iterator>
class Paginator {
public:
    Paginator(const Iterator& iterator_begin, const Iterator& iterator_end, const size_t& page_size)
    {
        if (distance(iterator_begin, iterator_end) <= page_size)
        {
            results_.push_back(IteratorRange(iterator_begin, iterator_end));
        }
        else
        {
            Iterator j = iterator_begin;
            for(auto i = iterator_begin; i != iterator_end; i++)
            {
                if(distance(j, iterator_end) >= page_size)
                {
                    advance(j, page_size);
                    results_.push_back(IteratorRange(i, j));
                    advance(i, 1);
                }
                else
                {
                    results_.push_back(IteratorRange(j, iterator_end));
                }
            }
        }
    }
    auto begin()const {
        return results_.begin();
    }
    auto end()const {
        return results_.end();
    }

private:
    std::vector<IteratorRange<Iterator>> results_;
};
std::ostream& operator<<(std::ostream& output, const Document& document) {
    output << "{ "
           << "document_id = " << document.id << ", "
           << "relevance = " << document.relevance << ", "
           << "rating = " << document.rating << " }";
    return output;
}

template <typename Iterator>
std::ostream& operator<<(std::ostream& output, const IteratorRange<Iterator>& document_range) {
    for (auto now : document_range) {
        output << now;
    }
    return output;
}
template<typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(c.begin(), c.end(), page_size);
}
