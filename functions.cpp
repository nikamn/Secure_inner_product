
#include "functions.h"

// concatenating two matrices horizontally
const mat_ZZ hCat(const mat_ZZ& A, const mat_ZZ& B) {
  assert(A.NumRows() == B.NumRows());

  int rows = A.NumRows(), colsA = A.NumCols(), colsB = B.NumCols();
  mat_ZZ result;
  result.SetDims(rows, colsA + colsB);

  // Copy A
  for(int i=0; i<rows; i++) {
    for(int j=0; j<colsA; j++) {
      result[i][j] = A[i][j];
    }
  }

  // Copy B
  for(int i=0; i<rows; i++) {
    for(int j=0; j<colsB; j++) {
      result[i][colsA+j] = B[i][j];
    }
  }

  return result;
}


// concatenating two matrices vertically
const mat_ZZ vCat(const mat_ZZ& A, const mat_ZZ& B) {
  assert(A.NumCols() == B.NumCols());

  int cols = A.NumCols(), rowsA = A.NumRows(), rowsB = B.NumRows();

  mat_ZZ result;
  result.SetDims(rowsA+rowsB, cols);

  // Copy A
  for(int i=0; i<rowsA; i++) {
    for(int j=0; j<cols; j++) {
      result[i][j] = A[i][j];
    }
  }

  // Copy B
  for(int i=0; i<rowsB; i++) {
    for(int j=0; j<cols; j++) {
      result[i+rowsA][j] = B[i][j];
    }
  }

  return result;
}


// returns c*
const vec_ZZ getBitVector(const vec_ZZ& c) {
  vec_ZZ result;
  int length = c.length();
  result.SetLength(length*l);
  for (int i=0; i<length; i++) {
    ZZ sign = (c[i] < ZZ(0)) ? ZZ(-1) : ZZ(1);
    ZZ value = c[i] * sign;
    for (int j=0; j<l; j++) {
      result[i*l+j] = sign*bit(value, j);
    }
  }
  return result;
}

// returns S*
const mat_ZZ getBitMatrix(const mat_ZZ& S) {
  mat_ZZ result;
  int rows = S.NumRows(), cols = S.NumCols();
  result.SetDims(rows, l*cols);

  vec_ZZ powers;
  powers.SetLength(l);
  powers[0] = 1;
  for (int i = 0; i < l-1; i++) {
    powers[i+1] = powers[i]*2;
  }

  for (int i=0; i<rows; i++) {
    for (int j=0; j<cols; j++) {
      for (int k=0; k<l; k++) {
        result[i][j*l+k] = S[i][j]*powers[k];
      }
    }
  }

  return result;
}

// generates random matrix having all elements < bound
const mat_ZZ getRandomMatrix(long row, long col, const ZZ& bound) {
  mat_ZZ A;
  A.SetDims(row, col);
  for (int i=0; i<row; i++) {
    for (int j=0; j<col; j++) {
      A[i][j] = RandomBnd(bound);
    }
  }
  return A;
}

// generates random vector having all elements < bound
const vec_ZZ getRandomVector(long row, const ZZ& bound) {
  vec_ZZ v;
  v.SetLength(row);
  for(int i=0; i<row; i++) {
    v[i] = RandomBnd(bound);
  }
  return v;
}

// returns S
const mat_ZZ getSecretKey(const mat_ZZ& T) {
  mat_ZZ I;
  ident(I, T.NumRows());
  return hCat(I, T);
}

const vec_ZZ encrypt(const mat_ZZ& T, const vec_ZZ& x) {
  mat_ZZ I, S, X;
  vec_ZZ e, wx, wxpluse, c;
  long l;
  ZZ div(1ll << 20ll);

  mul(wx, x, w);
  e = getRandomVector(x.length(), w/div);
  add(wxpluse, wx, e);

  S = getSecretKey(T);
  X=transpose(S);
  l =  LatticeSolve(c, X, wxpluse, 1);
  if(l==1) cout<< "cipher-text exists\n";
  cout << c << endl;
  return c;

}

const vec_ZZ encrypt2(const mat_ZZ& T, const vec_ZZ& x) {
  mat_ZZ I;
  ident(I, x.length());
  return keySwitch(keySwitchMatrix(I, T), w*x);
}

inline const ZZ nearestInteger(const ZZ& x, const ZZ& w) {
  return (x + (w+1)/2) / w;
}


const vec_ZZ decrypt(const mat_ZZ& S, const vec_ZZ& c) {
  vec_ZZ Sc = S*c;
  vec_ZZ output;
  output.SetLength(Sc.length());
  for (int i=0; i<Sc.length(); i++) {
    output[i] = nearestInteger(Sc[i], w);
  }
  cout<< output <<endl;
  return output;
}

const mat_ZZ vectorize(const mat_ZZ& M) {
  mat_ZZ ans;
  ans.SetDims(M.NumRows()*M.NumCols(), 1);
  for (int i=0; i<M.NumRows(); i++) {
    for (int j=0; j<M.NumCols(); j++) {
      ans[i*M.NumCols()+j][0] = M[i][j];
    }
  }
  return ans;
}

const mat_ZZ copyRows(const mat_ZZ& row, long numrows) {
  mat_ZZ ans;
  ans.SetDims(numrows, row.NumCols());
  for (int i=0; i<ans.NumRows(); ++i) {
    for (int j=0; j<ans.NumCols(); ++j) {
      ans[i][j] = row[0][j];
    }
  }
  return ans;
}

// finds c* then returns Mc*
const vec_ZZ keySwitch(const mat_ZZ& M, const vec_ZZ& c) {
  vec_ZZ cstar = getBitVector(c);
  return M*cstar;
}

// returns key-switch matrix M
const mat_ZZ keySwitchMatrix(const mat_ZZ& S, const mat_ZZ& T) {
  mat_ZZ Sstar = getBitMatrix(S);
  mat_ZZ A = getRandomMatrix(T.NumCols(), Sstar.NumCols(), aBound);
  mat_ZZ E = getRandomMatrix(Sstar.NumRows(), Sstar.NumCols(), eBound);
  return vCat(Sstar + E-T*A, A);
}


const mat_ZZ keySwitchingMatrix(const mat_ZZ& T) {
  mat_ZZ S = getSecretKey(T);
  mat_ZZ tvsts = transpose(vectorize(transpose(S)*S));
  mat_ZZ mvsts = copyRows(tvsts, T.NumRows());
  return mvsts;
}

const mat_ZZ keySwitchingMatrix2(const mat_ZZ& T1, const mat_ZZ& T2, const mat_ZZ& T3) {
  mat_ZZ S1 = getSecretKey(T1);
  mat_ZZ S2 = getSecretKey(T2);
  mat_ZZ tvsts = transpose(vectorize(transpose(S1)*S2));
  mat_ZZ mvsts = copyRows(tvsts, T3.NumRows());
  return keySwitchMatrix(mvsts, T3);
}


const vec_ZZ innerProd(const vec_ZZ& c1, const vec_ZZ& c2) {
  mat_ZZ cc1, cc2, cc;

  cc1.SetDims(c1.length(), 1);
  for (int i=0; i<c1.length(); i++) {
    cc1[i][0] = c1[i];
  }
  cc2.SetDims(1, c2.length());
  for (int i=0; i<c2.length(); i++) {
    cc2[0][i] = c2[i];
  }
  cc = vectorize(cc1*cc2);

  vec_ZZ output;
  output.SetLength(cc.NumRows());
  for (int i=0; i<cc.NumRows(); i++) {
    output[i] = nearestInteger(cc[i][0], w);
  }
  
  return output;
}

