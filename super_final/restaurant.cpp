#include "main.h"

class BST;
class Node_Heap;
class Heap;
class HuffNode;
class HuffTree;
class Node_BST;

int MAXSIZE;
int order_used = 0;
int order_Huff = 0;

vector<BST *> Gojo;
vector<queue<int>> order_Gojo;
vector<vector<int>> comb;
Heap *Sukuna;
map<char, pair<char, int>> m;
HuffTree *curr = nullptr;

// Huffman Tree
class HuffNode
{
public:
	char ch;
	int weight;
	HuffNode *pLeft, *pRight;
	bool isLeaf;
	HuffNode(const char &ch, const int &wgt) : ch(ch), weight(wgt), pLeft(nullptr), pRight(nullptr), isLeaf(true) {}
	HuffNode(HuffNode *left, HuffNode *right) : ch('0'), weight(left->weight + right->weight), pLeft(left), pRight(right), isLeaf(false) {}
	~HuffNode() {}
};

class HuffTree
{
public:
	HuffNode *root;
	HuffTree() : root(nullptr) {}
	HuffTree(const char &val, const int &freq) { root = new HuffNode(val, freq); }
	HuffTree(HuffTree *left, HuffTree *right) { root = new HuffNode(left->root, right->root); }
	~HuffTree() {}

public:
	void inOrder(HuffNode *root)
	{
		if (root != nullptr)
		{
			inOrder(root->pLeft);
			if (root->isLeaf)
				cout << root->ch << endl;
			else
				cout << root->weight << endl;
			inOrder(root->pRight);
		}
	}
	void inOrder() { inOrder(this->root); }
	int getHeight(HuffNode *root)
	{
		if (root->isLeaf)
			return 0;
		int heightLeft = getHeight(root->pLeft);
		int heightRight = getHeight(root->pRight);
		return max(heightLeft, heightRight) + 1;
	}
	int getBalance(HuffNode *root)
	{
		return getHeight(root->pLeft) - getHeight(root->pRight);
	}
	HuffNode *rotateLeft(HuffNode *root)
	{
		HuffNode *tmp = root->pRight;
		root->pRight = tmp->pLeft;
		tmp->pLeft = root;
		root = tmp;
		return root;
	}
	HuffNode *rotateRight(HuffNode *root)
	{
		HuffNode *tmp = root->pLeft;
		root->pLeft = tmp->pRight;
		tmp->pRight = root;
		root = tmp;
		return root;
	}
	void Balance(HuffNode *&root, int &count)
	{
		if (root->isLeaf)
			return;
		if (count == 0)
			return;
		int BalanceFactor = getBalance(root);
		if (BalanceFactor == 0 || abs(BalanceFactor) == 1)
			return;
		if (BalanceFactor > 1)
		{
			int BalanceFactorLeft = getBalance(root->pLeft);
			if (BalanceFactorLeft == 0 || BalanceFactorLeft >= 1)
			{
				count--;
				root = rotateRight(root);
			}
			else if (BalanceFactorLeft <= -1)
			{
				root->pLeft = rotateLeft(root->pLeft);
				root = rotateRight(root);
				count--;
			}
		}
		else if (BalanceFactor < -1)
		{
			int BalanceFactorRight = getBalance(root->pRight);
			if (BalanceFactorRight == 0 || BalanceFactorRight <= -1)
			{
				count--;
				root = rotateLeft(root);
			}
			else if (BalanceFactorRight >= 1)
			{
				root->pRight = rotateRight(root->pRight);
				root = rotateLeft(root);
				count--;
			}
		}
		Balance(root->pLeft, count);
		Balance(root->pRight, count);
		Balance(root, count);
	}
	void delHuff(HuffNode *root)
	{
		if (root)
		{
			delHuff(root->pLeft);
			delHuff(root->pRight);
			delete root;
		}
	}
	void clear() { delHuff(this->root); }
};

