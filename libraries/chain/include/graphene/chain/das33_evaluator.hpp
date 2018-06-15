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
#include <graphene/chain/protocol/das33_operations.hpp>
#include <graphene/chain/das33_object.hpp>

namespace graphene { namespace chain {

  class das33_project_create_evaluator : public evaluator<das33_project_create_evaluator>
  {
  public:
    typedef das33_project_create_operation operation_type;

    void_result do_evaluate( const operation_type& op );
    object_id_type do_apply( const operation_type& op );
  };

  class das33_project_update_evaluator : public evaluator<das33_project_update_evaluator>
  {
  public:
    typedef das33_project_update_operation operation_type;

    void_result do_evaluate( const operation_type& op );
    void_result do_apply( const operation_type& op );
  private:
    const das33_project_object* project_to_update = nullptr;
  };

  class das33_project_delete_evaluator : public evaluator<das33_project_delete_evaluator>
  {
  public:
    typedef das33_project_delete_operation operation_type;

    void_result do_evaluate( const operation_type& op );
    void_result do_apply( const operation_type& op );
  private:
    const das33_project_object* project_to_delete = nullptr;
  };

  class das33_pledge_asset_evaluator : public evaluator<das33_pledge_asset_evaluator>
  {
  public:
    typedef das33_pledge_asset_operation operation_type;

    void_result do_evaluate( const operation_type& op );
    object_id_type do_apply( const operation_type& op );

  private:
    void_result do_evaluate_cycles(const database &d, const das33_pledge_asset_operation &op, const account_object &account_obj) const;
    void_result do_evaluate_asset(const database &d, const das33_pledge_asset_operation &op, const account_object &balance_obj) const;
    void_result do_apply_cycles(database &d, const das33_pledge_asset_operation &op, const license_information_object &license_obj) const;
    void_result do_apply_asset(database &d, const das33_pledge_asset_operation &op, const account_balance_object &balance_obj) const;

    asset expected;
  };

} }  // namespace graphene::chain
