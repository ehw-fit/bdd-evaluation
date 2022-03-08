#include "arithmetic.hpp"

#include <ctime>

static inline double cpuTime(void) {
    return (double)clock() / CLOCKS_PER_SEC; 
	
}

static double startTime;
void bdd_set_start() {
	startTime = cpuTime();
}

static double timeLimit = -1;
void bdd_set_limit(double limit) {
	timeLimit = limit;
}


/******************************************************************************
 * Types                                                                      *
 ******************************************************************************/

/******************************************************************************
 * Private functions                                                          *
 ******************************************************************************/

/* move? */
bdd ite( const bdd& cond, const bdd& _then, const bdd& _else )
{
  return ( !cond & _else ) | ( cond & _then );
}

std::tuple<bdd, bdd> full_adder( const bdd& x, const bdd& y, const bdd& cin )
{
  const auto sum  = x ^ y ^ cin;
  const auto cout = ( x & y ) | ( x & cin ) | ( y & cin );

  return std::make_tuple( sum, cout );
}

/******************************************************************************
 * Public functions                                                           *
 ******************************************************************************/

std::vector<bdd> bdd_subtract( const std::vector<bdd>& minuend, const std::vector<bdd>& subtrahend, bool extend, bool sign)
{
  assert( minuend.size() == subtrahend.size() );
  assert( !minuend.empty() );

  std::vector<bdd> diff( minuend.size());
  bdd carry = bddtrue;

  for (auto i = 0ul; i < minuend.size(); i++ )
  {
    std::tie( diff[i], carry ) = full_adder( minuend[i], !subtrahend[i], carry );
  }
  if(extend)
  {
	int i = minuend.size() - 1;
	bdd val;
	if(sign)
		std::tie( val, carry ) = full_adder( minuend[i], !subtrahend[i], carry );
	else
		std::tie( val, carry ) = full_adder( bddfalse, !bddfalse, carry );
	
	diff.push_back(val);
	//diff.push_back(carry);
  }
	

  return diff;
}

void bdd_subtract(std::vector<bdd> & diff,  const std::vector<bdd>& minuend, const std::vector<bdd>& subtrahend, bool extend, bool sign )
{
  assert( minuend.size() == subtrahend.size() );
  assert( !minuend.empty() );
  assert( (!extend && minuend.size() == diff.size()) || (extend && (minuend.size() + 1) == diff.size()));

  bdd carry = bddtrue;

  for (auto i = 0ul; i < minuend.size(); i++ )
  {
    std::tie( diff[i], carry ) = full_adder( minuend[i], !subtrahend[i], carry );
  }
  if(extend)
  {
	int i = minuend.size() - 1;
	bdd val;
	
	if(sign)
		std::tie( diff[i + 1], carry ) = full_adder( minuend[i], !subtrahend[i], carry );
	else
		std::tie( diff[i + 1], carry ) = full_adder( bddfalse, !bddfalse, carry );
  }
}

void bdd_subtract_val(std::vector<bdd> & diff,  const std::vector<bdd>& minuend, const bdd & subtrahend, bool extend, bool sign )
{
  assert( !minuend.empty() );
  assert( (!extend && minuend.size() == diff.size()) || (extend && (minuend.size() + 1) == diff.size()));

  bdd carry = bddtrue;

  for (auto i = 0ul; i < minuend.size(); i++ )
  {
    std::tie( diff[i], carry ) = full_adder( minuend[i], !subtrahend, carry );
  }
  if(extend)
  {
	int i = minuend.size() - 1;
	bdd val;
	
	if(sign)
		std::tie( diff[i + 1], carry ) = full_adder( minuend[i], !subtrahend, carry );
	else
		std::tie( diff[i + 1], carry ) = full_adder( bddfalse, !bddfalse, carry );
  }
}

void bdd_add(std::vector<bdd> & sum, const std::vector<bdd>& a, const std::vector<bdd>& b, bool extend, bool sign )
{
	assert( a.size() == b.size() );
	assert( !a.empty() );
	assert( (!extend && a.size() == sum.size()) || (extend && (a.size() + 1) == sum.size()));
	
	//std::vector<bdd> sum(a.size());
	bdd carry = bddfalse;
	
	
	for (auto i = 0ul; i < a.size(); i++ )
	{
		std::tie( sum[i], carry ) = full_adder( a[i], b[i], carry );
	}
	if(extend)
  {
	int i = a.size() - 1;
	if(sign)
		std::tie( sum[i + 1], carry ) = full_adder( a[i], b[i], carry );
	else
		sum[i + 1] = carry;
  }
}

