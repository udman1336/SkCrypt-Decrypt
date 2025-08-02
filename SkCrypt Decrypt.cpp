#include <iostream>
#include <Windows.h>
#include "skCrypt.h"
#include <fstream>
#include <algorithm>
#include <unordered_set>
#include <cstdio>
#include <string>
#include <sstream>
#include <array>
#include <stdexcept>

std::string DecryptSkCrypter( std::string encrypted, char key1, char key2 ) {
    std::string decrypted;
    decrypted.reserve( encrypted.size( ) );

    for ( size_t i = 0; i < encrypted.size( ); ++i ) {
        decrypted += encrypted[ i ] ^ ( key1 + i % ( 1 + key2 ) );
    }

    return decrypted;
}

bool IsValidDecrypted( std::string str ) {
    for ( char c : str ) {
        if ( !( isprint( ( unsigned char )c ) || c == '\n' || c == '\t' ) )
            return false;
    }
    return true;
}

bool IsLikelyValidText( std::string s, size_t minUniqueAlpha = 3 ) {
    if ( s.empty( ) )
        return false;

    std::unordered_set<char> UniqueLetters;

    for ( char c : s ) {
        if ( std::isalpha( static_cast< unsigned char >( c ) ) ) {
            UniqueLetters.insert(
                static_cast< char >( std::tolower( static_cast< unsigned char >( c ) ) ) );
            if ( UniqueLetters.size( ) >= minUniqueAlpha ) {
                return true;
            }
        }
    }

    return false;
}


void BruteForceKeys( std::string encrypted ) {
    for ( char key1 = '0'; key1 <= ':'; ++key1 ) {
        for ( char key2 = '0'; key2 <= ':'; ++key2 ) {
            std::string decrypted = DecryptSkCrypter( encrypted, key1, key2 );
            if ( IsLikelyValidText( decrypted ) && IsValidDecrypted( decrypted ) ) {
                std::cout << "Possible keys: " << key1 << ", " << key2 << "\n";
                std::cout << "Decrypted text: " << decrypted << "\n";
            }
        }
    }
}

time_t GetCompileTime( std::string path ) {
    std::ifstream file( path, std::ios::binary );
    if ( !file )
        return 0;

    IMAGE_DOS_HEADER DosHeader;
    file.read( reinterpret_cast< char* >( &DosHeader ), sizeof( DosHeader ) );
    file.seekg( DosHeader.e_lfanew, std::ios::beg );

    DWORD signature;
    IMAGE_FILE_HEADER FileHeader;
    file.read( reinterpret_cast< char* >( &signature ), sizeof( signature ) );
    file.read( reinterpret_cast< char* >( &FileHeader ), sizeof( FileHeader ) );

    return FileHeader.TimeDateStamp;
}

std::pair<char, char> GetSkCryptKeysFromTimestamp( time_t timestamp ) {
    struct tm Timeinfo;
    localtime_s( &Timeinfo, &timestamp );
    char TimeStr[ 9 ];
    strftime( TimeStr, sizeof( TimeStr ), "%H:%M:%S", &Timeinfo );

    char key1 = TimeStr[ 4 ];
    char key2 = TimeStr[ 7 ];

    return { key1, key2 };
}

std::string RunCommandCaptureOutput( std::string command ) {
    std::array<char, 128> buffer {};
    std::string result;

    FILE* pipe = _popen( command.c_str( ), "r" );
    if ( !pipe ) {
        throw std::runtime_error( "popen() failed!" );
    }

    while ( fgets( buffer.data( ), buffer.size( ), pipe ) != nullptr ) {
        result += buffer.data( );
    }

    _pclose( pipe );
    return result;
}

std::string MakeCommandLineSafe( std::string input ) {
    std::string result = "\"";

    for ( char c : input ) {
        if ( c == '"' ) {
            result += "\\\"";
        }
        else {
            result += c;
        }
    }

    result += "\"";
    return result;
}


int main( int argc, char* argv[ ] ) {
    if ( argc < 2 ) {
        std::cerr << "Usage: Drag and drop a file onto this program.\n";
        Sleep( INFINITE );
    }

    std::string filename = argv[ 1 ];
    std::string SafeFilename = MakeCommandLineSafe( filename );

    time_t ts = GetCompileTime( filename );
    auto Keys = GetSkCryptKeysFromTimestamp( ts );

    std::string command = "llvm-strings " + SafeFilename;

    std::string output = RunCommandCaptureOutput( command );

    std::stringstream stream( output );
    std::string line;
    while ( std::getline( stream, line ) ) {
        auto decrypted = DecryptSkCrypter( line, Keys.first, Keys.second );

        printf( "%s\n", decrypted.c_str( ) );
    }

    Sleep( INFINITE );
}