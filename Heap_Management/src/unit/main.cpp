#include <gtest/gtest.h>
#include <string>
#include <assert.h>

using namespace std;

int main(){
	struct stat stat_buf_1;
	struct stat stat_buf_2;
	struct stat stat_buf_3;

	string small = "small.txt";
	string medium = "medium.txt";
	string large = "large.txt";

	int rc1 = stat(small.c_str(),&stat_buf_1);
	int rc2 = stat(medium.c_str(),&stat_buf_2);
	int rc3 = stat(large.c_str(),&stat_buf_3);

	size_t sm_sz = stat_buf_1.st_size;
	size_t md_sz = stat_buf_2.st_size;  
	size_t lg_sz = stat_buf_3.st_size;

	void *m_rtr_1 = malloc(sm_sz);
 	void *m_rtr_2 = malloc(md_sz);
        void *m_rtr_3 = malloc(lg_sz);	

	assert((size_t)sm_sz+1==(size_t)m_rtr_1);
	fprintf(stdout,"Test 1 Passed!");
	assert((size_t)md_sz+1==(size_t)m_rtr_2);
	fprintf(stdout,"Test 2 Passed!");
	assert((size_t)lg_sz+1==(size_t)m_rtr_3);
	fprintf(stdout,"Test 3 Passed!");


	free(m_rtr_1);
	free(m_rtr_2);
	free(m_rtr_3);
}

