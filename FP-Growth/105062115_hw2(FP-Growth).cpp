#include <iostream>
#include <stdio.h>
#include <fstream>
#include <set>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>
#include <math.h>
#include <iomanip>
using namespace std;
map<int,int> header_table; // THE HEADER TABLE WITHOUT POINTER
map<int,int> temp_header_table; // TO PRESERVE THE TEMP VALUE WHEN MINING
vector<pair<vector<int>,int>> final_result;
// DEFINTE FP-TREE NODE
class FP_Node
{
  public :
    FP_Node(int);
    int content;
    int count;
    vector <FP_Node*> child;
    FP_Node *parent;
    FP_Node *header;
    int tempValue;
};
FP_Node :: FP_Node(int input)
{
  //next.push_back(nullptr);
  content = input;
  count = 1;
  parent = nullptr;
  header = nullptr;
  tempValue = 0;
}

vector<pair<pair<int,int>,vector<FP_Node *>>> header_linker;
//STRING SPLIT FUNCTION
void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
  std::string::size_type pos1, pos2;
  pos2 = s.find(c);
  pos1 = 0;
  while(std::string::npos != pos2)
  {
    v.push_back(s.substr(pos1, pos2-pos1));

    pos1 = pos2 + c.size();
    pos2 = s.find(c, pos1);
  }
  if(pos1 != s.length())
    v.push_back(s.substr(pos1,s.length()-pos1));
}

// COMPARE FUNCTION USE IN SORTING DATABASE
bool compare(int a, int b)
{
  if(header_table[a] > header_table[b])
    return true;
  else
    return false;
}

bool compare_headerLinker(pair<pair<int,int>,vector<FP_Node *>> a, pair<pair<int,int>,vector<FP_Node *>> b)
{
  if(a.first.second > b.first.second)
    return true;
  else
    return false;
}

bool compare_final(pair<vector<int>,int> a,pair<vector<int>,int> b)
{
  if(a.first.size() < b.first.size())
    return true;
  else if(a.first.size() > b.first.size())
    return false;
  for(int s=0;s<a.first.size();s++)
  {
    if(a.first[s] < b.first[s])
      return true;
    else if(a.first[s] > b.first[s])
      return false;
  }
  return false;
}

void insert_headerlinker(FP_Node *a,int item)
{
  int i=0;
  for(;i<header_linker.size();i++)
  {
    if(header_linker[i].first.first == item)
      break;
  }
  if(i == header_linker.size()) cout << "MISTAKE!!!" << endl;
  header_linker[i].second.push_back(a);
  return;
}

void print_FPTree(FP_Node *now)
{
  for(int i=0;i<now->child.size();i++)
    print_FPTree(now->child[i]);
  if(now->child.size() == 0)
    cout << "LEAF  ";
  cout << now->content << " N " << now->count << endl;
}

void miningSub__FP_TREE(FP_Node *now)
{
  int plusValue = now->count;
  now = now->parent;
  while(now->content != -1)
  {
    temp_header_table[now->content] += plusValue;
    now = now->parent;
  }
}


vector<vector<int>> getPowerSet(vector<int> input, int set_size,int org)
{
    /*set_size of power set of a set with set_size
    n is (2**n -1)*/
    unsigned int pow_set_size = pow(2, set_size);
    int counter, j;
    vector<vector<int>> result_set;
    /*Run from counter 000..0 to 111..1*/
    for(counter = 0; counter < pow_set_size; counter++)
    {
      vector<int> a;
      int ans = 100001;
      for(j = 0; j < set_size; j++)
      {
        if(counter & (1 << j))
        {
          a.push_back(input[j]);
          //if(temp_header_table[input[j]] < ans) ans = temp_header_table[input[j]];
        }
      }
      a.push_back(org);
      stable_sort(a.begin(),a.end(),compare);
      //pair<vector<int>,int> result= make_pair(a,ans);
      result_set.push_back(a);
    }
    return result_set;
}

