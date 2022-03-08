
/* Test worst-case chyby pri aritmetickem rozhodovani */
#include "../arithmetic.hpp"
#include <vector>
#include <bdd.h>
#include <gtest/gtest.h>

using namespace std;


#define ADD_SIZE 8

TEST(Adder, TadderWC)
{
	bdd_init(10000, 10000);
	bdd_setvarnum(2*ADD_SIZE);
	
	vector<bdd> a(ADD_SIZE);
	vector<bdd> b(ADD_SIZE);
	
	for(int i = 0; i < ADD_SIZE; i++)
	{
		a[i] = bdd_ithvar(i);
		b[i] = bdd_ithvar(i + ADD_SIZE);
	}
	
	auto add1 = bdd_add(a, b);
	auto add2 = bdd_add(a, b);
	add2[2] = bddfalse;
	add2[1] = bddfalse;
	add2[0] = bddtrue;
	
	auto diff = bdd_subtract(add1, add2, false);
	
	auto absdiff = bdd_abs(diff);
	
	auto wc = worst_case(absdiff);
	cout << "Worst case: " << wc << endl;
	EXPECT_EQ(wc ,  6);
	bdd_done();
}


TEST(Adder, TadderAVG)
{
	bdd_init(10000, 10000);
	bdd_setvarnum(2*ADD_SIZE);
	
	vector<bdd> a(ADD_SIZE);
	vector<bdd> b(ADD_SIZE);
	
	for(int i = 0; i < ADD_SIZE; i++)
	{
		a[i] = bdd_ithvar(i);
		b[i] = bdd_ithvar(i + ADD_SIZE);
	}
	
	auto add1 = bdd_add(a, b);
	auto add2 = bdd_add(a, b);
	
/*	
	for(int i = 0; i < ADD_SIZE; i++)
	{
		char name[] = "adder_X.dot";
		name[6] = i + '0';
		bdd_fnprintdot(name, add1[i]);
	}
*/
	
	add2[2] = bddfalse;
	add2[1] = bddfalse;
	add2[0] = bddfalse;
	
	auto diff = bdd_subtract(add1, add2, false);
	auto absdiff = bdd_abs(diff);
	
	double val = 0;
	for(int i = 0; i < ADD_SIZE; i++)
	{
		auto v = bdd_satcount(absdiff[i]); 
		cout << v << endl;
		val += v * (1 << i);
		//a[i] = bdd_ithvar(i);
		//b[i] = bdd_ithvar(i + ADD_SIZE);
	}
	val /= (1 << (2*ADD_SIZE));
	cout << "AVG: " << val  << endl;
	EXPECT_FLOAT_EQ(val, 3.5); //, 0.01); 
	
	//auto wc = worst_case(absdiff);
	//cout << "Worst case: " << wc << endl;
	//EXPECT_EQ(wc ,  6);
	bdd_done();
}
	

TEST(Adder, Tavg) {
	bdd_init(10000, 10000);
	bdd_setvarnum(4);
	
	vector<bdd> a(4);
    a[0] = bddfalse;
    a[1] = bddtrue;
    a[2] = bddfalse;
//    a[3] = bddfalse;
    a[3] = bdd_ithvar(0);

	auto avg_abs = average_abs(a);
	auto avg = average(bdd_abs(a));
    cout << "Average " << avg_abs << " .. " << avg << endl;
    bdd_done();
}
