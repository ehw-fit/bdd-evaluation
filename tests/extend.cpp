#include <vector>
#include <bitset>

#include "../arithmetic.hpp"

#include <gtest/gtest.h>
#include <bdd.h>

#define INIT_SIZE 8
#define NEW_SIZE (sizeof(int16_t) * 8)

using namespace std;

TEST(Extension, Textend)
{
	bdd_init(100, 100);
	bdd_setvarnum(4);

	for (int val = 0; val < (1 << INIT_SIZE); val++)
	{
		vector<bdd> x;
		x = int2bdd(val, INIT_SIZE);
		auto v_int = bdd2int(x);
		bitset<INIT_SIZE> v(v_int);
		//cout << "init: " << v << endl;

		// ZERO - EXTEND
		auto r_int = bdd2int(zero_extend(x, NEW_SIZE));
		bitset<NEW_SIZE> r(r_int);
		//cout << "extended: " << r << endl;
		EXPECT_EQ(val, r_int);

		// SIGN - EXTEND
		auto sr_int = bdd2int(sign_extend(x, NEW_SIZE));
		bitset<NEW_SIZE> sr(sr_int);
		//cout << "sign extended: " << sr << " .. " << (int)((int8_t)(val)) << " = " <<  (int16_t)(sr_int) << endl;
		EXPECT_EQ((int16_t)((int8_t)(val)), (int16_t)(sr_int));
	}

	bdd_done();
}

/* TEST FULL ADDER */
TEST(Extension, Tfadd)
{
	bdd_init(100, 100);
	bdd_setvarnum(4);

	for (int a = 0; a <= 1; a++)
		for (int b = 0; b <= 1; b++)
			for (int c = 0; c <= 1; c++)
			{
				bdd d, carry;
				std::tie(d, carry) = full_adder(bool2bdd(a), bool2bdd(b), bool2bdd(c));
				//cout << "D, C = " << d << ", " << carry << " .. " << (a + b + c) << endl;
				EXPECT_TRUE((carry == bddtrue) == (bool)((a + b + c) & 2));
				EXPECT_TRUE((d == bddtrue) == (bool)((a + b + c) & 1));
			}
	bdd_done();
}

TEST(Extension, Tsub)
{

	bdd_init(100, 100);
	bdd_setvarnum(4);

	int8_t val_a = 0;
	int8_t val_b = 0;

	for (int i = 0; i < 256; i++)
	{
		val_a++;
		val_b = 0;
		for (int j = 0; j < 256; j++)
		{
			val_b++;

			//if(val_a != 0 || val_b != 64) continue;
			vector<bdd> a;
			a = int2bdd(val_a, INIT_SIZE);
			auto a_int = bdd2int(a);
			bitset<INIT_SIZE> va(a_int);
			//cout << "init a: " << va << endl;

			vector<bdd> b;
			b = int2bdd(val_b, INIT_SIZE);
			auto b_int = bdd2int(b);
			bitset<INIT_SIZE> vb(b_int);
			//cout << "init b: " << vb << endl;

			// Znamenkove rozsireni
			{
				vector<bdd> res = bdd_subtract(a, b, true, true);
				bdd2int(res);
				auto res_int = bdd2int(sign_extend(res, sizeof(int) * 8));
				bitset<sizeof(int) * 8> vres(res_int);
				// cout << "a - b: " << ((int)(val_a)) << "-" << ((int)(val_b) ) << "(" << ((int)(val_a)) - ((int)(val_b) ) << ") = " << (int)(vres.to_ulong()) << endl; // << " .. " << (int)((int8_t)(val)) << " = " <<  (int16_t)(sr_int) << endl;
				EXPECT_EQ((int)(val_a) - ((int)(val_b)), (int)(vres.to_ulong()));
			}
			// Bez znamenkoveho rozsireni
			{
				vector<bdd> res = bdd_subtract(a, b, false);
				//bdd2int(res);
				//cout << "AAA";
				auto res_int = bdd2int(sign_extend(res, sizeof(int) * 8));
				bitset<sizeof(int) * 8> vres(res_int);

				//cout << "a - b: " << ((int)(val_a)) << "-" << ((int)(val_b) ) << "(" << (int16_t)((int8_t)(val_a - val_b)) << ") = " << (int)(res_int) << endl; // << " .. " << (int)((int8_t)(val)) << " = " <<  (int16_t)(sr_int) << endl;
				EXPECT_EQ((int8_t)(val_a - val_b), (long int)(res_int));
			}
		}
	}

	bdd_done();
}

