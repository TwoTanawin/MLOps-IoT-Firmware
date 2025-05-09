#ifndef SECRETS_H
#define SECRETS_H

#define WIFI_SSID "SKYNET_2.4G"
#define WIFI_PASSWORD "Tanawin42869"


// #define WIFI_SSID "PUsasi_2.4G"
// #define WIFI_PASSWORD "0816315964"

// from setting
const char AWS_IOT_ENDPOINT[] = "a1ffx9w8z4hget-ats.iot.ap-southeast-1.amazonaws.com";       //change this
 
// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";
 
// Device Certificate                                               //change this
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUJn4YrOTD7DyaGp6yzwfLwU15h6AwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI1MDMyMzE4MDcz
NFoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAOSAq2hzx8qxFHzVC3Ce
5f6OW8n6B7PdXL9BzXImnbV1fmS9Jpx3iPNuoq9UH9BOuuJhV1E0sXepAXDMj3SB
OW9DYyFbWzttuxSeT5sl3mpAys5EET+umGBg4ZjcDTCw7t/As/Nt4R6FJj3NVqOd
VOKO8IP2OQ6/vvRI5ErHUzlxjXB0BtV4PsHbGQ+KjB6cJD13G0ubOLwwrHTUibv4
BEtRGwkEJb2k/lk5JZAqvQHe8hY08I5+zSDLJO8Y1O4wqHd01KfUeucB19sZ9K+S
ulaxrWY1WmEsr3aQO5xW0IVZ03Xap7qX/SjJlSohp4fOGDa3+4Xf9ZXBsu7qKEim
mHECAwEAAaNgMF4wHwYDVR0jBBgwFoAUraEDmqepVpbcAlxBR1oJUSgGzfEwHQYD
VR0OBBYEFNXL1Bz4+vOMVBVTNSZXnwffCDvaMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQDMkC/cXVHJmLFLOhHomd0pcED5
T3rE6acLa4+65BWm81Ot0Q/EKNpEbBtHznxInvXPhIo9eqaKfk/A5cENDX8W9MC4
WvZa99O2FJ3pXyBRA6vMNzyNkgvkV2XHVy6/b5X9nYoiIzwCdJAMpo0l7vO3X8rd
pp+YLSNffdiYAC2C5RWqoZMcMyBovNb/hJRAiUOpjtGrgmxl+djlF3T9GFWSHPGj
LQ923kOKtBYW8BAgAqLglsHVAEGKYmBmTHdbZkGiYLkXj6WXW+lKBj8teTbN/HnB
jnv5CaHlI47TMF8xiQOQFDLOSJ4+ae18V27uDW9A25/U7vDoGm6AF8n5WEfC
-----END CERTIFICATE-----
)KEY";
 
// Device Private Key                                               //change this
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEA5ICraHPHyrEUfNULcJ7l/o5byfoHs91cv0HNciadtXV+ZL0m
nHeI826ir1Qf0E664mFXUTSxd6kBcMyPdIE5b0NjIVtbO227FJ5PmyXeakDKzkQR
P66YYGDhmNwNMLDu38Cz823hHoUmPc1Wo51U4o7wg/Y5Dr++9EjkSsdTOXGNcHQG
1Xg+wdsZD4qMHpwkPXcbS5s4vDCsdNSJu/gES1EbCQQlvaT+WTklkCq9Ad7yFjTw
jn7NIMsk7xjU7jCod3TUp9R65wHX2xn0r5K6VrGtZjVaYSyvdpA7nFbQhVnTddqn
upf9KMmVKiGnh84YNrf7hd/1lcGy7uooSKaYcQIDAQABAoIBAE5ZC+yxnjSSiQrQ
Za7E3OluWGyljVSPtwhhAJd+RB7+3UqfTIjjt5J4QdHKD2p2RD/wzmaqoZr82OCb
UyjIUjAjM+8xCtQSqM3GBleVIXDfLGqTDjL+o4KQVWn7MY6bbBxOT6VM7J5xWw5f
xLAu9vcDudYVa7Tx7ACIqbY2KAQOlF0yIA7VZBQO0JFVkH6NPt6VZY2DcbxAbuqT
XuNtAdCnxgwNdp8SiPdImGdDrw8xCZ4cEfXUVMD9oRNaCFNtlbqqvMOpuplVdQyK
cb1Udpv9ZEnfK04IkacouhBUuii9MJ5BhKRlmM0Y9wKEoTiSH3JwaVSD5JFFY9CP
HYZKOJECgYEA+EiFlLP9q81NLNiFEgqfiKRR61xRgWmc0+c7YH0gvSCSxUkt3DP8
6a3s03vCmLocHzB8JN3KVIKT0HLrmCVzalTFE1avSILtAaC9z3Q01/YqeXMHAWT8
qk22Cx+eGx43pvFA0TjNaaATjyNUWvxpA9AqthJKPDsTK/L9FV72RZ0CgYEA65rD
CaO8LyniOy+OzbV0f00huO4aaWm5nxP7AXQ0VDFA1shI0cM+YYwq4yclexHLOORz
D8XSwc+ZHFrIv4qj0wg3bzfk3FH3fEjVilHdUjBcXTXeB1sWlZinUcxPx0ihw/EF
fIfCHATqECYipJVKfU6vrd0/VYUtFczxwVaUr+UCgYB5VhfSIFRsTZzVucrukjp8
8LEIMMm0UQTPmkWuVfhV9UFFIQpEfzOuj80ZXXYwZWOw6mUBL1LeEiJi0IbKiCLr
aBoX5VksOq/RB9k7NTNOB4OibtCRoeTI5cl9jioaXp4b+/MNTJE2eYCVh1KGVKyN
rIHzEOUuokAwTkPY2dvJEQKBgE2VV/Z7qYNqamoM/hx1TViCQMGR/raJrytYPAa9
imHTGkPgDkKs5YTf0Fuez3PYZzBj4hPmk8FoqEWzC3ovoETk5sREBqQahcGyhkcX
MCdZb4DuNR/8+dHnQ+nmurqoC0TnwGPCnMP3RlBtDmleE/pxh95cBfgWc8NDCrUc
2reVAoGBAN90HMjCZbOhbBIV/7OrwZWUf37JDh/DtIXk3fwUTK5rcgD579QoUiiK
MvsZ+31Zmc7LfTOBWnm1/AeFB0c5dOgP9mqg9QiyImFwhjXB0HqIDqS5NVBM1sg7
oKqSK7V/etITmWP2TOo3nMaRjo7meEUD3SvSjfa4nYEHcVGiL3xi
-----END RSA PRIVATE KEY-----
)KEY";

#endif
