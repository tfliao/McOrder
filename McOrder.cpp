#include <iostream>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <functional>
#include "item.h"
#include "group.h"
#include "config.h"
using namespace std;


unordered_map<size_t, Item> items;
vector<Group> groups;
vector<size_t> item_keys;

vector<size_t> result;
int result_cnt ;

int Group::next_id = 1;

void load_menu();
void load_group();

ostream *output_stream = NULL;

namespace group_operation
{
	void List();
	void Show();
	void New();
	void Delete();
	void Save();
	void Load();
	void Adjust();
	
	void Main();
}

bool preCheckLimit(Item& item)
{
	return all_of(groups.begin(), groups.end(), [&](const Group& g) { return g.preCheck(); } );
}

bool validate()
{
	return all_of(groups.begin(), groups.end(), [&](const Group& g) { return g.check(); } );
}

void dfs(int x, int left)
{
	if (left <= config::limit) {
		if (!validate()) return ;
		result_cnt ++;
		for (auto r : result) {
			const Item& i = items[r];
			(*output_stream) << i.name() << "(" << i.price() << ") ";
		}
		(*output_stream) << endl;
		return ;
	}
	
	for (int i=x + (config::allow_repeat?0:1);i<item_keys.size();++i) {
		Item& item = items[item_keys[i]];

		if (item.price() > left) continue;
		
		for_each(groups.begin(), groups.end(), [&](Group& g) { if (g.hasItem(item.key())) g.incOrder(); } );
		
		if (preCheckLimit(item)) {
			result.push_back(item.key());
			dfs(i, left-item.price());
			result.pop_back();
		}
		for_each(groups.begin(), groups.end(), [&](Group& g) { if (g.hasItem(item.key())) g.decOrder(); } );
		
	}
}

void find_order()
{
	item_keys.clear();
	for (auto i : items) { item_keys.push_back(i.first); }
	result.clear();
	result_cnt = 0;
	if (!config::output_file.empty()) {
		output_stream = new ofstream(config::output_file.c_str());
	} else {
		output_stream = &cout;
	}
	
	dfs(0, 180);
	(*output_stream) << "Total " << result_cnt << " orders availiable." << endl;
	if (!config::output_file.empty()) {
		delete output_stream ;
	}
}


int main(int argc, char *argv[])
{
	ProgramOption po(argv[0], "Synology McDonalds menu order");
	config::initProgramOption(po);
	if (!po.parse(argc, argv)) {
		cerr << po.getError() << endl;
		cerr << po.usage();
		exit(0);
	}
	
	load_menu();
	load_group();
	using namespace config;
	if (mode == "Group" || mode == "G") {
		group_operation::Main();
	} else if (mode == "Calculation" || mode == "Calc" || mode == "C") {
		find_order();
	} else {
		cerr << "Unknown mode: " << mode << endl;
		cerr << po.usage();
		exit(0);
	}
	return 0;
}

void load_menu()
{
	ifstream fin(config::menu_file.c_str());
	string buf;
	int price;
	string name;
	int id;
	int group_id = -1;

	while(getline(fin, buf)) {
		if (buf.length()==0) continue;
		if (buf[0] == '[') {
			string str_group = buf.substr(1, buf.length()-2);
			groups.emplace_back(group_id--, str_group);
		} else if(buf[0] =='#') {
			istringstream(buf.substr(1)) >> id >> price >> name;
			items[id] = (Item(id, price,name));
			groups.back().addItem(id);
		}
	}
}

void load_group()
{
	ifstream fin(config::group_file.c_str());
	string buf;
	size_t key;
	string name;
	int lower, upper;
	size_t item;
	while(getline(fin, buf)) {
		if (buf.length()==0) continue;
		istringstream iss(buf.substr(1));
		iss >> key >> name >> lower >> upper ;
		if (key < 0) continue;
		groups.emplace_back(key, name);
		groups.back().setBounds(lower, upper);
		while (iss >> item) {
			groups.back().addItem(item);
		}
	}
}

