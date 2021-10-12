#include "MQTTInterface.h"
#include "stm32f4xx_hal.h"

#include MBEDTLS_CONFIG_FILE
#include "mbedtls/platform.h"

#include <string.h>
#include "lwip.h"
#include "lwip/api.h"
#include "lwip/sockets.h"

#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
#include "mbedtls/memory_buffer_alloc.h"
#endif
#include "mbedtls/net_sockets.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/certs.h"

#include <string.h>

#define DEBUG_LEVEL 1

//Amazon ECC 256 certificate
const char mbedtls_aws_root_certificate[] =
		"-----BEGIN CERTIFICATE-----\r\n"												//radi
		"MIIEkjCCA3qgAwIBAgITBn+USionzfP6wq4rAfkI7rnExjANBgkqhkiG9w0BAQsF\r\n"
		"ADCBmDELMAkGA1UEBhMCVVMxEDAOBgNVBAgTB0FyaXpvbmExEzARBgNVBAcTClNj\r\n"
		"b3R0c2RhbGUxJTAjBgNVBAoTHFN0YXJmaWVsZCBUZWNobm9sb2dpZXMsIEluYy4x\r\n"
		"OzA5BgNVBAMTMlN0YXJmaWVsZCBTZXJ2aWNlcyBSb290IENlcnRpZmljYXRlIEF1\r\n"
		"dGhvcml0eSAtIEcyMB4XDTE1MDUyNTEyMDAwMFoXDTM3MTIzMTAxMDAwMFowOTEL\r\n"
		"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\r\n"
		"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\r\n"
		"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\r\n"
		"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\r\n"
		"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\r\n"
		"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\r\n"			//ovaj je najbolji od testiranih
		"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\r\n"
		"jgSubJrIqg0CAwEAAaOCATEwggEtMA8GA1UdEwEB/wQFMAMBAf8wDgYDVR0PAQH/\r\n"
		"BAQDAgGGMB0GA1UdDgQWBBSEGMyFNOy8DJSULghZnMeyEE4KCDAfBgNVHSMEGDAW\r\n"
		"gBScXwDfqgHXMCs4iKK4bUqc8hGRgzB4BggrBgEFBQcBAQRsMGowLgYIKwYBBQUH\r\n"
		"MAGGImh0dHA6Ly9vY3NwLnJvb3RnMi5hbWF6b250cnVzdC5jb20wOAYIKwYBBQUH\r\n"
		"MAKGLGh0dHA6Ly9jcnQucm9vdGcyLmFtYXpvbnRydXN0LmNvbS9yb290ZzIuY2Vy\r\n"
		"MD0GA1UdHwQ2MDQwMqAwoC6GLGh0dHA6Ly9jcmwucm9vdGcyLmFtYXpvbnRydXN0\r\n"
		"LmNvbS9yb290ZzIuY3JsMBEGA1UdIAQKMAgwBgYEVR0gADANBgkqhkiG9w0BAQsF\r\n"
		"AAOCAQEAYjdCXLwQtT6LLOkMm2xF4gcAevnFWAu5CIw+7bMlPLVvUOTNNWqnkzSW\r\n"
		"MiGpSESrnO09tKpzbeR/FoCJbM8oAxiDR3mjEH4wW6w7sGDgd9QIpuEdfF7Au/ma\r\n"
		"eyKdpwAJfqxGF4PcnCZXmTA5YpaP7dreqsXMGz7KQ2hsVxa81Q4gLv7/wmpdLqBK\r\n"
		"bRRYh5TmOTFffHPLkIhqhBGWJ6bt2YFGpn6jcgAKUj6DiAdjd4lpFw85hdKrCEVN\r\n"
		"0FE6/V1dN2RMfjCyVSRCnTawXZwXgWHxyvkQAiSr6w10kY17RSlQOYiypok1JR4U\r\n"
		"akcjMS9cmvqtmg5iUaQqqcT5NJ0hGA==\r\n"
		"-----END CERTIFICATE-----\r\n";