struct compare_HuffTree
{
	bool operator()(pair<HuffTree *, int> x, pair<HuffTree *, int> y) const
	{
		return x.first->root->weight != y.first->root->weight ? x.first->root->weight > y.first->root->weight : x.second > y.second;
	}
};
HuffTree *buildHuff(vector<pair<char, int>> vt)
{
	priority_queue<pair<HuffTree *, int>, vector<pair<HuffTree *, int>>, compare_HuffTree> pq;
	for (auto it : vt)
		pq.push({new HuffTree(it.first, it.second), order_Huff++});
	HuffTree *tmp1, *tmp2, *tmp3 = nullptr;
	while (pq.size() > 1)
	{
		tmp1 = pq.top().first;
		pq.pop();
		tmp2 = pq.top().first;
		pq.pop();
		tmp3 = new HuffTree(tmp1, tmp2);
		int cnt = 3;
		tmp3->Balance(tmp3->root, cnt);
		pq.push({tmp3, order_Huff++});
		delete tmp1;
		delete tmp2;
	}
	return pq.top().first;
}

// BST
class Node_BST
{
public:
	int result;
	Node_BST *pLeft, *pRight;

public:
	Node_BST(int result) : result(result), pLeft(nullptr), pRight(nullptr) {}
	~Node_BST() {}
};
class BST
{
public:
	Node_BST *root;

public:
	BST() : root(nullptr) {}
	~BST() { clear(); }

	void add(Node_BST *&root, const int &result)
	{
		if (root == nullptr)
		{
			root = new Node_BST(result);
			return;
		}
		if (result < root->result)
			add(root->pLeft, result);
		else
			add(root->pRight, result);
	}
	void add(const int &result) { add(this->root, result); }
	void del(Node_BST *&root, const int &result)
	{
		if (root)
		{
			if (result < root->result)
				del(root->pLeft, result);
			else if (result > root->result)
				del(root->pRight, result);
			else
			{
				if (root->pLeft && root->pRight)
				{
					Node_BST *p = root->pRight;
					while (p->pLeft)
						p = p->pLeft;
					root->result = p->result;
					del(root->pRight, root->result);
				}
				else if (root->pLeft)
				{
					Node_BST *p = root;
					root = root->pLeft;
					delete p;
				}
				else if (root->pRight)
				{
					Node_BST *p = root;
					root = root->pRight;
					delete p;
				}
				else
				{
					delete root;
					root = nullptr;
				}
			}
		}
	}
	void del(const int &result) { del(this->root, result); }

	void inOrder(Node_BST *root)
	{
		if (root != nullptr)
		{
			inOrder(root->pLeft);
			cout << root->result << endl;
			inOrder(root->pRight);
		}
	}
	void inOrder() { inOrder(this->root); }

	void preOrder(Node_BST *root, vector<int> &str)
	{
		if (root)
		{
			str.push_back(root->result);
			preOrder(root->pLeft, str);
			preOrder(root->pRight, str);
		}
	}
	vector<int> preOrder()
	{
		vector<int> str;
		preOrder(this->root, str);
		return str;
	}

	void delBST(Node_BST *root)
	{
		if (root)
		{
			delBST(root->pLeft);
			delBST(root->pRight);
			delete root;
		}
	}
	void clear() { delBST(this->root); }
};

// Min Heap
class Node_Heap
{
public:
	vector<pair<int, int>> area;
	int used;

public:
	Node_Heap(const int &ID, const int &result, const int &used)
	{
		this->area.push_back({ID, result});
		this->used = used;
	}
	~Node_Heap() {}
};
class Heap
{
public:
	vector<Node_Heap *> ele;
	Heap() { ele.resize(0); }
	~Heap()
	{
		for (auto node : ele)
			delete node;
		ele.clear();
		ele.resize(0);
		ele.shrink_to_fit();
	}

public:
	void reheapUp(int index)
	{
		while (index)
		{
			int pIdx = (index - 1) >> 1;
			if (greater(ele[index], ele[pIdx]))
				return;
			Node_Heap *temp(move(ele[index]));
			ele[index] = move(ele[pIdx]);
			ele[pIdx] = move(temp);
			index = pIdx;
		}
	}
	void reheapDown(int index)
	{
		while (index < (int)ele.size())
		{
			int cIdx = index * 2 + 1;
			if (cIdx >= (int)ele.size())
				return;
			if (cIdx + 1 < (int)ele.size())
				if (greater(ele[cIdx], ele[cIdx + 1]))
					cIdx++;
			if (greater(ele[cIdx], ele[index]))
				return;
			Node_Heap *temp(move(ele[index]));
			ele[index] = move(ele[cIdx]);
			ele[cIdx] = move(temp);
			index = cIdx;
		}
	}
	void push(const int &ID, const int &result)
	{
		for (int i = 0; i < (int)ele.size(); i++)
		{
			if (ID == ele[i]->area.front().first)
			{
				ele[i]->area.push_back({ID, result});
				ele[i]->used = order_used++;
				reheapDown(i);
				return;
			}
		}
		Node_Heap *tmp = new Node_Heap(ID, result, order_used++);
		ele.push_back(tmp);
		reheapUp(ele.size() - 1);
	}
	void updateHeap(int idx)
	{
		if (ele[idx]->area.size() > 0)
		{
			reheapUp(idx);
			return;
		}
		ele[idx] = ele[ele.size() - 1];
		ele.pop_back();
		reheapDown(idx);
		reheapUp(idx);
	}
	bool greater(Node_Heap *x, Node_Heap *y)
	{
		if (x->area.size() != y->area.size())
			return x->area.size() > y->area.size();
		return x->used > y->used;
	}
	void preOrder(int index, const int &num)
	{
		if (index < (int)ele.size())
		{
			int length = ele[index]->area.size() - 1;
			for (int i = 0; i < (int)((num > (int)ele[index]->area.size()) ? ele[index]->area.size() : num); i++, length--)
				cout << ele[index]->area[length].first << "-" << ele[index]->area[length].second << endl;
			preOrder(2 * index + 1, num);
			preOrder(2 * index + 2, num);
		}
	}
	void preOrder(int num) { preOrder(0, num); }
};

