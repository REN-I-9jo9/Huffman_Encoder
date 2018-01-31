#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
using namespace std;
struct node
{
    string symbol;
    int frequency;
    int father;
    int child_0;
    int child_1;
    string code;
    node(string a,int b)
    {
        symbol=a;
        frequency=b;
        father=0;
        child_0=0;
        child_1=0;
        code="";
    }
};
int main()
{
/** --------------------Read The File-------------------- **/
    ifstream in;
    string filename;
    while(true)
    {
        cout<<"File Name: ";
        cin>>filename;
        cout<<"Read '"<<filename<<"' ";
        in.open(filename.c_str());
        if(in)
            break;
        else
            cout<<"Fail , Again"<<endl;
    }
    cout<<"Success"<<endl;
    stringstream strStream;
    strStream << in.rdbuf();// Read the file
    string data = strStream.str();
    in.close();
/** --------------------Read The File-------------------- **/
/** --------------------Count --------------------------- **/
    string words="";
    vector<int> counts;
    for(int i=0;i<data.length();i++)
    {
        size_t found =words.find(data[i]);
        if(found!=string::npos)
            counts[found]++;
        else
        {
            string a(1,data[i]);
            words+=a;
            counts.push_back(1);
        }
    }
    vector<node> tree;
    int frequency_sum=0;
    int wordscount=words.length();
    for(int i=0;i<wordscount;i++)
    {
        frequency_sum+=counts[i];
        tree.push_back(node(string(1,words[i]),counts[i]));
    }
/** --------------------Count --------------------------- **/
/** --------------------Creat Tree----------------------- **/
    int biggest=0;
    for(int i=0;i<tree.size();i++)
        if(tree[i].frequency>=tree[biggest].frequency)
            biggest=i;
    int root_frequency=0;
    while(root_frequency<frequency_sum)
    {
        int smallest=biggest,second_small=biggest;
        for(int i=0;i<tree.size();i++)
            if(i==0||tree[i].frequency<=tree[smallest].frequency)
                smallest=i;
        for(int i=0;i<tree.size();i++)
            if((i!=smallest)&&(i==0||tree[i].frequency<=tree[second_small].frequency))
                second_small=i;

        root_frequency=tree[smallest].frequency+tree[second_small].frequency;
        node newnode(tree[second_small].symbol+tree[smallest].symbol,root_frequency);
        newnode.child_0=smallest;
        newnode.child_1=second_small;
        tree[smallest].father=tree.size();
        tree[second_small].father=tree.size();
        tree.push_back(newnode);

        tree[smallest].frequency+=frequency_sum;
        tree[second_small].frequency+=frequency_sum;
    }
/** --------------------Creat Tree----------------------- **/
/** --------------------Make The Code-------------------- **/
    for(int i=tree.size()-1;i>=words.length();i--)
    {
        tree[tree[i].child_1].code=tree[i].code+"1";
        tree[tree[i].child_0].code=tree[i].code+"0";
    }
/** --------------------Make The Code-------------------- **/
/** --------------------Print The Tree------------------- **/
    cout<<setfill('_')<<setw(wordscount*7)<<"_"<<setfill(' ')<<endl;
    for(int i=0;i<tree.size();i++)
    {
        if(tree[i].frequency>frequency_sum)
            tree[i].frequency-=frequency_sum;

        cout<<"_"<<setw(wordscount)<<tree[i].symbol<<"_";
        cout<<" "<<setw(5)<<tree[i].frequency<<" ";
        node f=tree[tree[i].father],c_0=tree[tree[i].child_0],c_1=tree[tree[i].child_1];
        if(i==tree.size()-1)
            cout<<setw(wordscount)<<"*ROOT"<<" "<<setw(wordscount)<<c_1.symbol<<" "<<setw(wordscount)<<c_0.symbol;
        else if(i>=words.length())
            cout<<setw(wordscount)<<f.symbol<<" "<<setw(wordscount)<<c_1.symbol<<" "<<setw(wordscount)<<c_0.symbol;
        else
            cout<<setw(wordscount)<<f.symbol<<" "<<setw(wordscount)<<"*LEAF"<<" "<<setw(wordscount)<<"*LEAF";
        cout<<endl;
    }
    cout<<endl;
    for(int i=0;i<wordscount;i++)
        cout<<tree[i].symbol<<" : "<<tree[i].code<<endl;
    cout<<endl;
/** --------------------Print The Tree------------------- **/
/** --------------------Transfer Original File----------- **/
    string code_data="";
    for(int i=0;i<data.length();i++)
        code_data+=tree[words.find(data[i])].code;

    int code_data_length=(int)(code_data.size()/8+1)*8;
    bool code_data_bool[code_data_length]={0};
    for(int i=0;i<code_data.size();i++)
        code_data_bool[i]=(code_data[i]=='1')?true:false;

    char new_data[code_data_length];
    for(int i=0;i<code_data_length/8;i++)
        new_data[i]=(code_data_bool[i*8]<<7)+
                    (code_data_bool[i*8+1]<<6)+
                    (code_data_bool[i*8+2]<<5)+
                    (code_data_bool[i*8+3]<<4)+
                    (code_data_bool[i*8+4]<<3)+
                    (code_data_bool[i*8+5]<<2)+
                    (code_data_bool[i*8+6]<<1)+
                    (code_data_bool[i*8+7]);
/** --------------------Transfer Original File----------- **/
/** --------------------Print New Data------------------- **/
    cout<<code_data.size()<<" "<<code_data<<endl;
    cout<<code_data_length/8<<" ";
    for(int i=0;i<code_data_length/8;i++)
        cout<<setw(8)<<(int)new_data[i];
/** --------------------Print New Data------------------- **/
/** --------------------Write To New File---------------- **/
    ofstream out;
    out.open((filename+"_Out.txt").c_str());
    out.close();
    out.open((filename+"_Out.txt").c_str(),ios::binary);
    if(!out)
        cout<<"out fail";
    for(int i=0;i<code_data_length/8;i++)
        out.put(new_data[i]);
    out.close();
/** --------------------Write To New File---------------- **/
}
