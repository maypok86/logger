#if !defined(LOG)
#error You need to define LOG macro
#else

LOG(debug, DEBUG)
LOG(info, INFO)
LOG(warning, WARNING)
LOG(error, ERROR)

#undef LOG

#endif