std::vector<bdd> bdd_add( const std::vector<bdd>& a, const std::vector<bdd>& b, bool extend, bool sign )
{
	assert( a.size() == b.size() );
	assert( !a.empty() );
	
	std::vector<bdd> sum(a.size());
	bdd carry = bddfalse;
	
	
	for (auto i = 0ul; i < a.size(); i++ )
	{
		std::tie( sum[i], carry ) = full_adder( a[i], b[i], carry );
	}
	if(extend)
	{
		if(sign)
		{
		int i = a.size() - 1;
		bdd val;
		std::tie( val, carry ) = full_adder( a[i], b[i], carry );
		sum.push_back(val);
		}
		else
		{
			sum.push_back(carry);
		}
	}

	return sum;	
}

std::vector<bdd> bdd_mult( const std::vector<bdd>& a, const std::vector<bdd>& b, bool extend)
{
	assert( a.size() == b.size() );
	assert( !a.empty() );
	
	std::vector<bdd> sum(2 * a.size());
	std::vector<bdd> carry(2 * a.size());
	
	for(auto i = 0; i < sum.size(); i++)
	{
		sum[i] = bddfalse;
		carry[i] = bddfalse;
	}
	
	for (auto i = 0ul; i < a.size(); i++ )
	{
		for(int j = a.size() - 1; j >= 0 ; j--)
		{
			//std::cout << i << " - " << j << std::endl;
			bdd res = a[j] & b[i];
			
			std::tie( sum[j + i], carry[j + i + 1] ) = full_adder( res, sum[j + i], carry[j + i] );
		}
		//std::tie( sum[i], carry ) = full_adder( a[i], b[i], carry );
	}
	
	bdd c = bddfalse;
	for(int i = a.size(); i < sum.size(); i++)
	{
		//std::cout << i << "-" << std::endl;
		std::tie( sum[i], c) = full_adder(sum[i], carry[i], c);
		//sum[i] = bddtrue;
	}
	
	if(extend)
		sum.push_back(c);

	return sum;	
}

std::vector<bdd> bdd_abs( const std::vector<bdd>& n )
{
  std::vector<bdd> mask( n.size(), n.back() );
  std::vector<bdd> result;
  
  for (auto i = 0ul; i < n.size(); i++ )
  {
    result.push_back( n[i] ^ mask[i] );
  }

  //auto diff = 
  bdd_subtract(result, result, mask );
  //std::copy( diff.begin(), diff.begin() + n.size(), result.begin() );

  return result;
}

void bdd_abs( std::vector<bdd>& res, const std::vector<bdd>& n )
{
	assert( res.size() == n.size() );
	
	//std::vector<bdd> mask( n.size(), n.back() );
	//std::vector<bdd> result;
  
	auto last = n[n.size() - 1];
	for (auto i = 0ul; i < n.size(); i++ )
	{
		res[i] = ( n[i] ^ last );
	}

	//auto diff = 
	bdd_subtract_val(res, res, last );
  //std::copy( diff.begin(), diff.begin() + n.size(), result.begin() );

  //return result;
}

std::vector<bdd> zero_extend( const std::vector<bdd>& n, unsigned to )
{
  assert( to >= n.size() );
  auto ze = n;
  ze.resize( to, bddfalse ); //.manager->bdd_bot() );
  return ze;
}

std::vector<bdd> sign_extend( const std::vector<bdd>& n, unsigned to )
{
  assert( to >= n.size() );
  auto se = n;
  se.resize( to, n.back() );
  return se;
}


// Worst case value, n must be UNSIGNED 
double worst_case(const std::vector<bdd> & n)
{
	double value = 0;
	bdd cond = bddtrue;
	for(int i = n.size() - 1; i >= 0; i--)
	{
		value = value * 2;
		if(bdd_satone(cond & n[i]) != bddfalse)
		{
			value += 1;
			cond &= n[i];
		}
	}
	return value;
}

// Worst case value, n is signed!
double worst_case_one(const std::vector<bdd> & n)
{
	double value = 0;
    bdd sign = n[n.size() - 1];
	bdd cond = bddtrue;

    int negative = bdd_satone(sign) != bddfalse;

	for(int i = n.size() - 2; i >= 0; i--)
	{
		value = value * 2;
        
        bdd wire = n[i];
        if(negative) wire = wire ^ sign; // speedup

		if(bdd_satone(cond & wire) != bddfalse)
		{
			value += 1;
			cond &= wire;
		}
	}
    if(negative && bdd_satone(cond & sign) != bddfalse) value ++; // error correction for negative error
	return value;
}

