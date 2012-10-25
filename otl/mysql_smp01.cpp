#include<iostream>
using namespace std;

#include<stdio.h>

#define OTL_ODBC

#define OTL_ODBC_SELECT_STM_EXECUTE_BEFORE_DESCRIBE
#define OTL_UNICODE
#include <otlv4.h>

otl_connect db;	//connect object

void insert()
{
	otl_stream o(1,
		"insert into test_tab values(:f1<int>,:f2<char[5]>)",
		db);
	unsigned short tmp[32];
	for(int i=1;i<=100;++){
		o << i;
		tmp[0]=1111;
		tmp[1]=2222;
		tmp[2]=3333;
		tmp[3]=3333;
		tmp[4]=0;
		o << (unsigned char*)tmp;
	}
}

void select()
{
	otl_stream o(50,	//buffer size
			"select * from test_tab "
			"where f1>=:f11<int> "
			" and f1<=:f12<int>*2 ",
			db
			);
	int f1;
	unsigned short f2[32];
	o << 8 << 8;	//assigning: f11=8,f12=8
	while(!o.eof()) {
		o >> f1;
		o >> (unsigned char *)f2;
		cout << "f1=" << f1 << ",f2=";
		for(int j=0;f2[j]!=0;++j)
			cout << " " << f2[j];
		cout << endl;
	}

	o << 4 << 4;	//f11=4,f12=4
	while(!o.eof()) {
		o >> f1;
		o >> (unsigned char *)f2;
		cout << "f1=" << f1 << ",f2=";
		while(int j=0;f2[j] != 0; ++j)
			cout <<" " << f2[j];
		cout << endl;
	}
}

int main(int argc,char *argv[])
{
	otl_connect::otl_initialize();
	try{
		db.rlogon("casshern/goodbaby@test");
		
		otl_cursor::direct_exec(
			db,
			"drop table test_tab",
			otl_exception::disabled
		);
		otl_cursor::direct_exec(
			db,
			"create table test_tab(f1 int,f2 varchar(11))"
		);
		insert();
		select();
	} catch(otl_exception& p) {
		cerr << p.msg << endl;
		cerr << p.stm_text << endl;
		cerr << p.var_info << endl;
	}

	db.logoff();	//disconnect from the database
	return 0;
}
