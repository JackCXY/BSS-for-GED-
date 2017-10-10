#include "BSED.h"
int main(int argc, char *argv[])
{
	if(argc < 7) {cout << "database n query m bound w" << endl; exit(0);}
	string db = argv[1]; 
	int totalGraph =  atoi(argv[2]);
	string query = argv[3];
	int totalQuery = atoi(argv[4]);
	const int ub = atoi(argv[5]);
	int width = atoi(argv[6]);
	const int sorted = 1;

	const char *db_out = sorted ? "./ged_db" : db.c_str(); 
	const char *query_out = sorted ? "./ged_query" : query.c_str();

	if(sorted){
			graph::reOrderGraphs(db.c_str(), db_out, totalGraph);
			graph::reOrderGraphs(query.c_str(), query_out, totalQuery);
			//cout << "preprocessing graph order done" << endl;
	}
	
	vector<graph> queryDB = graph::readGraphMemory(query_out, totalQuery);
	vector<graph> graphDB = graph::readGraphMemory(db_out, totalGraph);
	if(sorted){ std::remove(db_out); std::remove(query_out);}
	int i = 0, j = 0, bound = ub;	
	int sum = 0; 
	struct timeval start,end; 
	float timeuse; 
	gettimeofday(&start, NULL); 

	for(; i < graphDB.size();i++)
	{
		graph g = graphDB[i];
		g.starSet(graphStar);
		for(j = 0; j < queryDB.size(); j++)
		{
			graph q = queryDB[j]; 
			q.starSet(queryStar);
			BSEditDistance ed(width);
			int ged = ed.getEditDistance(q, g, queryStar, graphStar, bound);
			if(ged > -1) 
			{
				cout << "GED(" << i << "," << j << ")" << " = " << ged << endl; 
				sum++;
			}
		}
		FLAG = true;
	}
	cout << "total expand node " << totalExpandNode << endl;
	cout << "initFilter and total results " << initFilter  << " " << sum << endl;
	gettimeofday(&end, NULL); 
	timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec; 
	timeuse = timeuse * 1.0 / 1000000; 
	cout << "Total graph edit distance computation time " << timeuse << endl;

	return 0;
}