//		"-----BEGIN CERTIFICATE-----\r\n"											//radi
//		"MIIDxzCCAq+gAwIBAgITBn+USjDPzE90tfUwblTTt74KwzANBgkqhkiG9w0BAQsF\r\n"
//		"ADCBmDELMAkGA1UEBhMCVVMxEDAOBgNVBAgTB0FyaXpvbmExEzARBgNVBAcTClNj\r\n"
//		"b3R0c2RhbGUxJTAjBgNVBAoTHFN0YXJmaWVsZCBUZWNobm9sb2dpZXMsIEluYy4x\r\n"
//		"OzA5BgNVBAMTMlN0YXJmaWVsZCBTZXJ2aWNlcyBSb290IENlcnRpZmljYXRlIEF1\r\n"
//		"dGhvcml0eSAtIEcyMB4XDTE1MDUyNTEyMDAwMFoXDTM3MTIzMTAxMDAwMFowOTEL\r\n"
//		"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\r\n"
//		"b3QgQ0EgMzBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABCmXp8ZBf8ANm+gBG1bG\r\n"
//		"8lKlui2yEujSLtf6ycXYqm0fc4E7O5hrOXwzpcVOho6AF2hiRVd9RFgdszflZwjr\r\n"
//		"Zt6jggExMIIBLTAPBgNVHRMBAf8EBTADAQH/MA4GA1UdDwEB/wQEAwIBhjAdBgNV\r\n"
//		"HQ4EFgQUq7bb1waeN6wwhgeRcMecxBmxeMAwHwYDVR0jBBgwFoAUnF8A36oB1zAr\r\n"
//		"OIiiuG1KnPIRkYMweAYIKwYBBQUHAQEEbDBqMC4GCCsGAQUFBzABhiJodHRwOi8v\r\n"
//		"b2NzcC5yb290ZzIuYW1hem9udHJ1c3QuY29tMDgGCCsGAQUFBzAChixodHRwOi8v\r\n"
//		"Y3J0LnJvb3RnMi5hbWF6b250cnVzdC5jb20vcm9vdGcyLmNlcjA9BgNVHR8ENjA0\r\n"
//		"MDKgMKAuhixodHRwOi8vY3JsLnJvb3RnMi5hbWF6b250cnVzdC5jb20vcm9vdGcy\r\n"
//		"LmNybDARBgNVHSAECjAIMAYGBFUdIAAwDQYJKoZIhvcNAQELBQADggEBAG5Z+hfC\r\n"
//		"ycAuzqKJ4ClKK5VZUqjH4jYSzu3UVOLvHzoYk0rIVqFjjBJwa/L5MreP219CIdIX\r\n"
//		"di3s9at8ZZR+tKsD4T02ZqO/43FQqnSkzF/G+OxYo3malxhuT9j7bNiA9WkCuqVV\r\n"
//		"bUncQt79aEjDKht7viKQnoybiHB6dtWAXMNObcCviQMqTcoV+sQOpKJMvQanxUk+\r\n"
//		"fKQLGKlkpu9zKNr2kWdx874JVpYhDCUzW2RX9TtQ04VT6J0xTEew55OJj02jNxHu\r\n"
//		"Gijg0YLZtWLNWEXkNDkVpZozXbhuTM6GJKhwLn2rmgRgtFTWUDbeq3YE/7NHu+3a\r\n"
//		"LOL51JEnEI+4hac=\r\n"
//		"-----END CERTIFICATE-----\r\n";