// Worstcase - n is SIGNED
double worst_case_abs(const std::vector<bdd> & n)
{
    bdd sign = n[n.size() - 1];

    // positive 
	double value = 0;
	bdd cond = !sign;
	for(int i = n.size() - 2; i >= 0; i--)
	{
		value = value * 2;
		if(bdd_satone(cond & n[i]) != bddfalse)
		{
			value += 1;
			cond &= n[i];
		}
	}

    // negative
	double neg_value = 0;
	bdd neg_cond = sign;
    if(bdd_satone(neg_cond) != bddfalse) { // was ever negative?
        for(int i = n.size() - 2; i >= 0; i--)
        {
            neg_value = 2 * neg_value;
            if(bdd_satone(neg_cond & !n[i]) != bddfalse)
            {
                neg_value += 1;
                neg_cond &= !n[i];
            }
        }
        neg_value += 1;
    }

    //std::cout << " positive: " <<value<< " negative " <<neg_value << std::endl;

    if(neg_value >= value) return neg_value;
	return value;
}

static double pow2(int n) {
	if(n < 64)
		return (1UL << n);
	
	double v = 1;
	for(int i = 0; i < n; i++)
		v *= 2;
	return v;
}

// Basic error probability
double hamming(const std::vector<bdd> & a, const std::vector<bdd> & b) 
{
	bdd v = bddfalse;
	assert(a.size() == b.size());


	for(int i = a.size() - 1; i >= 0; i--)
	{
		v |= a[i] ^ b[i];
	}
	double val = bdd_satcount(v);


	val /= pow2(bdd_varnum());
	return val;
}


// Mean average value, N is SIGNED
double average_abs(const std::vector<bdd> & n)
{
	double val = 0;
    bdd sign = n[n.size() - 1];
	double neg_prob = bdd_satcount(sign) / pow2(bdd_varnum());
    
	for(int i = 0; i < n.size() - 1; i++)
	{
        // sign probability is included
		auto v_pos = bdd_satcount(!sign & n[i]); 
		auto v_neg = bdd_satcount(sign & !n[i]); 
		//std::cout << "Pos: "<< i << " = " << v_neg / pow2(bdd_varnum()) <<std::endl;

		val += v_pos * pow2(i);
		val += v_neg * pow2(i);
	}
	val /= pow2(bdd_varnum());
    if(neg_prob > 0.0)
        val += neg_prob * 1;
	return val;
}

// Mean average value, N is SIGNED
double average_one(const std::vector<bdd> & n)
{
	double val = 0;
    bdd sign = n[n.size() - 1];
	double neg_prob = bdd_satcount(sign) / pow2(bdd_varnum());
	for(int i = 0; i < n.size() - 1; i++)
	{
        bdd wire = n[i];
        if(neg_prob > 0) wire = wire ^ sign;
		auto v = bdd_satcount(wire); 
		val += v * pow2(i);
	}
	val /= pow2(bdd_varnum());
    if(neg_prob > 0.0)
        val += neg_prob * 1;
    
	return val;
}

// Mean average value, N is UNSIGNED
double average(const std::vector<bdd> & n)
{
	double val = 0;

	std::cout << std::endl;
	for(int i = 0; i < n.size(); i++)
	{
		double p = bdd_satcount(n[i]) / pow2(bdd_varnum());
		std::cout << "__Pos: "<< i << " = " << p <<std::endl ;

		auto v = bdd_satcount(n[i]); 
		val += v * pow2(i);
	}
	val /= pow2(bdd_varnum());
	return val;
}
	
unsigned long bdd2int(std::vector<bdd> v)
{
	unsigned long ret = 0;
	for(int i = 0; i < v.size(); i++)
	{
		bool set = bdd_satcount(v[i]) != 0;
		
		//cout << "SATONE(" << i << ")=" << set << endl;
		if(set)
			ret |= 1 << i;
	}
	return ret;
}

bdd bool2bdd(bool in)
{
	if(in) return bddtrue;
	else   return bddfalse;
}

std::vector<bdd> int2bdd(unsigned long val, int n)
{
	std::vector<bdd> ret;
	for(int i = 0; i < n; i++)
	{
		ret.push_back((val & (1 << i)) ? bddtrue : bddfalse);
	}
	return ret;
}

