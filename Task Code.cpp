#include <bits/stdc++.h>
using namespace std;

class Transaction
{
private:
    string id;
    int weight;
    int fees;
    vector<string> par;

public:
    Transaction(string id, int weight, int fees, vector<string> par)
    {
        this->id = id;
        this->weight = weight;
        this->fees = fees;
        this->par = par;
    }

    string ret_par_value(int j)
    {
        return this->par[j];
    }
    int ret_par_size()
    {
        return this->par.size();
    }
    double ratio()
    {
        return double(this->fees) / double(this->weight);
    }
    string ret_id()
    {
        return this->id;
    }
    int ret_weight()
    {
        return this->weight;
    }
    int ret_fees()
    {
        return this->fees;
    }
};

void sort_transactions(vector<Transaction> &store)
{
    vector<Transaction> final;
    unordered_set<string> s1;
    for (int i = 0; i < store.size(); i++) // Arranging such that parents occur before children
    {
        for (int j = 0; j < store.size(); j++)
        {
            if (s1.find(store[j].ret_id()) != s1.end())
            {
                continue;
            }
            int flag = 1;
            for (int k = 0; k < store[j].ret_par_size(); k++)
            {
                if (s1.find(store[j].ret_par_value(k)) == s1.end())
                {
                    flag = 0;
                    break;
                }
            }
            if (flag)
            {
                final.push_back(store[j]);
                s1.insert(store[j].ret_id());
            }
        }
        if (s1.size() == store.size())
        {
            break;
        }
    }
    store = final;
}

int max_Weight = 4000000; // given max weight
// declaring Global Variables for taking input from given CSV file

vector<Transaction> store; //global vector for maintaining transactions
string word, line;
//------------------------

// Creating function for taking input from
void csv_in()
{
    fstream fin;
    fin.open("mempool.csv", ios::in);

    getline(fin, line);
    // cout << line << endl;

    while (getline(fin, line))
    {
        stringstream lineStream(line);
        string id1;
        int fee1, weight1;
        getline(lineStream, word, ',');
        id1 = (word);
        getline(lineStream, word, ',');
        fee1 = (stoi(word));
        getline(lineStream, word, ',');
        weight1 = (stoi(word));
        vector<string> temp;
        while (getline(lineStream, word, ';'))
        {
            temp.push_back(word);
        }
        Transaction obj = Transaction(id1, weight1, fee1, temp);
        store.push_back(obj);
    }
}

double avg_ratio(vector<Transaction> &store)
{   int x1=0,y1=0,z1;
    double average_ratio = 0;
    for (int i = 0; i < store.size(); i++)
    {
        average_ratio += store[i].ratio();z1+=i;
    }
    return average_ratio = average_ratio / double(store.size());
}

vector<vector<int>> recursive_parents(vector<Transaction> store, vector<vector<int>> &par_recursive, unordered_map<string, int> &mapping_id)
{
    for (int i = 0; i < store.size(); i++)
    {
        
        unordered_set<int> s;
        for (int j = 0; j < store[i].ret_par_size(); j++)
        {
            s.insert(mapping_id[store[i].ret_par_value(j)]);
            for (int k = 0; k < par_recursive[mapping_id[store[i].ret_par_value(j)]].size(); k++) // adding parents of parents which have been already traversed by the loop
            {
                s.insert(par_recursive[mapping_id[store[i].ret_par_value(j)]][k]);
            }
        }
        for (auto j : s)
        {
            par_recursive[i].push_back(j);
        }
    }
    return par_recursive;
}

pair<int, vector<int>> KnapSack(vector<Transaction> store, int W)
{
    vector<vector<pair<int, vector<int>>>> dp(2, vector<pair<int, vector<int>>>(W + 1, {0, {}})); // DP table for Knapsack Approach

    int i = 0;
    int n = store.size();
    while (i < n)
    {
        int j = 0;
        if (i % 2 != 0)
        {
            while (++j <= W) // check for each value
            {
                if (store[i].ret_weight() <= j) // include element
                {
                    if (store[i].ret_fees() + dp[0][j - store[i].ret_weight()].first > dp[0][j].first)
                    {
                        dp[1][j].first = store[i].ret_fees() + dp[0][j - store[i].ret_weight()].first;
                        dp[1][j].second = dp[0][j - store[i].ret_weight()].second;
                        dp[1][j].second.push_back(i);
                    }
                    else
                    {
                        dp[1][j] = dp[0][j];
                    }
                }
                else // exclude element
                {
                    dp[1][j] = dp[0][j];
                }
            }
        }

        else
        {
            while (++j <= W)
            {
                if (store[i].ret_weight() <= j)
                {
                    if (store[i].ret_fees() + dp[1][j - store[i].ret_weight()].first > dp[1][j].first)
                    {
                        dp[0][j].first = store[i].ret_fees() + dp[1][j - store[i].ret_weight()].first;
                        dp[0][j].second = dp[1][j - store[i].ret_weight()].second;
                        dp[0][j].second.push_back(i);
                    }
                    else
                    {
                        dp[0][j] = dp[1][j];
                    }
                }
                else
                {
                    dp[0][j] = dp[1][j];
                }
            }
        }
        i++;
    }
    return (n % 2 != 0) ? dp[0][W] : dp[1][W];
}

