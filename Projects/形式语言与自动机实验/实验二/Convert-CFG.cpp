#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <deque>

using namespace std;

//生成式
struct PRO
{
    string key;              //生成符左边
    deque<string> dervation; //生成符右边
};

//格式化生成式
PRO makePro(deque<string> symbol);

//读入文法文件
bool readFile(vector<PRO> &pros);

//去除单个空转移符号
void removeEsp(vector<PRO> &pros, char canEsp);

//去除空转移
void emptyPro(vector<PRO> &pros);

//去除单生成式
void unitPro(vector<PRO> &pros);

//去除无用符号
void usefulPro(vector<PRO> &pros);

//去除非产生
void removeUnTo(vector<PRO> &pros);

//去除非可达
void removeUnFrom(vector<PRO> &pros);

//输出生成式
void printPro(vector<PRO> &pros);

//消除重复符号
void merge(vector<PRO> &pros);

//判断是否在vector中
bool inVector(vector<char> v, char element);

int main(void)
{
    vector<PRO> pros;
    if (!readFile(pros))
    {
        cout << "Error: open file CFG.txt" << endl;
        return 0;
    }
    cout << "Original productions:" << endl;
    printPro(pros);

    emptyPro(pros);
    cout << "After removing empty productions:" << endl;
    printPro(pros);

    unitPro(pros);
    cout << "After removing unit productions:" << endl;
    printPro(pros);

    usefulPro(pros);
    cout << "After removing useless symbols:" << endl;
    printPro(pros);

    system("pause");
    return 0;
}

//格式化生成式
PRO makePro(deque<string> symbol)
{
    PRO pro;
    pro.key = symbol[0];
    symbol.pop_front();
    pro.dervation = symbol;
    return pro;
}

//读入文法文件
bool readFile(vector<PRO> &pros)
{
    string line;
    ifstream file("CFG.txt", ios::in);
    deque<string> tmp;
    regex rgx("(\\->)|(\\|)"); //以->或|为间隔读取

    if (!file.good())
        return false;

    while (getline(file, line))
    {
        if (line == "")
            continue;

        sregex_token_iterator it(line.begin(), line.end(), rgx, -1);
        for (sregex_token_iterator end; it != end; it++)
            tmp.push_back(*it);

        pros.push_back(makePro(tmp));
        tmp.clear();
    }
    file.close();
    return true;
}

//输出生成式
void printPro(vector<PRO> &pros)
{
    for (int i = 0; i < pros.size(); i++)
    {
        cout << pros[i].key << " -> " << pros[i].dervation[0];
        for (int j = 1; j < pros[i].dervation.size(); j++)
        {
            cout << "|" << pros[i].dervation[j];
        }
        cout << "\n";
    }
}

//消除重复符号
void merge(vector<PRO> &pros)
{
    for (auto i = pros.begin(); i < pros.end(); i++)
    {
        for (auto j = (*i).dervation.begin(); j < (*i).dervation.end(); j++)
        {
            for (auto k = j + 1; k < (*i).dervation.end();)
            {
                if (*k == *j)
                    k = (*i).dervation.erase(k);
                else
                    k++;
            }
        }
    }
}

//去除单个空转移符号
void removeEsp(vector<PRO> &pros, char canEsp)
{
    for (auto k = pros.begin(); k < pros.end(); k++)
    {
        if ((*k).key == "S1")
            continue;

        for (auto j = (*k).dervation.begin(); j < (*k).dervation.end(); j++) //遍历产生式右边
        {
            const string ori = (*j);
            string tmp = ori;
            for (int i = 0; i < tmp.size(); i++) //可致空符号取自身或esp
            {
                if (tmp[i] == canEsp)
                {
                    tmp.erase(i, 1);
                    if (tmp != "")
                        (*k).dervation.push_back(tmp);
                    tmp = ori;
                }
            }
        }
    }
}

//去除空转移
void emptyPro(vector<PRO> &pros)
{
    bool flag;
    char key;
    for (auto i = pros.begin(); i < pros.end();) //遍历vector
    {
        flag = false;
        for (auto j = (*i).dervation.begin(); j < (*i).dervation.end();) //遍历deque
        {
            if ((*j) != "*") //不是esp
            {
                j++;
                continue;
            }

            if (i == pros.begin()) //插入新符号S1
            {
                PRO newpro;
                newpro.key = "S1";
                newpro.dervation.push_back("S");
                newpro.dervation.push_back("*");
                i = pros.insert(pros.begin(), newpro);
                break;
            }

            //对可致空符号的操作
            key = (*i).key[0];
            if ((*i).dervation.size() == 1)
            {
                i = pros.erase(i);
                flag = true;
            }
            else
            {
                j = (*i).dervation.erase(j);
            }                     //去除*符号,j+1
            removeEsp(pros, key); //可致空符号为key
            break;
        }
        if (flag == false)
            i++;
    }
    merge(pros);
}

