#include <iostream>
#include <stdio.h>
#include <sqlite3.h>
#include <string>
#include <vector>
#include "networknode.h"
#include "node.h"

using namespace std;

static int callback(void* data, int argc, char** argv, char** azColName)
{
	cout << (const char*)data << endl;
	for (int i = 0; i < argc; i++)
	{
		cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << endl;
	}
	cout << endl;
	return 0;
}

void sqlcheck(int rc, char* zErrMsg)
{
	if (rc != SQLITE_OK)
	{
		cout << "SQL Error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);
	}
}

int main()
{
	sqlite3* db;
	int rc = sqlite3_open("demo/demo1.db", &db);
	sqlite3_stmt* stmt;
	const char* unused;
	rc = sqlite3_prepare_v2(db, "select ID from Socket", 100, &stmt, &unused);
	int i;
	while((i = sqlite3_step(stmt)) != SQLITE_DONE)
	{
		if (i == SQLITE_ROW)
		{
			cout << sqlite3_column_int64(stmt, 0);
		}
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);

	//sqlite3* db;
	char* zErrMsg = 0;

	string dbloc = ":memory:";
	cout << "Database: ";
	cin >> dbloc;

	/*rc = sqlite3_open(dbloc.c_str(), &db);

	if(rc)
	{
		cout << rc << " | " << sqlite3_errmsg(db) << endl;
	}

	rc = sqlite3_exec(db, 
		"insert into NodeTree values (0); " \
		"insert into NodeTree values (1); ",
		callback, (void*)"insert", &zErrMsg);
	
	rc = sqlite3_exec(db,
		"select * from NodeTree; ", 
		callback, (void*)"select", &zErrMsg);

	sqlcheck(rc, zErrMsg);*/
	/*int(*f2)(int) = [](int x) -> int {return x;};
	Property prop = Property();
	prop.set(f2);
	cout << prop.get<int(*)(int)>()(10);*/
	
	//typedef Network::Neuron Neuron;
	//typedef Network::Synapse Synapse;
	
	cout << "1\n";

	SQL* sql = new SQL(dbloc.c_str());
	
	cout << "2\n";

	//cout << 1 << endl;
	//vector<Node*> node;
	//node.push_back(new Node(0, 0));
	//node.push_back(new Node(0, 1));	
	//node.push_back(new Node(2, 2));
	NodeTree nt = NodeTree(0, sql);
	
	cout << "3\n";

	nt.init<Neuron, Synapse>(sql);
	//node[0]->init<Neuron, Synapse>(sql);
	//node[1]->init<Neuron, Synapse>(sql);
	//node[2]->init<Neuron, Synapse>(sql);
	
	cout << "4\n";

	//SimpComp sc = SimpComp(3, 0);
	//sc.init<Neuron, Synapse>(sql);
	
	cout << "5\n";

	//cout << "simplicial complex " << sc.node[2]->output->data.getr<Synapse>().get() << endl;
	
	cout << "6\n";
	
	//cout << 3 << endl;
	//node[0]->output->data.getr<Synapse>().set(1.0);
	//node[1]->output->data.getr<Synapse>().set(2.0);

	//sc.node[0]->output->data.getr<Synapse>().set(1.0);
	//sc.node[1]->output->data.getr<Synapse>().set(2.0);
	
	//cout << 4 << endl;
	//node[2]->calc();
	//node[2]->data.getr<Neuron>().calc();
	
	//sc.node[2]->data.getr<Neuron>().calc();

	//cout << 5 << endl;
	//cout << "hello node " << node[2]->output->data.getr<Synapse>().get() << endl;
	//cout << "simplicial " << sc.node[2]->output->data.getr<Synapse>().get() << endl;
	//cout << "node " << nt.node[2]->output->data.getr<Synapse>().get() << endl;
	int junk;
	cin >> junk;
	return 0;
}