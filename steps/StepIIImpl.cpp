/*
 * StepIIImpl.cpp
 *
 *  Created on: Sep 15, 2012
 *      Author: user
 */

#include "StepIIImpl.h"
#ifdef dummy
StepIIImpl::StepIIImpl(oneVoxel *m,StepIImpl *stpp2)
#else
StepIIImpl::StepIIImpl(MatlabCommunicationManager *m, StepIImpl *stpp2)
#endif
				{
//StepIIImpl::StepIIImpl(oneVoxel *m, StepIImpl *stp2) {
	// TODO Auto-generated constructor stub
	mtlb = m;
	NUMBEAMLET = mtlb->getColInfMat();
	this->step2 = stpp2;
	grad = NULL;
//	printf("StepIIImpl col:%d\n",mtlb->getColInfMat());
//	printf("Numbeamlet:%d\n", NUMBEAMLET);
	printf("Hi from stepIII\n");

}

StepIIImpl::~StepIIImpl() {
	// TODO Auto-generated destructor stub

}
void StepIIImpl::getStartingPoint(double *x,double *z_l,double *z_u) {
	
	PrioritizedMain::Vector *w = step2->getW2();
	//double *ZL = step1->getZL();
	//double *ZU = step1->getZU();
	int nv = getNumVar();
	memcpy(x,step2->xn,sizeof(double)*nv);
	
	//int numconst = getNumConstraint();
	/*for(int i=0;i<numconst;i++)
	{
		//printf("zl[%d]:%lf\n",i,ZL[i]);
		//printf("zu[%d]:%lf\n",i,ZU[i]);
		z_l[i] = 1;
		z_u[i] = 1;
	}*/
}	
/**
 * The objective is f(x) = sum{i in RIII} D(w)i.
 *
 */
long double StepIIImpl::evaluateObjective(const double *x) {

	int num = mtlb->getNumStepIIIOar();
	PrioritizedMain::Vector **D;
	long double sm = 0;
	D = new PrioritizedMain::Vector *[num];
	mtlb->getDose(D, x, mtlb->getStepIIId(), num);
	this->divideByVoxel(D);
	for (int i = 0; i < num; i++)
		sm += D[i]->sum();
	return sm;
	for(int i=0;i<num;i++)
	{
		delete D[i];
	}
	delete[] D;
	//return step2->evaluateObjective(x);

}

/**
 * Evaluate the gradient of function.
 * The function F(x) = 1/|Vi| *sum{i in RIII} D(w)i
 * The gradient of a function is given by: \/f(x) = (df/dx1, df/dx2,df/dx3...df/dxn )
 * The gradient of the function equals to the sum of the influence matrix column wise  .
 *
 */
void StepIIImpl::evaluategradf(const double *x, double *g) {
	
	int numvar = getNumVar();
	if(grad == NULL)
	{
		grad = new double[numvar];
	
		for (int i = 0; i < getNumVar(); i++)
			grad[i] = 0;
		int num = mtlb->getNumStepIIIOar();
		int ttlVxls = mtlb->getTotalStepIIIVoxels();

		double **voxels = mtlb->getVoxels();
		int *numVoxel = mtlb->getNumVoxel();
		Matrices **m = new Matrices*[num];
		int *id = mtlb->getStepIIId();
		time_t start = time(NULL);
		for (int i = 0; i < num; i++) {
	//		voxels
	//		mtlb->getVoxelofOar(mtlb->getStepIIId(), num, vxls);
	//		printf("numVoxel[id[i]]:%d\n",numVoxel[id[i]]);
	//		for(int j=0;j<numVoxel[id[i]];j++)
	//		{
	//			double *r = voxels[id[i]];
	//			cout<<r[j]<<endl;
	//
	//		}
			m[i] = mtlb->influence->getSubMatrix(id[i], voxels[id[i]], numVoxel[id[i]]);
			//m[i] = mtlb->influence->getSubMatrix(-1, voxels[id[i]], numVoxel[id[i]]);
			//printf("id:%d\n",id[i]);
			//printf("m[%d]\n",i);
			//m[i]->printNonZeroEntry();
		}
		time_t end_t = time(NULL);
		double diff = difftime(end_t, start);
		printf("Submatrix time:%lf\n", diff);
		start = time(NULL);
		this->divideByVoxel(m);
		end_t = time(NULL);
		diff = difftime(end_t, start);
		printf("divide time:%lf\n", diff);
		int k=0;
		start = time(NULL);
		for (int i = 0; i < num; i++) {
			m[i]->colSums(grad);
			//k += numVoxel[id[i]];

		}
		end_t = time(NULL);
		diff = difftime(end_t, start);
		printf("colsum time:%lf\n", diff);
		for (int i = 0; i < num; i++) {
			delete m[i];
		}
		delete[] m;
	}
	
	for (int i = 0; i < getNumVar(); i++)
		g[i] = grad[i];
	
}
void StepIIImpl::divideByVoxel(Matrices **m) {
	int  numVoxel = 0;
	int num = mtlb->getNumStepIIIOar();
	int *stepIIIid = mtlb->getStepIIId();
	int *numVox = mtlb->getNumVoxel();
	for (int i = 0; i < num; i++) {

		numVoxel = numVox[stepIIIid[i]];
		//printf("numVoxel:%d\n",numVoxel);
		//		end = start + numVoxel;
		//		//printf("start:%d\n",start);
		//		//printf("end:%d\n",end);
		//		D->divide(numVoxel, start, end);
		m[i]->divide(numVoxel);

	}
}
/*
 * Get the number of variables in the optimization.
 */
