#if 0
TEST(Tmult)
{
	
	bdd_init(100, 100);
	bdd_setvarnum(4);
	

	int16_t val_a = 0;
	int16_t val_b = 0;
	
	for(int i = 0; i < 256; i++)
	{
		val_a ++ ;
		val_b = 0;
		for(int j = 0; j < 256; j++)
		{
			//val_b ++;
			val_a = i;
			val_b = j;
			
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
			
			
			
			
			{
				vector<bdd> res = bdd_mult(a, b);
				//bdd2int(res);			
				auto res_int = bdd2int(zero_extend(res, sizeof(int) * 8));
				bitset<sizeof(int) * 8> vres(res_int);
				//cout << "a * b: " << ((int)(val_a)) << "*" << ((int)(val_b) ) << "(" << ((int)(val_a)) * ((int)(val_b) ) << ") = " << (int)(vres.to_ulong()) << endl; // << " .. " << (int)((int8_t)(val)) << " = " <<  (int16_t)(sr_int) << endl;
				CHECK(((int)(val_a) * ((int)(val_b) )) == ((int)(vres.to_ulong())));
			}
			

		}	
	}	
	
	
	bdd_done();
}
#endif
