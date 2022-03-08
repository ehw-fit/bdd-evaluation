#include <vector>
#include <bitset>

#include "../arithmetic.hpp"

#include <gtest/gtest.h>


using namespace std;

TEST(WC, ShortTest)
{
	bdd_init(100, 100);
	bdd_setvarnum(4);
	std::vector<bdd> x;
	x.push_back(bdd_ithvar(3));
	x.push_back(bdd_ithvar(2));
	x.push_back(bddtrue);
	x.push_back(!bdd_ithvar(0));
	x.push_back(bddfalse);
	x.push_back(bdd_ithvar(0));
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	
	unsigned long value = worst_case(x);
	EXPECT_EQ(value, 0x27);
	
	//bitset<8>  r(value);
	//cout << "Max value: " << r << endl;
		
	bdd_done();
}

TEST(WC, Vars)
{
	bdd_init(100, 100);
	bdd_setvarnum(4);
	vector<bdd> x;
	x.push_back(bdd_ithvar(3));
	x.push_back(bdd_ithvar(2));
	x.push_back(bddtrue);
	x.push_back(!bdd_ithvar(1));
	x.push_back(bddfalse);
	x.push_back(bdd_ithvar(0));
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	
	unsigned long value = worst_case(x);
	EXPECT_EQ(value, 0x2f);
	
	bdd_done();
}


TEST(WC, Test65bits)
{
	bdd_init(100, 100);
	bdd_setvarnum(4);
	vector<bdd> x;

	// 0 - 31
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);

	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);

	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);

	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);

	// 32 - 63
	
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);

	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);

	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);

	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddtrue); // 63

	x.push_back(bddtrue); // 64
	
	

	auto value = worst_case(x);
	bitset<65>  r((uint64_t)value);
	cout << "Max value: " << r << endl;
	cout << " = " << value << endl;
	cout << " = " << value / (1UL << 62) << endl;
	EXPECT_EQ(value / (1UL << 62), 6);
	
	//bitset<8>  r(value);
	//cout << "Max value: " << r << endl;
		
	bdd_done();
}


TEST(WC, Test32bits)
{
	bdd_init(100, 100);
	bdd_setvarnum(4);
	vector<bdd> x;

	// 0 - 31
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);

	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);

	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);

	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddfalse);
	x.push_back(bddtrue);

	

	auto value = worst_case(x);
	bitset<32>  r((uint64_t)value);
	cout << "Max value: " << r << endl;
	cout << " = " << value << endl;
	cout << " ... " <<(double)(1 << 31) << endl;
	EXPECT_EQ(value, -(double)(1 << 31) );
	
	//bitset<8>  r(value);
	//cout << "Max value: " << r << endl;
		
	bdd_done();
}


TEST(WC, Twc02)
{
	bdd_init(100, 100);
	bdd_setvarnum(4);
	vector<bdd> x;

	// 0 - 31
	x.push_back(bddfalse);
	x.push_back(bddtrue);

	

	auto value = worst_case(x);
	bitset<3>  r((uint64_t)value);
	cout << "Max value: " << r << endl;
	cout << " = " << value << endl; 
	EXPECT_EQ(value, 2 );
	
	//bitset<8>  r(value);
	//cout << "Max value: " << r << endl;
		
	bdd_done();
}