//		"-----BEGIN CERTIFICATE-----\r\n"													//radi
//		"MIIBtjCCAVugAwIBAgITBmyf1XSXNmY/Owua2eiedgPySjAKBggqhkjOPQQDAjA5\r\n"
//		"MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6b24g\r\n"
//		"Um9vdCBDQSAzMB4XDTE1MDUyNjAwMDAwMFoXDTQwMDUyNjAwMDAwMFowOTELMAkG\r\n"
//		"A1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJvb3Qg\r\n"
//		"Q0EgMzBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABCmXp8ZBf8ANm+gBG1bG8lKl\r\n"
//		"ui2yEujSLtf6ycXYqm0fc4E7O5hrOXwzpcVOho6AF2hiRVd9RFgdszflZwjrZt6j\r\n"
//		"QjBAMA8GA1UdEwEB/wQFMAMBAf8wDgYDVR0PAQH/BAQDAgGGMB0GA1UdDgQWBBSr\r\n"
//		"ttvXBp43rDCGB5Fwx5zEGbF4wDAKBggqhkjOPQQDAgNJADBGAiEA4IWSoxe3jfkr\r\n"
//		"BqWTrBqYaGFy+uGh0PsceGCmQ5nFuMQCIQCcAu/xlJyzlvnrxir4tiz+OpAUFteM\r\n"
//		"YyRIHN8wfdVoOw==\r\n"
//		"-----END CERTIFICATE-----\r\n";
//		"-----BEGIN CERTIFICATE-----\r\n"													//radi
//		"MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\r\n"
//		"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\r\n"
//		"b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\r\n"
//		"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\r\n"
//		"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\r\n"
//		"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\r\n"
//		"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\r\n"
//		"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\r\n"
//		"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\r\n"
//		"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\r\n"
//		"jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\r\n"
//		"AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\r\n"
//		"A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\r\n"
//		"U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\r\n"
//		"N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\r\n"
//		"o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\r\n"
//		"5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\r\n"
//		"rqXRfboQnoZsG4q5WTP468SQvvG5\r\n"
//		"-----END CERTIFICATE-----\r\n";


//client certificate here
const char mbedtls_client_certificate[] =
		"-----BEGIN CERTIFICATE-----\r\n"
		"MIIDWjCCAkKgAwIBAgIVAIgU8PAYIRnAtNHcwyJGREXzGAQYMA0GCSqGSIb3DQEB\r\n"
		"CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\r\n"
		"IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMTA2MTYwODUx\r\n"
		"NDBaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\r\n"
		"dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDDIqW7+gF6WJ76bPqF\r\n"
		"g9kPf4mu2shyTcBlXA6FsMk8HzA1azd2jSRYyNSZwVFD+YY+tP+2q+vvRpl39XZc\r\n"
		"IG1/Fdj5UmFbjTKgn2DIRlHsuhLr8cebKadTMq2RkNVtX1jv6evaNtB/QF8D30WL\r\n"
		"hfR66HxFGG1JidHLD3X58d/kO4nHd+GsYgWJaNtzrxO1nfydhv4YjmjxFCuUdMrf\r\n"
		"4i7hRgBib8SAlAw5fLfUbW7y0I3X1Zq+vzsHX+7Y56Rufqhbrg6OIBNKWU3TGOPx\r\n"
		"JwWRfSlfOV2vbuvE0v4NaopNhdE1F+xxPiL4UzvmiFogBCFDhBvw2l/gjEcEtPg0\r\n"
		"yAX3AgMBAAGjYDBeMB8GA1UdIwQYMBaAFNWxPylrCKFn9fE0m+5HpwpCor4jMB0G\r\n"
		"A1UdDgQWBBSPz5CZ2lZeGaeVhI/nCXWCk0kz+TAMBgNVHRMBAf8EAjAAMA4GA1Ud\r\n"
		"DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAKStJcOh75uyk1PuL9gBcfMKc\r\n"
		"GMmR7Vah1r8R6E0JsZ+QfosPMbnRxbluvu8J5LxpdCzi67mBfHyCkjS+QlNtCYge\r\n"
		"Im77OGHdtwqvVndJWw4UIQGYzeRdJvuyBhvOB6/AH2FkKOYdmi9JW5y0LNMswmvr\r\n"
		"S8V/s0GQ82iO627M3P77Vq8c/4JhVz7eDY8LShp07+IzSD4dtwOe9akMG4k2k70S\r\n"
		"K559Zeu8wzMBCNOMRtkLG3V3R37uuE43ojvr8rtd8ZF5JgHdii9FLrR33FfJJ1/Y\r\n"
		"dhkIoSNde1XW7OS7eyArz84B+yCVae2MnIc+lneT9qxOhZGOpunvekRyAAfgUg==\r\n"
		"-----END CERTIFICATE-----\r\n";

