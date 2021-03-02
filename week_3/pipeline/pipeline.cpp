#include "test_runner.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;


struct Email {
    string from;
    string to;
    string body;

    Email() = default;
    Email(string& _from, string& _to, string& _body)
        : from(_from), to(_to), body(_body)
    {
    }
};


class Worker {
public:
    virtual ~Worker() = default;
    virtual void Process(unique_ptr<Email> email) = 0;
    virtual void Run() {
        // только первому worker-у в пайплайне нужно это имплементировать
        throw logic_error("Unimplemented");
    }

protected:
    // реализации должны вызывать PassOn, чтобы передать объект дальше
    // по цепочке обработчиков
    void PassOn(unique_ptr<Email> email) const
    {
        if (next_worker)
            next_worker->Process(move(email));
    }

public:
    void SetNext(unique_ptr<Worker> next)
    {
        next_worker = move(next);
    }

private:
    unique_ptr<Worker> next_worker;
};


class Reader : public Worker {
public:
    Reader(istream& in)
        : in_(in)
    {
    }

    void Process(unique_ptr<Email> email) override
    {
    }

    void Run() override
    {
        while (!in_.eof())
        {
            auto email = make_unique<Email>();
            getline(in_, email->from);
            getline(in_, email->to);
            getline(in_, email->body);

            PassOn(move(email));
        }
    }

private:
    istream& in_;
};


class Filter : public Worker {
public:
    using Function = function<bool(const Email&)>;

    Filter(Function predicate)
        : predicate_(predicate)
    {
    }

    void Process(unique_ptr<Email> email) override
    {
        if (predicate_(*email))
            PassOn(move(email));
    }


public:
    Function predicate_;
};


class Copier : public Worker {
public:
    Copier(const string& address)
        : address_(address)
    {
    }

    void Process(unique_ptr<Email> email) override
    {
        if (email->to != address_)
        {
            auto copy_email = make_unique<Email>(email->from, address_, email->body);
            PassOn(move(email));
            PassOn(move(copy_email));
        }
        else
            PassOn(move(email));
    }


private:
    string address_;

};


class Sender : public Worker {
public:
    Sender(ostream& os)
        : os_(os)
    {
    }

    void Process(unique_ptr<Email> email) override
    {
        os_ << email->from << endl
            << email->to << endl
            << email->body << endl;

        PassOn(move(email));
    }
    
private:
    ostream& os_;
};


// реализуйте класс
class PipelineBuilder {
public:
    // добавляет в качестве первого обработчика Reader
    explicit PipelineBuilder(istream& in)
    {
        workers.push_back(make_unique<Reader>(in));
    }

    // добавляет новый обработчик Filter
    PipelineBuilder& FilterBy(Filter::Function filter)
    {
        workers.push_back(make_unique<Filter>(move(filter)));
        return *this;
    }

    // добавляет новый обработчик Copier
    PipelineBuilder& CopyTo(string recipient)
    {
        workers.push_back(make_unique<Copier>(move(recipient)));
        return *this;
    }

    // добавляет новый обработчик Sender
    PipelineBuilder& Send(ostream& out)
    {
        workers.push_back(make_unique<Sender>(out));
        return *this;
    }

    // возвращает готовую цепочку обработчиков
    unique_ptr<Worker> Build()
    {
        for (size_t i = workers.size() - 1; i > 0; --i)
            workers[i - 1]->SetNext(move(workers[i]));

        return move(workers[0]);
    }

private:
    vector<unique_ptr<Worker>> workers;

};


void TestSanity() {
    string input = (
        "erich@example.com\n"
        "richard@example.com\n"
        "Hello there\n"

        "erich@example.com\n"
        "ralph@example.com\n"
        "Are you sure you pressed the right button?\n"

        "ralph@example.com\n"
        "erich@example.com\n"
        "I do not make mistakes of that kind\n"
        );
    istringstream inStream(input);
    ostringstream outStream;

    PipelineBuilder builder(inStream);
    builder.FilterBy([](const Email& email) {
        return email.from == "erich@example.com";
    });
    builder.CopyTo("richard@example.com");
    builder.Send(outStream);
    auto pipeline = builder.Build();

    pipeline->Run();

    string expectedOutput = (
        "erich@example.com\n"
        "richard@example.com\n"
        "Hello there\n"

        "erich@example.com\n"
        "ralph@example.com\n"
        "Are you sure you pressed the right button?\n"

        "erich@example.com\n"
        "richard@example.com\n"
        "Are you sure you pressed the right button?\n"
        );

    ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSanity);
    return 0;
}