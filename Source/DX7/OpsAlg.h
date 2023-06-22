//------------------------------------------------------------------------------
// Copyright (c) 2022 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------
//
// \brief OPS YM21280 Algorithm

#pragma once

#include "OpsBase.h"

class OpsAlg : public OpsBase
{
public:
   OpsAlg() = default;

   int32_t alg1()
   {
      (void) op<6, /* SEL */ 1, /* A */ 1, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) op<5, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) op<4, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) op<3, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 2>();
      (void) op<2, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return op<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 2>();
   }

   int32_t alg2()
   {
      (void) op<6, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) op<5, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) op<4, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) op<3, /* SEL */ 5, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 2>();
      (void) op<2, /* SEL */ 1, /* A */ 1, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return op<1, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 2>();
   }

   int32_t alg3()
   {
      (void) op<6, /* SEL */ 1, /* A */ 1, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) op<5, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) op<4, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 2>();
      (void) op<3, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) op<2, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return op<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 2>();
   }

   int32_t alg4()
   {
      (void) op<6, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) op<5, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) op<4, /* SEL */ 0, /* A */ 1, /* C */ 0, /* D */ 1, /* COM */ 2>();
      (void) op<3, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) op<2, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return op<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 2>();
   }

   int32_t alg5()
   {
      (void) op<6, /* SEL */ 1, /* A */ 1, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) op<5, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 3>();
      (void) op<4, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) op<3, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 3>();
      (void) op<2, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return op<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 3>();
   }

   int32_t alg6()
   {
      (void) op<6, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) op<5, /* SEL */ 0, /* A */ 1, /* C */ 0, /* D */ 1, /* COM */ 3>();
      (void) op<4, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) op<3, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 3>();
      (void) op<2, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return op<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 3>();
   }

   int32_t alg7()
   {
      (void) op<6, /* SEL */ 1, /* A */ 1, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) op<5, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) op<4, /* SEL */ 2, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) op<3, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 2>();
      (void) op<2, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return op<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 2>();
   }

   int32_t alg8()
   {
      (void) op<6, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) op<5, /* SEL */ 5, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) op<4, /* SEL */ 2, /* A */ 1, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) op<3, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 2>();
      (void) op<2, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return op<1, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 2>();
   }

   int32_t alg9()
   {
      (void) op<6, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) op<5, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) op<4, /* SEL */ 2, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) op<3, /* SEL */ 5, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 2>();
      (void) op<2, /* SEL */ 1, /* A */ 1, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return op<1, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 2>();
   }

   int32_t alg10()
   {
      (void) op<6, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) op<5, /* SEL */ 2, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) op<4, /* SEL */ 5, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 2>();
      (void) op<3, /* SEL */ 1, /* A */ 1, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) op<2, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return op<1, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 2>();
   }

   int32_t alg11()
   {
      (void) op<6, /* SEL */ 0, /* A */ 1, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) op<5, /* SEL */ 2, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) op<4, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 2>();
      (void) op<3, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) op<2, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return op<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 2>();
   }

   int32_t alg12()
   {
      (void) op<6, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) op<5, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) op<4, /* SEL */ 2, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) op<3, /* SEL */ 5, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 2>();
      (void) op<2, /* SEL */ 1, /* A */ 1, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return op<1, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 2>();
   }

   int32_t alg13()
   {
      (void) op<6, /* SEL */ 0, /* A */ 1, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) op<5, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) op<4, /* SEL */ 2, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) op<3, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 2>();
      (void) op<2, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return op<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 2>();
   }

   int32_t alg14()
   {
      (void) op<6, /* SEL */ 0, /* A */ 1, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) op<5, /* SEL */ 2, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) op<4, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) op<3, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 2>();
      (void) op<2, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return op<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 2>();
   }

   int32_t alg15()
   {
      (void) op<6, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) op<5, /* SEL */ 2, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) op<4, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) op<3, /* SEL */ 5, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 2>();
      (void) op<2, /* SEL */ 1, /* A */ 1, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return op<1, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 2>();
   }

   int32_t alg16()
   {
      (void) op<6, /* SEL */ 1, /* A */ 1, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) op<5, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) op<4, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) op<3, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) op<2, /* SEL */ 2, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      return op<1, /* SEL */ 5, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
   }

   int32_t alg17()
   {
      (void) op<6, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) op<5, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) op<4, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) op<3, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) op<2, /* SEL */ 2, /* A */ 1, /* C */ 1, /* D */ 1, /* COM */ 1>();
      return op<1, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
   }

   int32_t alg18()
   {
      (void) op<6, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) op<5, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) op<4, /* SEL */ 5, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) op<3, /* SEL */ 0, /* A */ 1, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) op<2, /* SEL */ 2, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      return op<1, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
   }

   int32_t alg19()
   {
      (void) op<6, /* SEL */ 1, /* A */ 1, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) op<5, /* SEL */ 4, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 3>();
      (void) op<4, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 3>();
      (void) op<3, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) op<2, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return op<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 3>();
   }

   int32_t alg20()
   {
      (void) op<6, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) op<5, /* SEL */ 2, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) op<4, /* SEL */ 5, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 3>();
      (void) op<3, /* SEL */ 1, /* A */ 1, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) op<2, /* SEL */ 4, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 3>();
      return op<1, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 3>();
   }

   int32_t alg21()
   {
      (void) op<6, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) op<5, /* SEL */ 3, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 4>();
      (void) op<4, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
      (void) op<3, /* SEL */ 1, /* A */ 1, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) op<2, /* SEL */ 4, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
      return op<1, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
   }

   int32_t alg22()
   {
      (void) op<6, /* SEL */ 1, /* A */ 1, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) op<5, /* SEL */ 4, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 4>();
      (void) op<4, /* SEL */ 4, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
      (void) op<3, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
      (void) op<2, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return op<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
   }

   int32_t alg23()
   {
      (void) op<6, /* SEL */ 1, /* A */ 1, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) op<5, /* SEL */ 4, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 4>();
      (void) op<4, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
      (void) op<3, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) op<2, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
      return op<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
   }

   int32_t alg24()
   {
      (void) op<6, /* SEL */ 1, /* A */ 1, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) op<5, /* SEL */ 4, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 5>();
      (void) op<4, /* SEL */ 4, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 5>();
      (void) op<3, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 5>();
      (void) op<2, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 5>();
      return op<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 5>();
   }

   int32_t alg25()
   {
      (void) op<6, /* SEL */ 1, /* A */ 1, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) op<5, /* SEL */ 4, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 5>();
      (void) op<4, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 5>();
      (void) op<3, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 5>();
      (void) op<2, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 5>();
      return op<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 5>();
   }

   int32_t alg26()
   {
      (void) op<6, /* SEL */ 0, /* A */ 1, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) op<5, /* SEL */ 2, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) op<4, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 3>();
      (void) op<3, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) op<2, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 3>();
      return op<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 3>();
   }

   int32_t alg27()
   {
      (void) op<6, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) op<5, /* SEL */ 2, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) op<4, /* SEL */ 5, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 3>();
      (void) op<3, /* SEL */ 1, /* A */ 1, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) op<2, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 3>();
      return op<1, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 3>();
   }

   int32_t alg28()
   {
      (void) op<6, /* SEL */ 5, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 3>();
      (void) op<5, /* SEL */ 1, /* A */ 1, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) op<4, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) op<3, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 3>();
      (void) op<2, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return op<1, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 3>();
   }

   int32_t alg29()
   {
      (void) op<6, /* SEL */ 1, /* A */ 1, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) op<5, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 4>();
      (void) op<4, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) op<3, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
      (void) op<2, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
      return op<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
   }

   int32_t alg30()
   {
      (void) op<6, /* SEL */ 5, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 4>();
      (void) op<5, /* SEL */ 1, /* A */ 1, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) op<4, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) op<3, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
      (void) op<2, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
      return op<1, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
   }

   int32_t alg31()
   {
      (void) op<6, /* SEL */ 1, /* A */ 1, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) op<5, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 5>();
      (void) op<4, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 5>();
      (void) op<3, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 5>();
      (void) op<2, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 5>();
      return op<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 5>();
   }

   int32_t alg32()
   {
      (void) op<6, /* SEL */ 0, /* A */ 1, /* C */ 0, /* D */ 1, /* COM */ 6>();
      (void) op<5, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 6>();
      (void) op<4, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 6>();
      (void) op<3, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 6>();
      (void) op<2, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 6>();
      return op<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 6>();
   }
};
