#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <sstream>

class Item 
{
	private:
		size_t _key;
		int _price;
		std::string _name;
	public:
		Item(size_t k=0, int p=0, const std::string& n="") :
			_key(k), _price(p), _name(n) {}
		
		inline size_t key() const { return _key; }
		inline int price() const { return _price; }
		inline const std::string& name() const { return _name; }

		std::string toString() const {
			std::ostringstream oss;
			oss << "#" << _key << " " <<_price << " " << _name;
			return oss.str();
		}
};

#endif // ITEM_H
