#include "gtest/gtest.h"

void *malloc(size_t);


TEST (TestMalloc, Functional){
	struct stat stat_buf_1;
	struct stat stat_buf_2;
	struct stat stat_buf_3;

	string small = "small.txt";
	string medium = "medium.txt";
	string large = "large.txt";


	int rc1 = stat(small.c_str(),&stat_buf_1);
	int rc2 = stat(medium.c_str(),&stat_buf_2);
	int rc3 = stat(large.c_str(),&stat_buf_3);

	size_t sm_sz; //= need to figure out
	size_t md_sz; //= how to cat / find  
	size_t lg_sz; //= file size to call malloc

	ASSERT_EQ(stat_buf_1.size+1,malloc(sm_sz));
	ASSERT_EQ(stat_buf_2.size+1,malloc(md_sz));
	ASSERT_EQ(stat_buf_3.size+1,malloc(lg_sz));
}


TEST (TestMalloc, TestSmall){
	size_t size = 5;
	ASSERT_EQ(size+1,malloc(size));
	ASSERT_EQ(size*2+1,malloc(size*2));
	ASSERT_EQ(size*3+1,malloc(size*3));
}


TEST (TestMalloc, TestMedium){
	size_t size = 50;
	ASSERT_EQ(size+1,malloc(size));
	ASSERT_EQ(size*2+1,malloc(size*2));
	ASSERT_EQ(size*3+1,malloc(size*3));
}


TEST (TestMalloc, TestLarge){
	size_t size = 500;
	ASSERT_EQ(size+1,malloc(size));
	ASSERT_EQ(size*2+1,malloc(size*2));
	ASSERT_EQ(size*3+1,malloc(size*3));

}

int main(int argc, char **argv){
	::testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();	
}

