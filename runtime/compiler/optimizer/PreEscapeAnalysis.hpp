/*******************************************************************************
 * Copyright (c) 2019, 2019 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#ifndef PREESCAPEANALYSIS_INCL
#define PREESCAPEANLAYSIS_INCL

#include <stdint.h>                           // for int32_t
#include "optimizer/Optimization.hpp"         // for Optimization
#include "optimizer/OptimizationManager.hpp"  // for OptimizationManager

namespace TR { class Block; class Node; }

typedef TR::typed_allocator<TR::Node *, TR::Region &> NodeDequeAllocator;
typedef std::deque<TR::Node *, NodeDequeAllocator> NodeDeque;

/**
 * Pass over trees before Escape Analysis if Voluntary OSR is
 * enabled.
 * @see TR_EscapeAnalysis
 */
class TR_PreEscapeAnalysis : public TR::Optimization
   {
   public:
   TR_PreEscapeAnalysis(TR::OptimizationManager *manager)
      : TR::Optimization(manager)
      {}
   static TR::Optimization *create(TR::OptimizationManager *manager)
      {
      return new (manager->allocator()) TR_PreEscapeAnalysis(manager);
      }

   virtual int32_t perform();
   virtual const char * optDetailString() const throw();

   private:
   NodeDeque *_loads;
   void insertFakePrepareForOSR(TR::Block *block, TR::Node *induceCall);
   void processAutosAndPendingPushes(TR::ResolvedMethodSymbol *rms, TR_OSRMethodData *methodData, int32_t byteCodeIndex);
   void processSymbolReferences(TR_Array<List<TR::SymbolReference>> *symbolReferences, TR_BitVector *deadSymRefs);
   };

#endif