namespace group_operation
{

void _show(const Group& g)
{
	cout << g.brief() << endl;
	for (auto i : g.items()) {
		cout << "\t" << items[i].toString() << endl;
	}
}

void List()
{
	for_each(groups.begin(), groups.end(), 
		[&](const Group& g) { cout << g.brief() << endl; }
	);
}


void Show(int id)
{
	for_each(groups.begin(), groups.end(), [&](const Group& g) { 
		if (id? g.key() == id: g.key() < 0) _show(g);
	});
}

void Save()
{
	ofstream fout(config::group_file.c_str());
	for_each(groups.begin(), groups.end(), 
		[&](const Group& g) {
			fout << g.toString() << endl;
		}
	);
	cerr << "Groups were saved to " << config::group_file << endl;
}

bool _adjust(Group& g)
{
	string buf, op;
	int lb = g.bounds().first, ub = g.bounds().second, id;
	string name = g.name();

	while ( getline(cin, buf) ) {
		if (buf.empty()) continue;
		istringstream iss(buf);
		iss >> op;
		if (op == "name") {
			iss >> name;
			g.setName(name);
		} else if (op == "lower_bound") {
			iss >> lb;
			g.setBounds(lb, ub);
		} else if (op == "upper_bound") {
			iss >> ub;
			g.setBounds(lb, ub);
		} else if (op == "add_items") {
			while (iss >> id) {
				g.addItem(id);
			}
		} else if (op == "del_items") {
			while (iss >> id) {
				g.delItem(id);
			}
		} else if (op == "submit") {
			_show(g);
			cerr << "Sure to submit? [Y/n]" ; getline(cin, buf);
			if (buf[0] == 'n') continue;
			return true;
		} else if (op == "discard") {
			return false;
		} else {
			cerr << "Accept operations:" << endl;
			cerr << "  name <name>" << endl;
			cerr << "  lower_bound <lower_bound>" << endl;
			cerr << "  upper_bound <upper_bound>" << endl;
			cerr << "  add_items <items> ..." << endl;
			cerr << "  del_items <items> ..." << endl;
			cerr << "  submit" << endl;
			cerr << "  discard" << endl;
		}
	}
	return false;
}

void New()
{
	int key = Group::nextId();
	char buf[16]; sprintf(buf, "%d", key);
	Group g(key, string("Group_") + buf);
	if (_adjust(g)) {
		groups.push_back(g);
		cerr << "create new group " << g.name() << endl;
	}
}

void Delete(int id)
{
	auto g = find_if(groups.begin(), groups.end(), [&](const Group& g) { return g.key() == id && id > 0; });
	if (g != groups.end()) {
		string name = g->name();
		groups.erase(g);
		cerr << "Group " << name << " was deleted." << endl;
	} else {
		cerr << "No such Group." << endl;
	}
}

void Adjust(int id)
{
	auto g = find_if(groups.begin(), groups.end(), [&](const Group& g) { return g.key() == id && id > 0; });

	if (g != groups.end()) {
		Group tmp = *g;
		if (_adjust(tmp)) {
			*g = tmp;
			cerr << "Group " << g->name() << "has adjusted" << endl;
		} 
	} else {
		cerr << "No such Group." << endl;
	}
}

void Main()
{
	string buf, op;
	int id;
	while (getline(cin, buf)) {
		if (buf.empty()) continue;
		istringstream iss(buf);
		iss >> op;
		if (op == "list") {
			List();
		} else if (op == "new") {
			New();
		} else if (op == "show") {
			iss >> id; Show(id);
		} else if (op == "adjust") {
			iss >> id; Adjust(id);
		} else if (op == "delete") {
			iss >> id; Delete(id);
		} else if (op == "save") {
			Save();
		} else if (op == "reload") {
			Load();
		} else if (op == "exit") {
			break;
		} else {
			cerr << "Accept operations:" << endl;
			cerr << "  list" << endl;
			cerr << "  new" << endl;
			cerr << "  show <id>" << endl;
			cerr << "  adjust <id> ..." << endl;
			cerr << "  delete <id> ..." << endl;
			cerr << "  save" << endl;
			cerr << "  reload" << endl;
			cerr << "  exit" << endl;
		}
	}
}
void Load()
{
	groups.clear();
	load_group();
}

}

