#ifndef __DISPLAY_TEST_H__
#define __DISPLAY_TEST_H__

#define DBG(x...) printf(x)
#define TRACE_IN  printf("\n%s(+):", __FUNCTION__)
#define TRACE_OUT /* printf("\n%s(-):", __FUNCTION__)*/

#ifndef SUCCESS
#define SUCCESS 0
#define FAILURE -1
#endif

#endif /* __DISPLAY_TEST_H__ */
