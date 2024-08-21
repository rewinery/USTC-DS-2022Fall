#include <vector>
#include <unordered_map>
#include <ctime>
using namespace std;


class RandomizedCollection {
private:
	vector<int> v;
	unordered_map<int, pair<unordered_map<int, int>, vector<int>>> ump;
public:
	RandomizedCollection() {
		srand((unsigned)time(NULL));
	}

	bool insert(int val) {
		auto it = ump.find(val);
		if (it == ump.end()) {
			ump[val].first[0] = v.size();
			ump[val].second.push_back(v.size());
			v.push_back(val);
			return true;
		}
	}

	bool remove(int val) {

	}

	int getRandom() {
		int n = v.size();
		int index = rand() % n;
		return v[index];
	}
};

/**
 * Your RandomizedCollection object will be instantiated and called as such:
 * RandomizedCollection* obj = new RandomizedCollection();
 * bool param_1 = obj->insert(val);
 * bool param_2 = obj->remove(val);
 * int param_3 = obj->getRandom();
 */