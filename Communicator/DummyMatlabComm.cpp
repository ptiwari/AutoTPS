///*
// * DummyMatlabComm.cpp
// *
// *  Created on: Nov 23, 2011
// *      Author: user
// */
//
//#include "DummyMatlabComm.h"
//#include "FileManager.h"
//
//DummyMatlabComm::DummyMatlabComm() {
//	// TODO Auto-generated constructor stub
//	printf("Super executed\n");
//
//	voxels = (double **) calloc(13, sizeof(double *));
//
//	voxels[10] = (double *) calloc(1, sizeof(double));
//	voxels[11] = (double *) calloc(1, sizeof(double));
//	voxels[12] = (double *) calloc(1, sizeof(double));
//	stepIorganId = new int[1];
//	targetsId = new int[3];
//	stepIorganId[0] = 1;
//	targetsId[0] = 10;
//	targetsId[1] = 11;
//	targetsId[2] = 12;
//
//	numVoxelsRow = new int[13];
//	numStepIOar = 1;
//
//	init();
//}
//
//DummyMatlabComm::~DummyMatlabComm() {
//	// TODO Auto-generated destructor stub
//}
//
//void DummyMatlabComm::init() {
//
//	getTargetVoxel();
//	getInfluenceMatrix();
//	getVoxelStepI();
//	ttlTargetRow = 0;
//	for (int i = 0; i < numTargets; i++) {
//		ttlTargetRow += numVoxelsRow[targetsId[i]];
//	}
//	ttlStepIOarRow = 0;
//	for (int i = 0; i < numStepIOar; i++) {
//		ttlStepIOarRow += numVoxelsRow[stepIorganId[i]];
//	}
//	//influence->setNumFullRow(500);
//	//influence->setNumFullCol(500);
//	//printf("numCol:%d\n",numCol);
//	//printf("numRow:%d\n",numRow);
//}
//void DummyMatlabComm::getTargetVoxel() {
//
//	double *row, *row1, *row2;
//	FileManager *f = new FileManager(
//			"/Users/user/CoinIpopt/Ipopt/examples/Project/Debug/tarVox.txt");
//	row = (double *) malloc(sizeof(double) * 5206);
//	f->readFile(row);
//	delete f;
//
////	row = (double *) malloc(sizeof(double) * 10);
//	//numTargets = 3;
//	numTargets=1;
////
////	for (int i = 1; i <= 10; i++) {
////		row[i - 1] = i;
////	}
//	numVoxelsRow[targetsId[0]] = 5206;
//	voxels[targetsId[0]] = row;
//	FileManager *f1 = new FileManager(
//			"/Users/user/CoinIpopt/Ipopt/examples/Project/src/tarVox12.txt");
//	row1 = (double *) malloc(sizeof(double) * 2412);
//	f->readFile(row1);
//	delete f1;
//	numVoxelsRow[targetsId[1]] = 2412;
//	voxels[targetsId[1]] = row1;
//
//	FileManager *f2 = new FileManager(
//			"/Users/user/CoinIpopt/Ipopt/examples/Project/src/tarVox13.txt");
//	row2 = (double *) malloc(sizeof(double) * 5832);
//	f2->readFile(row2);
//	delete f2;
//	numVoxelsRow[targetsId[2]] = 5832;
//	voxels[targetsId[2]] = row2;
//}
//void DummyMatlabComm::getVoxelStepI() {
////	double *row;
////	row = (double *) malloc(sizeof(double) * 10);
////	for (int i = 21; i <= 31; i++) {
////		row[i - 21] = i;
////	}
//	double *row;
//	row = (double *) malloc(sizeof(double) * 671);
//	FileManager *f = new FileManager(
//			"/Users/user/CoinIpopt/Ipopt/examples/Project/src/stepIVox.txt");
//	f->readFile(row);
//	delete f;
//	voxels[stepIorganId[0]] = row;
//	numVoxelsRow[stepIorganId[0]] = 671;
//}
//void DummyMatlabComm::getInfluenceMatrix() {
//
//	double *row, *col, *val;
//	printf("############################################## Assigning ####################################################\n");
//	FileManager *f;
//	row = (double *) malloc(sizeof(double) * 2922139);
//	f = new FileManager(
//			"/Users/user/CoinIpopt/Ipopt/examples/Project/src/r.txt");
//	f->readFile(row);
//	delete f;
//	printf("Done with reading r\n");
//	col = (double *) malloc(sizeof(double) * 2922139);
//	f = new FileManager(
//			"/Users/user/CoinIpopt/Ipopt/examples/Project/src/c.txt");
//	f->readFile(col);
//	delete f;
//	printf("Done with reading c\n");
//	val = (double *) malloc(sizeof(double) * 2922139);
//	f = new FileManager(
//			"/Users/user/CoinIpopt/Ipopt/examples/Project/src/val.txt");
//	f->readFile(val);
//	delete f;
//	printf("Done with reading val\n");
////	for (int i = 1; i <= 5; i = i + 2) {
////		//totalTargetRow("Assigning\n");
////		row[i - 1] = i;
////		col[i - 1] = i;
////		row[i] = i;
////		col[i] = i + 1;
////
////	}
////	for (int i = 1; i <= 5; i++) {
////		row[i + 5 - 1] = i + 20;
////		col[i + 5 - 1] = i;
////	}
////	for (int i = 0; i < 10; i++) {
////		val[i] = (i + 1) * 0.01;
////	}
//	influence = new SparseMatrix(row, col, val, 1262837, 5898240, 1091,4);
//	//influence = new Matrix(row,col,val,15,500,20);
//
//}

