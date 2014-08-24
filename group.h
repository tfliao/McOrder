#ifndef GROUP_H
#define GROUP_H

#include <string>
#include <sstream>
#include <unordered_set>

class Group
{
	private:
		int _key;
		std::string _name;
		std::unordered_set<size_t> _items;
		int _lower, _upper ;
		int _order;
		
		static int next_id ;
		
	public:
		Group(int k=0, const std::string& n="") :
			_key(k), _name(n), _lower(0), _upper(9999), _order(0) { if (k >= next_id) next_id = k+1; }
		
		inline int key() const { return _key; }
		inline const std::string& name() const { return _name; }
		inline std::pair<int, int> bounds() const { return std::make_pair(_lower, _upper); }
		
		inline void setName(const std::string& n) { _name = n; }
		inline void setBounds(int lower, int upper) { _lower=lower, _upper=upper; } 
		
		inline bool hasItem(size_t item_key) const { return _items.count(item_key) > 0; }
		
		inline void addItem(size_t item_key) { _items.insert(item_key); }
		inline void delItem(size_t item_key) { _items.erase(item_key); }

		inline const std::unordered_set<size_t>& items() const { return _items; }
		
		inline void incOrder() { _order++; }
		inline void decOrder() { _order--; }

		inline bool preCheck() const { return _order <= _upper; }
		inline bool check() const { return _order <= _upper && _order >= _lower; }
		
		std::string toString() const {
			std::ostringstream oss;
			oss << "#" << _key ;
			oss << " " << _name << " " << _lower << " " << _upper << " ";
			for ( auto i : _items ) { oss << i << " "; }
			return oss.str();
		}
		
		std::string brief() const {
			std::ostringstream oss;
			if (_key > 0)
				oss << "#" << _key ;
			else
				oss << "[default" << -_key << "]" ;
			oss << " " << _name << " (" << _lower << ", " << _upper << ")";
			return oss.str();			
		}
		
		static int nextId() {
			return next_id++;
		}
		
};

#endif // GROUP_H
