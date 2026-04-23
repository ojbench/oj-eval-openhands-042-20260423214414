#ifndef SPEEDCIRCULARLIST_H
#define SPEEDCIRCULARLIST_H
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

constexpr int s_prime = 31;

inline int log2(int x) {
	int ans = 0;
	while (x != 1) {
		x /= 2;
		++ans;
	}
	return ans;
}

template<typename T, int b_prime>
class SpeedCircularLinkedList {
public:
    struct Node {
    	std::map<std::string, T> kv_map;
        int bound = 0;
        Node* next = nullptr;
        Node** fast_search_list = nullptr;
		
    	Node(int bound, int fast_search_list_size) {
    		this->bound = bound;
    		if (fast_search_list_size > 0) {
    			fast_search_list = new Node*[fast_search_list_size];
    			for (int i = 0; i < fast_search_list_size; i++) {
    				fast_search_list[i] = nullptr;
    			}
    		}
    	}

    	~Node() {
    		if (fast_search_list != nullptr) {
    			delete[] fast_search_list;
    		}
    	}
    };

private:
	Node* head = nullptr;
	int fast_search_list_size = 0;
	int list_size = 0;

	static int GetHashCode(std::string str) {
		long long ans = 0;
		for (auto& ch : str) {
			ans = (ans * s_prime + ch) % b_prime;
		}
		return static_cast<int>((ans + b_prime) % b_prime);
	}

	void BuildFastSearchList() {
		if (list_size == 0 || fast_search_list_size == 0) {
			return;
		}
		
		Node* cur = head;
		for (int i = 0; i < list_size; i++) {
			Node* temp = cur;
			for (int k = 0; k < fast_search_list_size; k++) {
				int steps = 1 << k;
				for (int j = 0; j < steps; j++) {
					temp = temp->next;
				}
				cur->fast_search_list[k] = temp;
			}
			cur = cur->next;
		}
	}

public:

    explicit SpeedCircularLinkedList(std::vector<int> node_bounds) {
    	list_size = node_bounds.size();
    	if (list_size == 0) {
    		return;
    	}
    	
    	fast_search_list_size = log2(list_size);
    	
    	head = new Node(node_bounds[0], fast_search_list_size);
    	Node* prev = head;
    	
    	for (int i = 1; i < list_size; i++) {
    		Node* new_node = new Node(node_bounds[i], fast_search_list_size);
    		prev->next = new_node;
    		prev = new_node;
    	}
    	
    	prev->next = head;
    	
    	BuildFastSearchList();
    }

    ~SpeedCircularLinkedList() {
    	if (head == nullptr) {
    		return;
    	}
    	
    	Node* cur = head;
    	for (int i = 0; i < list_size; i++) {
    		Node* next_node = cur->next;
    		delete cur;
    		cur = next_node;
    	}
    }

    void put(std::string str, T value) {
	    int code = GetHashCode(str);
	    
	    Node* cur = head;
	    
	    while (code > cur->bound) {
	    	if (fast_search_list_size > 0) {
	    		bool jumped = false;
	    		for (int k = fast_search_list_size - 1; k >= 0; k--) {
	    			Node* next = cur->fast_search_list[k];
	    			if (next->bound > cur->bound && next->bound < code) {
	    				cur = next;
	    				jumped = true;
	    				break;
	    			}
	    		}
	    		if (!jumped) {
	    			cur = cur->next;
	    		}
	    	} else {
	    		cur = cur->next;
	    	}
	    }
	    
	    cur->kv_map[str] = value;
    }

	T get(std::string str) {
    	int code = GetHashCode(str);
    	
    	Node* cur = head;
    	
    	while (code > cur->bound) {
    		if (fast_search_list_size > 0) {
    			bool jumped = false;
    			for (int k = fast_search_list_size - 1; k >= 0; k--) {
    				Node* next = cur->fast_search_list[k];
    				if (next->bound > cur->bound && next->bound < code) {
    					cur = next;
    					jumped = true;
    					break;
    				}
    			}
    			if (!jumped) {
    				cur = cur->next;
    			}
    		} else {
    			cur = cur->next;
    		}
    	}
    	
    	auto it = cur->kv_map.find(str);
    	if (it != cur->kv_map.end()) {
    		return it->second;
    	}
    	return T();
    }

    void print() {
    	if (head == nullptr) {
    		return;
    	}
    	
    	Node* cur = head;
    	for (int i = 0; i < list_size; i++) {
    		std::cout << "[Node] Bound = " << cur->bound << ", kv_map_size = " << cur->kv_map.size() << '\n';
    		cur = cur->next;
    	}
    }

    int size() const {
    	return list_size;
    }
};
#endif //SPEEDCIRCULARLIST_H
