// ThrowExceptionInsideDLLInMemory.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MemoryModule.h"

#include <fstream>
#include <vector>


namespace
{
   typedef bool (__cdecl *DLLFUNC)(int);

   const std::string dllName = "DLLToLoad.dll";
   const std::string functionName = "FuncInDLL";

   bool LoadLibraryDynamicaly(int inputToFunction)
   {
      bool result = false;

      const HMODULE module = LoadLibrary(dllName.c_str());
      if(module)
      {
         const FARPROC proc = GetProcAddress(module, functionName.c_str());
         const DLLFUNC funcInDll = reinterpret_cast<DLLFUNC>(proc);
         result = funcInDll(inputToFunction);

         FreeLibrary(module);
      }

      return result;
   }

   bool LoadLibraryInMemory(int inputToFunction)
   {
      bool result = false;

      std::ifstream file(dllName, std::ios::in | std::ios::binary | std::ios::ate);
      if(file.is_open())
      {
         const std::ifstream::pos_type size = file.tellg();
         std::vector<char> memfile(static_cast<unsigned int>(size));

         file.seekg(std::ios::beg);
         file.read(&memfile.front(), size);
         file.close();

         const HMEMORYMODULE module = MemoryLoadLibrary(&memfile.front());
         if(module)
         {
            const FARPROC proc = MemoryGetProcAddress(module, functionName.c_str());
            const DLLFUNC funcInDll = reinterpret_cast<DLLFUNC>(proc);
            result = funcInDll(inputToFunction);

            MemoryFreeLibrary(module);
         }

         file.close();
      }

      return result;
   }
}

int _tmain(int argc, _TCHAR* argv[])
{
   // Both functions loads the same dll and calls the same function inside it. 
   // The function will throw an exception if the value differs from 7 and catch it,
   // if an exception is thrown the function returns false, else it
   // will return true.

   // This function always works.
   const bool dynResult = LoadLibraryDynamicaly(11);
   
   // This function will crash if an exception is thrown inside the dll.
   // Change the input to anything else than 7 and it will crash.
   const bool memResult = LoadLibraryInMemory(7);

	return 0;
}