//client private key here
const char mbedtls_client_key[] =
		"-----BEGIN RSA PRIVATE KEY-----\r\n"
		"MIIEpAIBAAKCAQEAwyKlu/oBelie+mz6hYPZD3+JrtrIck3AZVwOhbDJPB8wNWs3\r\n"
		"do0kWMjUmcFRQ/mGPrT/tqvr70aZd/V2XCBtfxXY+VJhW40yoJ9gyEZR7LoS6/HH\r\n"
		"mymnUzKtkZDVbV9Y7+nr2jbQf0BfA99Fi4X0euh8RRhtSYnRyw91+fHf5DuJx3fh\r\n"
		"rGIFiWjbc68TtZ38nYb+GI5o8RQrlHTK3+Iu4UYAYm/EgJQMOXy31G1u8tCN19Wa\r\n"
		"vr87B1/u2Oekbn6oW64OjiATSllN0xjj8ScFkX0pXzldr27rxNL+DWqKTYXRNRfs\r\n"
		"cT4i+FM75ohaIAQhQ4Qb8Npf4IxHBLT4NMgF9wIDAQABAoIBACeEZLvkrVfxioB8\r\n"
		"gV94jASvd1qJw/4h8MyWD/rTCm26gzDOPbUscCfqw+M/Ww3BAobAcOi1eFpEmd/J\r\n"
		"Peb6SjJqYj5biIvZ1F7i596nHwApzpspo5qwnMokgTHxesdjf8dWQAU5BJBAoP1P\r\n"
		"we23ewNJAaGciMVTu8C7qNCtQwIdEXNkhkfpG7Ytj3v/gOQ/CUmjbVvOvMmfdlmW\r\n"
		"SYH/dmdgvvxjqQTbHHlgdaXWJ2h030uzqZDuE6NjIM5rR+i9GxOn7v6ztSgdavug\r\n"
		"5GPD8JrVhV2Q56flc6S4iqIEOItb7RnA6l/Bi6iBBvkgWreJU9ErIlxOpI4VF6n/\r\n"
		"Gjg5K4ECgYEA+pg1BQ8OIoY/mCp3BNPLHnn+L79xod1WQFfGr5dlRBvfZemWNwtu\r\n"
		"hNqdNmqyd9Njc9ilRXQtcuwTf39q/DjBOFh4a8lsGQ94VZX9ximMz1p06ZWa7bFB\r\n"
		"9QNnWyVKuRVWJJp5WkfV3GD1soPpEpRKyZrKIzzJWk2rBPCEwQKroBkCgYEAx1gx\r\n"
		"PPj/31KbMEadf+s87RgGR37+Xfw0berOc7J65z6M51+lciKC3E8JziNKrlC5Btnx\r\n"
		"bW3pzfEyHxQffHZ0ERjovQcBDtVylBJJkDAQVCy6yCNOZ32Je0QKR4GHxc8D12R+\r\n"
		"tBjSEQGYLzoNOm57PsRePk7R5HXWf/mEO68JWI8CgYEAuFhN8JnBqS4fDD547aks\r\n"
		"LBBMKC3qVsuvTogD1lpGGZNzhNIQOhADzmHP8x8MiM+NwsPl2LD9WiRGt60xM7hA\r\n"
		"k8WpWImFJu5VdIhxdlxMhKEjjk4K+b5DKg3F86v8SyliBG1Kxlo//e2p8RfO1mcW\r\n"
		"mTZwavmlmfO20lxpHF9th8kCgYALNW1HVWTlxLpPI5lViP+bAT/RI1XgKP81sv86\r\n"
		"yC1a9Uxs8hbWbRRYmOUfPyLC3G0a8oQ9t/FukAJWdwYyNGLgVzs27b7ke6H+q2yR\r\n"
		"e/JfGUjDWiDddtSVJsVBMgpVWAeKQ+9P5xlgtWs+NJBr2ax6YgY+kKYCp5GRDpGd\r\n"
		"3YC0BwKBgQCrkAg3++GO9aBzysjSqQG4rUjBS+YPPGw0OKw9TmeRkqImgySKSf7C\r\n"
		"55P8MsRZMbFdKF1QAwqMaU77CJ1vs4Iow6s4dJ/NnFk8Ma5ZSS2zt1PZc01BXdDd\r\n"
		"jjVMWlv1vpzRTBnUq8EVBV64Hyz9OM3H9mhXtNUkJWA6jNW3mNsgkw==\r\n"
		"-----END RSA PRIVATE KEY-----\r\n";