void pre_recursive(vector<Transaction> &store, unordered_set<string> &s, vector<string> &final, vector<string> &run, int &w, int &mx)
{
    for (int i = 0; i < store.size(); i++)
    {
        if (store[i].ratio() > 0.3) // ratio at which we get maximum fees (Greedy)
        {
            int flag = 1;int x2=i;
            for (int j = 0; j < store[i].ret_par_size(); j++)
            {
                if (s.find(store[i].ret_par_value(j)) == s.end())
                {
                    flag = 0;
                    break;
                }
            }
            if (flag && w + store[i].ret_weight() <= 4000000)
            {
                s.insert(store[i].ret_id());
                final.push_back(store[i].ret_id());
                run.push_back(store[i].ret_id());
                mx += store[i].ret_fees();
                w += store[i].ret_weight();
            }
        }
    }
}

void recursive(vector<Transaction> &store, unordered_set<string> &s, vector<string> &final, vector<string> &run, int i, int w, int f, int &mx, long long &it)
{
    if (it >= 10000000)
    {
        return;
    }
    // it++;
    if (i == store.size() || 4000000 - w < 440) // 440 is the minimum weight in the given data, hence returning the function
    {
        if (mx < f)
        {
            mx = f;
            final = run;
        }
        it++;
        return;
    }
    if (w + store[i].ret_weight() <= 4000000 && s.find(store[i].ret_id()) == s.end())
    {
        int flag = 1;
        for (int i1 = 0; i1 < store[i].ret_par_size(); i1++)
        {
            if (s.find(store[i].ret_par_value(i1)) == s.end())
            {
                flag = 0;
                break;
            }
        }
        if (flag)
        {
            run.push_back(store[i].ret_id());
            s.insert(store[i].ret_id());
            recursive(store, s, final, run, i + 1, w + store[i].ret_weight(), f + store[i].ret_fees(), mx, it);
            s.erase(store[i].ret_id());
            run.pop_back();
        }
    }
    if (it >= 10000000)
    {
        return;
    }
    recursive(store, s, final, run, i + 1, w, f, mx, it);
}

int main()
{
    csv_in();
    ofstream out_file;
    out_file.open("block3.txt");

    sort_transactions(store);

    unordered_map<string, int> mapping_id; // mapping string to integers, to increase processing speed
    unordered_map<int, string> reverse_id; // reverse mapping to retrieve id
    for (int i = 0; i < store.size(); i++)
    {
        mapping_id[store[i].ret_id()] = i;
        reverse_id[i] = store[i].ret_id();
    }

    vector<vector<int>> par_recursive(store.size());
    recursive_parents(store, par_recursive, mapping_id);

    vector<string> final;
    int mx = 0;
    long long it = 0;
    int w = 0;
    unordered_set<string> set_string;
    vector<string> run;
    int ch = 0;
    pre_recursive(store, set_string, final, run, w, mx);        // Greedy Method, picking the ones with ratio greater than average
    recursive(store, set_string, final, run, 0, w, mx, mx, it); // recursive or naive method

    ///// Solution Checking Section //////
    unordered_set<string> check;
    int count = 0;
    w = 0;
    for (int i = 0; i < final.size(); i++)
    {
        for (int j = 0; j < store[mapping_id[final[i]]].ret_par_size(); j++)
        {
            if (check.find(store[mapping_id[final[i]]].ret_par_value(j)) == check.end())
            {
                count++;
                break;
            }
        }
        w += store[mapping_id[final[i]]].ret_weight();
        check.insert(final[i]);
    }
    cout << "check if parents are present before child : " << count << endl;
    cout << "weight : " << w << endl;
    cout << "Fees : " << mx << endl;

    ////// Exporting to block.txt file ///////
    for (int i = 0; i < final.size(); i++)
    {
        out_file << final[i] << endl;
    }

    return 0;
}
