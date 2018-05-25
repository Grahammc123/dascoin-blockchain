/*
 * MIT License
 *
 * Copyright (c) 2018 Tech Solutions Malta LTD
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <graphene/chain/evaluator.hpp>
#include <graphene/chain/protocol/daspay_operations.hpp>

namespace graphene { namespace chain {

    class set_daspay_transaction_ratio_evaluator : public evaluator<set_daspay_transaction_ratio_evaluator>
    {
    public:
      typedef set_daspay_transaction_ratio_operation operation_type;

      void_result do_evaluate(const operation_type& op);
      void_result do_apply(const operation_type& op);
    };

    class add_daspay_authority_evaluator : public evaluator<add_daspay_authority_evaluator>
    {
    public:
      typedef add_daspay_authority_operation operation_type;

      void_result do_evaluate( const operation_type& op );
      void_result do_apply( const operation_type& op );

    private:
      const account_object* _account_obj = nullptr;
    };

    class daspay_debit_evaluator : public evaluator<daspay_debit_evaluator>
    {
    public:
      typedef daspay_debit_operation operation_type;

      void_result do_evaluate( const operation_type& op );
      void_result do_apply( const operation_type& op );

    private:
      const account_object* _account_obj = nullptr;
    };

  } }  // namespace graphene::chain
