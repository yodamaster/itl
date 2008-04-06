/*----------------------------------------------------------------------------+
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+-----------------------------------------------------------------------------+
Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
+----------------------------------------------------------------------------*/
#ifndef __StatVarInscriptorAT_JOFA_041006_H__
#define __StatVarInscriptorAT_JOFA_041006_H__

#include <itl/ctxstringlistt.hpp>
#include <itl/varpermutationt.hpp>
#include <itl/dvarpermutationt.hpp>

namespace itl
{
	template <int IVarCountV>
	class StatVarInscriptorAT
	{
	protected:
		typedef VarPermutationT<IVarCountV> IVarPermutationTD;

	public:
		/** Die Permutation sorgt daf�r, dass die Auswahl und Reihenfolge der Beschriftung
			der unabh�ngigen Variablen ge�ndert werden kann. */
		void setIVarPermutation(const IVarPermutationTD& perm) { m_IPerm = perm; }

		/** Die Permutation sorgt daf�r, dass die Auswahl und Reihenfolge der Beschriftung
			der abh�ngigen Variablen ge�ndert werden kann. */
		void setDVarPermutation(const DVarPermutationT& perm) { m_DPerm = perm; }

		/** Permutation der abh�ngigen Variablen liefern */
		DVarPermutationT getDVarPermutation() { return m_DPerm; }

		/** Die Liste der Titel f�r unabh�ngige Variablen anf�gen */
		virtual void addIVarTitles(StringListT& headers)const=0;

		/** Die Liste der Titel f�r unabh�ngige Variablen anf�gen */
		virtual void addDVarTitles(StringListT& headers)const=0;

	protected:
		IVarPermutationTD m_IPerm;
		DVarPermutationT  m_DPerm;
	};

}

#endif // __StatVarInscriptorAT_JOFA_041006_H__