TEST(Extension, Tsubmem)
{

	bdd_init(100, 100);
	bdd_setvarnum(4);

	int8_t val_a = 0;
	int8_t val_b = 0;

	vector<bdd> res_ex(INIT_SIZE + 1);
	vector<bdd> res(INIT_SIZE);
	for (int i = 0; i < 256; i++)
	{
		val_a++;
		val_b = 0;
		for (int j = 0; j < 256; j++)
		{
			val_b++;

			//if(val_a != 0 || val_b != 64) continue;
			vector<bdd> a;
			a = int2bdd(val_a, INIT_SIZE);
			auto a_int = bdd2int(a);
			bitset<INIT_SIZE> va(a_int);
			//cout << "init a: " << va << endl;

			vector<bdd> b;
			b = int2bdd(val_b, INIT_SIZE);
			auto b_int = bdd2int(b);
			bitset<INIT_SIZE> vb(b_int);
			//cout << "init b: " << vb << endl;

			// Znamenkove rozsireni
			{
				bdd_subtract(res_ex, a, b, true, true);
				bdd2int(res_ex);
				auto res_int = bdd2int(sign_extend(res_ex, sizeof(int) * 8));
				bitset<sizeof(int) * 8> vres(res_int);
				//cout << "a - b: " << ((int)(val_a)) << "-" << ((int)(val_b) ) << "(" << ((int)(val_a)) - ((int)(val_b) ) << ") = " << (int)(vres.to_ulong()) << endl; // << " .. " << (int)((int8_t)(val)) << " = " <<  (int16_t)(sr_int) << endl;
				EXPECT_EQ((int)(val_a) - ((int)(val_b)), (int)(vres.to_ulong()));
			}
			// Bez znamenkoveho rozsireni
			{
				bdd_subtract(res, a, b, false);
				//bdd2int(res);
				//cout << "AAA";
				auto res_int = bdd2int(sign_extend(res, sizeof(int) * 8));
				bitset<sizeof(int) * 8> vres(res_int);

				//cout << "a - b: " << ((int)(val_a)) << "-" << ((int)(val_b) ) << "(" << (int16_t)((int8_t)(val_a - val_b)) << ") = " << (int)(res_int) << endl; // << " .. " << (int)((int8_t)(val)) << " = " <<  (int16_t)(sr_int) << endl;
				EXPECT_EQ((int8_t)(val_a - val_b), (long int)(res_int));
			}
		}
	}

	bdd_done();
}

TEST(Extension, Tadd)
{

	bdd_init(100, 100);
	bdd_setvarnum(4);

	int8_t val_a = 0;
	int8_t val_b = 0;

	for (int i = 0; i < 256; i++)
	{
		val_a++;
		val_b = 0;
		for (int j = 0; j < 256; j++)
		{
			val_b++;

			//if(val_a != 0 || val_b != 64) continue;
			vector<bdd> a;
			a = int2bdd(val_a, INIT_SIZE);
			auto a_int = bdd2int(a);
			bitset<INIT_SIZE> va(a_int);
			//cout << "init a: " << va << endl;

			vector<bdd> b;
			b = int2bdd(val_b, INIT_SIZE);
			auto b_int = bdd2int(b);
			bitset<INIT_SIZE> vb(b_int);
			//cout << "init b: " << vb << endl;

			// Znamenkove rozsireni
			{
				vector<bdd> res = bdd_add(a, b, true, true);
				bdd2int(res);
				auto res_int = bdd2int(sign_extend(res, sizeof(int) * 8));
				bitset<sizeof(int) * 8> vres(res_int);
				//				cout << "a + b: " << ((int)(val_a)) << "+" << ((int)(val_b) ) << "(" << ((int)(val_a)) + ((int)(val_b) ) << ") = " << (int)(vres.to_ulong()) << endl; // << " .. " << (int)((int8_t)(val)) << " = " <<  (int16_t)(sr_int) << endl;
				EXPECT_EQ((int)(val_a) + ((int)(val_b)), (int)(res_int));
			}
			// Bez znamenkoveho rozsireni
			{
				vector<bdd> res = bdd_add(a, b, false);
				//bdd2int(res);
				//cout << "AAA";
				auto res_int = bdd2int(sign_extend(res, sizeof(int) * 8));
				bitset<sizeof(int) * 8> vres(res_int);

				//cout << "a - b: " << ((int)(val_a)) << "-" << ((int)(val_b) ) << "(" << (int16_t)((int8_t)(val_a - val_b)) << ") = " << (int)(res_int) << endl; // << " .. " << (int)((int8_t)(val)) << " = " <<  (int16_t)(sr_int) << endl;
				EXPECT_EQ((int8_t)(val_a + val_b), (long int)(res_int));
			}
		}
	}
	bdd_done();
}

