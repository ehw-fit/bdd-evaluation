#pragma once

#include <vector>
#include <tuple>
#include <cassert>

#include <bdd.h>

	
std::tuple<bdd, bdd> full_adder( const bdd& x, const bdd& y, const bdd& cin );

/* LSB is at bit 0 */
std::vector<bdd> bdd_subtract( const std::vector<bdd>& minuend, const std::vector<bdd>& subtrahend, bool extend = false, bool sign = false  );
void bdd_subtract(std::vector<bdd>& res,  const std::vector<bdd>& minuend, const std::vector<bdd>& subtrahend, bool extend = false, bool sign = false );
// Subtract just one value
void bdd_subtract_val(std::vector<bdd>& res,  const std::vector<bdd>& minuend, const bdd & subtrahend, bool extend = false , bool sign = false );

std::vector<bdd> bdd_add( const std::vector<bdd>& a, const std::vector<bdd>& b, bool extend = false, bool sign = false );
void bdd_add( std::vector<bdd>& res, const std::vector<bdd>& a, const std::vector<bdd>& b, bool extend = false, bool sign = false);

std::vector<bdd> bdd_abs( const std::vector<bdd>& n );
void bdd_abs( std::vector<bdd>& res, const std::vector<bdd>& n );


std::vector<bdd> bdd_mult( const std::vector<bdd>& a, const std::vector<bdd>& b, bool extend = false);

std::vector<bdd> zero_extend( const std::vector<bdd>& n, unsigned to );
std::vector<bdd> sign_extend( const std::vector<bdd>& n, unsigned to );

// Worst-case error, input is unsigned absolute value |a-b|
double worst_case(const std::vector<bdd> & n);
// Worst-case error, input is signed value  a-b (two branches positive and negative error)
double worst_case_abs(const std::vector<bdd> & n);
// Worst-case error, input is signed value  a-b (absolute value is calculated inherently)
double worst_case_one(const std::vector<bdd> & n);


// Mean average error, input is unsigned absolute value |a-b|
double average(const std::vector<bdd> & n);
// Mean average error, input is signed value  a-b (two branches positive and negative error)
double average_abs(const std::vector<bdd> & n);
// Mean average error, input is signed value  a-b (absolute value is calculated inherently)
double average_one(const std::vector<bdd> & n);

// Error probability
double hamming(const std::vector<bdd> & a, const std::vector<bdd> & b);

unsigned long bdd2int(std::vector<bdd> v);
bdd bool2bdd(bool in);
std::vector<bdd> int2bdd(unsigned long val, int n);

void bdd_set_start();
void bdd_set_limit(double limit);