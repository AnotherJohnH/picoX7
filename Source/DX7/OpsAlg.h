//------------------------------------------------------------------------------
// The data in this file is a derivative of data reverse engineered from die
// photographs of the YM21280 published in Ken Shirriff's blog. That publicly
// available document does not contain a copyright notice or any mention of
// permissions or restrictions that apply to the use of the data. However,
// according to law that does not necessarily mean that the data is free from
// restrictions on its use. At the time of publication, the copyright holder 
// is probably Yamaha. If the copyright holder wishes to retrospectively declare
// reasonable and legal restrictions on the data, then either those restrictions
// must be obeyed or this file shouled be deleted.
//
// The statements above shall be included in all copies or substantial portions
// of the data.
//
// THE DATA IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE DATA OR THE USE OR OTHER DEALINGS IN THE
// DATA.
//------------------------------------------------------------------------------
//
// \brief OPS YM21280 Algorithm
//
// Credit to Ken Shirriff for his decode of the algorithm ROM in the YM21280
// via decapping and die photography

#pragma once

#include "EgsOps.h"

//! Implement the 32 DX7 OP algorithms
class OpsAlg : public EgsOps</* NUM_OP */ 6>
{
public:
   OpsAlg() = default;

   //! Return next sample for the selected algorithm
   int32_t operator()()
   {
      return ((*this).*alg_ptr)();
   }

