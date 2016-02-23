#ifndef __SEED_TEST_H
#define __SEED_TEST_H

#define $TEST(V) static void test_##V () { char* _test_result; char* _test_name = "" #V "";
#define $START
#define $END goto success;success:printf("pass: %s\n", _test_name);return;fail:printf("fail: %s\n\t%s\n", _test_name, _test_result);return;}
#define $ERROR(M) \
	do{ \
		_test_result = M; \
		goto fail; \
	}while(0); \

#endif
