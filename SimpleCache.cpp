#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
using namespace std;
struct Block {
  int tag;
  bool valid;
  int last_access_time;
  Block(): valid(false), last_access_time(0) {}
};
class Cache {
  public:
      Cache(int s, int a, int b): size(s), associativity(a), block_size(b) {
          assert(associativity <= size);
          blocks.resize(size);
      }
      int read(int address) {
          int offset = address % block_size;
          int tag = address / block_size;
          int set = tag % size;
          for (int way=0; way<associativity; ++way) {
              int index = set*associativity + way;
              if (blocks[index].valid && blocks[index].tag == tag) {
                  ++hits;
                  blocks[index].last_access_time = time;
                  return way;
              }
          }
          ++misses;
          int victim_way;
          if (associativity == 1) {
              victim_way = 0;
          } else {
              victim_way = findVictim(set);
          }
          int victim_index = set*associativity + victim_way;
          blocks[victim_index].tag = tag;
          blocks[victim_index].valid = true;
          blocks[victim_index].last_access_time = time;
          return victim_way;
      }
      void printStats() {
          cout << "Hits: " << hits << endl;
          cout << "Misses: " << misses << endl;
          cout << "Hit Rate: " << (double)hits/(hits+misses) << endl;
          cout << "Miss Rate: " << (double)misses/(hits+misses) << endl;
      }
  private:
      int size;
      int associativity;
      int block_size;
      int hits;
      int misses;
      int time;
      vector<Block> blocks;
      int findVictim(int set) {
          int victim_way = 0;
          int victim_time = blocks[set*associativity].last_access_time;
          for (int way=1; way<associativity; ++way) {
              int index = set*associativity + way;
              if (blocks[index].last_access_time < victim_time) {
                  victim_way = way;
                  victim_time = blocks[index].last_access_time;
              }
          }
          return victim_way;
      }
};
int main() {
  int size = 8;
  int associativity = 1;
  int block_size = 1;
  int address = 1;
  Cache cache(size, associativity, block_size);
  cache.read(address);
  cache.printStats();
  associativity = 2;
  Cache cache1(size, associativity, block_size);
  cache1.read(address);
  cache1.printStats();
  associativity = 4;
  Cache cache2(size, associativity, block_size);
  cache2.read(address);
  cache2.printStats();
  return 0;
}
