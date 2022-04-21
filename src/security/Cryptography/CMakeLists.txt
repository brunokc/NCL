add_library(wcl_cryptography OBJECT
	CryptographicException.h
	HashAlgorithm.cpp
	HashAlgorithm.h
	MD5.cpp
	MD5.h
	SHA1.cpp
	SHA1.h
	SHA256.cpp
	SHA256.h
	SHA512.cpp
	SHA512.h
	Util.h
)

target_link_libraries(wcl_cryptography PRIVATE 
	bcrypt.lib
)