string decode(HuffNode *root, const char &ch, const string &path)
{
	if (root->isLeaf && root->ch == ch)
		return path;
	else
	{
		string leftPath = "";
		string rightPath = "";
		if (root->pLeft)
			leftPath = decode(root->pLeft, ch, path + '0');
		if (root->pRight)
			rightPath = decode(root->pRight, ch, path + '1');
		return leftPath.empty() ? rightPath : leftPath;
	}
}
string convert_name_to_binary(const string &name, HuffNode *root)
{
	string binary = "";
	for (auto it : name)
		binary += decode(root, m[it].first, "");
	return binary;
}

bool compare_freq_ascii(const pair<char, int> &x, const pair<char, int> &y)
{
	if (x.second != y.second)
		return x.second < y.second;
	if (x.first <= 'Z' && y.first >= 'a')
		return false;
	else if (x.first >= 'a' && y.first <= 'Z')
		return true;
	else
		return x.first < y.first;
}
char Caecar(char x, const int &step)
{
	if ('a' <= x && x <= 'z')
		return (x - 'a' + step % 26) % 26 + 'a';
	else if ('A' <= x && x <= 'Z')
		return (x - 'A' + step % 26) % 26 + 'A';
	else
		return x;
}

int convert_binary_to_decimal(const string &binary)
{
	int ans = 0;
	int i = binary.length() - 1;
	for (int j = 0; j < min(10, (int)binary.length()); j++)
		ans = ans * 2 + (binary[i--] - '0');
	return ans;
}

bool compare_used(Node_Heap *x, Node_Heap *y)
{
	return x->area.size() != y->area.size() ? x->area.size() < y->area.size() : x->used < y->used;
}

long long DFS(vector<int> &arr)
{
	int n = arr.size();
	if (n <= 2)
		return 1;
	vector<int> left, right;
	for (int i = 1; i < n; i++)
	{
		if (arr[i] < arr[0])
			left.push_back(arr[i]);
		else
			right.push_back(arr[i]);
	}
	long long res = comb[n - 1][left.size()];
	res = (res % MAXSIZE * DFS(left) % MAXSIZE) % MAXSIZE;
	res = (res % MAXSIZE * DFS(right) % MAXSIZE) % MAXSIZE;
	return (int)res;
}
long long countBST(vector<int> &arr)
{
	if (arr.size() <= 1)
		return (int)arr.size() % MAXSIZE;
	int n = arr.size();
	comb = vector<vector<int>>((n + 1), vector<int>(n + 1));
	comb[0][0] = 1;
	for (int i = 1; i <= n; i++)
	{
		comb[i][0] = 1;
		for (int j = 1; j <= i; j++)
			comb[i][j] = ((comb[i - 1][j - 1] % MAXSIZE + comb[i - 1][j] % MAXSIZE)) % MAXSIZE;
	}
	return DFS(arr) % MAXSIZE;
}

