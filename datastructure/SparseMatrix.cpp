/*
 * SparseMatrix.cpp
 * Matrix follows the C Index style, starting from 0.
 *
 *  Created on: Nov 20, 2011
 *      Author: user
 */

#include "SparseMatrix.h"
/*
 * Creating new sparse matrix.
 * @param r: Row indicies
 * c: Column indicies
 * val:  Value of matrix entry. Notice that the size of r,c and val must be same. And the val[i] indicates r[i],c[i] entry of the matrix.
 * nonzeroel: Total number of nonzero element in the matrix.
 * rf: Number of rows in the matrix.
 * cf: Number of columns in the matrix
 * n: Size of hashmap
 */
SparseMatrix::SparseMatrix(double* r, double* c, double *val,
		long unsigned int nonzeroel, long unsigned int rf,
		long unsigned int cf) {
	// TODO Auto-generated constructor stub
	using namespace boost::numeric::ublas;
//	htbl = new HashTable();
//	htbl->initialize(r, c, val, nonzeroel);
//	row = rf;
//	col = cf;
//	hmap = new HashMap(n);
	rowIndex = r;
	colIndex = c;
//	value = val;
	nzEntry = nonzeroel;
	printf("rf:%d\n", rf);
	printf("cf:%d\n", cf);
	value = new compressed_matrix<double, ublas::row_major>(rf, cf, nonzeroel);
	//value->set_filled(0,nonzeroel);
	//value.resize(rf,cf,false);
	printf("Nonzero El:%d\n", nonzeroel);
	//nonzeroel =1000;
	int rn = 0;
	int cn = 0;
	printf("Inside sparse\n");
	struct timeval start, end;
	long mtime, seconds, useconds;
	gettimeofday(&start, NULL);
	for (long unsigned int i = 0; i < nonzeroel; i++) {
		//printf("Iterating:%d\n",i);
		assert(r[i]<rf);
		assert(c[i]<cf);
		//We need to make sure that the rows are sorted.
		//Inserting unsorted row in the compressed matrix takes too much time.
		if (i < nonzeroel - 1)
			assert(r[i] < r[i+1]);
		//(*value).insert_element(r[i], c[i], val[i]);
		unsigned long int row = (unsigned long int) r[i];
		unsigned long int col = (unsigned long int) c[i];
		//printf("r:%lu c:%lu:%lf\n", row, col, val[i]);
		//value->push_back(row, col, val[i]);
		//value->insert_element(r[i], c[i], val[i]);

		//printf("row:%lu\n",row);
		
		value->insert_element(row, col, val[i]);
		
	}
	gettimeofday(&end, NULL);
	seconds  = end.tv_sec  - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
	printf("Elapsed time: %ld milliseconds\n", mtime);
	printf("Done sparse\n");
//
	//value->complete_index1_data();
	//printf("Done with initialization");
}
/*
 * Creating new sparse matrix for the Hessian. As hessian involves just scanning the list. So don't want to use hashtable.
 * @param r: Row indicies
 * c: Column indicies
 * val:  Value of matrix entry. Notice that the size of r,c and val must be same. And the val[i] indicates r[i],c[i] entry of the matrix.
 * nonzeroel: Total number of nonzero element in the matrix.
 * row: Number of rows in the matrix.
 * col: Number of columns in the matrix
 */
//SparseMatrix::SparseMatrix(double* r, double* c, double *val, int nonzeroel,
//		int row, int col) {
//	rowIndex = r;
//	colIndex = c;
//	nzEntry = nonzeroel;
//	value = val;
//	this->row = row;
//	this->col = col;
//
//}
SparseMatrix::~SparseMatrix() {
	// TODO Auto-generated destructor stub
	//delete hmap;
	//delete rowIndex;
	//delete colIndex;
	//delete value;
	//delete htbl;
}
/*
 * Get the value at i,0th location of the matrix.
 * @param:
 * 	i: Row Number.
 * @return:
 * 	The value at (i,0) of the matrix.
 */
double SparseMatrix::getValAt(long unsigned int i) {
	double v = (*value)(getRowNumber(i), getColNumber(i));
	//printf("Row:%lu col:%lu:%lf\n", getRowNumber(i), getColNumber(i), v);
	//return (*value)(getRowNumber(i),getColNumber(i));
	return v;
}
/*
 * Return the row index
 */
long unsigned int SparseMatrix::getRowNumber(long unsigned int i) {
	long unsigned int r = (long unsigned int) rowIndex[i];
	return r;
}
long unsigned int SparseMatrix::getColNumber(long unsigned int i) {
	long unsigned int c = (long unsigned int) colIndex[i];
	return c;
}
/*
 * Get the value at i,jth entry of the matrix.
 * The matrix indicies starts at zero.
 * @param:
 * 	i: The row number.
 * 	j: The column number.
 */
double SparseMatrix::getAt(long unsigned int i, long unsigned int j) {

	//return htbl->search(i, j);
	double v = (*value)(i, j);
	//printf("Row:%lu col:%lu:%lf\n",i,j,v);
	return v;
}

/*
 * This will give the submatrix from the influence matrix. The array rows holds the row number in the influence Matrices.
 * The influence matrix index starts from one, while the submatrix retunrned from this code is indexed from zero.
 *
 */
Matrices * SparseMatrix::getSubMatrix(int id, double *rows,
		long unsigned int numRow) {

	assert(id>=0);

	using namespace boost::numeric::ublas;
	long unsigned int numCol = this->getCol();
	//matrix_range<compressed_matrix<double> > mr(value, range (0, numRow), range (0, numCol));
	compressed_matrix<double, row_major> *m;
	Matrices *result;
	result = mp[id];

	if(result != NULL)
	{
		Matrices *cloneResult = new Matrices(result);
		return cloneResult;
	}
	result = new Matrices();
	m = new compressed_matrix<double, row_major>(numRow, numCol);
	for (int i = 0; i < numRow; i++) {
		int r = rows[i];
		for (int j = 0; j < numCol; j++) {
			double v = (*value)(r, j);
			m->push_back(i, j, v);

			if (v != 0) {
				result->addElement(v);
				result->addRowNumber(i);
				result->addColNumber(j);

			}
		}
	}
	//result = new Matrices(m);
	result->setMatrix(m);
	Matrices *cloneResult = new Matrices(result);
	mp[id] = result;
	return cloneResult;
	//return result;
//	result = new Matrices(numRow, col);
//
//	if (result != NULL) {
//
//		for (int i = 0; i < numRow; i++) {
//
//			for (int j = 0; j < col; j++) {
//				if (rows != NULL) {
//					int r = (int) rows[i];
//					double val = htbl->search(r, j);
//					result->getMat()[i][j] = val;
//				} else
//					result->getMat()[i][j] = 0;
//			}
//		}
//	}
//
//

}

//int SparseMatrix::searchr(int r,int c)
//{
//	//printf("Inside the searchr\n");
//	//printf("Searchr:%d\n",numRow);
//	for(int i=0;i<numRow;i++)
//	{
//		//printf("i:%d\n",i);
//		if(row[i]==r && col[i]==c)
//		{
//			return i;
//		}
//	}
//	//printf("End of Searchr\n");
//	return -1;
//}
//void SparseMatrix::printMatrix()
//{
//	printf("Row:%d\n",numRow);
//	for(int i=0;i<numRow;i++)
//	{
//		printf("i:%d\n",i);
//		printf("%lf %lf:%lf\n",row[i],col[i],value[i]);
//	}
//}
//	}
//}
