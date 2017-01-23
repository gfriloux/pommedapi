#include <Test.h>

typedef enum _Expect_Level
{
   EXPECT_SUCCESS,
   EXPECT_WARNING,
   EXPECT_DANGER
} Expect_Level;

int expect_init(void);
int expect_shutdown(void);

Expect_Level expect_test(Test *t);
Expect_Level expect_test_http_code(Test *t);
Expect_Level expect_test_latency(Test *t);
