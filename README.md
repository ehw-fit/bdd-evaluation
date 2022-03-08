# Fast BDD-based evaluation
This repository provides codes for fast evaluation of approximate adders and subtractors by means of BDD diagrams. It employs BuDDy library but can be easily trasfered to CuDD (with C++ interface). The code style is inspired by Mathias Soeken tool [Cirkit](https://github.com/msoeken/cirkit). Note that this repository provides only the evaluation algorithms, not the searching algorithm.


## Usage in your application
```c++
#include <iostream>
#include <cstdlib>
#include <vector>
#include <bdd.h>

#include "arithmetic.hpp"

using namespace std;

static void bdd_gbchandler(int pre, bddGbcStat *s)
{ }

int main(int argc, char **argv)
{
    const int inputs = 16; // 8-bit adder
    const bool is_signed = false; // unsigned output
    vector<bdd> bdd_res(9);
    vector<bdd> bdd_ref(9);


    bdd_init(10000000, 10000000);
    bdd_setvarnum(inputs);
    bdd_gbc_hook(bdd_gbchandler); // disable messages about garbage collector
    bdd_set_limit(120);
    bdd_set_start();

    double time = cpuTime();

    // TODO: create reference and golden models
    // fill vectors bdd_res and bdd_ref (e.g. )
    // bdd a = bdd_ithvar(0);
    // bdd b = a ^ bdd_ithvar(2)
    // bdd_res[2] = a & b;


    // Example of calculation
    auto sub = bdd_subtract(bdd_res, bdd_ref, true, is_signed);
    auto res = bdd_abs(sub);
    cout << "# subtract created at " << cpuTime() - time << endl;
    cout << "Signed: " << is_signed << endl;

    cout << "Baseline mae: " << average(res) << endl;
    cout << "Ones complement mae: " << average_one(sub) << endl;
    cout << "Noabs mae: " << average_abs(sub) << endl;

    cout << "Baseline wc: " << worst_case(res) << endl;
    cout << "Ones complement wc: " << worst_case_one(sub) << endl;
    cout << "Noabs wc: " << worst_case_abs(sub) << endl;

    cout << "EP: " << hamming(bdd_res, bdd_ref) << endl;

    cout << "# finished at " << cpuTime() - time << endl;

    bddcgp_done();

    return 0;
}
```

## Building and testing
To build a library run a following commands
```sh
mkdir build
cd build
cmake ..
make
# Run tests
make test
```