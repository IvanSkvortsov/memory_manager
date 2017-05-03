#include"log.info.head.h"
#undef  __log_info__
#if !defined( __log_info_forced__ ) || defined( __log_info_forced_dummy__ )
#define __log_info__( address ) __log_info_dummy__( address )
#elif defined( __log_info_forced_start__ )
#define __log_info__( address ) __log_info_start__( address )
#elif defined( __log_info_forced_short__ )
#define __log_info__( address ) __log_info_short__( address )
#else
#error in .._forced_.. macro
#endif