const size_t mbedtls_aws_root_certificate_len = sizeof(mbedtls_aws_root_certificate);
const size_t mbedtls_client_certificate_len = sizeof(mbedtls_client_certificate);
const size_t mbedtls_client_key_len = sizeof(mbedtls_client_key);

//if you want to use static memory, enable MBEDTLS_MEMORY_BUFFER_ALLOC_C
#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
#define MEMORY_HEAP_SIZE      (1024*64)
uint8_t alloc_buf[MEMORY_HEAP_SIZE];
#endif

mbedtls_net_context server_fd;
const char *pers = "mbedtls";

mbedtls_entropy_context entropy;
mbedtls_ctr_drbg_context ctr_drbg;
mbedtls_ssl_context ssl;
mbedtls_ssl_config conf;
mbedtls_x509_crt cacert;
mbedtls_x509_crt cli_cert;
mbedtls_pk_context cli_key;

//freertos calloc & free
#if !defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
void *mbedtls_calloc( size_t n, size_t size )
{
	const size_t poolSize = n * size;
	void *p = pvPortMalloc(poolSize);
	if (p != NULL)
	{
		memset(p, 0, poolSize);
	}
	return p;
}

void mbedtls_free( void *ptr )
{
	vPortFree(ptr);
}
#endif

static void my_debug(void *ctx, int level, const char *file, int line, const char *str) {
	((void) level);
	mbedtls_fprintf((FILE*) ctx, "%s:%04d: %s", file, line, str);
	fflush((FILE*) ctx);
}

int net_init(Network *n, char *host) {
	int ret;

	//if you want to use static memory, enable MBEDTLS_MEMORY_BUFFER_ALLOC_C
#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
	mbedtls_memory_buffer_alloc_init(alloc_buf, sizeof(alloc_buf));
#endif

#if defined(MBEDTLS_DEBUG_C)
	mbedtls_debug_set_threshold(DEBUG_LEVEL);
#endif

	//mbedtls_net_init(&server_fd); //MX_LWIP_Init() is called already in "StartDefaultTask"
	mbedtls_ssl_init(&ssl);
	mbedtls_ssl_config_init(&conf);

	mbedtls_x509_crt_init(&cacert);
	mbedtls_x509_crt_init(&cli_cert);
	mbedtls_pk_init(&cli_key);

	mbedtls_ctr_drbg_init(&ctr_drbg);
	mbedtls_entropy_init(&entropy);

	if ((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char*) pers,
			strlen(pers))) != 0) {
		return -1;
	}

	//parse root CA certificate
	ret = mbedtls_x509_crt_parse(&cacert, (const unsigned char*) mbedtls_aws_root_certificate, mbedtls_aws_root_certificate_len);
	if (ret < 0) {
		printf("mbedtls_x509_crt_parse failed.\n");
		return -1;
	}

	//parse client certificate
	ret = mbedtls_x509_crt_parse(&cli_cert, (const unsigned char *) mbedtls_client_certificate, mbedtls_client_certificate_len);
	if (ret < 0) {
		printf("mbedtls_x509_crt_parse failed.\n");
		return -1;
	}

	//parse client private key
	ret = mbedtls_pk_parse_key(&cli_key, (const unsigned char *)mbedtls_client_key, mbedtls_client_key_len , (unsigned char const *)"", 0);
	if (ret < 0) {
		printf("mbedtls_pk_parse_key failed.\n");
		return -1;
	}

	//configure ssl
	ret = mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT,
			MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT);
	if (ret < 0) {
		printf("mbedtls_ssl_config_defaults failed.\n");
		return -1;
	}

	mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_OPTIONAL);
	mbedtls_ssl_conf_ca_chain(&conf, &cacert, NULL);
	mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);
	mbedtls_ssl_conf_dbg(&conf, my_debug, stdout);

	//config client certificate & key
	ret = mbedtls_ssl_conf_own_cert(&conf, &cli_cert, &cli_key);
	if (ret < 0) {
		printf("mbedtls_ssl_conf_own_cert failed.\n");
		return -1;
	}

	//set timeout 1000ms, mbedtls_ssl_conf_read_timeout has problem with accurate timeout
	mbedtls_ssl_conf_read_timeout(&conf, 1000);

	//ssl setup
	ret = mbedtls_ssl_setup(&ssl, &conf);
	if (ret < 0) {
		printf("mbedtls_ssl_setup failed.\n");
		return -1;
	}

	//set hostname
	ret = mbedtls_ssl_set_hostname(&ssl, host);
	if (ret < 0) {
		printf("mbedtls_ssl_set_hostname failed.\n");
		return -1;
	}

	//set bio
	mbedtls_ssl_set_bio(&ssl, &server_fd, mbedtls_net_send, mbedtls_net_recv, NULL);

	//register functions for MQTT
	n->mqttread = net_read; //receive function
	n->mqttwrite = net_write; //send function
	n->disconnect = net_disconnect; //disconnection function

	return 0;
}