int StepIIImpl::getNumVar() {

	return step2->getNumVar();

}
/*
 * Divide the matrix D by number of voxel.
 * The D corresponds to the dose to different target structures.
 * So different target structure might have different number of voxel.
 * The code finds the number of voxel corresponding to dose entry and divide it by that number.
 */
void StepIIImpl::divideByVoxel(PrioritizedMain::Vector **D) {
	int start = 0, end = 0, numVoxel = 0;
	int num = mtlb->getNumStepIIIOar();
	int *stepIIIid = mtlb->getStepIIId();
	int *numVox = mtlb->getNumVoxel();
	for (int i = 0; i < num; i++) {
		start += numVoxel;
		numVoxel = numVox[stepIIIid[i]];
		//printf("numVoxel:%d\n",numVoxel);
//		end = start + numVoxel;
//		//printf("start:%d\n",start);
//		//printf("end:%d\n",end);
//		D->divide(numVoxel, start, end);
		D[i] = (*D[i])/numVoxel;

	}
}

/*
 * Get the number of constraints.
 *
 */
int StepIIImpl::getNumConstraint() {
#ifdef stp2
	int m = step2->getNumConstraint();
	//return m;
	return m + this->getNumNewConst();
#else
	return this->getNumNewConst();
#endif
}

/*
 * @Input: None
 * @Return:
 * 	Number of non-zero entries in Jacobian matrix.
 * @Author:
 * 	Paras Babu Tiwari
 */
int StepIIImpl::getNumJac() {
#ifdef stp2
	int m = step2->getNumJac() + this->getNumNewConstJac();
	return m;
#else
	int m = this->getNumNewConstJac();
	return m;
#endif
}
void StepIIImpl::getBounds(double *x_l, double *x_u, double *g_l, double *g_u) {
	double slip = (1 + Constant::SLIP) * (1 + Constant::SLIP)* (1 + Constant::SLIP);
	printf("s3:%lf\n",slip);
	this->getBounds(x_l, x_u, g_l, g_u, slip);
}
/**
 * Get the upper and lower bounds of variables.
 * @Input Param: None
 * @Output Param:
 * 	x_l: lower bound for the variables.
 * 	x_u: Upper bound for the variables.
 * 	g_l: Lower bound for constraints.
 * 	g_u: Upper bound for constraints.
 */
void StepIIImpl::getBounds(double *x_l, double *x_u, double *g_l, double *g_u,double slip,double s2) {

	//double moh = step2->getMialphaMax();
	double moh = step2->getObjectiveFnValue();

	int index = 0;
#ifdef stp2
	//Set the upper and lower bound of infinity or max beamlet weight and 0 for variables.

	step2->getBounds(x_l, x_u, g_l, g_u, slip,s2);
	index = step2->getNumConstraint();

	//printf("Moh:%lf\n", moh);
	for (int i = 0; i < getNumNewConst(); i++) {
		g_l[index] = -2e19;
		g_u[index] = moh;
		index++;
	}
#else
	for (int i = 0; i < getNumNewConst(); i++) {
		g_l[index] = -2e19;
		g_u[index] = moh;
		index++;
	}
#endif
}
void StepIIImpl::evalg(const double *x, double *g) {
	int total=0;
	evalg(x,g,total);
}
/*
 * Evaluate the constraint.
 * @Param:
 * 	 x: Beamlet weight.
 * 	 g: Output parameter, holds the constraints value at x.
 * 	@author:
 * 		Paras Babu Tiwari
 */

