// DLLToLoad'.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdexcept>

#define DLLEXPORT __declspec(dllexport)
extern "C" DLLEXPORT bool FuncInDLL(int input);

bool FuncInDLL(int input)
{
   bool returnValue = false;

   try
   {
      if(input != 7)
         throw std::runtime_error("input is not seven");

      returnValue = true;
   }
   catch(const std::runtime_error&)
   { }
   catch(...)
   { }

   return returnValue;
}

