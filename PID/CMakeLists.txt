set (sources
  PID_v1.h 
  PID_v1.cpp
)

add_library( PID_v1 ${sources} )
target_link_libraries(PID_v1 mbed-os)

