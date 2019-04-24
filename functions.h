#pragma once

#include <NTL/tools.h>
#include <NTL/ZZ.h>
#include <NTL/vec_ZZ.h>
#include <NTL/mat_ZZ.h>
#include <NTL/LLL.h>
#include <cassert>
//#include <queue>
#include <list>


/*
// mersene random generator
// generate algorithm

#include <random>
#include <algorithm>
#include <functional>
#include <iterator>
*/


using namespace std;
using namespace NTL;

const ZZ w(1ll << 50ll);
const ZZ aBound(1000), tBound(aBound), eBound(1000);
const int l = 100;


const mat_ZZ hCat(const mat_ZZ& A, const mat_ZZ& B);
const mat_ZZ vCat(const mat_ZZ& A, const mat_ZZ& B);

const vec_ZZ getBitVector(const vec_ZZ& c);	// returns c*
const mat_ZZ getBitMatrix(const mat_ZZ& S);	// returns S*
const mat_ZZ getRandomMatrix(long row, long col, const ZZ& bound);
const vec_ZZ getRandomVector(long row, const ZZ& bound);
const mat_ZZ getSecretKey(const mat_ZZ& T);	// returns S

const mat_ZZ keySwitchMatrix(const mat_ZZ& S, const mat_ZZ& T);	// returns M
const vec_ZZ keySwitch(const mat_ZZ& M, const vec_ZZ& c);	// finds c* then returns Mc*

// as described, treating I as the secret key and wx as ciphertext
const vec_ZZ encrypt2(const mat_ZZ& T, const vec_ZZ& x);

const vec_ZZ encrypt(const mat_ZZ& T, const vec_ZZ& x);
const vec_ZZ decrypt(const mat_ZZ& S, const vec_ZZ& c);

// computes an inner product, given two ciphertexts and the keyswitch matrix
const vec_ZZ innerProd(const vec_ZZ& c1, const vec_ZZ& c2);

// returns M, the key switch matrix from vec(S^t S) to S,
// to be sent to the server
const mat_ZZ keySwitchingMatrix(const mat_ZZ& T);
const mat_ZZ keySwitchingMatrix2(const mat_ZZ& T1, const mat_ZZ& T2, const mat_ZZ& T3);

const mat_ZZ vectorize(const mat_ZZ& M);	// returns a column vector
const mat_ZZ copyRows(const mat_ZZ& row, long numrows);




