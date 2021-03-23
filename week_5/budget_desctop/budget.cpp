#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <ctime>

using namespace std;


constexpr int DAY_SEC = 86'400;

class Date
{
public:
    friend istream& operator>> (istream& is, Date& date);

    time_t AsTimestamp() const 
    {
        std::tm t;
        t.tm_sec = 0;
        t.tm_min = 0;
        t.tm_hour = 0;
        t.tm_mday = day_;
        t.tm_mon = month_ - 1;
        t.tm_year = year_ - 1900;
        t.tm_isdst = 0;

        return mktime(&t);
    }

private:
    int day_;
    int month_;
    int year_;
    
};


class BudgetManager
{
public:
    static size_t daysCount(const Date& from, const Date& to)
    {
        return (to.AsTimestamp() - from.AsTimestamp()) / DAY_SEC;
    }

    void earn(const Date& from, const Date& to, int sum)
    {
        size_t days_count = daysCount(from, to);
        double profit_for_day = static_cast<double>(sum) / days_count;
        
        time_t begin = from.AsTimestamp();
        for (size_t i = 0; i < days_count; ++i)
            profit[begin + (i * DAY_SEC)] += profit_for_day;
    }

    double compute_income(const Date& from, const Date& to)
    {
        double compute_profit = 0;

        size_t days_count = daysCount(from, to);
        time_t begin = from.AsTimestamp();
        for (size_t i = 0; i < days_count + 1; ++i)
            compute_profit += profit[begin + (i * DAY_SEC)];

        return compute_profit;
    }

    void pay_tax(const Date& from, const Date& to)
    {
        size_t days_count = daysCount(from, to);
        time_t begin = from.AsTimestamp();
        for (size_t i = 0; i < days_count + 1; ++i)
            profit[begin + (i * DAY_SEC)] *= 0.87;
    }

private:
    unordered_map<time_t, double> profit;
};


istream& operator >> (istream& is, Date& date)
{
    string str_date;
    cin >> str_date;
    stringstream date_stream(move(str_date));
    
    date_stream >> date.year_;
    date_stream.ignore();
    date_stream >> date.month_;
    date_stream.ignore();
    date_stream >> date.day_;

    return is;
}


int main() 
{
    int Q;
    cin >> Q;
    BudgetManager budget;

    for (int i = 0; i < Q; ++i)
    {
        string query;
        Date from, to;
        cin >> query;
        cin >> from >> to;

        if (query == "Earn")
        {
            int value;
            cin >> value;
            budget.earn(from, to, value);
        }
        else if (query == "ComputeIncome")
        {
            cout << setprecision(25) 
                << budget.compute_income(from, to) << endl;
        }
        else if (query == "PayTax")
        {
            budget.pay_tax(from, to);
        }
    }

    return 0;
}