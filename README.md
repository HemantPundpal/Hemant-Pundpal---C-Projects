# C-Projects
C Project Code

Ring Buffer API can be used by including ring_buffer_api.h and ring_buffer_port.h files.

INTERFACE:
ring_buffer_api.h:
- All definitions and api function declarations required by the application to use the ring buffer is included in this file.
- Application code should #include this file to use the ring buffer api.
- It is possible to enable or disable the error assert (ERROR_ASSERT) during api use (by default the error assert is enabled).
- It is possible to enable or disable to error assert with abort (ERROR_ASSERT_ABORT) during api use. Error assert should be enabled for error assert with abort to work (by default error assert with abort is disabled).
- It is possible to enable or disable the error checking (DISABLE_ERROR_CHECK) once code is stablized (by default error checking is enabled).
- It is possible to configure the ring buffer size (RINGBUFFER_SIZE_MAX) (by default size is set to 1024).
- It is possible to configure the max number of ring buffer (RINGBUFFER_MAX_COUNT) that can be created (by default max count is set to 30).

ring_buffer_port.h:
- All data types definations required by the ring buffer API are defined in this file.
- The data types defination should be modified as per the platform used.


DESIGN - IMPLEMENTATION
Source Code also contains the following files:
ring_buffer.h
- All definition and function declarations required by the ring buffer module are in this file.
- Application code should not #include this file to use the ring buffer api.

ring_buffer.c
- All Ring Buffer functions and variables are defined in this file.

error_assert.h
 - 'assert' macros are defined in this header file.
 - 'assert' can be with or without abort().
 
 error_assert.c
 - All error assert functions and are defined in this file.
 
 
 APPLICATION - DEMO (using Ring Buffer API)
 ring_buffer_main.c
 - THIS IS A DEMO CODE JUST TO EXCERCISE / DEMONSTRATE THE RING BUFFER API