//去除单生成式
void unitPro(vector<PRO> &pros)
{
    string key;
    for (auto i = pros.begin(); i < pros.end(); i++)
    {
        for (auto j = (*i).dervation.begin(); j < (*i).dervation.end();)
        {
            if ((*j).size() != 1 || ((*j))[0] > 'Z' || (*j)[0] < 'A')
            {
                j++;
                continue;
            }

            key = (*j);
            j = (*i).dervation.erase(j); //去除单生成式符号

            for (auto k = pros.begin(); k < pros.end(); k++) //补全
            {
                if ((*k).key == key)
                {
                    (*i).dervation.insert((*i).dervation.end(), (*k).dervation.begin(), (*k).dervation.end());
                }
            }
        }
    }
    merge(pros);
}

//去除无用符号,先删去非产生，再删去非可达
void usefulPro(vector<PRO> &pros)
{
    removeUnTo(pros);
    removeUnFrom(pros);
}

//去除非产生
void removeUnTo(vector<PRO> &pros)
{
    bool isTo = true, flag;
    vector<char> to;
    to.push_back('*');
    for (char c = 'a'; c <= 'z'; c++)
        to.push_back(c);
    int preSize = to.size();

    for (;;) //构造非产生符号集
    {
        for (auto i = pros.begin(); i < pros.end(); i++)
        {
            for (auto j = (*i).dervation.begin(); j < (*i).dervation.end(); j++)
            {
                isTo = true;
                for (int k = 0; k < (*j).size(); k++)
                {
                    if (!inVector(to, (*j)[k])) //某个符号非产生
                    {
                        isTo = false;
                        break;
                    }
                }
                if (isTo == true && !inVector(to, (*i).key[0]))
                {
                    to.push_back((*i).key[0]);
                    break;
                }
            }
        }
        if (preSize != to.size()) //产生符号数量不再增长
            preSize = to.size();
        else
            break;
    }

    for (auto i = pros.begin(); i < pros.end();)
    {
        if (!inVector(to, (*i).key[0]))
        {
            i = pros.erase(i);
            continue;
        }
        for (auto j = (*i).dervation.begin(); j < (*i).dervation.end();)
        {
            flag = false;
            for (int k = 0; k < (*j).size(); k++)
            {
                if (!inVector(to, (*j)[k])) //某个符号非产生
                {
                    j = (*i).dervation.erase(j);
                    flag = true;
                    break;
                }
            }
            if (flag == false)
                j++;
        }
        i++;
    }
}

//去除非可达
void removeUnFrom(vector<PRO> &pros)
{
    bool flag;
    vector<char> from;
    from.push_back('S');
    from.push_back('*');
    for (char c = 'a'; c <= 'z'; c++)
        from.push_back(c);
    int preSize = from.size();
    for (;;)
    {
        for (auto i = pros.begin(); i < pros.end(); i++)
        {
            if (!inVector(from, (*i).key[0]))
                continue;
            for (auto j = (*i).dervation.begin(); j < (*i).dervation.end(); j++)
            {
                for (int k = 0; k < (*j).size(); k++)
                {
                    if (!inVector(from, (*j)[k]) && (*j)[k] >= 'A' && (*j)[k] <= 'Z')
                        from.push_back((*j)[k]);
                }
            }
        }
        if (preSize != from.size()) //可达符号数量不再增长
            preSize = from.size();
        else
            break;
    }

    bool isFrom;
    for (auto i = pros.begin(); i < pros.end();)
    {
        if (!inVector(from, (*i).key[0]))
        {
            i = pros.erase(i);
            continue;
        }
        for (auto j = (*i).dervation.begin(); j < (*i).dervation.end();)
        {
            flag = false;
            for (int k = 0; k < (*j).size(); k++)
            {
                if (!inVector(from, (*j)[k])) //某个符号非可达
                {
                    j = (*i).dervation.erase(j);
                    flag = true;
                    break;
                }
            }
            if (flag == false)
                j++;
        }
        i++;
    }
}

//vector中是否存在元素
bool inVector(vector<char> v, char element)
{
    auto it = find(v.begin(), v.end(), element);
    if (it != v.end())
        return true;
    else
        return false;
}