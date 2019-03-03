Compiler settings - 
-std=c11 -O2 -g3 -pedantic -pedantic-errors -Wall -Wextra -Wconversion -c -fmessage-length=0 -fPIC

==================================================================================================================================================================

Reference:
1. https://en.wikipedia.org/wiki/X.690
2. Information technology – ASN.1 encoding rules: Specification of Basic Encoding Rules (BER), Canonical Encoding Rules (CER) and Distinguished Encoding Rules (DER) 

==================================================================================================================================================================


PHASE 1: TLV encoder and decoder V0.1
- TLV encoder for universal tag (integer and UTF8 string) numbers and application tag (application class and context specific class) numbers.
- App data layer that abstracts the TLV encoder and decoder from application.
- Definite and indefinite length encoding


PHASE 1: TLV encoder and decoder V0.1.1
- Improved DEMO for (indefinite length) container within (indefinite length) container.
- Improved Error handing for tlv_app_data_ec.h and tlv_ec.h

Check API description below for details

PHASE 2 - 1: TLV encoder and decoder V0.1.9 
- TLV parser support for univeral (integer and UTF8 string) and application tag (application class and context specific class) numbers.
- Including app data layer abstraction for the TLV decoding.
- Function to parse TLV data buffer (first found TLV object is parsed.)
- The parse_tlv_object() find the first valid tag and decodes the TLV encoded data, else returns error tag not found or bad TLV data buffer
- If the tag found of a definite length then the application gets TLV object parsed with value.
NOTE: If the tag found is of indefinite length (container type), then the caller gets the container TLV object of indefinite length and the caller application should call tlv_search_tag() api to parse a child TLV 0bjects in the container TLV object. Application can also parse the entire container TLV object of indefinite length with all its child TLV objects by calling tlv_search_tag() api.
- NO IMPACT TO THE API

PHASE 2 - 1: TLV encoder and decoder V0.2
- search and parse universal and indefinite lenght application container objects
- Optimization of the tlv_object.
- NO IMPACT TO THE API

< NOTE: There will be minor updated as 0.2.1 and 0.2.2 with improved code and testing. >

Upcoming updates:
PHASE 3: TLV encoder and decoder V0.3 (to be planned)
- TLV encoder and decoder support for univeral tag numbers - 'TAG_REAL', TAG_SET_OF and TAG_SEQUENCE_OF.
- UPDATES WILL NOT IMPACT THE API

PHASE 4: TLV encoder and decoder V0.4 (to be planned)
- Integration with serial driver and ring buffer.


==================================================================================================================================================================

TLV encoder and Decoder V0.1

INTERFACE:

Interface Option 1:
- TLV API can be used indirectly by including tlv_app_data_api.h, tlv_tag_numbers.h and tlv_port.h in application code.
- Including tlv_app_data_api.h abstracts the application from TLV encoder and decoder.
- The tlv app data layer maps the application data to tlv objects.
- The tlv app data layer enables application to seamlessly transmit and receive data with tag numbers, also natively access the transmitted and received data in application variables.

Interface Option 2:
- TLV API can be used directly by including tlv_api.h, tlv_tag_numbers.h and tlv_port.h in application code.
- Including tlv_api.h, application will have to keep a mapping of application data to TLV objects.
- Application will have to read and write data to and from the TLV value field.

tlv_app_data_api.h and tlv_api.h:
- Application should either include tlv_app_data_api.h OR tlv_api.h interface [NOT BOTH].
- It is possible to enable or disable the error assert (ERROR_ASSERT) during api use (by default the error assert is enabled).
- It is possible to enable or disable the error assert with abort (ERROR_ASSERT_ABORT) during api use. Error assert should be enabled for error assert with abort to work (by default error assert with abort is disabled).
- It is possible to enable or disable the error checking (DISABLE_ERROR_CHECK) once code is stabilized (by default error checking is enabled).

tlv_tag_numbers.h:
- It is possible to configure application specific tag numbers in the tlv_tag_numbers.h header file.
- Universal tag numbers range <0 to 30>. (CAUTON: Application should not modify the universal tag numbers in range 0 to 30.)
- Application tag numbers are in range <32 to 254> (127). (CAUTION: Application should not have tag numbers 0 to 31 to avoid conflict with universal tag numbers.)
- Default application class tag numbers range <32 to 158>. (The application class tag range can be modified but should be always within application tag number range.)
- Default context specific class tag numbers range <159 to 254>. (The context specific class tag range can be modified but should be always within application tag number range and should not conflict with application class tag numbers.)
- Max value length allowed is 65535. (Additional value length limits can be set for supported universal tags, e.g. - MAX_INTEGER_BYTES, MAX_UTF8STRING_LENGTH)

tlv_port.h
- All data types definitions required by the tlv encoder and decoder API are defined in this file.
- The data types definition should be modified as per the platform used.
- The standard library api are not abstracted. Standard library api abstraction will help portability.


DESIGN - IMPLEMENTATION:
tlv_app_data.h
- All definitions and api function declarations required for the tlv app data layer.
- These functions abstract the TLV encoder and decoder for the application.
- Application code should not #include this file, instead tlv_app_data_api.h should be included to use the tlv app data layer abstraction for TLV encoder and decoder.
- Application can choose to directly access TLV encoder and decoder by including tlv_api.h

tlv_app_data_encoder.c
- All API definitions required for the app data layer to abstract TLV encoder.

tlv_app_data_parser.c
-  All API definitions required for the app data layer to abstract parsing (decoding) of TLV encoded objects.

tlv_app_data_ec.h
- Error checking for all tlv app data layer functions.

tlv.h
- All definitions and api function declarations required for direct use of the TLV encoder and decoder.
- Application code should not #include this file, instead tlv_api.h should be included to use the TLV encoder and decoder directly in the application.

tlv.c
- All tlv app data layer functions and variables are defined in this file.

tlv_ec.c
- Error checking for all tlv encoder and decoder functions.

tlv_universal_class.h
- This is internal component of TLV encoder and decoder.
- Has all definitions and api function declarations required for encoding and decoding universal TLV objects.
- Application code should not #include this file, instead tlv_api.h should be included to use the TLV encoder and decoder directly in the application.

tlv_universal_class_encoder.c
- All API definitions required for the TLV universal tags encoder.

tlv_application_class.h
- This is internal component of TLV encoder and decoder.
- Has all definitions and api function declarations required for encoding and decoding application TLV objects.
- Application code should not #include this file, instead tlv_api.h should be included to use the TLV encoder and decoder directly in the application.

tlv_application_class_encoder.c
- All API definitions required for the TLV application tags encoder.

tlv_object_parser.h
- All definitions and api function declarations required for the TLV application and universal tags decoder.

tlv_object_parser.c
- All API definitions required for the TLV application and universal tags decoder.

tlv_definition.h
- All definitions and declarations required for TLV encoding / decoding and associated constraints.

tlv_definition.c
- All definitions required for TLV encoding and decoding and associated constraints.

error_assert.h
- 'assert' macros are defined in this header file.
- 'assert' can be with or without abort().
 
error_assert.c
- All error assert functions and are defined in this file.

The code is written such that it should be easy to use in multithreaded environment by protecting critical sections. Critical section is unified (not distributed) in the functions and is kept as small as possible.

APPLICATION - DEMO (using TLV encoder and decoder API)
tlv_app_main.c
- THIS IS A DEMO CODE JUST TO EXCERCISE / DEMONSTRATE THE TLV ENCODER AND DECODER API (may not follow all coding standards)