void LAPSE(const string &name)
{
	m.clear();
	map<char, int> encode;
	vector<pair<char, int>> vt;
	for (int i = 0; i < (int)name.length(); i++)
		m[name[i]].second++;
	if (m.size() < 3)
		return;
	for (auto &it : m)
		it.second.first = Caecar(it.first, it.second.second);
	for (auto it : m)
		encode[it.second.first] += it.second.second;
	for (auto it : encode)
		vt.push_back({it.first, it.second});
	sort(vt.begin(), vt.end(), compare_freq_ascii);
	if (curr)
	{
		curr->clear();
		delete curr;
		curr = nullptr;
	}
	curr = buildHuff(vt);
	string binary = convert_name_to_binary(name, curr->root);
	int result = convert_binary_to_decimal(binary);
	int ID = result % MAXSIZE + 1;
	if (result % 2 == 0) // Sukuna's restaurant
	{
		Sukuna->push(ID, result);
	}
	else // Gojo's restaurant
	{
		int area = result % MAXSIZE;
		order_Gojo[area].push(result);
		Gojo[area]->add(result);
	}
}
void KOKUSEN()
{
	for (int i = 0; i < MAXSIZE; i++)
	{
		if (!order_Gojo[i].empty())
		{

			vector<int> arr = Gojo[i]->preOrder();
			map<int, int> m;
			for (int it : arr)
				m[it]++;
			int Y = countBST(arr);
			while (Y--)
			{
				if (!order_Gojo[i].empty())
				{
					int head = order_Gojo[i].front();
					order_Gojo[i].pop();
					Gojo[i]->del(head);
				}
				else
					break;
			}
		}
	}
}
void KEITEIKEN(const int &num)
{
	vector<Node_Heap *> vt;
	for (auto it : Sukuna->ele)
		vt.push_back(it);
	sort(vt.begin(), vt.end(), compare_used);
	int size_heap = (num > (int)vt.size()) ? vt.size() : num;
	for (int i = 0; i < size_heap; i++)
	{
		Node_Heap *update_Node = vt.front();
		vt.erase(vt.begin());
		int size_area = num > (int)update_Node->area.size() ? update_Node->area.size() : num;
		for (int j = 0; j < size_area; j++)
		{
			auto it_del = update_Node->area.begin();
			cout << (*it_del).second << "-" << (*it_del).first << endl;
			update_Node->area.erase(it_del);
		}
		for (int k = 0; k < (int)Sukuna->ele.size(); k++)
			if (Sukuna->ele[k] == update_Node)
			{
				Sukuna->ele[k]->used = order_used++;
				Sukuna->updateHeap(k);
				break;
			}
	}
}
void HAND()
{
	if (curr)
		curr->inOrder();
}
void LIMITLESS(const int &num)
{
	if (num < 0 || num >= MAXSIZE || Gojo[num]->root == nullptr)
		return;
	Gojo[num]->inOrder();
}
void CLEAVE(int num)
{
	Sukuna->preOrder(num);
}

void simulate(string filename)
{
	ifstream ss(filename);
	string str, maxsize, name, num;
	while (ss >> str)
	{
		if (str == "MAXSIZE")
		{
			ss >> maxsize;
			MAXSIZE = stoi(maxsize);
			Gojo.resize(MAXSIZE);
			order_Gojo.resize(MAXSIZE);
			for (int i = 0; i < MAXSIZE; i++)
				Gojo[i] = new BST();
			Sukuna = new Heap();
		}
		else if (str == "LAPSE")
		{
			ss >> name;
			LAPSE(name);
		}
		else if (str == "KOKUSEN")
		{
			KOKUSEN();
		}
		else if (str == "KEITEIKEN")
		{
			ss >> num;
			KEITEIKEN(stoi(num));
		}
		else if (str == "HAND")
		{
			HAND();
		}
		else if (str == "LIMITLESS")
		{
			ss >> num;
			LIMITLESS(stoi(num) - 1);
		}
		else if (str == "CLEAVE")
		{
			ss >> num;
			CLEAVE(stoi(num));
		}
	}
	for (int i = 0; i < MAXSIZE; i++)
		delete Gojo[i];
	delete Sukuna;
	Gojo.clear();
	Gojo.resize(0);
	Gojo.shrink_to_fit();
	order_Gojo.clear();
	order_Gojo.resize(0);
	order_Gojo.shrink_to_fit();
	if (curr)
	{
		curr->clear();
		delete curr;
		curr = nullptr;
	}
	return;
}
