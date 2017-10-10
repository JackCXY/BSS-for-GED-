#ifndef _HUNGARIAN
#define _HUNGARIAN
#include "stdafx.h"
#define HUNGARIAN_MODE_MINIMIZE_COST 0
#define HUNGARIAN_MODE_MAXIMIZE_UTIL 1

#define HUNGARIAN_NOT_ASSIGNED 0 
#define HUNGARIAN_ASSIGNED 1

#define INF (100000.0)
#define verbose (0)
#define EPASON 0.000001
#define hungarian_test_alloc(X) do {if ((void *)(X) == NULL) fprintf(stderr, "Out of memory in %s, (%s, line %d).\n", __FUNCTION__, __FILE__, __LINE__); } while (0)
//nice, ~~haha~~

class hungarian
{
public:
	int rows;
	int cols;
	vector<vector<int> > assignment;
	vector<vector<double> >initCostMatix;
	double maxCost;
	int mode; 
public:
	//sloving the minimal cost problem
	//initCostMatrix and cost must be init
	hungarian()
	{
		rows = cols = 0;
		maxCost = 0.0;
		mode = HUNGARIAN_MODE_MINIMIZE_COST; 
	}
	void init(vector<vector<double> > matrix, int r, int c, int mode)
	{
		int i, j, org_cols, org_rows;
		maxCost = 0.0;

		org_cols = c;
		org_rows = r;

		rows = c < r ? r : c;
		cols = rows;
		this->mode = mode;

		vector<double> temp(rows, 0.0); 
		initCostMatix.resize(rows, temp);
		vector<int> tempAss(rows, 0);
		assignment.resize(rows, tempAss);
		//here, init has some problem
		for (i = 0; i <rows; i++)
		{
			for (j = 0; j < cols; j++)
			{
				initCostMatix[i][j] = (i < org_rows && j < org_cols) ? matrix[i][j] : 0.0;
				assignment[i][j] = 0;
				if (maxCost < initCostMatix[i][j])
					maxCost = initCostMatix[i][j];
			}
		}
		if (this->mode == HUNGARIAN_MODE_MAXIMIZE_UTIL) 
		{
			for (i = 0; i < rows; i++) 
			{
				for (j = 0; j < cols; j++)
				{
					initCostMatix[i][j] = maxCost - initCostMatix[i][j]; // has nothing with the origin matrix
				}
			}
		}
		else if (this->mode == HUNGARIAN_MODE_MINIMIZE_COST)
		{
			// nothing to do
		}
		else
			fprintf(stderr, "%s: unknown mode. Mode was set to HUNGARIAN_MODE_MINIMIZE_COST !\n", __FUNCTION__);
	}
	void printAssignment()
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
				cout << assignment[i][j] << "\t";
			cout << endl;
		}
	}
	void slove(vector<vector<double >> costMatrix)
	{
		int i, j, m, n, k, l, t, q, unmatched;
		double s, cost;
		int* col_mate;
		int* row_mate;
		int* parent_row;
		int* unchosen_row;
		double* row_dec;
		double* col_inc;
		double* slack;
		int* slack_row;

		cost = 0.0;
		m = rows;
		n = cols;

		col_mate = (int*)calloc(rows, sizeof(int));
		hungarian_test_alloc(col_mate);
		unchosen_row = (int*)calloc(rows, sizeof(int));
		hungarian_test_alloc(unchosen_row);
		row_dec = (double*)calloc(rows, sizeof(double));
		hungarian_test_alloc(row_dec);
		slack_row = (int*)calloc(rows, sizeof(int));
		hungarian_test_alloc(slack_row);

		row_mate = (int*)calloc(cols, sizeof(int));
		hungarian_test_alloc(row_mate);
		parent_row = (int*)calloc(cols, sizeof(int));
		hungarian_test_alloc(parent_row);
		col_inc = (double*)calloc(cols, sizeof(double));
		hungarian_test_alloc(col_inc);
		slack = (double*)calloc(cols, sizeof(double));
		hungarian_test_alloc(slack);

		for (i = 0; i < rows; i++) {
			col_mate[i] = 0;
			unchosen_row[i] = 0;
			row_dec[i] = 0.0;
			slack_row[i] = 0;
		}
		for (j = 0; j < cols; j++) {
			row_mate[j] = 0;
			parent_row[j] = 0;
			col_inc[j] = 0.0;
			slack[j] = 0.0;
		}

		for (i = 0; i < rows; ++i)
			for (j = 0; j < cols; ++j)
				this->assignment[i][j] = HUNGARIAN_NOT_ASSIGNED;

		// Begin subtract column minima in order to start with lots of zeroes 12
		if (verbose)
			fprintf(stderr, "Using heuristic\n");
		for (l = 0; l < n; l++)
		{
			s = costMatrix[0][l];
			for (k = 1; k < m; k++)
				if (costMatrix[k][l] < s)
					s = costMatrix[k][l];
			cost += s;
			if (s > EPASON)
				for (k = 0; k < m; k++)
					costMatrix[k][l] -= s;
		}
		// End subtract column minima in order to start with lots of zeroes 12

		// Begin initial state 16
		t = 0;
		for (l = 0; l < n; l++)
		{
			row_mate[l] = -1;
			parent_row[l] = -1;
			col_inc[l] = 0;
			slack[l] = INF;
		}
		for (k = 0; k < m; k++)
		{
			s = costMatrix[k][0];
			for (l = 1; l < n; l++)
				if (costMatrix[k][l] < s)
					s = costMatrix[k][l];
			row_dec[k] = s;
			for (l = 0; l<n; l++)
				if (fabs(s - costMatrix[k][l]) < EPASON && row_mate[l] < 0)
				{
					col_mate[k] = l;
					row_mate[l] = k;
					if (verbose)
						fprintf(stderr, "matching col %d==row %d\n", l, k);
					goto row_done;
				}
			col_mate[k] = -1;
			if (verbose)
				fprintf(stderr, "node %d: unmatched row %d\n", t, k);
			unchosen_row[t++] = k;
		row_done:
			;
		}
		// End initial state 16

		// Begin Hungarian algorithm 18
		if (t == 0)
			goto done;
		unmatched = t;
		while (1)
		{
			if (verbose)
				fprintf(stderr, "Matched %d rows.\n", m - t);
			q = 0;
			while (1)
			{
				while (q < t)
				{
					// Begin explore node q of the forest 19
					{
						k = unchosen_row[q];
						s = row_dec[k];
						for (l = 0; l<n; l++)
							if (slack[l] > 0.0)
							{
								double del;
								del = costMatrix[k][l] - s + col_inc[l];
								if (del < slack[l])
								{
									if (del < EPASON)
									{
										if (row_mate[l] < 0)
											goto breakthru;
										slack[l] = 0.0;
										parent_row[l] = k;
										if (verbose)
											fprintf(stderr, "node %d: row %d==col %d--row %d\n",
											t, row_mate[l], l, k);
										unchosen_row[t++] = row_mate[l];
									}
									else
									{
										slack[l] = del;
										slack_row[l] = k;
									}
								}
							}
					}
					// End explore node q of the forest 19
					q++;
				}

				// Begin introduce a new zero into the matrix 21
				s = INF;
				for (l = 0; l<n; l++)
					if (slack[l] > EPASON && slack[l] < s)
						s = slack[l];
				for (q = 0; q < t; q++)
					row_dec[unchosen_row[q]] += s;
				for (l = 0; l < n; l++)
					if (slack[l] > EPASON)
					{
						slack[l] -= s;
						if (slack[l] < EPASON)
						{
							// Begin look at a new zero 22
							k = slack_row[l];
							if (verbose)
								fprintf(stderr,
								"Decreasing uncovered elements by %d produces zero at [%d,%d]\n",
								s, k, l);
							if (row_mate[l] < 0)
							{
								for (j = l + 1; j < n; j++)
									if (slack[j] < EPASON)
										col_inc[j] += s;
								goto breakthru;
							}
							else
							{
								parent_row[l] = k;
								if (verbose)
									fprintf(stderr, "node %d: row %d==col %d--row %d\n", t, row_mate[l], l, k);
								unchosen_row[t++] = row_mate[l];
							}
							// End look at a new zero 22
						}
					}
					else
						col_inc[l] += s;
				// End introduce a new zero into the matrix 21
			}
		breakthru:
			// Begin update the matching 20
			if (verbose)
				fprintf(stderr, "Breakthrough at node %d of %d!\n", q, t);
			while (1)
			{
				j = col_mate[k];
				col_mate[k] = l;
				row_mate[l] = k;
				if (verbose)
					fprintf(stderr, "rematching col %d==row %d\n", l, k);
				if (j < 0)
					break;
				k = parent_row[j];
				l = j;
			}
			// End update the matching 20
			if (--unmatched == 0)
				goto done;
			// Begin get ready for another stage 17
			t = 0;
			for (l = 0; l<n; l++)
			{
				parent_row[l] = -1;
				slack[l] = INF;
			}
			for (k = 0; k<m; k++)
				if (col_mate[k] < 0)
				{
					if (verbose)
						fprintf(stderr, "node %d: unmatched row %d\n", t, k);
					unchosen_row[t++] = k;
				}
			// End get ready for another stage 17
		}
	done:

		// Begin doublecheck the solution 23
		for (k = 0; k<m; k++)
			for (l = 0; l < n; l++)
				if (costMatrix[k][l] < row_dec[k] - col_inc[l])
					exit(0);
		for (k = 0; k<m; k++)
		{
			l = col_mate[k];
			if (l < 0 || fabs(costMatrix[k][l] - row_dec[k] + col_inc[l]) > EPASON)
				exit(0);
		}
		k = 0;
		for (l = 0; l < n; l++)
			if (col_inc[l] > EPASON)
				k++;
		if (k > m)
			exit(0);
		// End doublecheck the solution 23
		// End Hungarian algorithm 18

		for (i = 0; i < m; ++i)
		{
			this->assignment[i][col_mate[i]] = HUNGARIAN_ASSIGNED;
			/*TRACE("%d - %d\n", i, col_mate[i]);*/
		}
		for (k = 0; k < m; ++k)
		{
			for (l = 0; l < n; ++l)
			{
				/*TRACE("%d ",costMatrix[k][l]-row_dec[k]+col_inc[l]);*/
				costMatrix[k][l] = costMatrix[k][l] - row_dec[k] + col_inc[l];
			}
			/*TRACE("\n");*/
		}
		for (i = 0; i<m; i++)
			cost += row_dec[i];
		for (i = 0; i<n; i++)
			cost -= col_inc[i];
		if (verbose)
			fprintf(stderr, "Cost is %d\n", cost);


		free(slack);
		free(col_inc);
		free(parent_row);
		free(row_mate);
		free(slack_row);
		free(row_dec);
		free(unchosen_row);
		free(col_mate);
	}
	double costValue()
	{
		double c = 0.0;
		for (int i = 0; i < initCostMatix.size(); i++)
			for (int j = 0; j < initCostMatix[i].size(); j++)
				if (assignment[i][j] != 0)
					c += initCostMatix[i][j];
		if (mode == HUNGARIAN_MODE_MINIMIZE_COST)
			return c;
		else
			return rows  * maxCost - c;
	}
	void printMatrix(vector<vector<double > >matrix)
	{
		for (int i = 0; i < matrix.size(); i++)
		{
			for (int j = 0; j < matrix[i].size(); j++)
				cout << matrix[i][j] << "\t";
			cout << endl;
		}
	}

};
#endif