TEST(Extension, Taddmem)
{

	bdd_init(100, 100);
	bdd_setvarnum(4);

	int8_t val_a = 0;
	int8_t val_b = 0;

	vector<bdd> res_ex(INIT_SIZE + 1);
	vector<bdd> res(INIT_SIZE);
	for (int i = 0; i < 256; i++)
	{
		val_a++;
		val_b = 0;
		for (int j = 0; j < 256; j++)
		{
			val_b++;

			//if(val_a != 0 || val_b != 64) continue;
			vector<bdd> a;
			a = int2bdd(val_a, INIT_SIZE);
			auto a_int = bdd2int(a);
			bitset<INIT_SIZE> va(a_int);
			//cout << "init a: " << va << endl;

			vector<bdd> b;
			b = int2bdd(val_b, INIT_SIZE);
			auto b_int = bdd2int(b);
			bitset<INIT_SIZE> vb(b_int);
			//cout << "init b: " << vb << endl;

			// Znamenkove rozsireni
			{
				bdd_add(res_ex, a, b, true, true);
				bdd2int(res_ex);
				auto res_int = bdd2int(sign_extend(res_ex, sizeof(int) * 8));
				bitset<sizeof(int) * 8> vres(res_int);
				//cout << "a - b: " << ((int)(val_a)) << "-" << ((int)(val_b) ) << "(" << ((int)(val_a)) - ((int)(val_b) ) << ") = " << (int)(vres.to_ulong()) << endl; // << " .. " << (int)((int8_t)(val)) << " = " <<  (int16_t)(sr_int) << endl;
				EXPECT_EQ((int)(val_a) + ((int)(val_b)), (int)(res_int));
			}
			// Bez znamenkoveho rozsireni
			{

				bdd_add(res, a, b, false);
				//bdd2int(res);
				//cout << "AAA";
				auto res_int = bdd2int(sign_extend(res, sizeof(int) * 8));
				bitset<sizeof(int) * 8> vres(res_int);

				//cout << "a - b: " << ((int)(val_a)) << "-" << ((int)(val_b) ) << "(" << (int16_t)((int8_t)(val_a - val_b)) << ") = " << (int)(res_int) << endl; // << " .. " << (int)((int8_t)(val)) << " = " <<  (int16_t)(sr_int) << endl;
				EXPECT_EQ((int8_t)(val_a + val_b), (long int)(res_int));
			}
		}
	}
	bdd_done();
}

TEST(Extension, Tabs)
{

	bdd_init(100, 100);
	bdd_setvarnum(4);

	int8_t val_a = 0;
	int8_t val_b = 0;

	for (int i = 0; i < 256; i++)
	{
		val_a++;

		if (val_a == -128)
			continue; // toto cislo nejde

		vector<bdd> a;
		a = int2bdd(val_a, INIT_SIZE);
		auto a_int = bdd2int(a);
		bitset<INIT_SIZE> va(a_int);
		//cout << "init a: " << va << endl;
		vector<bdd> res = bdd_abs(a);

		//bdd2int(res);
		//cout << "AAA";
		auto res_int = bdd2int(sign_extend(res, sizeof(int) * 8));
		bitset<sizeof(int) * 8> vres(res_int);

		//cout << "a - b: " << ((int)(val_a)) << "-" << ((int)(val_b) ) << "(" << (int16_t)((int8_t)(val_a - val_b)) << ") = " << (int)(res_int) << endl; // << " .. " << (int)((int8_t)(val)) << " = " <<  (int16_t)(sr_int) << endl;
		EXPECT_EQ(abs(val_a), (int)(res_int));
	}
	bdd_done();
}

TEST(Extension, Tabsmem)
{

	bdd_init(100, 100);
	bdd_setvarnum(4);

	int8_t val_a = 0;
	int8_t val_b = 0;

	vector<bdd> res(INIT_SIZE);

	for (int i = 0; i < 256; i++)
	{
		val_a++;

		if (val_a == -128)
			continue; // toto cislo nejde

		vector<bdd> a;
		a = int2bdd(val_a, INIT_SIZE);
		auto a_int = bdd2int(a);
		bitset<INIT_SIZE> va(a_int);
		//cout << "init a: " << va << endl;
		bdd_abs(res, a);

		//bdd2int(res);
		//cout << "AAA";
		auto res_int = bdd2int(sign_extend(res, sizeof(int) * 8));
		bitset<sizeof(int) * 8> vres(res_int);

		//cout << "a - b: " << ((int)(val_a)) << "-" << ((int)(val_b) ) << "(" << (int16_t)((int8_t)(val_a - val_b)) << ") = " << (int)(res_int) << endl; // << " .. " << (int)((int8_t)(val)) << " = " <<  (int16_t)(sr_int) << endl;
		EXPECT_EQ(abs(val_a), (int)(res_int));
	}
	bdd_done();
}
