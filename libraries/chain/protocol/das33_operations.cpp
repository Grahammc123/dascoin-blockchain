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

#include <graphene/chain/protocol/das33_operations.hpp>

// TODO: blacklist operation

namespace graphene { namespace chain {

  void das33_project_create_operation::validate() const
  {
    FC_ASSERT( fee.amount >= 0 );
    //FC_ASSERT( ratio. >= 0 && ratio.second >= 0);
    const size_t len = name.size();
    FC_ASSERT ( len >= GRAPHENE_MIN_ACCOUNT_NAME_LENGTH );
    FC_ASSERT ( len <= GRAPHENE_MAX_ACCOUNT_NAME_LENGTH );
  }

  void das33_project_update_operation::validate() const
  {
    FC_ASSERT( fee.amount >= 0 );
    //if (ratio)
    //  FC_ASSERT( (*ratio).first >= 0 && (*ratio).second >= 0);
    const size_t len = name.size();
    FC_ASSERT ( len >= GRAPHENE_MIN_ACCOUNT_NAME_LENGTH );
    FC_ASSERT ( len <= GRAPHENE_MAX_ACCOUNT_NAME_LENGTH );
  }

  void das33_project_delete_operation::validate() const
  {
    FC_ASSERT( fee.amount >= 0 );
  }

  void das33_pledge_cycles_operation::validate() const
  {
    FC_ASSERT( fee.amount >= 0 );
  }

} } // namespace graphene::chain