int net_connect(Network *n, char *host, char* port) {
	int ret;

	//connect
	ret = mbedtls_net_connect(&server_fd, host, port, MBEDTLS_NET_PROTO_TCP);
	if (ret < 0) {
		printf("mbedtls_net_connect failed.\n");
		return -1;
	}

	//handshake
	while ((ret = mbedtls_ssl_handshake(&ssl)) != 0) {
		if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
			if(ret == MBEDTLS_ERR_X509_CERT_VERIFY_FAILED)
			{
				printf("mbedtls_ssl_handshake certificate verification failed.\n");
			}
			else
			{
				printf("mbedtls_ssl_handshake failed.\n");
			}

			return -1;
		}
	}

	//verify
	ret = mbedtls_ssl_get_verify_result(&ssl);
	if (ret < 0) {
		printf("mbedtls_ssl_get_verify_result failed.\n");
		return -1;
	}

	return 0;
}

//receive data
int net_read(Network *n, unsigned char *buffer, int len, int timeout_ms) {
	int ret;
	int received = 0;
	int error = 0;
	int complete = 0;

	//set timeout
	if (timeout_ms != 0) {
		mbedtls_ssl_conf_read_timeout(&conf, timeout_ms);
	}

	//read until received length is bigger than variable len
	do {
		ret = mbedtls_ssl_read(&ssl, buffer, len);
		if (ret > 0) {
			received += ret;
		} else if (ret != MBEDTLS_ERR_SSL_WANT_READ) {
			error = 1;
		}
		if (received >= len) {
			complete = 1;
		}
	} while (!error && !complete);

	return received;
}

//send data
int net_write(Network *n, unsigned char *buffer, int len, int timeout_ms) {
	int ret;
	int written;

	//check all bytes are written
	for (written = 0; written < len; written += ret) {
		while ((ret = mbedtls_ssl_write(&ssl, buffer + written, len - written)) <= 0) {
			if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
				return ret;
			}
		}
	}

	return written;
}

//disconnect ssl
void net_disconnect(Network *n) {
	int ret;

	do {
		ret = mbedtls_ssl_close_notify(&ssl);
	} while (ret == MBEDTLS_ERR_SSL_WANT_WRITE);

	mbedtls_ssl_session_reset(&ssl);
	mbedtls_net_free(&server_fd);
}

//clear resources
void net_clear() {
	mbedtls_net_free(&server_fd);
	mbedtls_x509_crt_free(&cacert);
	mbedtls_x509_crt_free(&cli_cert);
	mbedtls_pk_free(&cli_key);
	mbedtls_ssl_free(&ssl);
	mbedtls_ssl_config_free(&conf);
	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);

#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
	mbedtls_memory_buffer_alloc_free();
#endif
}

uint32_t MilliTimer;

//Timer functions
char TimerIsExpired(Timer *timer) {
	long left = timer->end_time - MilliTimer;
	return (left < 0);
}

void TimerCountdownMS(Timer *timer, unsigned int timeout) {
	timer->end_time = MilliTimer + timeout;
}

void TimerCountdown(Timer *timer, unsigned int timeout) {
	timer->end_time = MilliTimer + (timeout * 1000);
}

int TimerLeftMS(Timer *timer) {
	long left = timer->end_time - MilliTimer;
	return (left < 0) ? 0 : left;
}

void TimerInit(Timer *timer) {
	timer->end_time = 0;
}

