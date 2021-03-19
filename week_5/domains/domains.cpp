#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

using namespace std;


bool IsSubdomain(string_view subdomain_view, string_view domain_view) {
    const string subdomain(subdomain_view.rbegin(), subdomain_view.rend());
    const string domain(domain_view.rbegin(), domain_view.rend());
    int i = subdomain.size() - 1;
    int j = domain.size() - 1;
    while (i >= 0 && j >= 0) {
        if (subdomain[i--] != domain[j--]) {
            return false;
        }
    }
    return (i < 0 && j < 0) || (i < 0 && domain[j] == '.')
        || (j < 0 && subdomain[i] == '.');
}


vector<string> ReadDomains() 
{
    size_t count;
    cin >> count;

    vector<string> domains(count);
    for (string& domain : domains)
        cin >> domain;
    
    return domains;
}

void CheckDomains(vector<string>& banned_domains, 
    vector<string>& domains_to_check)
{
    for (string& domain : banned_domains) 
        reverse(begin(domain), end(domain));

    sort(begin(banned_domains), end(banned_domains));

    size_t insert_pos = 0;
    for (string& domain : banned_domains) {
        if (insert_pos == 0 || !IsSubdomain(domain, banned_domains[insert_pos - 1])) {
            swap(banned_domains[insert_pos++], domain);
        }
    }
    banned_domains.resize(insert_pos);

    for (string& domain : domains_to_check) {
        reverse(domain.begin(), domain.end());
        if (const auto it = upper_bound(begin(banned_domains), end(banned_domains), domain);
            it != begin(banned_domains) && IsSubdomain(domain, *prev(it))) {
            cout << "Bad" << endl;
        }
        else {
            cout << "Good" << endl;
        }
    }
}


int main() 
{
    vector<string> banned_domains = ReadDomains();
    vector<string> domains_to_check = ReadDomains();
    CheckDomains(banned_domains, domains_to_check);
    
    return 0;
}