void StepIIImpl::evalg(const double *x, double *g, int& total) {

#ifdef stp2
	step2->evalg(x, g, total);
	evalNewConst(x, g, total);
	assert(total == this->getNumConstraint());
#else
	evalNewConst(x, g, total);
	assert(total == this->getNumConstraint());
#endif

}
/**
 * yi^(alpha)+1/(1-alpha)*sum{ j in 1 to |Vi|} pji^(alpha)<=Mialpha^{max}.
 * The # of constraints equal to the # of stepII organs.
 *	@return the # of  constraints
 *	@Author: Paras Babu Tiwari
 */
int StepIIImpl::getNumNewConst() {
	int numStepII = mtlb->getNumStepIIOar();
	return numStepII;
}
/**
 * yi^(alpha)+1/(1-alpha)*sum{ j in 1 to |Vi|} pji^(alpha)<=Mialpha^{max}.
 * The # of jacobian entries because of y equals to the # of organs in step II because yi qppears for each i in RII.
 * The # of jacobian entries because of pji equals to the total number of voxels in the step II organs.
 *	@return the # of  Jacobian entries
 *	@Author: Paras Babu Tiwari
 */
int StepIIImpl::getNumNewConstJac() {
	int numStepII = mtlb->getNumStepIIOar();
	int numVox = mtlb->getTotalStepIIVoxels();
	return numStepII + numVox;
}
/**
 * Evaluate the constraint yi^(alpha)+1/(1-alpha)*sum{ j in 1 to |Vi|} pji^(alpha)<=Mialpha^{max}.
 * Input:
 *		x: Beamlet weight
 *		total: index in the constraint array.
 *		g: Constraint array(output variable).
 *	@Author: Paras Babu Tiwari
 */
void StepIIImpl::evalNewConst(const double *x, double *g, int& total) {

	double sm = 0;

	assert(x != NULL);
	int YINDEX = step2->yindex();

	int PINDEX = step2->pindex();

	double pjsm = 0;

	int k = 0;
	double **alpha = mtlb->getAlpha();
	//double *alpha = mtlb->getAlpha();
	int *numVoxel = mtlb->getNumVoxel();
	for (int i = 0; i < mtlb->getNumStepIIOar(); i++) {
		sm += x[YINDEX + i];
		int id = mtlb->getStepIId(i);
		int numVox = numVoxel[id];

		int index = 0;
		for (int j = 0; j < numVox; j++) {
			index = PINDEX + k;

			assert(index < getNumVar());
			pjsm += x[index];
			k++;
		}
		double factr = (1 - alpha[i][0] / 100) * numVox;
		double f = 1.0 / factr;
		pjsm = pjsm * f;
		sm = sm + pjsm;
		g[total] = sm;
		total++;
		sm = 0;

	}

}
/**
 * Evaluate the Jacobian of yi^(alpha)+1/(1-alpha)*sum{ j in 1 to |Vi|} pji^(alpha)<=Mialpha^{max}.
 * The Jacobian of the constraint is same as the gradient of the objective function in the second step.
 * The function F(x) = yi(alpha) + 1/(1-alpha)*|vi| sum{j=1 to |vi|} pji(alpha).
 * The Jacobian of a function is given by: \/f(x) = (df/dx1, df/dx2,df/dx3...df/dxn df/dy1 df/dy2..df/dp1 df/dp2...df/dz1 df/dz2..)
 * The jacobian w.r.to x's are zero as there is no x term in the objective function.
 * The jacobian w.r.to y's equals to one as y's terms are linear.
 * The jacobian w.r.to p's equals to 1/(1-alpha)*|vi| because p's are also linear.
 * The gradient w.r.to z's are zero as there is no z term in the objective function.
 */
