#include "Common.h"
#include <mutex>
#include <unordered_map>
#include <list>

using namespace std;

class LruCache : public ICache {
public:
    LruCache(
        shared_ptr<IBooksUnpacker> books_unpacker,
        const Settings& settings
    ) : book_unpack_(move(books_unpacker)), settings_(settings)
    {
    }

    BookPtr GetBook(const string& book_name) override {
        lock_guard<mutex> lock(mx);

        if (auto it = cache_iter.find(book_name); it != cache_iter.end())
        {
            cache.splice(cache.begin(), cache, it->second);
            return cache.front();
        }

        auto book = book_unpack_->UnpackBook(book_name);

        if (book->GetContent().size() > settings_.max_memory)
        {
            cache.clear();
            cache_iter.clear();
            cache_size = 0;

            return move(book);
        }

        cache_size += book->GetContent().size();
        while (cache_size > settings_.max_memory)
        {
            cache_size -= cache.back()->GetContent().size();
            cache_iter.erase(cache.back()->GetName());
            cache.pop_back();
        }

        cache.push_front(move(book));
        cache_iter[cache.front()->GetName()] = cache.begin();

        return *cache_iter[book_name];
    }

private:
    shared_ptr<IBooksUnpacker> book_unpack_;
    Settings settings_;
    unordered_map<string, list<BookPtr>::iterator> cache_iter;
    list<BookPtr> cache;
    int cache_size = 0;
    mutex mx;
};


unique_ptr<ICache> MakeCache(
    shared_ptr<IBooksUnpacker> books_unpacker,
    const ICache::Settings& settings
) {
    return make_unique<LruCache>(books_unpacker, settings);
}