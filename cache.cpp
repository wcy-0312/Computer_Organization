#include <bits/stdc++.h>
using namespace std;

class Block
{
public:
    Block() : valid(false), tag(0){};
    bool valid;
    unsigned int tag;
};

// function
void direct_mapped();
void four_way_set_associative();
void fully_associative();

// file in & out
ifstream ifs; // for input file
ofstream ofs; // for output file

// global variables
int cache_size, block_size, associativity, replace_algorithm;
int blocks, tag, addr;
float access = 0.0f, hit = 0.0f;

int main(int argc, char *argv[])
{
    ifs.open(argv[1]), ofs.open(argv[2]);
    ifs >> cache_size >> block_size >> associativity >> replace_algorithm;
    blocks = cache_size / block_size;

    if (associativity == 0)
        direct_mapped();
    else if (associativity == 1)
        four_way_set_associative();
    else
        fully_associative();

    ofs << "Miss rate = " << 1 - hit / access;
    
    ifs.close();
    ofs.close();
    return 0;
}

void direct_mapped()
{
    int index;
    Block *cache = new Block[blocks];
    while (ifs >> addr)
    {
        access++;
        addr /= block_size;
        tag = addr / blocks;
        index = addr % blocks;

        if (cache[index].valid)
        {
            if (cache[index].tag == tag)
            {
                ofs << -1 << endl;
                hit++;
            }
            else
            {
                ofs << cache[index].tag << endl;
                cache[index].tag = tag;
            }
        }
        else
        {
            ofs << -1 << endl;
            cache[index].valid = true;
            cache[index].tag = tag;
        }
    }
}

void four_way_set_associative()
{
    int index;
    Block temp;
    bool find;
    blocks /= 4;
    list<Block> *cache = new list<Block>[blocks];
    for (int i = 0; i < blocks; i++)
        for (int j = 0; j < 4; j++)
            cache[i].push_back(temp);

    while (ifs >> addr)
    {
        find = false;
        access++;
        addr /= block_size;
        tag = addr / blocks;
        index = addr % blocks;
        list<Block>::iterator it = cache[index].begin();

        for (; it != cache[index].end() && it->valid; it++)
        {
            if (it->tag == tag) // Hit
            {
                hit++;
                find = true;
                break;
            }
        }

        if (replace_algorithm == 0) // FIFO
        {
            if (!find) // Not hit
            {
                if (it == cache[index].end())
                {
                    ofs << cache[index].begin()->tag << endl;
                    cache[index].pop_front();
                    temp.tag = tag, temp.valid = true;
                    cache[index].push_back(temp);
                }
                else // Not valid
                {
                    ofs << -1 << endl;
                    it->tag = tag;
                    it->valid = true;
                }
            }
            else
                ofs << -1 << endl;
        }
        else // LRU
        {
            if (it == cache[index].end()) // Not hit
            {
                ofs << cache[index].rbegin()->tag << endl;
                cache[index].pop_back();
                temp.tag = tag, temp.valid = true;
                cache[index].push_front(temp);
            }
            else // Hit or not valid
            {
                ofs << -1 << endl;
                cache[index].erase(it);
                temp.tag = tag, temp.valid = true;
                cache[index].push_front(temp);
            }
        }
    }
}

void fully_associative()
{
    Block temp;
    bool find;
    list<Block> cache;
    for (int i = 0; i < blocks; i++)
        cache.push_back(temp);
    
    while (ifs >> addr)
    {
        find = false;
        access++;
        addr /= block_size;
        tag = addr;
        
        list<Block>::iterator it = cache.begin();

        for (; it != cache.end() && it->valid; it++)
        {
            if (it->tag == tag) // Hit
            {
                hit++;
                find = true;
                break;
            }
        }

        if (replace_algorithm == 0) // FIFO
        {
            if (!find) // Not hit
            {
                if (it == cache.end())
                {
                    ofs << cache.begin()->tag << endl;
                    cache.pop_front();
                    temp.tag = tag, temp.valid = true;
                    cache.push_back(temp);
                }
                else // Not valid
                {
                    ofs << -1 << endl;
                    it->tag = tag;
                    it->valid = true;
                }
            }
            else
                ofs << -1 << endl;
        }
        else // LRU
        {
            if (it == cache.end()) // Not hit
            {
                ofs << cache.rbegin()->tag << endl;
                cache.pop_back();
                temp.tag = tag, temp.valid = true;
                cache.push_front(temp);
            }
            else // Hit or not valid
            {
                ofs << -1 << endl;
                cache.erase(it);
                temp.tag = tag, temp.valid = true;
                cache.push_front(temp);
            }
        }
    }
}