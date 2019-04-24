
#include "functions.h"
#include <fstream>
#include <time.h>
//#define VECLENGTH 10
//#define THINSIZE 10


int main() {
  cin.tie(NULL);
  ios_base::sync_with_stdio(false);

  ofstream myfile;
  myfile.open("switchMatrices", ios::app); 
  ZZ a, b, c, d, p = GenPrime_ZZ(10, 80), q = GenPrime_ZZ(1024, 80);
  long l, THINSIZE; char ch; int m, VECLENGTH;

  list<vec_ZZ*> vecA, vecB, vecT;
  list<vec_ZZ*>cvecA, cvecB, cvecT;
  list<vec_ZZ*>innerAB;
  list<mat_ZZ*> matA, matB, matT, matC, matint;
  list<mat_ZZ*> switchMatA, switchMatB, switchMatAB, switchMatT;
  list<mat_ZZ*> switchAint, switchBint, switchintC;
  //list<mat_ZZ*> 
  //list<mat_ZZ*>::iterator itA, itB, itC, itD;
  string op;
  clock_t startA, startB, startT, endA, endB, endT;
  double cpu_time_usedA, cpu_time_usedB, cpu_time_usedT;

  clock_t start, end;
  double switch_timeA, switch_timeB, switch_timeAB, switch_timeT;

  cout << "Available commands:" <<endl;
  cout << "vector\tmatrix\tencrypt\tdecrypt\tswitch-matrix-algo1\tswitch-matrix-algo3\tkey-switch-single-vector\texit" << endl;
  cout << "innerProdClient\tinnerProdCloud\tinnerProdBoth" << endl;

  while (cin>>op) { 

    if (op == "vector") {
      cout << "1.random(R)\t2.manual(M)" << endl;
      cin >> ch;

      if (ch == 'M') {
       	cout << "How many vectors?(<MAX_INT). Give vectors of same length.." << endl;
	cin >> m;
	while (m--) {
	  vec_ZZ* v = new vec_ZZ();
	  cin >> (*v);
	  vecT.push_back(v);
	}
      } else if (ch == 'R') {
	cout << "How many random vectors?(<MAX_INT)" << endl;
	cin >> m;
	cout << "Give vector dimension:" << endl;
	cin >> VECLENGTH;
	while (m--) {
	  vec_ZZ* vA = new vec_ZZ();
	  vec_ZZ* vB = new vec_ZZ();
	  vec_ZZ* vT = new vec_ZZ();
	  for (int i = 0; i < VECLENGTH; i++) {
	    l = RandomBits_long(1);
	    (l==0) ? (*vA).append(RandomBnd(p)):(*vB).append(RandomBnd(p));
	    (l==0) ? (*vB).append(d):(*vA).append(d);
	  }
	  cout << "vA:" << *vA << endl;
	  cout << "vB:" << *vB << endl;
	  *vT = *vA + *vB;
	  cout << "Total vector:" << *vT << endl;
	  vecA.push_back(vA);
	  vecB.push_back(vB);
	  vecT.push_back(vT);
	}
      } else {
	cerr << "Wrong choice\n";
      }
    }

    else if (op == "matrix") {
      mat_ZZ* mA = new mat_ZZ();
      mat_ZZ* mB = new mat_ZZ();
      mat_ZZ* mT = new mat_ZZ();
      mat_ZZ* mC = new mat_ZZ();
      mat_ZZ* mAB = new mat_ZZ();
      cout << "Give the column size of thin matrix" << endl;
      cin >> THINSIZE;
      cout << "Random party A matrix created" << endl;
      vec_ZZ& x = *vecT.front();
      (*mA) = getRandomMatrix(x.length(), THINSIZE, tBound);
      matA.push_back(mA);
      cout << *mA << endl;

      cout << "Random party B matrix created" << endl;
      (*mB) = getRandomMatrix(x.length(), THINSIZE, tBound);
      matB.push_back(mB);
      cout << *mB << endl;

      cout << "Random party T matrix created" << endl;
      (*mT) = getRandomMatrix(x.length(), THINSIZE, tBound);
      matT.push_back(mT);
      cout << *mT << endl;
    
      cout << "Random cloud matrix created" << endl;
      (*mC) = getRandomMatrix(x.length(), THINSIZE, tBound);
      matC.push_back(mC);
      cout << *mC << endl;

      cout << "Random party A and party B agreement matrix created" << endl;
      (*mAB) = getRandomMatrix(x.length(), THINSIZE, tBound);
      matint.push_back(mAB);
      cout << *mAB << endl;
    }

    else if (op == "encrypt") {
      cout << "Encrypting vector Total:" << endl;
      startT = clock();
      while (!vecT.empty()) {
	mat_ZZ& T = *matT.front();
	vec_ZZ& x = *vecT.front();
	vecT.pop_front();
	vec_ZZ* v = new vec_ZZ();
	(*v) = encrypt(T, x);
	cvecT.push_back(v);
      }
      endT = clock();
      cout << "Encrypting vector vA:" << endl;
      startA = clock();
      while (!vecA.empty()) {
	mat_ZZ& T = *matA.front();
	vec_ZZ& x = *vecA.front();
	vecA.pop_front();
	vec_ZZ* v = new vec_ZZ();
	(*v) = encrypt(T, x);
	cvecA.push_back(v);
      }
      endA = clock();
      cout << "Encrypting vector vB:" << endl;
      startB =clock();
      while (!vecB.empty()) {
	mat_ZZ& T = *matB.front();
	vec_ZZ& x = *vecB.front();
	vecB.pop_front();
	vec_ZZ* v = new vec_ZZ();
	(*v) = encrypt(T, x);
	cvecB.push_back(v);
      }
      endB = clock();
      
      cpu_time_usedT = ((double) (endT-startT)) / CLOCKS_PER_SEC;
      cout << "Time to encrypt vT:" << cpu_time_usedT << endl;
      
      cpu_time_usedA = ((double) (endA-startA)) / CLOCKS_PER_SEC;
      cout << "Time to encrypt vA:" << cpu_time_usedA << endl;
      
      cpu_time_usedB = ((double) (endB-startB)) / CLOCKS_PER_SEC;
      cout << "Time to encrypt vB:" << cpu_time_usedB << endl;
    }

    else if (op == "decrypt") {
      cout << "Decrypting vector Total:" << endl;
      startT = clock();
      while (!cvecT.empty()) {
	mat_ZZ& T = *matT.front();
	vec_ZZ& c = *cvecT.front();
	cvecT.pop_front();
	mat_ZZ S = getSecretKey(T);
	vec_ZZ *x = new vec_ZZ();
	(*x) = decrypt(S, c);
	vecT.push_back(x);
      }
      endT = clock();
      cout << "Decrypting vector vA:" << endl;
      startA = clock();
      while (!cvecA.empty()) {
	mat_ZZ& T = *matA.front();
	vec_ZZ& c = *cvecA.front();
	cvecA.pop_front();
	mat_ZZ S = getSecretKey(T);
	vec_ZZ* x = new vec_ZZ();
	(*x) = decrypt(S, c);
	vecA.push_back(x);
      }
      endA = clock();
      cout << "Decrypting vector vB:" << endl;
      startB = clock();
      while (!cvecB.empty()) {	
	mat_ZZ& T = *matB.front();
	vec_ZZ& c = *cvecB.front();
	cvecB.pop_front();
	mat_ZZ S = getSecretKey(T);
	vec_ZZ* x = new vec_ZZ();
	(*x) = decrypt(S, c);
	vecB.push_back(x);
      }
      endB = clock();

      cpu_time_usedT = ((double) (endT-startT)) / CLOCKS_PER_SEC;
      cout << "Time to decrypt vT:" << cpu_time_usedT << endl;
      
      cpu_time_usedA = ((double) (endA-startA)) / CLOCKS_PER_SEC;
      cout << "Time to decrypt vA:" << cpu_time_usedA << endl;
      
      cpu_time_usedB = ((double) (endB-startB)) / CLOCKS_PER_SEC;
      cout << "Time to decrypt vB:" << cpu_time_usedB << endl;
    }

    else if (op == "switch-matrix-algo1") {
      mat_ZZ& TA = *matA.front();
      mat_ZZ& TB = *matB.front();
      mat_ZZ& TT = *matT.front();
      mat_ZZ& TC = *matC.front();

      mat_ZZ* mA = new mat_ZZ();
      mat_ZZ* mB = new mat_ZZ();
      mat_ZZ* mT = new mat_ZZ();
      mat_ZZ* mAB = new mat_ZZ();

      start = clock();
      (*mA) = keySwitchingMatrix2(TA, TA, TC);
      end = clock();
      switchMatA.push_back(mA);
      switch_timeA = ((double) (end-start)) / CLOCKS_PER_SEC;
      cout << "Time for switchMatA:" << switch_timeA << endl; 
      myfile << *mA;
      
      start = clock();
      (*mB) = keySwitchingMatrix2(TB, TB, TC);
      end = clock();
      switchMatB.push_back(mB);
      switch_timeB = ((double) (end-start)) / CLOCKS_PER_SEC;
      cout << "Time for switchMatB:" << switch_timeB << endl; 
      myfile << *mB;
      
      start = clock();      
      (*mT) = keySwitchingMatrix2(TT, TT, TC);
      end = clock();
      switchMatT.push_back(mT);
      switch_timeT = ((double) (end-start)) / CLOCKS_PER_SEC;
      cout << "Time for switchMatT:" << switch_timeT << endl; 

      start = clock();      
      (*mAB) = keySwitchingMatrix2(TA, TB, TC);
      end = clock();
      switchMatAB.push_back(mAB);
      switch_timeAB = ((double) (end-start)) / CLOCKS_PER_SEC;
      cout << "Time for switchMatAB:" << switch_timeAB << endl;
      myfile << *mAB;
    }
    else if (op == "key-switch-algo3") {
      mat_ZZ& TA = *matA.front();
      mat_ZZ& TB = *matB.front();
      mat_ZZ& TC = *matC.front();
      mat_ZZ SA = getSecretKey(TA);
      mat_ZZ SB = getSecretKey(TB);
      mat_ZZ* mA = new mat_ZZ();
      mat_ZZ* mB = new mat_ZZ();
      

      
    }
    else if (op == "key-switch-single-vector") {
      vec_ZZ& c = *cvecT.front();
      mat_ZZ& TT = *matT.front();
      mat_ZZ& TC = *matC.front();
      mat_ZZ S = getSecretKey(TT);
      vec_ZZ cstar = getBitVector(c);
      mat_ZZ Sstar = getBitMatrix(S);
      vec_ZZ Sc = S*c;
      vec_ZZ Sstarcstar = Sstar*cstar;
      if (Sc == Sstarcstar)
	cout << "(S*)*(c*) equal S*c" << endl;
      else
	cout << "(S*)*(c*) not equal S*c" << endl;

      mat_ZZ M = keySwitchMatrix(S, TC);
      vec_ZZ cdash = M*cstar;
      vec_ZZ x = decrypt(getSecretKey(TC), cdash);
    }
    
    else if (op == "innerProdClient") {
      vec_ZZ& v1 = *cvecT.front();
      vec_ZZ& v2 = *cvecT.back();
      mat_ZZ& T = *matT.front();
      vec_ZZ* c = new vec_ZZ();
      (*c) = innerProd(v1, v2);
      vec_ZZ x = decrypt(keySwitchingMatrix(T), *c);
    }

    else if (op == "innerProdCloud") {
      mat_ZZ& T = *matT.front();
      mat_ZZ& TC = *matC.front();
      mat_ZZ& M = *switchMatT.front();
     
      vec_ZZ& v1 = *cvecT.front();
      vec_ZZ& v2 = *cvecT.back();
      vec_ZZ c = innerProd(v1, v2);

      vec_ZZ cstar = getBitVector(c);
      vec_ZZ cdash = M*cstar;
      vec_ZZ x = decrypt(getSecretKey(TC), cdash);
    }

    else if (op == "innerProdBoth") {
      mat_ZZ& TC = *matC.front();
      mat_ZZ S = getSecretKey(TC);
      
      mat_ZZ& MA = *switchMatA.front();
      mat_ZZ& MB = *switchMatB.front();
      mat_ZZ& MAB = *switchMatAB.front();

      /* for (itA = cvecA.begin(), itB = cvecB.begin(); itA != cvecA.end(); itA++, itB++) {
	for (itC = itA+1, itD = itB+1; itC != cvecA.end(); itC++, itD++) {
	  
	  vec_ZZ& v1 = *itA;
	  vec_ZZ& v2 = *itC;
	  vec_ZZ& v3 = *itB;
	  vec_ZZ& v4 = *itD;
      */
	  vec_ZZ& v1 = *cvecA.front();
	  vec_ZZ& v2 = *cvecA.back();
	  vec_ZZ& v3 = *cvecB.front();
	  vec_ZZ& v4 = *cvecB.back();

	  vec_ZZ c1 = innerProd(v1, v2);
	  vec_ZZ c2 = innerProd(v3, v4);
	  vec_ZZ c3 = innerProd(v1, v4);
	  vec_ZZ c4 = innerProd(v2, v3);

	  vec_ZZ cdash1 = MA*getBitVector(c1);
	  vec_ZZ cdash2 = MB*getBitVector(c2);
	  vec_ZZ cdash3 = MAB*getBitVector(c3);
	  vec_ZZ cdash4 = MAB*getBitVector(c4);
      
	  vec_ZZ cfinal = cdash1+cdash2+cdash3+cdash4;
	  vec_ZZ x = decrypt(S, cfinal);
	  vec_ZZ x1 = decrypt(S, cdash1);
	  vec_ZZ x2 = decrypt(S, cdash2);
	  vec_ZZ x3 = decrypt(S, cdash3);
	  vec_ZZ x4 = decrypt(S, cdash4);
	  vec_ZZ xfinal = x1+x2+x3+x4;
	  innerAB.push_back(&xfinal);
	  cout << xfinal << endl;
	  //}
	  //}
    }
    
    else if (op == "exit") {
      cout << "program exiting" << endl;
      exit(1);
    }

    else{
      cerr << "Unknown command: " << op << endl;
    }
  }
  
  return 0;
}
 