   //! Select the algorithm
   void prog(const SysEx::Packed* sysex)
   {
      EgsOps::prog(sysex);

      switch (sysex->alg + 1)
      {
      case  1: alg_ptr = &OpsAlg::alg1;  break;
      case  2: alg_ptr = &OpsAlg::alg2;  break;
      case  3: alg_ptr = &OpsAlg::alg3;  break;
      case  4: alg_ptr = &OpsAlg::alg4;  break;
      case  5: alg_ptr = &OpsAlg::alg5;  break;
      case  6: alg_ptr = &OpsAlg::alg6;  break;
      case  7: alg_ptr = &OpsAlg::alg7;  break;
      case  8: alg_ptr = &OpsAlg::alg8;  break;
      case  9: alg_ptr = &OpsAlg::alg9;  break;
      case 10: alg_ptr = &OpsAlg::alg10; break;
      case 11: alg_ptr = &OpsAlg::alg11; break;
      case 12: alg_ptr = &OpsAlg::alg12; break;
      case 13: alg_ptr = &OpsAlg::alg13; break;
      case 14: alg_ptr = &OpsAlg::alg14; break;
      case 15: alg_ptr = &OpsAlg::alg15; break;
      case 16: alg_ptr = &OpsAlg::alg16; break;
      case 17: alg_ptr = &OpsAlg::alg17; break;
      case 18: alg_ptr = &OpsAlg::alg18; break;
      case 19: alg_ptr = &OpsAlg::alg19; break;
      case 20: alg_ptr = &OpsAlg::alg20; break;
      case 21: alg_ptr = &OpsAlg::alg21; break;
      case 22: alg_ptr = &OpsAlg::alg22; break;
      case 23: alg_ptr = &OpsAlg::alg23; break;
      case 24: alg_ptr = &OpsAlg::alg24; break;
      case 25: alg_ptr = &OpsAlg::alg25; break;
      case 26: alg_ptr = &OpsAlg::alg26; break;
      case 27: alg_ptr = &OpsAlg::alg27; break;
      case 28: alg_ptr = &OpsAlg::alg28; break;
      case 29: alg_ptr = &OpsAlg::alg29; break;
      case 30: alg_ptr = &OpsAlg::alg30; break;
      case 31: alg_ptr = &OpsAlg::alg31; break;
      case 32: alg_ptr = &OpsAlg::alg32; break;
      }
   }

private:
   int32_t alg1()
   {
      (void) ops<6, /* SEL */ 1, /* A */ 1, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) ops<5, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) ops<4, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) ops<3, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 2>();
      (void) ops<2, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return ops<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 2>();
   }

   int32_t alg2()
   {
      (void) ops<6, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) ops<5, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) ops<4, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) ops<3, /* SEL */ 5, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 2>();
      (void) ops<2, /* SEL */ 1, /* A */ 1, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return ops<1, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 2>();
   }

   int32_t alg3()
   {
      (void) ops<6, /* SEL */ 1, /* A */ 1, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) ops<5, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) ops<4, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 2>();
      (void) ops<3, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) ops<2, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return ops<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 2>();
   }

   int32_t alg4()
   {
      (void) ops<6, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) ops<5, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) ops<4, /* SEL */ 0, /* A */ 1, /* C */ 0, /* D */ 1, /* COM */ 2>();
      (void) ops<3, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) ops<2, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return ops<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 2>();
   }

   int32_t alg5()
   {
      (void) ops<6, /* SEL */ 1, /* A */ 1, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) ops<5, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 3>();
      (void) ops<4, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) ops<3, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 3>();
      (void) ops<2, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return ops<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 3>();
   }

   int32_t alg6()
   {
      (void) ops<6, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) ops<5, /* SEL */ 0, /* A */ 1, /* C */ 0, /* D */ 1, /* COM */ 3>();
      (void) ops<4, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) ops<3, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 3>();
      (void) ops<2, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return ops<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 3>();
   }

   int32_t alg7()
   {
      (void) ops<6, /* SEL */ 1, /* A */ 1, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) ops<5, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) ops<4, /* SEL */ 2, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) ops<3, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 2>();
      (void) ops<2, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return ops<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 2>();
   }

   int32_t alg8()
   {
      (void) ops<6, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) ops<5, /* SEL */ 5, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) ops<4, /* SEL */ 2, /* A */ 1, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) ops<3, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 2>();
      (void) ops<2, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return ops<1, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 2>();
   }

   int32_t alg9()
   {
      (void) ops<6, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) ops<5, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) ops<4, /* SEL */ 2, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) ops<3, /* SEL */ 5, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 2>();
      (void) ops<2, /* SEL */ 1, /* A */ 1, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return ops<1, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 2>();
   }

   int32_t alg10()
   {
      (void) ops<6, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) ops<5, /* SEL */ 2, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) ops<4, /* SEL */ 5, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 2>();
      (void) ops<3, /* SEL */ 1, /* A */ 1, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) ops<2, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return ops<1, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 2>();
   }

   int32_t alg11()
   {
      (void) ops<6, /* SEL */ 0, /* A */ 1, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) ops<5, /* SEL */ 2, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) ops<4, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 2>();
      (void) ops<3, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) ops<2, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return ops<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 2>();
   }

   int32_t alg12()
   {
      (void) ops<6, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) ops<5, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) ops<4, /* SEL */ 2, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) ops<3, /* SEL */ 5, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 2>();
      (void) ops<2, /* SEL */ 1, /* A */ 1, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return ops<1, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 2>();
   }

   int32_t alg13()
   {
      (void) ops<6, /* SEL */ 0, /* A */ 1, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) ops<5, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) ops<4, /* SEL */ 2, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) ops<3, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 2>();
      (void) ops<2, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return ops<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 2>();
   }

   int32_t alg14()
   {
      (void) ops<6, /* SEL */ 0, /* A */ 1, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) ops<5, /* SEL */ 2, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) ops<4, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) ops<3, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 2>();
      (void) ops<2, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return ops<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 2>();
   }

   int32_t alg15()
   {
      (void) ops<6, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) ops<5, /* SEL */ 2, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) ops<4, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) ops<3, /* SEL */ 5, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 2>();
      (void) ops<2, /* SEL */ 1, /* A */ 1, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return ops<1, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 2>();
   }

   int32_t alg16()
   {
      (void) ops<6, /* SEL */ 1, /* A */ 1, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) ops<5, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) ops<4, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) ops<3, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) ops<2, /* SEL */ 2, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      return ops<1, /* SEL */ 5, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
   }

   int32_t alg17()
   {
      (void) ops<6, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) ops<5, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) ops<4, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) ops<3, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) ops<2, /* SEL */ 2, /* A */ 1, /* C */ 1, /* D */ 1, /* COM */ 1>();
      return ops<1, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
   }

   int32_t alg18()
   {
      (void) ops<6, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) ops<5, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) ops<4, /* SEL */ 5, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) ops<3, /* SEL */ 0, /* A */ 1, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) ops<2, /* SEL */ 2, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      return ops<1, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
   }

   int32_t alg19()
   {
      (void) ops<6, /* SEL */ 1, /* A */ 1, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) ops<5, /* SEL */ 4, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 3>();
      (void) ops<4, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 3>();
      (void) ops<3, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) ops<2, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return ops<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 3>();
   }

   int32_t alg20()
   {
      (void) ops<6, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) ops<5, /* SEL */ 2, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) ops<4, /* SEL */ 5, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 3>();
      (void) ops<3, /* SEL */ 1, /* A */ 1, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) ops<2, /* SEL */ 4, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 3>();
      return ops<1, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 3>();
   }

   int32_t alg21()
   {
      (void) ops<6, /* SEL */ 1, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) ops<5, /* SEL */ 3, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 4>();
      (void) ops<4, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
      (void) ops<3, /* SEL */ 1, /* A */ 1, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) ops<2, /* SEL */ 4, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
      return ops<1, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
   }

   int32_t alg22()
   {
      (void) ops<6, /* SEL */ 1, /* A */ 1, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) ops<5, /* SEL */ 4, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 4>();
      (void) ops<4, /* SEL */ 4, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
      (void) ops<3, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
      (void) ops<2, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return ops<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
   }

   int32_t alg23()
   {
      (void) ops<6, /* SEL */ 1, /* A */ 1, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) ops<5, /* SEL */ 4, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 4>();
      (void) ops<4, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
      (void) ops<3, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) ops<2, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
      return ops<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
   }

   int32_t alg24()
   {
      (void) ops<6, /* SEL */ 1, /* A */ 1, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) ops<5, /* SEL */ 4, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 5>();
      (void) ops<4, /* SEL */ 4, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 5>();
      (void) ops<3, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 5>();
      (void) ops<2, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 5>();
      return ops<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 5>();
   }

   int32_t alg25()
   {
      (void) ops<6, /* SEL */ 1, /* A */ 1, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) ops<5, /* SEL */ 4, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 5>();
      (void) ops<4, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 5>();
      (void) ops<3, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 5>();
      (void) ops<2, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 5>();
      return ops<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 5>();
   }

   int32_t alg26()
   {
      (void) ops<6, /* SEL */ 0, /* A */ 1, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) ops<5, /* SEL */ 2, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) ops<4, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 3>();
      (void) ops<3, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) ops<2, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 3>();
      return ops<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 3>();
   }

   int32_t alg27()
   {
      (void) ops<6, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 1>();
      (void) ops<5, /* SEL */ 2, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 1>();
      (void) ops<4, /* SEL */ 5, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 3>();
      (void) ops<3, /* SEL */ 1, /* A */ 1, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) ops<2, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 3>();
      return ops<1, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 3>();
   }

   int32_t alg28()
   {
      (void) ops<6, /* SEL */ 5, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 3>();
      (void) ops<5, /* SEL */ 1, /* A */ 1, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) ops<4, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) ops<3, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 3>();
      (void) ops<2, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      return ops<1, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 3>();
   }

   int32_t alg29()
   {
      (void) ops<6, /* SEL */ 1, /* A */ 1, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) ops<5, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 4>();
      (void) ops<4, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) ops<3, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
      (void) ops<2, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
      return ops<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
   }

   int32_t alg30()
   {
      (void) ops<6, /* SEL */ 5, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 4>();
      (void) ops<5, /* SEL */ 1, /* A */ 1, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) ops<4, /* SEL */ 1, /* A */ 0, /* C */ 1, /* D */ 0, /* COM */ 1>();
      (void) ops<3, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
      (void) ops<2, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
      return ops<1, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 4>();
   }

   int32_t alg31()
   {
      (void) ops<6, /* SEL */ 1, /* A */ 1, /* C */ 0, /* D */ 0, /* COM */ 1>();
      (void) ops<5, /* SEL */ 0, /* A */ 0, /* C */ 0, /* D */ 1, /* COM */ 5>();
      (void) ops<4, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 5>();
      (void) ops<3, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 5>();
      (void) ops<2, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 5>();
      return ops<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 5>();
   }

   int32_t alg32()
   {
      (void) ops<6, /* SEL */ 0, /* A */ 1, /* C */ 0, /* D */ 1, /* COM */ 6>();
      (void) ops<5, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 6>();
      (void) ops<4, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 6>();
      (void) ops<3, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 6>();
      (void) ops<2, /* SEL */ 0, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 6>();
      return ops<1, /* SEL */ 5, /* A */ 0, /* C */ 1, /* D */ 1, /* COM */ 6>();
   }

   int32_t (OpsAlg::*alg_ptr)() {&OpsAlg::alg1};
};