void count_support(int support)
{
  //sort(mining_power_result.begin(),mining_power_result.end());
  for(int i=header_linker.size()-1;i>=0;i--)
  {
      //temp_header_table[header_linker[i].first.first] = new_min_support;
      for (int j=0;j<header_linker[i].second.size();j++)
      {
        miningSub__FP_TREE(header_linker[i].second[j]);
      }
  }
}

bool vector_equal(vector<int> a,vector<int> b)
{
  if(a.size() != b.size()) return false;
  for(int k=0;k<a.size();k++)
  {
    if(a[k] != b[k])
      return false;
  }
  return true;
}
int main(int argc, char *argv[])
{
    string line;
    float min_support = stof(argv[1]);
    ifstream input_file(argv[2]);
    int count = 0;

    // FILE INPUT
    vector <string> input;
    if(input_file.is_open())
    {
      while(getline(input_file,line))
      {
        input.push_back(line);
        count++;
      }
      input_file.close();
    }

    // DATABASE CONSTRUCTION
    vector <vector <int>> database;
    for(int i=0;i<count;i++)
    {
      vector <string> split_result;
      vector<int> now;
      database.push_back(now);
      SplitString(input[i],split_result,",");
      for(vector<string>::const_iterator j = split_result.begin(); j != split_result.end(); ++j)
        database[i].push_back(stoi(*j));
    }


    /*for(vector<vector<int>>::const_iterator j = database.begin(); j != database.end(); ++j)
    {
      for(vector<int>::const_iterator i = (*j).begin(); i != (*j).end();++i)
        cout << *i << " ";
      cout << endl;
    }*/


    // HEADER_TABLE CONSTRUCTION
    //map<int,int> header_table;
    for(vector<vector<int>>::const_iterator j = database.begin(); j != database.end(); ++j)
    {
      for(vector<int>::const_iterator i = (*j).begin(); i != (*j).end();++i)
      {
        if(header_table.find(*i) != header_table.end())
          header_table[*i] = header_table[*i] + 1;
        else
          header_table[*i] = 1;
      }
    }


    /*for(map<int,int>::const_iterator j = header_table.begin(); j != header_table.end(); ++j)
    {
      cout << (*j).first << " " << (*j).second << endl;
    }*/


    //DELETE ELEMENT FEWER THAN MIN-SUPPORT IN HEADER_TABLE
    float new_min_support = count * min_support;
    vector<map<int,int>::const_iterator> erase_list_ht;
    vector<int> erase_list_db;
    for(map<int,int>::const_iterator j = header_table.begin(); j != header_table.end(); ++j)
    {
      if((*j).second < new_min_support)
      {
        erase_list_ht.push_back(j);
        erase_list_db.push_back((*j).first);
      }
    }
    for(int i=0;i<erase_list_ht.size();i++)
      header_table.erase(erase_list_ht[i]);

    /*cout << "HEADER_TABLE" << endl;
    for(map<int,int>::const_iterator j = header_table.begin(); j != header_table.end(); ++j)
    {
      cout << (*j).first << " " << (*j).second << endl;
    }
    cout << "HEADER_TABLE" << endl;*/


    //DELETE ELEMENT FEWER THAN MIN-SUPPORT IN DATABASE AND TURN IT INTO SORTED VECTOR
    for(int i=0;i<count;i++)
      for(int j=0;j<erase_list_db.size();j++)
        if(find(database[i].begin(),database[i].end(),erase_list_db[j]) != database[i].end())
            database[i].erase(find(database[i].begin(),database[i].end(),erase_list_db[j]));


    /*for(vector<vector<int>>::const_iterator j = database.begin(); j != database.end(); ++j)
    {
      for(vector<int>::const_iterator i = (*j).begin(); i != (*j).end();++i)
        cout << *i << " ";
      cout << endl;
    }
    cout << endl;*/

    //SORT DATABASE IN ORDER
    for(int i=0;i<count;i++)
      stable_sort(database[i].begin(),database[i].end(),compare);


    /*cout << "DATABASE" << endl;
    for(vector<vector<int>>::const_iterator j = database.begin(); j != database.end(); ++j)
    {
      for(vector<int>::const_iterator i = (*j).begin(); i != (*j).end();++i)
        cout << *i << " ";
      cout << endl;
    }
    cout << "DATABASE" << endl;*/




    // CONSTRUCT HEADER_LINKER TABLE
    for(map<int,int>::const_iterator j = header_table.begin(); j != header_table.end(); ++j)
    {
      vector<FP_Node *> a;
      header_linker.push_back(make_pair(make_pair(j->first,j->second),a));
    }

    stable_sort(header_linker.begin(),header_linker.end(),compare_headerLinker);

    FP_Node *root = new FP_Node(-1);
    FP_Node *now = root;
    bool isNext = false;
    /*
    FP_Node *test = new FP_Node(1);
    header_linker[0].second.push_back(root);
    header_linker[1].second.push_back(test);

    for(int i=0;i<header_linker.size();i++)
    {
      cout << header_linker[i].first.first << "   " << header_linker[i].first.second << endl;
      for (int j=0;j<header_linker[i].second.size();j++)
        cout << (header_linker[i].second)[j]->content << endl;
    }*/



    //cout << root->content << endl;

    //CONSTRUCT TREE
    for (int i=0;i<count;i++)
    {
      now = root;
      for(int j=0;j<database[i].size();j++)
      {
        //cout << "NOW POINTER : " << now->content << endl;
        //cout << "NOW PROCESSING : " << database[i][j] << endl;
        for(int k=0;k<now->child.size();k++)
        {
          if(now->child[k]->content == database[i][j])
          {
            now = now->child[k];
            (now->count)++;
            isNext = true;
            break;
          }
        }
        if(!isNext)
        {
          FP_Node *temp = new FP_Node(database[i][j]);
          now->child.push_back(temp);
          temp->parent = now;
          now = temp;
          insert_headerlinker(temp,database[i][j]);
        }
        isNext = false;
      }
      //cout << "THIS TURN RESULT : " << endl;
      //print_FPTree(root);
    }
    //TEST FP_TREE
    //print_FPTree(root);
    //TEST HEADER_LINKER TABLE
    /*for(int i=0;i<header_linker.size();i++)
    {
      cout << header_linker[i].first.first << "   " << header_linker[i].first.second << endl;
      for (int j=0;j<header_linker[i].second.size();j++)
        cout << header_linker[i].second[j]->content << "  N : " << header_linker[i].second[j]->count << endl;
    }*/





    //CONSTRUCT TEMP_HEADER_TABLE
    /*for(map<int,int>::const_iterator j = header_table.begin(); j != header_table.end(); ++j)
      temp_header_table[(*j).first] = 0;*/

    //MINING FP_TREE
    //vector<vector<int>>mining_result;
    //set<set<int>> check_result;
    int count0 = 0;
    for(int i=header_linker.size()-1;i>=0;i--)
    {
        //temp_header_table[header_linker[i].first.first] = new_min_support;
        vector<vector<int>> mining_power_result;
        for (int j=0;j<header_linker[i].second.size();j++)
        {
          miningSub__FP_TREE(header_linker[i].second[j]);
        }

        // PRINT TEMP_HEADER_TABLE AND CLEAR
        //cout << "{";
        vector <int>tempp;

        for(map<int,int>::const_iterator j = temp_header_table.begin(); j != temp_header_table.end(); ++j)
        {
          //cout << (*j).first << " " << (*j).second << endl;

          if((*j).second >= new_min_support)
          {
              tempp.push_back((*j).first);
          }
          temp_header_table[(*j).first] = 0;
        }
        //cout << "---------------------------------------" << endl;
        mining_power_result = getPowerSet(tempp,tempp.size(),header_linker[i].first.first);
        sort(mining_power_result.begin(),mining_power_result.end());
        for(int k=0;k<mining_power_result.size();k++)
        {
          final_result.push_back(make_pair(mining_power_result[k],0));
        }


        for (int j=0;j<header_linker[i].second.size();j++)
        {
          //cout << "IN" << endl;
          vector <int>tempp2;
          vector<vector<int>> power_temp;
          miningSub__FP_TREE(header_linker[i].second[j]);
          for(int w=0;w<tempp.size();w++)
          {
            if(temp_header_table[tempp[w]] > 0)
              tempp2.push_back(tempp[w]);
          }
          power_temp = getPowerSet(tempp2,tempp2.size(),header_linker[i].first.first);
          sort(power_temp.begin(),power_temp.end());
          for(int s = count0;s<count0+mining_power_result.size();s++)
          {
            for(int l = 0;l<power_temp.size();l++)
            {
              if(vector_equal(final_result[s].first,power_temp[l]))
              {
                final_result[s].second += header_linker[i].second[j]->count;
              }
            }
          }

          for(map<int,int>::const_iterator j = temp_header_table.begin(); j != temp_header_table.end(); ++j)
          {
            temp_header_table[(*j).first] = 0;
          }
        }
        count0 = count0+mining_power_result.size();
        /*cout << "FINAL RESULT" << endl;
        for(int s = 0;s<final_result.size();s++)
        {
          for(int q = 0;q<final_result[s].first.size();q++)
          {
            cout << final_result[s].first[q] << " ";
          }
          cout << "NUM : ";
          cout << float(final_result[s].second)/20;
          cout << endl;
        }
        cout << "FINAL RESULT" << endl;
        cout << "----------------" << endl;*/
        //powerSets.clear();
        //cout << "}" << endl;
        //cout << endl;
        //tempp.push_back(header_linker[i].first.first);
        //mining_result.push_back(tempp);
    }

    vector<vector<pair<vector<int>,int>>::const_iterator> erase_list_fn;
    vector<pair<vector<int>,int>> final;
    for(vector<pair<vector<int>,int>>::const_iterator s = final_result.begin(); s != final_result.end(); ++s)
    {
      if((*s).second >= new_min_support)
      {
        vector<int> tem = (*s).first;
        sort(tem.begin(),tem.end());
        final.push_back(make_pair(tem,(*s).second));
      }

    }

    try
    {
      sort(final.begin(),final.end(),compare_final);
    }
    catch (invalid_argument& e)
    {
      cerr << e.what() << endl;
      return -1;
    }

    string output_file = argv[3];
    ofstream write_file(output_file);
    if(write_file.is_open())
    {
      write_file.setf(ios::showpoint);
      for(int s = 0;s<final.size();s++)
      {
        for(int q = 0;q<final[s].first.size();q++)
        {
          if(q != final[s].first.size()-1)
            write_file << final[s].first[q] << ",";
          else
            write_file << final[s].first[q];
        }
        write_file << ":";
        write_file << setprecision(4) << float(final[s].second)/20;
        write_file << endl;
      }
      write_file.close();
    }
    //count_support(new_min_support);
    //PRINT MINING_POWER_RESULT
    /*for(vector<pair<vector<int>,int>>::const_iterator j = mining_power_result.begin(); j != mining_power_result.end(); ++j)
    {
      for(vector<int>::const_iterator i = (*j).first.begin(); i != (*j).first.end();++i)
          cout << *i << " ";
      cout << " S : ";
      cout << (*j).second;
      cout << endl;
    }*/

    /*for(vector<vector<int>>::const_iterator j = mining_power_result.begin(); j != mining_power_result.end(); ++j)
    {
      for(vector<int>::const_iterator i = (*j).begin(); i != (*j).end();++i)
        cout << *i << " ";
      cout << endl;
    }*/

    // CALCULATE RESULT





}