void StepIIImpl::handleNewConst(int *iRow, int *iCol, double *value,
		const double *x, int &total, int &l) {

	double factor;
	int YINDEX = step2->yindex();
	int PINDEX = step2->pindex();
	int index = 0;
	int *stepIId = mtlb->getStepIId();
	int *numVoxels = mtlb->getNumVoxel();
	int numStepIIOar = mtlb->getNumStepIIOar();

	if (value == NULL) {
		for (int i = 0; i < numStepIIOar; i++) {
			iRow[total] = l;
			iCol[total] = YINDEX + i;
			total++;
			int id = stepIId[i];
			int numVox = numVoxels[id];

			for (int j = 0; j < numVox; j++) {
				index = PINDEX + j;
				assert(index < getNumVar());

				iRow[total] = l;
				iCol[total] = index;

				total++;

			}
			l++;
		}
	} else {
		value[total] = 1;
		total++;
		double **alpha = mtlb->getAlpha();
		//double *alpha = mtlb->getAlpha();
		for (int i = 0; i < numStepIIOar; i++) {
			int id = stepIId[i];
			int numVox = numVoxels[id];
			factor = (1 - alpha[i][0] / 100) * numVox;
			factor = 1.0 / factor;
			for (int j = 0; j < numVox; j++) {

				value[total] = factor;
				//printf("total:%d\n",total);

				//value[total] = 1;

				total++;
			}
		}
	}
}
void StepIIImpl::getJacobian(int *iRow, int *iCol, double *value,
		const double *x)
{
	int total = 0;
	int l = 0;
	this->getJacobian(iRow,iCol,value,x,total,l);
	assert(total == getNumJac());
}
/*
 * Return either the sparsity structure of the Jacobian of the constraints, or the values for the Jacobian of the constraints at the point x.
 * The detail about Jacobian matrix can be found at http://en.wikipedia.org/wiki/Jacobian_matrix_and_determinant
 * @Input:
 * 	iRow: Output Parameter, holds the row # of non-zero entry in the Jacobian matrix
 * 	iCol: Output Parameter, holds the column # of non-zero entry in the Jacobian matrix.
 * 	value: Output Parameter, holds the value of non-zero entry of the Jacobian matrix. The value[i] corresponds to iRow[i] and iCol[i] entry in the matrix.
 * 	x: Input Parameter: The solution vector found during the optimization.
 *  @Author: Paras Babu Tiwari
 *
 */

void StepIIImpl::getJacobian(int *iRow, int *iCol, double *value,
		const double *x, int& total, int& l){

#ifdef stp2
	step2->getJacobian(iRow, iCol, value, x, total, l);
	this->handleNewConst(iRow, iCol, value, x, total, l);
	assert(total == this->getNumJac());
#else
	this->handleNewConst(iRow, iCol, value, x, total, l);
	assert(total == this->getNumJac());
#endif

}
/*
 * Print the dose
 */
void StepIIImpl::printDose() {

	printDose(w3);
}
void StepIIImpl::printDose(PrioritizedMain::Vector *w) {

	int *numvoxel = mtlb->getNumVoxel();
	int start, end;
	start = end = 0;
	int id = 0;
	double v;
	int *stepIIIid = mtlb->getStepIIId();
	//Matrices *D = new Matrices(mtlb->getTotalStepIIIVoxels(), 1);
	int num3rdOar = mtlb->getNumStepIIIOar();
	PrioritizedMain::Vector **D = new PrioritizedMain::Vector*[num3rdOar];
	mtlb->getDose(D, w, stepIIIid, mtlb->getNumStepIIIOar());
//	double *metrics = mtlb->getMetrics(step2->Step1()->getW1(), step2->getW2(),
//			w3);
//	int k = 0;
//	for (int i = 0; i < mtlb->getNumTargets(); i++) {
//		printf("D95 of %d:%lf\n", i, metrics[i]);
//		k++;
//	}
//	for (int i = 0; i < mtlb->getNumStepIIOar(); i++) {
//		printf("moh of %d:%lf\n", i, metrics[k]);
//		k++;
//	}
//	printf("Step3 Dose\n");
	for (int i = 0; i < mtlb->getNumStepIIIOar(); i++) {
		id = stepIIIid[i];
		v = D[i]->getMin();
		printf("Dose of %d\n", id);
		printf("Minimum Dose:%lf\n", v);
		v = D[i]->getMax();
		printf("Maximum Dose:%lf\n", v);
		v = D[i]->average();
		printf("Average Dose:%lf\n", v);
		start = end;
	}
	for(int i=0;i<num3rdOar;i++)
	{
		delete D[i];
	}
	delete[] D;

}
void StepIIImpl::setW(const double *x,const double *z_l,const double *z_u,const double *lambda) {
	w3 = new PrioritizedMain::Vector(x, NUMBEAMLET);
	int nv = getNumVar();
	xn = new double[nv];
	memcpy(xn,x,sizeof(double)*nv);
}
bool StepIIImpl::getHessian(int *iRow, int *iCol, double *value,
		double obj_factor, const double *lambda,int& total,bool step4) {
	return step2->getHessian(iRow, iCol, value, obj_factor, lambda,total,step4);
}
/**
 * Get the Hessian. We have stored the hessian in matlab index format, so need to substract one.
 * Hessian of this step is same as that of the step II
 */
bool StepIIImpl::getHessian(int *iRow, int *iCol, double *value,
		double obj_factor, const double *lambda) {
#ifdef stp2
	return step2->getHessian(iRow, iCol, value, obj_factor, lambda);
#else
	return true;
#endif
}
/*
 * Get number of element in the hessian matrix.
 * @return number of non-zero element in the hessian Matrices
 */
int StepIIImpl::getNumHess() {

#ifdef stp2
	return step2->getNumHess();
#else
	return 0;
#endif
}
