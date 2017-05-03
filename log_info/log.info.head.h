#include"log.info.h"
#if defined( __log_info_forced__ )
  #if defined( __log_info_forced_dummy__ ) && defined( __log_info_forced_start__ ) && defined( __log_info_forced_short__ )
  #error invalid .._forced_.. defines: one should define only 1 of .._forced_.. defines
  #endif
#endif
